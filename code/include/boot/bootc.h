#pragma once

#include <stdint.h>
#include "video.h"
#define max_mem_info 50


typedef struct
{
    uint64_t base_addr;
    uint64_t length;
    uint32_t type;
} MemInf;

typedef struct
{
    uint32_t mem_upper;

    framebuffer_inf fb_inf;

    boot_color_info color_info;

    uint64_t mem_info_size;
    MemInf mem_info[max_mem_info];
} BootInf;