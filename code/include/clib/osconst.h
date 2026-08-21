#pragma once

#define _OS_API     //This means the function will set err_code


#define OS_SUCCES 0
#define OS_FAILED 1	

#define max_process 64

#define kernel_pid 0

typedef unsigned long size_t;
// C 编译环境：此处定义宏

extern unsigned int TSS64_Table[26];
extern uint16_t ERR_CODE[max_process];