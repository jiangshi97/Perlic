#include <stdint.h>

typedef struct {
    uint8_t x[16];
}__attribute__((packed)) gate_struct;

typedef struct {
    gate_struct idt_tab[256];
    uint16_t IDT_LIMIT;
    void *IDT_BASE;
}__attribute__((packed)) IDT;

extern gate_struct IDT_Table[];
extern uint64_t GDT_Table[];

extern IDT nIDT;
extern uint64_t IDT_POINTER;

void set_tss_to_gdt(uint64_t *gdt_tab, void *tss_addr, uint64_t num, uint64_t length);

void _set_gate(gate_struct *gate, uint64_t attr, uint64_t ist, void *addr);

void set_intr_gate(uint32_t n, uint8_t ist, void *addr);

void set_trap_gate(uint32_t n, uint8_t ist, void *addr);

void set_system_gate(uint32_t n, uint8_t ist, void *addr);

void set_system_intr_gate(uint32_t n, uint8_t ist, void *addr);
