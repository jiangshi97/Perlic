/*This function will use the third page,
 which means the kernel can only use 1 to 4mib memory to store elf section*/

#include "memlib.h"
#include "mem.h"
#include "lib.h"
#include "serial.h"
#include "klib.h"
#include <stdint.h>

#define heap_page_max 30 

MemPage *heapPage[heap_page_max];
uint16_t heap_page_num;


extern MemPages kuPages[max_page_num / 8];

void mem_lib_init()
{
    heap_page_num = os_first_page - 2;
    for(int a = 0; a < heap_page_num; a++)
    {
        heapPage[a] = &(kuPages[(a + 2) / 8].pages[(a + 2)%8]);
        ((MemBlock *)(heapPage[a]->virt_addr))->block_size = 0x200000;
        ((MemBlock *)(heapPage[a]->virt_addr))->pred = null_ptr;
        ((MemBlock *)(heapPage[a]->virt_addr))->succ = null_ptr;
    }

}
void *kmalloc(size_t size)
{
    uint64_t size_a = (uintptr_t)align8(size);
    if(size_a <= 0x200000 - sizeof(MemBlock))
    {
        uint8_t isFound = 0;
        for(uint16_t a = 0; a < heap_page_num; a++)
        {
            MemBlock *now_ptr = (MemBlock *)(heapPage[a] -> virt_addr);
            MemBlock *up_ptr = (MemBlock *)(heapPage[a] -> virt_addr + 0x200000);
            debug_printf("0x%x\n", now_ptr -> block_size);
            while(now_ptr < up_ptr)
            {
                if(((now_ptr -> block_size & 0x1) == 0) && (((now_ptr -> block_size >> 1) << 1) >= (size_a + 2 * sizeof(MemBlock))) )
                {
                    isFound = 1;
                    MemBlock *next = (MemBlock *)(((uint8_t *)now_ptr) + sizeof(MemBlock) + size_a);
                    next -> succ = now_ptr -> succ;
                    now_ptr -> succ = next;
                    next -> pred = now_ptr;
                    next -> block_size = now_ptr -> block_size - (sizeof(MemBlock) + size_a);
                    now_ptr -> block_size = sizeof(MemBlock) + size_a + 1;
                    break;
                }
                else if(((now_ptr -> block_size & 0x1) == 0) && (((now_ptr -> block_size >> 1) << 1) >= (size_a + sizeof(MemBlock))))
                {
                    isFound = 1;
                    now_ptr -> block_size += 1;
                    break;
                }
                else if(now_ptr -> succ == null_ptr)
                {
                    break;
                }
                else
                {
                    now_ptr = now_ptr -> succ;
                }
            }
            if(isFound == 1)
            {
                ERR_CODE[get_pid()] = OS_SUCCES;
                return now_ptr -> mem;
            }
        }
        ERR_CODE[get_pid()] = OS_FAILED;
        return null_ptr;
    }
    else
    {
        debug_printf("Failed to allocate memory > 2mib\n")
        ERR_CODE[get_pid()] = OS_FAILED;
        return null_ptr;
    }
};
void kfree(void *ptr)
{
    if(ptr <= sizeof(MemPage))
    {
        ERR_CODE[get_pid()] = OS_FAILED;
        return;
    }
    MemBlock *now = ptr - sizeof(MemPage);
    now -> block_size = now -> block_size & 0xFFFFFFFFFFFFFFFEULL;
    if(now -> pred == null_ptr)
    {
        if(now -> succ != null_ptr)
        {
            if(is_free(now -> succ -> block_size))
            {
                now -> block_size = now -> block_size + now -> succ -> block_size;
                now -> succ = now -> succ ->succ;
            }
        }
    }
    else
    {
        if(now -> succ == null_ptr)
        {
            
            if(is_free(now -> pred -> block_size))
            {
                now -> pred -> block_size = now -> block_size + now -> pred -> block_size;
                now -> pred -> succ = null_ptr;
            }
        }
        else
        {
            if(is_free(now -> pred -> block_size))
            {
                if(is_free(now -> succ -> block_size))
                {
                    now -> pred -> block_size = now -> block_size + now -> pred -> block_size + now -> succ -> block_size;
                    now -> pred -> succ = now -> succ ->succ;
                    if(now -> succ -> succ != null_ptr)
                    {
                        now -> succ -> succ ->pred = now -> pred;
                    }
                }
                else
                {
                    
                    now -> pred -> block_size = now -> block_size + now -> pred -> block_size;
                    now -> pred -> succ = now -> succ;
                    now -> succ -> pred = now -> pred;
                }
            }
            else
            {
                if(is_free(now -> succ -> block_size))
                {
                    now -> block_size = now -> block_size + now -> succ -> block_size;
                    now -> succ = now -> succ -> succ;
                }
            }
        }
    }
    
};