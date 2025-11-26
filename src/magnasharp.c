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

// Maybe don't use this method?
void
app_init(void)
{
    // Load Font
}

void
app_update_and_render(struct back_buffer *buf,
                      const struct key_events *input)
{
    static int xOffset = 0;
    static int yOffset = 0;
    for(int i = 0; i < input->len; i++) {
        switch(input->codes[i]) {
        case KEY_A:
            xOffset--;
            break;
        case KEY_D:
            xOffset++;
            break;
        case KEY_W:
            yOffset--;
            break;
        case KEY_S:
            yOffset++;
            break;
        default:
            break;
        }
    }
    render_gradient(buf, xOffset, yOffset);
}
