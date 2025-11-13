#include <stdint.h>

#include "platform.h"

internal void
render_gradient(struct back_buffer *buf, int xOffset, int yOffset)
{
    uint32_t *pixel = buf->memory;
    for(int r = 0; r < buf->height; r++) {
        for(int c = 0; c < buf->width; c++) {
            uint8_t red = (r + yOffset);
            uint8_t green = (c + xOffset);
            uint8_t blue = 0;

            *pixel++ = (red << 16) | (green << 8) | blue;
        }
    }
}

void
update_and_render(struct back_buffer *buf)
{
    static xOffset = 0;
    static yOffset = 0;
    xOffset++;
    yOffset++;
    render_gradient(buf, xOffset, yOffset);
}
