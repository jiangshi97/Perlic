#include <stdint.h>

//this defination come from 田宇,must be replaced int the future
/*#define _set_gate(gate_selector_addr,attr,ist,code_addr)	\
do								\
{	unsigned long __d0,__d1;				\
	__asm__ __volatile__	(	"movw	%%dx,	%%ax	\n\t"	\
					"andq	$0x7,	%%rcx	\n\t"	\
					"addq	%4,	%%rcx	\n\t"	\
					"shlq	$32,	%%rcx	\n\t"	\
					"addq	%%rcx,	%%rax	\n\t"	\
					"xorq	%%rcx,	%%rcx	\n\t"	\
					"movl	%%edx,	%%ecx	\n\t"	\
					"shrq	$16,	%%rcx	\n\t"	\
					"shlq	$48,	%%rcx	\n\t"	\
					"addq	%%rcx,	%%rax	\n\t"	\
					"movq	%%rax,	%0	\n\t"	\
					"shrq	$32,	%%rdx	\n\t"	\
					"movq	%%rdx,	%1	\n\t"	\
					:"=m"(*((unsigned long *)(gate_selector_addr)))	,					\
					 "=m"(*(1 + (unsigned long *)(gate_selector_addr))),"=&a"(__d0),"=&d"(__d1)		\
					:"i"(attr << 8),									\
					 "3"((unsigned long *)(code_addr)),"2"(0x8 << 16),"c"(ist)				\
					:"memory"		\
				);				\
}while(0)*/
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
