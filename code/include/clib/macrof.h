#pragma once

#include "stdint.h"

#define max_num(a,b) ((a > b)?(a):(b))
#define min_num(a,b) ((a < b)?(a):(b))
#define align8(addr) ((((uintptr_t)addr) % 8 == 0)?((void *)addr):((void *)((((uintptr_t)addr / 8) + 1) * 8)))
#define aligns(addr, align_num) ((((uintptr_t)(addr)) % (1 << (align_num)) == 0)?((void *)(addr)):((void *)((((uintptr_t)(addr) / (1 << align_num)) + 1) * (1 << (align_num)))))


#define VirToPhy(addr) ((void *)((uintptr_t)addr & 0x00007fffffffffff))
#define PhyToVir(addr) ((void *)((uintptr_t)addr | 0xffff800000000000))
#define VirPhyToEach(addr) ((addr >= 0xffff800000000000)?(VirToPhy(addr)):(PhyToVir(addr)))

#define CheckWithRet(arg) do{ if(ERR_CODE[get_pid()] != OS_SUCCES){return arg;}}while(0)
#define CheckWithText(fmt,arg...) do{ \
    if(ERR_CODE[get_pid()] != OS_SUCCES){   \
        debug_printf(fmt,##arg);    \
        hlt_loop(); \
    }}while(0)
#define RetErr(arg) do  \
{   \
    ERR_CODE[get_pid()] = OS_FAILED;    \
    return arg; \
}while(0);
#define RetSucces(arg) do  \
{   \
    ERR_CODE[get_pid()] = OS_SUCCES;    \
    return arg; \
}while(0);

#define up_to_add_one(ptr, up_num, last_num) do \
{   \
    if((*(ptr)) >= (up_num))    \
    {   \
        (*(ptr)) = (last_num);  \
    }   \
    else    \
    {   \
        (*(ptr)) += 1;  \
    }   \
}while(0); 


void wait_write(uint8_t port, uint8_t ibf);
void wait_read(uint8_t port, uint8_t ibf);

