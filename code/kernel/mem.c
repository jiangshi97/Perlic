#include <stdint.h>
#include "bootc.h"
#include "mem.h"
#include "serial.h"
#include "lib.h"

MemPages MainPages[max_page_num / 8];
MemPages OtherPages[max_mem_info - 1][max_page_num / 64];
uint8_t zone_num = 0;
MemZone zones[max_mem_info];

void mem_init(uint64_t mem_info_size, MemInf mem_inf[])
{
    uint8_t is_bigger_1mb = 0;
    for(uint64_t a = 0; a < mem_info_size; a++)
    {
        if(is_bigger_1mb == 0)
        {
            if(mem_inf[a].base_addr >= 0x100000)
            {
                is_bigger_1mb = 1;
                zone_num = 1;
                zones[0].zone_addr = 0x0;
                zones[0].pages = MainPages;
                zones[0].page_num = (mem_inf[a].base_addr + mem_inf[a].length) / 0x200000;
                zones[0].used_page_num = 13;
            }
        }
        else
        {

        }
    }
    if(is_bigger_1mb == 0)
    {
        debug_printf("Your memory is smaller than 1mib\n");
        hlt_loop();
    }
}

void framebuffer_init(void *frame_addr)
{

}