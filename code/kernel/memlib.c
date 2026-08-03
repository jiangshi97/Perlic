/*This function will use the third page,
 which means the kernel can only use 1 to 4mib memory to store elf section*/

#include "memlib.h"
#include "mem.h"
#include "lib.h"
#include "serial.h"
#include <stdint.h>

#define heap_page_max 30 

MemPage *heapPage[heap_page_max];
uint16_t heap_page_num;

void mem_lib_init()
{
    heap_page_num = ;
}
void *kmalloc(size_t size)
{
    if(size <= 0x200000)
    {

    }
    else
    {
        debug_printf("Failed to allocate memory > 2mib\n");
        ERR_CODE[get_pid()] = OS_FAILED;
        return 0x0;
    }
};
void kfree(void *ptr)
{

};