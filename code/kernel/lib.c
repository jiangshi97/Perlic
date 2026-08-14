#include "lib.h"
#include "memlib.h"
#include "klib.h"
#include <stdint.h>

#define memset_qword_num 32

int strlen(char * String)
{
	register int __res;
	__asm__	__volatile__	(	"cld	\n\t"
					"repne	\n\t"
					"scasb	\n\t"
					"notl	%0	\n\t"
					"decl	%0	\n\t"
					:"=c"(__res)
					:"D"(String),"a"(0),"0"(0xffffffff)
					:
				);
	return __res;
}

void set_tss64(unsigned long rsp0,unsigned long rsp1,unsigned long rsp2,unsigned long ist1,unsigned long ist2,unsigned long ist3,
unsigned long ist4,unsigned long ist5,unsigned long ist6,unsigned long ist7)
{
	*(unsigned long *)(TSS64_Table+1) = rsp0;
	*(unsigned long *)(TSS64_Table+3) = rsp1;
	*(unsigned long *)(TSS64_Table+5) = rsp2;

	*(unsigned long *)(TSS64_Table+9) = ist1;
	*(unsigned long *)(TSS64_Table+11) = ist2;
	*(unsigned long *)(TSS64_Table+13) = ist3;
	*(unsigned long *)(TSS64_Table+15) = ist4;
	*(unsigned long *)(TSS64_Table+17) = ist5;
	*(unsigned long *)(TSS64_Table+19) = ist6;
	*(unsigned long *)(TSS64_Table+21) = ist7;	
}

uint16_t get_pid()
{
    return 0;
}

_OS_API void **init_darray(uint32_t x, uint32_t y, size_t size)
{
	void *ptr = kmalloc_init(x * y * size);
	CheckWithRet(null_ptr);
	void **dptr = kmalloc(x * sizeof(void *));
	for(uint32_t a = 0; a < x; a++)
	{
		dptr[a] = (void *)((unsigned char *)ptr + y * a);
	}
	RetSucces(dptr);
}

void *kmemset(void *s, unsigned char c, size_t n)
{
	if(n >= memset_qword_num)
	{
		uint64_t c2 = (uint64_t)c;
		uint64_t c1 = (c2 | (c2 << 8) | (c2 << 16) | (c2 << 24) | (c2 << 32) | (c2 << 40) | (c2 << 48) | (c2 << 56));
		size_t n1 = n / 8;
		uint8_t n2 = n % 8;
		uint64_t *s1_ptr = (uint64_t *)s;
		unsigned char *s2_ptr = (unsigned char *)s;
		for(uint64_t a = 0; a < n1; a++)
		{
			s1_ptr[a] = c1;
		}
		c1 = ((n / 8) * 8);
		for(uint64_t a = 0; a < n2; a++)
		{
			s2_ptr[c1 + a] = c;
		}
	}
	else
	{
		unsigned char *s1_ptr = (unsigned char *)s;
		for(uint64_t a = 0; a < n; a++)
		{
			s1_ptr[a] = c;
		}
	}
};
