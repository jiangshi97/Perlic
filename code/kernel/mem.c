#include <stdint.h>
#include "bootc.h"
#include "mem.h"
#include "serial.h"
#include "lib.h"
#include "memlib.h"
#include "klib.h"


//This is physical pages
pMemPage MainPages[max_page_num];
pMemPage OtherPages[max_mem_info - 1][max_page_num / 8];
uint8_t zone_num = 0;
MemZone zones[max_mem_info];

//This is kernel virtual pages
MemPages kdPages[max_page_num / 8];
MemPages kuPages[max_page_num / 8];

_OS_API pMemPage *FindpPage(void *addr);
_OS_API MemPage *FindPage(void *addr);

extern uint64_t _PDPTE[];
extern uint64_t _PDE[];

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
                    MainPages[b].father = &(zones[0]);
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
                    kdPages[b / 8].pages[b % 8].father = &(kdPages[b / 8]);
                    kuPages[b / 8].pages[b % 8].ptr_page = &(MainPages[b]);
                    kuPages[b / 8].pages[b % 8].virt_addr = MainPages[b].phy_addr + 0xffff800000000000ULL;
                    kuPages[b / 8].pages[b % 8].attribute = page_allocated;
                    kuPages[b / 8].pages[b % 8].father = &(kuPages[b / 8]);
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
                    kdPages[b / 8].pages[b % 8].father = &(kdPages[b / 8]);
                    kuPages[b / 8].pages[b % 8].father = &(kuPages[b / 8]);
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
                OtherPages[zone_num - 1][b].father = &(zones[zone_num]);
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

_OS_API void link_pagetab(void *vir_addr, void *phy_addr, uint8_t pcid)
{
    if(((((uintptr_t)vir_addr) % page_size) != 0) || ((((uintptr_t)phy_addr) % page_size) != 0))
    {
        RetErr();
    }
    uint64_t pml4, pdpt, pdt, index;
    pml4 = (((uintptr_t)vir_addr) & 0xFF8000000000ULL) >> 39;
    pdpt = (((uintptr_t)vir_addr) & 0x7FC0000000ULL) >> 30;
    pdt = (((uintptr_t)vir_addr) & 0x3FE00000ULL) >> 21;
    index = (((uintptr_t)vir_addr) & 0x1FFFFFULL);
    //debug_printf("pml4:0x%x pdpt:0x%x, pdt:0x%x, index:0x%x\n", pml4, pdpt, pdt, index);
    uint64_t *PDPTE = (uint64_t *)(PhyToVir((_PML4E[pml4] & 0xFFFFFFFFFFFFF000)));
    uint64_t *PDE;
    //debug_printf("_PML4E:0x%x\n", _PML4E);
    //debug_printf("_PML4E[0]:0x%x\n", _PML4E[0]);
    //debug_printf("_PDPTE:0x%x\n", _PDPTE);
    //debug_printf("_PDPTE[0]:0x%x\n", _PDPTE[0]);
    //debug_printf("_PDE[0]:0x%x\n", _PDE[0]);
    //debug_printf("_PDE[1]:0x%x\n", _PDE[1]);
    if(PDPTE == PhyToVir(null_ptr))
    {
        //debug_printf("a\n");
        PDPTE = kmallocWithAlign_init(2 * page_tab_size, page_tab_align_num);
        CheckWithRet();
        PDE = (uint64_t *)((uintptr_t)PDPTE + page_tab_size);
        _PML4E[pml4] = (uintptr_t)VirToPhy(PDPTE) | 0x27;
        PDPTE[pdpt] = (uintptr_t)VirToPhy(PDE) | 0x23;
        PDE[pdt] = (uintptr_t)phy_addr | pcid;
    }
    else
    {
        //debug_printf("b\n");
        //debug_printf("PDPTE: 0x%x\n", PDPTE);
        PDE = (uint64_t *)(PhyToVir((PDPTE[pdpt] & 0xFFFFFFFFFFFFF000)));
        if(PDE == PhyToVir(null_ptr))
        {
            //debug_printf("c\n");
            PDE = kmallocWithAlign_init(page_tab_size, page_tab_align_num);
            //debug_printf("PDE:0x%lx\n", PDE);
            CheckWithRet();
            PDPTE[pdpt] = (uintptr_t)VirToPhy(PDE) | 0x23;
        }
        PDE[pdt] = (uintptr_t)phy_addr | pcid;
        //debug_printf("hello\n0x%x\n0x%x\n", PDE[pdt - 1], PDE[pdt]);
    }
    //debug_printf("\n_PML4E[pml4]:0x%lx\nPDPTE[pdpt]:0x%lx\nPDE[pdt]:0x%lx\n", _PML4E[pml4], PDPTE[pdpt], PDE[pdt]);
    RetSucces();
}

void framebuffer_init(void *frame_addr)
{
    pMemPage *destp = FindpPage(VirToPhy(frame_addr));
    if(ERR_CODE[get_pid()] != OS_SUCCES)
    {
        destp = kmalloc(sizeof(pMemPage));
        CheckWithRet();
        destp -> father = &(zones[zone_num]);
        destp -> phy_addr = (((uintptr_t)frame_addr / page_size) * page_size);
        destp -> ref_count = 0;
        zones[zone_num].attribute = zone_reserved;
        zones[zone_num].page_num = 1;
        zones[zone_num].zone_addr = destp -> phy_addr;
        zones[zone_num].used_page_num = 1;
        zones[zone_num].pages = destp;
        zone_num += 1;
    }
    MemPage *destpage = FindPage(frame_addr);
    CheckWithText("frame_addr isn't a valid address\n");
    destpage -> attribute = page_allocated;
    destpage -> ptr_page = destp;
    destp -> ref_count += 1;
    link_pagetab(frame_addr, VirToPhy(frame_addr), os_rw_pcid);
    CheckWithText("Failed to link framebuffer page\n");
}

_OS_API pMemPage *FindpPage(void *addr)
{
    uint64_t page_num;
    for(uint8_t a = 0; a < zone_num; a++)
    {
        if(((uintptr_t)addr) >= zones[a].zone_addr)
        {
            page_num = (((uintptr_t)addr) - zones[a].zone_addr) / page_size;
            if((page_num + 1) <= zones[a].page_num)
            {
                ERR_CODE[get_pid()] = OS_SUCCES;
                return &(zones[a].pages[page_num]);
            }
        }
    }
    ERR_CODE[get_pid()] = OS_FAILED;
    return null_ptr;
}

_OS_API MemPage *FindPage(void *addr)
{
    uintptr_t addr_ = (uintptr_t)addr;
    MemPages *dest_pages;
    uint64_t page_num;
    if(addr_ >= half_mem)
    {
        dest_pages = kuPages;
        addr_ = (uintptr_t)VirToPhy(addr_);
    }
    else
    {
        dest_pages = kdPages;
    }
    page_num = (addr_ / page_size);
    if(page_num > max_page_num)
    {
        RetErr(null_ptr);
    }
    RetSucces(&(dest_pages[page_num / 8] . pages[page_num % 8]));
}
/*void AllocatePage(MemPage *virPage, uint8_t memType)    //type:00 os; 01:application
{
    if((virPage == null_ptr) || (memType >= 2))
    {
        ERR_CODE[get_pid()] = OS_FAILED;
        return;
    }
    uint8_t num = ((uintptr_t)virPage - (uintptr_t)(virPage -> father -> pages)) / sizeof(MemPage);
    pMemPage *destPage;
    if((get_pid() == kernel_pid))
    {
        if((virPage -> virt_addr) >= 0xffff800000000000)
        {
            virPage -> attribute = page_allocated;
            destPage = FindpPage(VirToPhy(virPage -> virt_addr));
            CheckWithRet();
            virPage -> ptr_page = destPage;
            
            
        }
        else
        {
            
        }
    }
    else
    {   //todo
        if((virPage -> virt_addr) >= 0xffff800000000000)
        {

        }
        else
        {
            
        }
    }
}*/
