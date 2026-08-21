#include "lib.h"
#include "driver.h"
#include "driver_cpp.h"
#include "pic.h"
#include <stdint.h>

void pic_init()
{
    //disable interuption
    outb(PIC0_IMR, 0xff);
    outb(PIC1_IMR, 0xff);

    //init pic0
    outb(PIC0_ICW1, 0x11);
    outb(PIC0_ICW2, IRQ_base);
    outb(PIC0_ICW3, 1 << 2);
    outb(PIC0_ICW4, 0x01);

    //init pic1
    outb(PIC1_ICW1, 0x11);
    outb(PIC1_ICW2, IRQ_base + 8);
    outb(PIC1_ICW3, 2);
    outb(PIC1_ICW4, 0x01);

    outb(PIC0_IMR, 0xfb);
    outb(PIC1_IMR, 0xff);

    enable_int();

    debug_printf("8259 OK\n");
}

void driver_init()
{
    pic_init();
    keyboader_init();
    
    enable_int();
    debug_printf("\ndriver do not ok\n");
}