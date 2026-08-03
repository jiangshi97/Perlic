#pragma once

#include <stdint.h>
#define max_mem_info 50

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
    uint64_t base_addr;
    uint64_t length;
    uint32_t type;
} MemInf;

typedef struct
{
    uint32_t mem_upper;

    uintptr_t framebuffer_addr;
    uint32_t framebuffer_pitch;
    uint32_t framebuffer_width;
    uint32_t framebuffer_height;
    uint8_t framebuffer_bpp;
    uint8_t framebuffer_type;

    boot_color_info color_info;

    uint64_t mem_info_size;
    MemInf mem_info[max_mem_info];
} BootInf;