#pragma once

#include "lib.h"
#include <stdint.h>

#define RGB_pixel(color_inf, r, g, b) (((r << (color_inf ->framebuffer_red_field_position))   \
                        | (b << (color_inf ->framebuffer_blue_field_position))    \
                        | (g << (color_inf ->framebuffer_green_field_position))))

typedef union
{
    struct 
    {
        uint8_t framebuffer_red_field_position;
        uint8_t framebuffer_red_mask_size;
        uint8_t framebuffer_green_field_position;
        uint8_t framebuffer_green_mask_size;
        uint8_t framebuffer_blue_field_position;
        uint8_t framebuffer_blue_mask_size;
    };
} boot_color_info;

typedef struct
{
    uintptr_t framebuffer_addr;
    uint32_t framebuffer_pitch;
    uint32_t framebuffer_width;
    uint32_t framebuffer_height;
    uint8_t framebuffer_bpp;
    uint8_t framebuffer_type;
} framebuffer_inf;

typedef struct
{
    uint8_t red;
    uint8_t green;
    uint8_t blue;
} RGB_color;

typedef struct
{
    uint32_t red;
    uint32_t green;
    uint32_t blue;
    uint32_t yellow;
    uint32_t brown;
    uint32_t pink;
    uint32_t purple;
    uint32_t grey;
    uint32_t orange;
    uint32_t white;
    uint32_t black;
} color_pixels;

_OS_API void video_init(framebuffer_inf *fb, boot_color_info *ci);
_OS_API void drawPoint(uint32_t x, uint32_t y, uint32_t pixel);
_OS_API void drawRect(uint32_t x1, uint32_t y1, uint32_t x2, uint32_t y2, uint32_t pixel);

extern color_pixels pixels;
extern framebuffer_inf *fb_inf;
