#pragma once

#include "lib.h"
#include <stdint.h>

/*Ideally,slab memory pool is the best choice,however......*/
typedef struct MemBlock MemBlock;
struct MemBlock
{
    uint32_t block_size;
    MemBlock *pred;
    MemBlock *succ;
    uint8_t mem[0];
};


void mem_lib_init();
void *kmalloc(size_t size);
void kfree(void *ptr);