//
// Created by Matthew Philyaw on 9/10/17.
//

#ifndef N5_GFX_H_H
#define N5_GFX_H_H

#include "ssd1306.h"
#include "font.h"

#define TEXTSIZE    1
#define TEXTBGCOLOR BLACK
#define TEXTCOLOR   WHITE

#define WRAP        1
#define SCROLL      1

void gfx_fillRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color);
void gfx_drawChar(int16_t x, int16_t y, unsigned char c, uint16_t color, uint16_t bg, uint8_t size);
void gfx_writeChar(uint8_t c);

void gfx_writeStr(char *str);
void gfx_write_uint(uint32_t n);
void gfx_write_int(int32_t n);


static inline void gfx_display() {
  ssd1306_display();
}

void gfx_init(void);
void gfx_clear(void);
void gfx_reset_cursor(void);
void gfx_set_cursor_x(uint16_t x);
void gfx_set_cursor_y(uint16_t y);

#endif //N5_GFX_H_H
