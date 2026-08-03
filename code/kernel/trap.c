#include "gate.h"
#include "serial.h"
#include "ptrace.h"
#include "traps.h"
#include "sprint.h"
#include "lib.h"
#include "debug.h"

void debug_print_IDT(gate_struct *tab,int low,int high);

void do_divide_error(struct pt_regs * regs,unsigned long error_code)
{
	debug_printf("divide_error happen!! rip:0x%x\n",regs->rip);
	while(1){asm_hlt();}
}

void do_nmi(struct pt_regs * regs,unsigned long error_code)
{
	debug_printf("nmi happen!! rip:0x%x\n",regs->rip);
	while(1){asm_hlt();}
}

//void do_int3(struct pt_regs * regs,unsigned long error_code)

void do_overflow(struct pt_regs * regs,unsigned long error_code)
{
	debug_printf("do_overflow happen!! rip:0x%x\n",regs->rip);
	while(1){asm_hlt();}
}

void do_double_fault(struct pt_regs * regs,unsigned long error_code)
{
	debug_printf("do_double_fault happen!! rip:0x%x\n",regs->rip);
	while(1){asm_hlt();}
}

void do_coprocessor_segment_overrun(struct pt_regs * regs,unsigned long error_code)
{
	debug_printf("do_coprocessor_segment_overrun happen!! rip:0x%x\n",regs->rip);
	while(1){asm_hlt();}
}

void do_invalid_TSS(struct pt_regs * regs,unsigned long error_code)
{
	debug_printf("invalid_TSS happen!! rip:0x%x\n",regs->rip);
	while(1){asm_hlt();}
}

void do_segment_not_present(struct pt_regs * regs,unsigned long error_code)
{
	debug_printf("segment_not_present happen!! rip:0x%x\n",regs->rip);
	while(1){asm_hlt();}
}

void do_stack_segment_fault(struct pt_regs * regs,unsigned long error_code)
{
	debug_printf("stack_segment_fault happen!! rip:0x%x\n",regs->rip);
	while(1){asm_hlt();}
}

void do_general_protection(struct pt_regs * regs,unsigned long error_code)
{
	debug_printf("general_protection happen!! rip:0x%x\n",regs->rip);
	while(1){asm_hlt();}
}

void do_page_fault(struct pt_regs * regs,unsigned long error_code)
{
	debug_printf("page_fault happen!! rip:0x%x\n",regs->rip);
	while(1){asm_hlt();}
}

void do_alignment_check(struct pt_regs * regs,unsigned long error_code)
{
	debug_printf("alignment_check happen!! rip:0x%x\n",regs->rip);
	while(1){asm_hlt();}
}

void do_machine_check(struct pt_regs * regs,unsigned long error_code)
{
	debug_printf("machine_check happen!! rip:0x%x\n",regs->rip);
	while(1){asm_hlt();}
}

void do_SIMD_exception(struct pt_regs * regs,unsigned long error_code)
{
	debug_printf("SIMD_exception happen!! rip:0x%x\n",regs->rip);
	while(1){asm_hlt();}
}

void unknown_error(void)
{
	serial_puts("unknown_interruption!\n");
}

extern void ignore_int();

void sys_vector_init()
{
	set_trap_gate(0,1,divide_error);
	//set_trap_gate(1,1,debug);
	set_intr_gate(2,1,nmi);
	//set_system_gate(3,1,int3);
	set_system_gate(4,1,overflow);
	//set_system_gate(5,1,bounds);
	//set_trap_gate(6,1,undefined_opcode);
	//set_trap_gate(7,1,dev_not_available);
	set_trap_gate(8,1,double_fault);
	set_trap_gate(9,1,coprocessor_segment_overrun);
	//set_trap_gate(10,1,invalid_TSS);
	set_trap_gate(11,1,segment_not_present);
	set_trap_gate(12,1,stack_segment_fault);
	set_trap_gate(13,1,general_protection);
	set_trap_gate(14,1,page_fault);
	//15 Intel reserved. Do not use.
	//set_trap_gate(16,1,x87_FPU_error);
	set_trap_gate(17,1,alignment_check);
	set_trap_gate(18,1,machine_check);
	set_trap_gate(19,1,SIMD_exception);	
	//set_trap_gate(20,1,virtualization_exception);
	//set_system_gate(SYSTEM_CALL_VECTOR,7,system_call);

	__asm__ __volatile__("lidt %0"::"m"(IDT_POINTER):"memory");
}
void debug_print_IDT(gate_struct *tab,int low,int high)
{
	//serial_printf("IDT low:0x%x\nhigh:0x%x\n",((uint64_t *)(IDT_Table))[0],((uint64_t *)(IDT_Table))[1]);
	//serial_printf("p1:0x%x p2:0x%x\n",((uint64_t *)(IDT_Table))[-2],((uint64_t *)(IDT_Table))[-1]);
	serial_printf("addr of ignore_int:0x%x\n",ignore_int);
	serial_printf("aadr of divide_error:0x%x\n",divide_error);
	serial_printf("IDT_TABLE:\n");
	for(int a = low; a <= high; a++)
	{
		serial_printf("IDT_table[%d]:[1]:0x%x [2]:0x%x\n",a,((uint64_t *)(IDT_Table))[2 * a],((uint64_t *)(IDT_Table))[2 * a + 1]);
	}
	serial_putchar('\n');
}