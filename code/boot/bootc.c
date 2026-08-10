#include <stdint.h>
#include "serial.h"
#include "bootc.h"
#include "sprint.h"
#include "lib.h"
#include "trap.h"
#include "test.h"
#include "gate.h"
#include "mem.h"
#include "video.h"
#include "memlib.h"

#define KerSucces 0
#define LoadErr 1
#define ColorErr 2
extern uint32_t eax_magicnum;
extern uint32_t ebx_bootinf;

void KerErr(uint16_t errcode);
void ker_panic();

_OS_API void link_pagetab(void *vir_addr, void *phy_addr, uint8_t pcid);

extern void *_start;
extern void *prt; 
uint16_t setBootinfs(volatile uint8_t *bootinf);

BootInf bootinfs;

void prttt(void)
{
    serial_printf("WAN DAN LA!!!\n");
}
void IntInit()
{
    #define _stack_start 0xffff800000007e00
    set_tss_to_gdt(GDT_Table, TSS64_Table, 10, 26 * sizeof(unsigned int));
    load_TR(10);
    set_tss64(_stack_start, _stack_start, _stack_start, 0xffff800000007c00, 0xffff800000007c00, 0xffff800000007c00, 0xffff800000007c00, 0xffff800000007c00, 0xffff800000007c00, 0xffff800000007c00);

    sys_vector_init();
}
void KernelMain(void)
{
    uint16_t Status = 0;
    uint32_t *frame_addr = 0;
    if(eax_magicnum != 0x36d76289)
    {
        KerErr(LoadErr);
    }

    serial_init();
    serial_printf("ebx_bootinf:0x%x\n",ebx_bootinf);
    prt = prttt;
    
    uintptr_t bootinfp = (uintptr_t) ebx_bootinf;
    Status = setBootinfs((uint8_t *)bootinfp);
    if(Status != KerSucces)
    {
        KerErr(Status);
    }
    IntInit();
    mem_init(bootinfs.mem_info_size, bootinfs.mem_info);
    video_init(&(bootinfs.fb_inf), &(bootinfs.color_info));

    debug_printf("Kernel process sleep\n");

    while(1){asm_hlt();}
}

uint16_t setBootinfs(volatile uint8_t *bootinf)
{
    uint32_t total_size = *(uint32_t *)bootinf;
    uint8_t isEnd = 0;
    uint32_t type = 0;
    uint32_t size = 0;
    if(total_size <= 16)
    {
        return LoadErr;
    }
    bootinf += 8;
    serial_printf("Start read bootinfo\n");
    while(isEnd == 0)
    {
        type = *(uint32_t *)bootinf;
        size = *(uint32_t *)(bootinf + 4);
        serial_printf("type: %d size: %d\n",type,size);
        switch(type)
        {
            case 0:
                isEnd = 1;
                break;
            case 4: //Basic memory information
                bootinfs.mem_upper = *(uint32_t *)(bootinf + 12);
                //serial_printf("mem_upper:0x%x\n",bootinfs.mem_upper);
                break;
            case 8:
                bootinfs.fb_inf.framebuffer_addr = *(uintptr_t *)(bootinf + 8) + 0xffff800000000000;
                bootinfs.fb_inf.framebuffer_pitch = *(uint32_t *)(bootinf + 16);
                bootinfs.fb_inf.framebuffer_width = *(uint32_t *)(bootinf + 20);
                bootinfs.fb_inf.framebuffer_height = *(uint32_t *)(bootinf + 24);
                bootinfs.fb_inf.framebuffer_bpp = *(uint8_t *)(bootinf + 28);
                bootinfs.fb_inf.framebuffer_type = *(uint8_t *)(bootinf + 29);
                serial_printf("framebuffer_addr: 0x%x\n", bootinfs.fb_inf.framebuffer_addr);
                //serial_printf("framebuffer_bpp: %d\n", bootinfs.framebuffer_bpp);
                if(bootinfs.fb_inf.framebuffer_type != 1)
                {
                    return ColorErr;
                }
                if(bootinfs.fb_inf.framebuffer_type == 1)
                {   //This part is against the Multiboot2 Specificatio??????
                    bootinfs.color_info.framebuffer_red_field_position = *(uint8_t *)(bootinf + 32);
                    bootinfs.color_info.framebuffer_red_mask_size = *(uint8_t *)(bootinf + 33);
                    bootinfs.color_info.framebuffer_green_field_position = *(uint8_t *)(bootinf + 34);
                    bootinfs.color_info.framebuffer_green_mask_size = *(uint8_t *)(bootinf + 35);
                    bootinfs.color_info.framebuffer_blue_field_position = *(uint8_t *)(bootinf + 36);
                    bootinfs.color_info.framebuffer_blue_mask_size = *(uint8_t *)(bootinf + 37);
                }
                break;
            case 6:
                uint32_t entry_size = *(uint32_t *)(bootinf + 8);
                if(entry_size > 0 && entry_size <= max_mem_info)
                {
                    uint32_t entry_num = (size - 16) / entry_size;
                    bootinfs.mem_info_size = entry_num;
                    for(uint64_t a = 0; a < entry_num; a++)
                    {
                        bootinfs.mem_info[a].base_addr = *(uint64_t *)(bootinf + 16 + 24 * a);
                        bootinfs.mem_info[a].length = *(uint64_t *)(bootinf + 24 + 24 * a);
                        bootinfs.mem_info[a].type = *(uint32_t *)(bootinf + 32 + 24 * a);
                        serial_printf("mem[%d]:base_addr:0x%x length:0x%x type:%d\n", a, bootinfs.mem_info[a].base_addr, bootinfs.mem_info[a].length, bootinfs.mem_info[a].type);
                    }
                }
                else
                {
                    serial_printf("ERROR!:No valid memory information\n");
                    hlt_loop();
                }
            default:
                break;

        }
        bootinf += size;
        bootinf = align8(bootinf);

    }
    return KerSucces;
}

void KerErr(uint16_t errcode)
{
    ker_panic();    //temporary deal
    while(1)
    {
        asm("hlt \n\t");
    }
}

void ker_panic()
{
    struct {
    uint16_t limit;
    uint64_t base;
    } __attribute__((packed)) bad_idt = { 0x100, 0xFFFFFFFF00000000 };

// 加载无效IDT
    __asm__ volatile("lidt %0" : : "m"(bad_idt));

// 触发任意软中断，CPU 查找 IDT 时找不到入口，立即 Triple Fault -> 机器重启或死机
    __asm__ volatile("int $0x80");
}

