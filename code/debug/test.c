#include <stdint.h>
#include "lib.h"

#define NMI_ENABLE_PORT 0x70
#define NMI_REASON_PORT 0x61

uint16_t ERR_CODE[max_process];

// 触发NMI的函数
void trigger_nmi_rtc(void) {
    // 可选：读取NMI原因
    // uint8_t reason = inb(NMI_REASON_PORT);

    // 1. 禁用NMI (设置bit 7)
    outb(0x8f, NMI_ENABLE_PORT);
    // 2. 虚拟读操作，产生延迟
    inb(0x71);
    // 3. 重新启用NMI (清除bit 7)
    outb(0x0f, NMI_ENABLE_PORT);
    // 4. 虚拟读操作
    inb(0x71);
}