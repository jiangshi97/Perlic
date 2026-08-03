#pragma once

#include <stdint.h>

#define asm_hlt() __asm__ __volatile__("hlt \n\t")
#define hlt_loop() while(1){asm_hlt();}
#define load_TR(n) 							\
do{									\
	__asm__ __volatile__(	"ltr	%%ax"				\
				:					\
				:"a"(n << 3)				\
				:"memory");				\
}while(0)
#define max_num(a,b) ((a > b)?(a):(b))
#define min_num(a,b) ((a < b)?(a):(b))
#define align8(addr) ((((uintptr_t)addr) % 8 == 0)?((void *)addr):((void *)((((uintptr_t)addr / 8) + 1) * 8)))

#define OS_SUCCES 0
#define OS_FAILED 1	

#define max_process 64


typedef unsigned long long size_t;

extern unsigned int TSS64_Table[26];
extern uint16_t ERR_CODE[max_process];

int strlen(char * String);


static inline void outb(uint16_t port, uint8_t val) {
    asm volatile ( "outb %0, %1" : : "a"(val), "Nd"(port) );
}

static inline uint8_t inb(uint16_t port) {
    uint8_t ret;
    asm volatile ( "inb %1, %0" : "=a"(ret) : "Nd"(port) );
    return ret;
}

void set_tss64(unsigned long rsp0,unsigned long rsp1,unsigned long rsp2,unsigned long ist1,unsigned long ist2,unsigned long ist3,
unsigned long ist4,unsigned long ist5,unsigned long ist6,unsigned long ist7);

uint16_t get_pid();