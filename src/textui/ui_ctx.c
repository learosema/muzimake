#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdint.h>
#include "ui_ctx.h"
#include "ui_common.h"

void ui_context_init(ui_context_t * const ctx)
{
	memset(ctx, 0, sizeof(ui_context_t));
}

void ui_context_dispose(ui_context_t * const ctx)
{
	
}

void ui_context_begin_frame(ui_context_t * const ctx)
{

	(void)ctx;
}