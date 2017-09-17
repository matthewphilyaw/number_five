#include "display/gfx.h"
#include "stdlib.h"

static uint16_t cursor_x;
static uint16_t cursor_y;

static bool clear_first_on_next_char_write = false;

void gfx_fill_rect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color) {
  for (int16_t i=x; i<x+w; i++) {
    ssd1306_drawFastVLine(i, y, h, color);
  }
}

void gfx_draw_char(int16_t x, int16_t y, unsigned char c, uint16_t color, uint16_t bg, uint8_t size) {
  if((x >= SSD1306_LCDWIDTH)            || // Clip right
     (y >= SSD1306_LCDHEIGHT)           || // Clip bottom
     ((x + 6 * size - 1) < 0) || // Clip left
     ((y + 8 * size - 1) < 0)) {   // Clip top
    return;
  }

  if(c >= 176) c++; // Handle 'classic' charset behavior

  for(int8_t i=0; i<5; i++ ) { // Char bitmap = 5 columns
    uint8_t line = font[c * 5 + i];
    for(int8_t j=0; j<8; j++, line >>= 1) {
      if(line & 1) {
        if(size == 1)
          ssd1306_drawPixel(x+i, y+j, color);
        else
          gfx_fill_rect(x + i * size, y + j * size, size, size, color);
      } else if(bg != color) {
        if(size == 1)
          ssd1306_drawPixel(x+i, y+j, bg);
        else
          gfx_fill_rect(x + i * size, y + j * size, size, size, bg);
      }
    }
  }
  if(bg != color) { // If opaque, draw vertical line for last column
    if(size == 1) ssd1306_drawFastVLine(x+5, y, 8, bg);
    else gfx_fill_rect(x + 5 * size, y, size, 8 * size, bg);
  }
}

void gfx_write_char(uint8_t c) {
  if (clear_first_on_next_char_write) {
    ssd1306_clearDisplay();
    clear_first_on_next_char_write = false;
  }

  if(c == '\n') {
    cursor_x  = 0;
    uint16_t next_line = cursor_y + (uint16_t)(TEXTSIZE * 8);
    if (next_line >= SSD1306_LCDHEIGHT) {
      next_line = 0;
      clear_first_on_next_char_write = true;
    }
    cursor_y = next_line;          // advance y one line
  } else if(c != '\r') {                 // Ignore carriage returns
    if(WRAP && ((cursor_x + TEXTSIZE * 6) >= SSD1306_LCDWIDTH)) { // Off right?
      cursor_x  = 0;                 // Reset x to zero,
      uint16_t next_line = cursor_y + (uint16_t)(TEXTSIZE * 8);
      if (SCROLL && next_line > SSD1306_LCDHEIGHT) {
        next_line = 0;
        // if we are scrolling clear the display before wrapping around;
        ssd1306_clearDisplay();
      }
      cursor_y = next_line;          // advance y one line
    }
    gfx_draw_char(cursor_x, cursor_y, c, TEXTCOLOR, TEXTBGCOLOR, TEXTSIZE);
    cursor_x += TEXTSIZE * 6;          // Advance x one char
  }
}

void gfx_write_str(const char *str) {
  for (;*str != '\0'; str++) {
    gfx_write_char(*str);
  }
}

// largest size
void gfx_write_uint(uint32_t n) {
  char itoa_buffer[(sizeof(uint32_t) * 8) + 1];
  itoa(n, itoa_buffer, 10);

  gfx_write_str(itoa_buffer);
}

void gfx_write_int(int32_t n) {
  char itoa_buffer[(sizeof(int32_t) * 8) + 1];
  itoa(n, itoa_buffer, 10);

  gfx_write_str(itoa_buffer);
}

void gfx_init() {
  cursor_x = 0;
  cursor_y = 0;

  ssd1306_init(SSD1306_SWITCHCAPVCC, false);
}

// reset cursory and clear screen
void gfx_clear() {
  cursor_x = 0;
  cursor_y = 0;

  ssd1306_clearDisplay();
}

void gfx_reset_cursor() {
  cursor_x = 0;
  cursor_y = 0;
}

void gfx_set_cursor_x(uint16_t x) {
  cursor_x = x;
}

void gfx_set_cursor_y(uint16_t y) {
  cursor_y = y;
}
