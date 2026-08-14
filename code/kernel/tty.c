#include "tty.h"
#include "lib.h"
#include "video.h"
#include "serial.h"
#include "klib.h"
#include <stdint.h>

#define last_line_part (2.0f / 3.0f)


uint8_t tty_fonts[256][16];
uint32_t tty_width, tty_height;
unsigned char **tty_buffer;
uint32_t volatile tty_x = 0, tty_y = 0;
uint32_t up_line = 0;   //Must be below tty_height
uint8_t isUptoline = 0;

static inline void font_init()
{
    for(uint64_t a = 0; a < hankaku_bin_len; a++)
    {
        tty_fonts[a / 16][a % 16] = hankaku_bin[a];
    }
}

static inline void draw_letter(uint32_t x, uint32_t y, unsigned char letter)
{
    //debug_printf("draw to (%d, %d) tty_x is %d tty_y is %d up_line is %d\n", x, y, tty_x, tty_y, up_line);
    uint32_t b_x, b_y;
    b_x = x * 8;
    b_y = y * 16;
    for(uint8_t a = 0; a < 16; a++)
    {
        for(uint8_t b = 0; b < 8; b++)
        {
            if(((1 << b) & tty_fonts[letter][a]) >> b)
            {
                drawPoint(b_x + 8 - b, b_y + a, pixels.green);
            }
            else
            {
                drawPoint(b_x + 8 - b, b_y + a, pixels.black);
            }
        }
    }
}

void framebuffer_refresh()
{
    unsigned char now;
    if(tty_y >= up_line)
    {
        for(uint64_t a = up_line; a <= tty_y; a++)
        {
            for(uint64_t b = 0; b < tty_width; b++)
            {
                now = tty_buffer[a][b];
                if(now == 0)
                {
                    break;
                }
                else
                {
                    draw_letter(b, a - up_line, now);
                }
            }
        }
    }
    else
    {
        for(uint64_t a = up_line; a <= tty_height; a++)
        {
            for(uint64_t b = 0; b < tty_width; b++)
            {
                now = tty_buffer[a][b];
                if(now == 0)
                {
                    break;
                }
                else
                {
                    draw_letter(b, a - up_line, now);
                }
            }
        }
        for(uint64_t a = 0; a <= tty_y; a++)
        {
            for(uint64_t b = 0; b < tty_width; b++)
            {
                now = tty_buffer[a][b];
                if(now == 0)
                {
                    break;
                }
                else
                {
                    draw_letter(b, a + tty_height - up_line + 1, now);
                }
            }
        }
    }
}

static inline void change_line()
{
    //debug_printf("change line\n");
    tty_x = 0;
    up_to_add_one(&(tty_y), tty_height - 1, 0);
    if(isUptoline == 0)
    {   
        if((float)tty_y >= ((float)tty_height * last_line_part))
        {
            isUptoline = 1;
            up_line = 1;
            kmemset(tty_buffer[0], 0, tty_width * sizeof(unsigned char));
            framebuffer_refresh();
        }
    }
    else
    {
        kmemset(tty_buffer[up_line], 0, tty_width * sizeof(unsigned char));
        up_to_add_one(&(up_line), tty_height - 1, 0);
        framebuffer_refresh();
    }
}

void tty_putchar(unsigned char letter)
{
    if(letter <= 31 || letter == 127)
    {
        switch (letter)
        {
            case '\n':
                change_line();
                break;
        
            default:
                break;
        }
    }
    else
    {
        tty_buffer[tty_y][tty_x] = letter;
        if(tty_y >= up_line)
        {
            draw_letter(tty_x, tty_y - up_line, letter);
        }
        else
        {
            draw_letter(tty_x, tty_height - (up_line - tty_y), letter);
        }
        if(tty_x == tty_width)
        {
            change_line();
        }
        else
        {
            tty_x += 1;
        }

    }
}

void tty_puts(const unsigned char *str)
{
    const unsigned char *now = str;
    while(1)
    {
        if(*now == '\0')
        {
            break;
        }
        tty_putchar(*now);
        now += 1;
    }
}

_OS_API void tty_init()
{
    font_init();
    tty_width = (fb_inf -> framebuffer_width) / 8;
    tty_height = (fb_inf -> framebuffer_height) / 16;
    debug_printf("tty_width is %d, tty_height is %d\n", tty_width, tty_height);
    tty_buffer = (unsigned char **)init_darray(tty_height, tty_width, sizeof(unsigned char));
    is_set_screen = 1;
    debug_printf("tty ok\n");
    RetSucces();
}