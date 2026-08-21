#include <stdint.h>
#include "cppl.hpp"
#include "driver_struct.hpp"
#include "keyboard.hpp"
extern "C"
{
#include "pic.h"
#include "serial.h"
#include "lib.h"
#include "trap.h"
}

// --- I/O 端口定义 ---
#define KBD_DATA_PORT   0x60
#define KBD_CMD_PORT    0x64
#define KBD_STATUS_PORT 0x64

// --- 8042 命令 ---
#define CMD_READ_CCB    0x20  // 读取命令字节
#define CMD_WRITE_CCB   0x60  // 写入命令字节
#define CMD_DISABLE_KBD 0xAD  // 禁用键盘
#define CMD_ENABLE_KBD  0xAE  // 启用键盘

// --- 键盘命令 ---
#define KBD_CMD_RESET   0xFF  // 复位
#define KBD_CMD_SET_LED 0xED  // 设置LED
#define KBD_CMD_SET_RATE 0xF3 // 设置速率
#define KBD_CMD_SET_SCAN 0xF0 // 设置扫描码集
#define KBD_CMD_ENABLE  0xF4  // 启用扫描

// --- 状态寄存器位 (0x64读) ---
#define KBD_STAT_OBF    0x01  // 输出缓冲区满 (有数据可读)
#define KBD_STAT_IBF    0x02  // 输入缓冲区满 (不能写入)

// --- 键盘回复 ---
#define KBD_REPLY_ACK   0xFA
#define KBD_REPLY_RESEND 0xFE
#define KBD_REPLY_BAT   0xAA  // 自检通过

static const uint32_t buff_size = 50;

namespace ker_driver{

static bool isCreated_keyboard = false;
driver_keyboard *keyboard_dri;


static void get_irq(void)
{
    keyboard_dri->irq_handler();
}

void driver_keyboard::irq_handler()
{
    debug_printf("irq get\n");
    uint8_t code = inb(KBD_DATA_PORT);
    debug_printf("code:%d\n", code);    
    wait_write(KBD_STATUS_PORT, KBD_STAT_IBF);
    outb(PIC0_ICW2, 0x61);
    outb(0x20, 0x20);
}

driver_keyboard::driver_keyboard()
{
    this->key_buff = new ker_lib::qdeque<uint8_t>(buff_size);
    keyboard_func = get_irq;
}

uint32_t driver_keyboard::add_call_func(DriverCall func)
{
    return 0;
};

_OS_API void driver_keyboard::delete_call_func(uint32_t node)
{

};

driver_keyboard *driver_keyboard::getDriver()
{
    if(!isCreated_keyboard)
    {
        keyboard_dri = new driver_keyboard();
        isCreated_keyboard = true;
    }
    return keyboard_dri;
}

uint32_t driver_keyboard::get_type()
{
    return keyboard_type;
}

}  
extern "C" void keyboader_init()
{
    ker_driver::driver_keyboard::getDriver();
    enable_irq(keyboard_irq);
}