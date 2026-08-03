/*
    Due to the limit, this way can only support up to 16 Gib memory(8192 pages)
    It will be fixed in the future

    Page 1 is special due to its structure(complex 0 to 1mb space),
    which must be specially dealt
*/

#pragma once

#include "bootc.h"
#include <stdint.h>

#define max_page_num (16 * 1024 / 2)

typedef struct
{
    uintptr_t phy_addr;
    uintptr_t virt_addr;
    uint64_t attribute;
    uint64_t ref_count;
} MemPage;

typedef struct
{
    MemPage pages[8];
    uint64_t bits_map;
} MemPages;


typedef struct 
{
    uintptr_t zone_addr;
    uint64_t page_num;
    uint64_t used_page_num;
    MemPages *pages;
} MemZone;

void mem_init(uint64_t mem_info_size, MemInf mem_inf[]);

void framebuffer_init(void *frame_addr);