#include "geometry.h"
#include <stdint.h>
#include <stdbool.h>

bool rect_test_point_in_bounds(const rect_t * const rect, const uint8_t x, const uint8_t y)
{
	uint8_t x0 = rect->x;
	uint8_t x1 = x0 + rect->width - 1;
	uint8_t y0 = rect->y;
	uint8_t y1 = y0 + rect->height - 1;
	return (
		x >= x0 && x <= x1 && y >= y0 && y <= y1
	);
}

bool rect_test_mouse(const rect_t * const rect, const uint16_t mouseX, const uint16_t mouseY)
{
	return rect_test_point_in_bounds(rect, (uint8_t)(mouseX>>3), (uint8_t)(mouseY>>3));
}

rect_t get_clientrect(const rect_t * const bounding_rect)
{
	rect_t clientrect = *bounding_rect;
	if (clientrect.height > 2 && clientrect.width > 2)
	{
		clientrect.x += 1;
		clientrect.y += 1;
		clientrect.width -= 2;
		clientrect.height -= 2;
	}
	return clientrect;
}
