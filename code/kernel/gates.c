#include "gate.h"
#include <stdint.h>
IDT nIDT;
void set_tss_to_gdt(uint64_t *gdt_tab, void *tss_addr, uint64_t num, uint64_t length)
{
    uint64_t _low, _high;
    uintptr_t addr = (uintptr_t)tss_addr;
    _high = addr >> 32;
    _low = (length & 0xFFFFULL) |
            ((addr & 0xFFFFULL) << 16) |
            ((addr & 0xFF0000ULL) << 16) |
            ((uint64_t)137 << 40) |
            ((length & 0xF0000ULL) << 32) |
            ((addr & 0xFF000000ULL) << 32);
    gdt_tab[num] = _low;
    gdt_tab[num + 1] = _high;
}

void _set_gate(gate_struct *gate, uint64_t attr, uint64_t ist, void *addr)
{
    uintptr_t __addr = (uintptr_t)(addr);                             
	uint64_t __low = (__addr & 0xFFFFULL)								
					| (0x08 << 16)
					| (ist << 32)
					| (attr << 40)
					| ((__addr & 0xFFFF0000ULL) << 32);												
    uint64_t __high = (__addr >> 32);                                  
    ((uint64_t *)(gate))[0] = __low;                                  
    ((uint64_t *)(gate))[1] = __high;     
}

void set_intr_gate(uint32_t n, uint8_t ist, void *addr)
{
    _set_gate(IDT_Table + n, 0x8E, ist, addr);

}

void set_trap_gate(uint32_t n, uint8_t ist, void *addr)
{
    _set_gate(IDT_Table + n, 0x8F, ist, addr);
}

void set_system_gate(uint32_t n, uint8_t ist, void *addr)
{
    _set_gate(IDT_Table + n, 0xEF, ist, addr);
}

void set_system_intr_gate(uint32_t n, uint8_t ist, void *addr)
{
    _set_gate(IDT_Table + n, 0xEE, ist, addr);
}