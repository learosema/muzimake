#include <stdio.h>

#include <events.h>

/* === Beispiel-Event-Handler === */

int MyEventHandler(const EventParam_t* const event) {
	printf("MyEventHandler aufgerufen: EventID = %u, Prio = %d\n", event->eventId, event->priority);
	return 0;
}

/* === Hauptprogramm === */

int main() {
	EventManager_t manager;
	InitEventManager(&manager);

	// Registrierung eines Handlers für Event-ID 1001
	RegisterListener(&manager, MyEventHandler, 1001);

	// Simuliere schnelles Eintreffen mehrerer Events mit verschiedenen Prioritäten
	for (unsigned int i = 0; i < 5; i++) {
			EventParam_t ev;
			ev.eventId = 1001;
			ev.wParam = NULL;
			ev.lParam = NULL;
			ev.priority = (i == 0) ? 1 : (i + 1); // Das erste Event erhält höchste Priorität (1); danach höhere Werte.
			PostMessage(&manager, &ev);

	}

	// Verarbeite die gepufferten Events an definierten Stellen im Programmablauf
	ProcessMessageQueue(&manager);

	// Deregistrierung des Handlers und Aufräumen
	UnregisterListener(&manager, MyEventHandler);
	DestroyEventManager(&manager);

	return 0;
}
