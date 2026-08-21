#pragma once

#include "lib.h"
#include <stdint.h>

#define is_free(size) (((uint64_t)(size) & 0x1ULL )== 0ULL)
#define free_to_use(num_ptr) do{(*(num_ptr)) = (*(num_ptr)) | 0x1;    }while(0)
#define use_to_free(num_ptr) do{(*(num_ptr)) = (*(num_ptr)) & (0xFFFFFFFFFFFFFFFEULL);    }while(0)

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
_OS_API void *kmallocWithAlign(size_t size, uint8_t align_num);
_OS_API void *kmalloc_init(size_t size);
_OS_API void *kmallocWithAlign_init(size_t size, uint8_t align_num);