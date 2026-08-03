#pragma once

#include "lib.h"
#include <stdint.h>

#define is_free(size) (((uint64_t)size & 0x1ULL )== 0ULL)

/*Ideally,slab memory pool is the best choice,however......*/
typedef struct MemBlock MemBlock;
struct MemBlock
{
    uint64_t block_size;    //the end is 0 when free
    MemBlock *pred;
    MemBlock *succ;
    uint8_t mem[0];
};


void mem_lib_init();
void *kmalloc(size_t size);
void kfree(void *ptr);