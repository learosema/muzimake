# Immediate-Mode UI Library – Architektur-Notizen

## Misconception

You don't need Callbacks, Listener, Event Pools (as in Win32, GTK, DOM) in Immediate Mode.

In Immediate Mode, every Widget is called each frame. The return value of the widget is the event.

```c
if (ui_button(&gui, "Speichern", rect)) {
    save_file(); // das IST die Event-Behandlung, im selben Frame
}
```

No event pool necessary.

## Input-Layer (`ui_ctx.h`)

The existing header cleanly separates the following:

- **Raw input** (`ui_mouse_state_t`, `ui_keyboard_state_t`) – populated per frame by the platform layer (SDL events *or* DOS keyboard interrupt/scancode table).
- **Edge detection** via a `last_event` bitmask that is reset during `begin_frame`/`end_frame` – allowing a distinction between "key is down" (level, `buttons` field) and "key was just pressed" (edge, `is_*_pressed()`).

## Hit-Testing (`geometry.h`)

```c
typedef struct rect_s { int x, y, width, height; } rect_t;

bool rect_test_point_in_bounds(const rect_t*, uint8_t x, uint8_t y); // Zellraum (0-255)
bool rect_test_mouse(const rect_t*, uint16_t mouseX, uint16_t mouseY); // Pixelraum
rect_t get_clientrect(const rect_t* bounding_rect); // inneres Rect, Border abgezogen (wie Win32 GetClientRect)
```

TODO: use pixel coords everywhere (uint16), not character coords for react_test and pixel coords for mouse

---

## hot/active/focused State – the Core of the Refactor

- `cmponent.c / h` is obsolete (retained mode mindset, unnecessary overhead)

```c
typedef struct ui_gui_s {
    ui_context_t input;
    uint32_t hot_id, active_id, focused_id;
    uint32_t focus_order[UI_MAX_FOCUSABLE]; // rebuilt every frame
    uint16_t focus_count;
    bool tab_pressed, tab_reverse;
} ui_gui_t;
```

**Widget-IDs** per Hash instead of manual counting

```c
uint32_t ui_hash_str(const char *s);                    // Label-Hash (FNV-1a)
uint32_t ui_hash_indexed(const char *s, int idx);        // für Listen/Loops
```

**Button-Reference implementation:**

```c
bool ui_button(ui_gui_t *gui, const char *label, rect_t rect) {
    uint32_t id = ui_hash_str(label);
    if (ui_context_is_mouse_inside_rect(&gui->input, &rect)) gui->hot_id = id;
    if (gui->hot_id == id && ui_context_is_mouse_pressed(&gui->input, 0))
        gui->active_id = id;
    bool clicked = false;
    if (gui->active_id == id && ui_context_is_mouse_released(&gui->input, 0)) {
        if (gui->hot_id == id) clicked = true;
        gui->active_id = 0;
    }
    draw_button(rect, label, gui->active_id==id, gui->hot_id==id, gui->focused_id==id);
    return clicked;
}
```

Important principle:
The state is owned by the caller not by the widget. The widget function gets a pointer to the user-owned variable and mutates that directly.

**Tab-Navigation:** will be rebuilt every frame from the called widgets. Can never point to a disposed widget.

## Music-specific widgets

**Piano:** Beyond the standard "hot/active" state, it requires minimal internal state (e.g., `piano_last_note`) to correctly translate glissandos (sliding the mouse across multiple keys while holding the button) into Note-On/Note-Off pairs:

```c
typedef struct { uint8_t note; bool note_on, note_off; } ui_piano_event_t;
int ui_piano(ui_gui_t*, rect_t, uint8_t base_note, uint8_t num_keys,
ui_piano_event_t *out_events, int max_events);
```

**Sheet (Step-sequencer grid):** The value is a user-provided `bool grid[rows*cols]`; the widget modifies it directly upon clicking. The playhead position is passed as an explicit parameter (sequencer timing resides in the audio engine, not the UI widget):

```c
bool ui_sheet(ui_gui_t*, rect_t, uint8_t rows, uint8_t cols,
bool *grid, int playhead_col);
```

**Guiding principle:** Musical state (notes, patterns) resides in the audio engine/song model, which often operates on a different timing cycle (PIT interrupt/audio callback vs. render frame). The UI widget sees only a snapshot of this per frame and immediately relays changes back via return values ​​or output parameters.

## License: Nuked-OPL3 (LGPL) in an MIT project

- LGPL is not an issue when using shared objects or open-source distribution.

## AccessKit integration (a11y)

- Official `accesskit-c` repo with a `cbindgen`-generated header, including a ready-made SDL example. - AccessKit is explicitly designed for immediate-mode toolkits: **push-based** (app sends out `TreeUpdate`s) rather than pull-based—fitting the "re-describe every frame" model perfectly.
- Node IDs = the same hash IDs as `hot_id`/`active_id` (`ui_hash_str(label)`); no second ID system required.
- Only the SDL branch is relevant; DOS receives no-op stubs for all `ui_a11y_*` calls (no Rust runtime target for 16/32-bit DOS, and no screen reader ecosystem there).

**Instead of "update only on screen change"** (too coarse-grained—focus changes, live values ​​like BPM/volume, and enabled/disabled status changes require immediate updates even within a single screen): **dirty tracking via content hash** per widget.

```c
typedef struct { uint32_t id; uint32_t content_hash; } ui_a11y_snapshot_t;
// GUI maintains a11y_prev[] (previous frame) and a11y_cur[] (current frame)
```

Each widget calculates a hash based on its accessibility-relevant fields (label, bounds, disabled state, etc.) and compares it with the snapshot from the previous frame. The node is included in the `TreeUpdate` only if a change has occurred. Focus changes are always pushed separately (low cost, involving just a single field). Screen changes are handled automatically (a completely new set of IDs means everything is "dirty") without requiring an explicit signal to be set anywhere.

Additionally, as a safety net: `ui_a11y_force_refresh(gui)` sets `a11y_prev_count = 0`, forcing a complete re-push in the next frame—useful for scenarios like "new song/patch loaded" where you want to be absolutely certain everything is up to date.

**A genuine departure from the pure immediate-mode ideal:**

Screen reader actions (e.g., Voice Control triggering a "click" on Node X) arrive asynchronously via an `ActionHandler` callback, rather than synchronously within the frame. Solution: The callback generates a synthetic event that is fed into `ui_context_input_click()` in the standard way before the next frame begins—the only point where a small intermediate buffer for *external* accessibility triggers is required.
