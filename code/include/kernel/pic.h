#pragma once

#define IRQ_count 16
#define IRQ_base 0x20

#define PIC0_ICW1  0x0020  /* 主PIC ICW1命令端口 */
#define PIC0_ICW2  0x0021  /* 主PIC ICW2-4数据端口 */
#define PIC0_ICW3  0x0021 /* ICW3也使用这个端口 */
#define PIC0_ICW4  0x0021 /* ICW4也使用这个端口 */
#define PIC0_IMR   0x0021  /* 主PIC IMR数据端口 (与ICW2-4共用) */


#define PIC1_ICW1  0x00A0  /* 从PIC ICW1命令端口 */
#define PIC1_ICW2  0x00A1  /* 从PIC ICW2-4数据端口 */
#define PIC1_ICW3  0x00A1  /* 从PIC ICW2-4数据端口 */
#define PIC1_ICW4  0x00A1  /* 从PIC ICW2-4数据端口 */
#define PIC1_IMR   0x00A1  /* 从PIC IMR数据端口 (与ICW2-4共用) */

#define PIC_EOI 0x20

#define pit_irq 0
#define keyboard_irq 1
#define mouse_irq 12

typedef void (*void_func_ptr)(void);

void irq0();
void irq1();
void irq2();
void irq3();
void irq4();
void irq5();
void irq6();
void irq7();
void irq8();
void irq9();
void irq10();
void irq11();
void irq12();
void irq13();
void irq14();
void irq15();

void send_eoi(uint8_t irq);
void enable_irq(uint8_t irq);
void disable_irq(uint8_t irq);