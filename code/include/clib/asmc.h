#pragma once

#include "stdint.h"

#define _OS_API     //This means the function will set err_code

#define enable_int() do{ asm volatile("sti");}while(0);
#define io_wait() do{ outb(0x80, 0);}while(0);

#define asm_hlt() __asm__ __volatile__("hlt \n\t")
#define nop() __asm__ __volatile__("nop \n\t")
#define hlt_loop() while(1){asm_hlt();}
#define load_TR(n) 							\
do{									\
	__asm__ __volatile__(	"ltr	%%ax"				\
				:					\
				:"a"(n << 3)				\
				:"memory");				\
}while(0)

typedef unsigned long size_t;

static inline void outb(uint16_t port, uint8_t val) {
    asm volatile ( "outb %0, %1" : : "a"(val), "Nd"(port) );
}

static inline uint8_t inb(uint16_t port) {
    uint8_t ret;
    asm volatile ( "inb %1, %0" : "=a"(ret) : "Nd"(port) );
    return ret;
}

int strlen(char * String);

void set_tss64(unsigned long rsp0,unsigned long rsp1,unsigned long rsp2,unsigned long ist1,unsigned long ist2,unsigned long ist3,
unsigned long ist4,unsigned long ist5,unsigned long ist6,unsigned long ist7);

uint16_t get_pid();
void ker_panic();
_OS_API void **init_darray(uint32_t x, uint32_t y, size_t size);
void *kmemset(void *s, unsigned char c, size_t n);
