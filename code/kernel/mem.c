#include <stdint.h>
#include "bootc.h"
#include "mem.h"
#include "serial.h"
#include "lib.h"
#include "memlib.h"

//This is physical pages
pMemPage MainPages[max_page_num];
pMemPage OtherPages[max_mem_info - 1][max_page_num / 8];
uint8_t zone_num = 0;
MemZone zones[max_mem_info];

//This is kernel virtual pages
MemPages kdPages[max_page_num / 8];
MemPages kuPages[max_page_num / 8];

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
                zones[0].page_num = min_num((mem_inf[a].base_addr + mem_inf[a].length) / 0x200000 , max_page_num);
                zones[0].used_page_num = 13;
                zones[0].attribute = zone_free;
                for(uint64_t b = 0; b < zones[0].page_num; b++)
                {
                    MainPages[b].phy_addr = b * 0x200000;
                }
                for(uint64_t b = 0; b < 13; b++)
                {
                    MainPages[b].ref_count = 2;
                }
                for(uint64_t b = 13; b < zones[0].page_num; b++)
                {
                    MainPages[b].ref_count = 0;
                }
                
                for(uint64_t b = 0; b < os_first_page; b++)
                {
                    kdPages[b / 8].pages[b % 8].ptr_page = &(MainPages[b]);
                    kdPages[b / 8].pages[b % 8].virt_addr = MainPages[b].phy_addr;
                    kdPages[b / 8].pages[b % 8].attribute = page_allocated;
                    kuPages[b / 8].pages[b % 8].ptr_page = &(MainPages[b]);
                    kuPages[b / 8].pages[b % 8].virt_addr = MainPages[b].phy_addr + 0xffff800000000000;
                    kuPages[b / 8].pages[b % 8].attribute = page_allocated;
                }
                kdPages[0].bits_map = 0x0202020202010101ULL;
                kdPages[1].bits_map = 0x0101010101000000ULL;
                kuPages[0].bits_map = 0x0202020202010101ULL;
                kuPages[1].bits_map = 0x0101010101000000ULL;
                for(uint64_t b = os_first_page; b < max_page_num; b++)
                {
                    kdPages[b / 8].pages[b % 8].attribute = page_free;
                    kuPages[b / 8].pages[b % 8].attribute = page_free;
                    kdPages[b / 8].bits_map = 0;
                    kuPages[b / 8].bits_map = 0;
                }
            }
        }
        else
        {
            zones[zone_num].zone_addr = mem_inf[a].base_addr;
            zones[zone_num].page_num = min_num((mem_inf[a].length) / 0x200000 , max_page_num);
            zones[zone_num].used_page_num = 0;
            zones[zone_num].pages = OtherPages[zone_num - 1];
            switch (mem_inf[a].type)
            {
                case 1:
                    zones[zone_num].attribute = zone_free;
                    break;
                case 3:
                    zones[zone_num].attribute = zone_acpi;
                    break;
                case 4:
                    zones[zone_num].attribute = zone_hib;
                    break;
                case 5:
                    zones[zone_num].attribute = zone_def;
                    break;
                default:
                    zones[zone_num].attribute = zone_reserved;
                    break;
            }
            for(uint64_t b = 0; b < zones[zone_num].page_num; b++)
            {
                OtherPages[zone_num - 1][b].attribute = page_free;
                OtherPages[zone_num - 1][b].phy_addr = mem_inf[a].base_addr + 0x200000 * b;
                OtherPages[zone_num - 1][b].ref_count = 0;
            }
            zone_num++;
        }
    }
    if(is_bigger_1mb == 0)
    {
        debug_printf("Your memory is smaller than 1mib\n");
        hlt_loop();
    }
    mem_lib_init();
}

void framebuffer_init(void *frame_addr)
{

}

