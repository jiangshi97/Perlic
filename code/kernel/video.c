#include "video.h"
#include "mem.h"
#include "lib.h"
#include <stdint.h>

#define xy_check_error(x,y) ((x > (fb_inf -> framebuffer_width)) || (y > (fb_inf -> framebuffer_height)))

framebuffer_inf *fb_inf;
boot_color_info *color_inf;

color_pixels pixels;
uint32_t *fb_addr;

void set_colors()
{
    pixels.blue = RGB_pixel(color_inf, 0, 0, 255);
    pixels.brown = RGB_pixel(color_inf, 153, 76, 96);
    pixels.green = RGB_pixel(color_inf, 0, 255, 0);
    pixels.grey = RGB_pixel(color_inf, 96, 96, 96);
    pixels.orange = RGB_pixel(color_inf, 255, 128, 255);
    pixels.pink = RGB_pixel(color_inf, 255, 0, 127);
    pixels.purple = RGB_pixel(color_inf, 153, 0, 153);
    pixels.red = RGB_pixel(color_inf, 255, 0, 0);
    pixels.yellow = RGB_pixel(color_inf, 255, 255, 0);
    pixels.white = RGB_pixel(color_inf, 255, 255, 255);
    pixels.black = RGB_pixel(color_inf, 0, 0, 0);
}

static inline void do_drawPoint(uint32_t x, uint32_t y, uint32_t pixel)
{
    fb_addr[x + y * ((fb_inf -> framebuffer_pitch) / 4)] = pixel;
}

_OS_API void drawPoint(uint32_t x, uint32_t y, uint32_t pixel)
{
    if(xy_check_error(x, y))
    {
        RetErr();
    }
    do_drawPoint(x, y, pixel);
    RetSucces();
}

_OS_API void drawRect(uint32_t x1, uint32_t y1, uint32_t x2, uint32_t y2, uint32_t pixel)
{
    if(xy_check_error(x1, y1) || xy_check_error(x2, y2) || (x2 < x1) || (y2 < y1))
    {
        RetErr();
    }
    for(uint32_t x = x1; x <= x2; x++)
    {
        for(uint32_t y = y1; y <= y2; y++)
        {
            do_drawPoint(x, y, pixel);
        }
    }
    RetSucces();
}

_OS_API void video_init(framebuffer_inf *fb, boot_color_info *ci)
{
    fb_inf = fb;
    color_inf = ci;
    framebuffer_init((void *)(fb_inf -> framebuffer_addr));
    framebuffer_init((void *)(fb_inf -> framebuffer_addr + page_size));
    fb_addr = (uint32_t *)(fb_inf -> framebuffer_addr);

    set_colors();

    CheckWithRet();
}