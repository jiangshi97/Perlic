#include "sprint.h"

#define serial_printf(fmt,arg...)   ({\
    tsprintf((char *)&string_buff,fmt,##arg);\
    serial_puts((char *)&string_buff);\
})

void serial_init(void);
void serial_putchar(char c);
void serial_puts(const char* str);

extern uint8_t is_set_screen;

#define debug_printf(fmt,arg...) {\
    if(is_set_screen == 0){\
        serial_printf(fmt,##arg);\
    }\
}while(0)