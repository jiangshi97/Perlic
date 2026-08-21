#pragma once

#include <stdarg.h>
#include <stdint.h>
extern uint8_t string_buff;

int skip_atoi(const char **s);
static char * number(char * str, long num, int base, int size, int precision,	int type);
int vsprintf(char * buf,const char *fmt, va_list args);
int tsprintf(char * buf,const char *fmt, ...);
