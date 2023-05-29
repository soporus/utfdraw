#include "term.h"
/*
bitwise AND operator: extract a subset of bits in a value
bitwise OR operator: set a subset of bits in a value
bitwise XOR operator: toggle a subset of bits in a value
*/
/*
uint32_t setColor(uint32_t *color, uint32_t ch) {
  uint8_t x = (*color >> 24) & 0xFF;
  uint8_t r = (*color >> 16) & 0xFF;
  uint8_t g = (*color >>  8) & 0xFF;
  uint8_t b = (*color & 0xFF);

  switch (ch) {
    case 'z': r += 8;
              break;
    case 'x': g += 8;
              break;
    case 'c': b += 8;
              break;
    default : break;
  }
  return *color = (x << 24) | (r << 16) | (g << 8) | b;
}
*/
/*
void setColor(uint32_t *color, uint32_t ch) {
  uint8_t x = (*color >> 24) & 0xFF;
  uint8_t r = (*color >> 16) & 0xFF;
  uint8_t g = (*color >>  8) & 0xFF;
  uint8_t b = (*color & 0xFF);

  switch (ch) {
    case 'z': r += 8;
              break;
    case 'x': g += 8;
              break;
    case 'c': b += 8;
              break;
    default : break;
  }
  *color = (x << 24) | (r << 16) | (g << 8) | b;
}
*/

void setColor(Color *color, uint32_t ch) {
 // uint8_t x = (*color >> 24) & 0xFF;
  //uint8_t r = (*color >> 16) & 0xFF;
  //uint8_t g = (*color >>  8) & 0xFF;
  //uint8_t b = (*color & 0xFF);

  switch (ch) {
    case 'z': color->r += 8;
              break;
    case 'x': color->g += 8;
              break;
    case 'c': color->b += 8;
              break;
    default : break;
  }
  //*color = (x << 24) | (r << 16) | (g << 8) | b;
}

/*
void setColor(uint32_t *color, uint32_t ch) {
  switch (ch) {
    case 'z': *color = ((uint32_t) \
	      ((uint8_t) (((*color >> 16) & 0xFF) + 8)) \
		      & 0x00FF0000) | (*color & 0x0000FFFF);
              break;
    case 'x': *color = *color;
              break;
    case 'c': *color = *color;
              break;
    default : break;
  }
  // *color = (*color | *color & 0xFF000000);
}
*/
void drawPalette(
    const uint16_t arr[],
    const uint16_t len,
    const uint16_t *c) 
{
  hLine(0, (uint16_t) tb_height()-1, 0, 0x101010, ' ');
  const uint16_t y = (uint16_t) tb_height()-1;
  uint32_t cfg = 0x20000000;
  uint32_t cbg = 0x40C0FF;
  for(uint16_t i = 0; i < len; ++i) {
    cfg = ((arr[i] == *c) ? 0xF0F0F0 : 0x808080);
    cbg = ((arr[i] == *c) ? 0x20000000 : 0x101010);
    tb_set_cell(i, y, arr[i], cfg, cbg);
  }
} // draw palette characters at screen bottom

const uint32_t randRange(const uint32_t min, const uint32_t max) {
  return min + rand() / (RAND_MAX / (max - min + 1) + 1); 
} // random integer within range given

void boxFill(uint16_t x, uint16_t y, uint16_t c, 
    const uint16_t w, const uint16_t h, uint32_t colorfg, uint32_t colorbg) {
  for (; y < h; ++y) {
    for (uint16_t i =  x; i < w; ++i) {
      tb_set_cell(i, y, c, colorfg, colorbg);
    }
  }
} // draw box with fill

void randCell(const uint16_t quant, const uint16_t size, uint16_t c) {
  for(uint16_t i = 0; i < quant; ++i) {
    const uint16_t x = (uint16_t) randRange(0, (uint16_t) tb_width() -1);
    const uint16_t y = (uint16_t) randRange(0, (uint16_t) tb_height()-1);
    const uint16_t sizex = x + size;
    const uint16_t sizey = y + size;
    const uint32_t colorfg = randRange(0x404040, 0xFFFFFF);
    const uint32_t colorbg = randRange(0x000000, 0x101010);
    boxFill(x, y, c, sizex, sizey, colorfg, colorbg);
  }
} // draw box at random cell within bounds of terminal

void hLine(uint16_t x, uint16_t y, uint32_t fgCol, uint32_t bgCol, uint16_t c) {
  for (; x < tb_width(); ++x) {
    tb_set_cell(x, y, c, fgCol, bgCol);
  }
} // draw a horizontal line

void vLine(uint16_t x, uint16_t y, uint32_t fgCol, uint32_t bgCol, uint16_t c) {
  for (; y < tb_height(); ++y) {
    tb_set_cell(x, y, ' ', fgCol, bgCol);
  }
} // draw a vertical line

void randLine(void) {
  const uint16_t y = (uint16_t) randRange(0, (uint16_t) tb_height() - 1);
  const uint16_t x = (uint16_t) randRange(0, (uint16_t) tb_width() -  1);
  const uint32_t colorfg = randRange(0x000001,  0x200008);
  const uint32_t colorbg = randRange(0x000001,  0x020008);
  hLine(0, y, colorfg, colorbg, L'\u2591');
  vLine(x, 0, colorfg, colorbg, L'\u2591');
} // draw a full width line at random y 

