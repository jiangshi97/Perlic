#include "lib.h"
#include "stdint.h"
#include "pic.h"

void send_eoi(uint8_t irq)
{
    if (irq >= 8)
    {
        outb(PIC1_ICW1, PIC_EOI);
    }
    outb(PIC0_ICW1, PIC_EOI);
}

void enable_irq(uint8_t irq)
{
    uint16_t port = (irq < 8) ? PIC0_ICW2 : PIC1_ICW2;
    uint8_t mask = inb(port) & ~(1 << (irq & 7));
    outb(port, mask);
}

void disable_irq(uint8_t irq)
{
    uint16_t port = (irq < 8) ? PIC0_ICW2 : PIC1_ICW2;
    uint8_t mask = inb(port) | (1 << (irq & 7));
    outb(port, mask);
}