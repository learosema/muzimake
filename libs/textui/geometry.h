#ifndef __GEOMETRY_H__
#define __GEOMETRY_H__

#ifdef __cplusplus
extern "C" {
#endif

typedef struct rect_s
{
	int x;
	int y;
	int width;
	int height;
} rect_t;

bool rect_test_point_in_bounds(const rect_t * const rect, const uint8_t x, const uint8_t y);
bool rect_test_mouse(const rect_t * const rect, const uint16_t mouseX, const uint16_t mouseY);
rect_t get_clientrect(const rect_t * const bounding_rect);


#ifdef __cplusplus
}
#endif
#endif
