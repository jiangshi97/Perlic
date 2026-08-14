#pragma once

#include <stdint.h>
#include "lib.h"

#define tty_printf(fmt,arg...)   ({\
    tsprintf((char *)&string_buff,fmt,##arg);\
    tty_puts((char *)&string_buff);\
})
#define _OS_API     //This means the function will set err_code

extern unsigned char hankaku_bin[];
extern unsigned int hankaku_bin_len;

_OS_API void tty_init();
void tty_puts(const unsigned char *str);
void tty_putchar(unsigned char letter);
