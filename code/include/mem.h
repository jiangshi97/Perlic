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
#define first_page 13

#define page_free 0
#define page_allocated 1
#define page_applied 2

#define zone_free 0
#define zone_reserved 1
#define zone_acpi 2
#define zone_hib 3
#define zone_def 4

#define os_first_page 5

typedef struct
{
    uintptr_t phy_addr;
    uint64_t attribute;
    uint64_t ref_count;
} pMemPage;

typedef struct
{
    uintptr_t virt_addr;
    uint64_t attribute;
    pMemPage *ptr_page;
} MemPage;

typedef struct
{
    MemPage pages[8];
    uint64_t bits_map;  //00:not on page_table 01:not allocated 02:os 03:application
} MemPages;


typedef struct 
{
    uintptr_t zone_addr;
    uint64_t page_num;
    uint64_t used_page_num;
    uint64_t attribute;
    pMemPage *pages;
} MemZone;

void mem_init(uint64_t mem_info_size, MemInf mem_inf[]);

void framebuffer_init(void *frame_addr);