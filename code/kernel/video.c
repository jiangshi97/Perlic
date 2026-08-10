#include "video.h"
#include "mem.h"
#include "lib.h"
#include <stdint.h>


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
}

_OS_API void video_init(framebuffer_inf *fb, boot_color_info *ci)
{
    fb_inf = fb;
    color_inf = ci;
    framebuffer_init((void *)(fb_inf -> framebuffer_addr));
    fb_addr = (uint32_t *)(fb_inf -> framebuffer_addr);

    set_colors();
    for(uint64_t a = 0; a < 100000ULL; a++)
    {
        fb_addr[a] = pixels.purple;
    }

    CheckWithRet();
}