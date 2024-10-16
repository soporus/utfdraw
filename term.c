#include "term.h"
// inputs
void checkInput(struct tb_event *restrict ev, Color *restrict color, const wchar_t **c,
                const wchar_t *restrict arr, uint16_t *restrict sX, uint16_t *restrict sY) {
  const uint32_t key = (ev->ch > 0) ? ev->ch : (uint32_t) ev->key;
  // flag when a draw is needed
  uint8_t draw = 0;
  // stores previous keypress persistently
  // static uint32_t keyTest = 0;
  // setup boundaries for movement, and check within boundary after resize
  const uint16_t width  = (uint16_t) tb_width() - 1;
  const uint16_t height = (uint16_t) tb_height() - 2;
  // check we are within boundary
  *sX = (*sX < width + 1) ? *sX : width;
  *sY = (*sY < height + 1) ? *sY : height;
  switch (key) {
    // movement and paints character
    case TB_KEY_MOUSE_LEFT :
      *sX = (uint16_t) ev->x;
      *sY = (uint16_t) ev->y;
      ++draw;
      break;
    // movement only, but must draw to paint cursor position
    case TB_KEY_ARROW_LEFT  : *sX = (*sX > 0) ? *sX - 1 : 0; break;
    case TB_KEY_ARROW_UP    : *sY = (*sY > 0) ? *sY - 1 : 0; break;
    case TB_KEY_ARROW_DOWN  : *sY = (*sY < height) ? *sY + 1 : *sY; break;
    case TB_KEY_ARROW_RIGHT : *sX = (*sX < width) ? *sX + 1 : *sX; break;
    // lowercase: move only  uppercase: move and draw
    case 'w' : *sY = (*sY > 0) ? *sY - 1 : 0; break;                    // ▲  wasd movement
    case 'a' : *sX = (*sX > 0) ? *sX - 1 : 0; break;                    // ◀︎
    case 's' : *sY = (*sY < height) ? *sY + 1 : *sY; break;             // ▼
    case 'd' : *sX = (*sX < width) ? *sX + 1 : *sX; break;              // ▶︎
    case 'k' : *sY = (*sY > 0) ? *sY - 1 : 0; break;                    // ▲  vi movement
    case 'h' : *sX = (*sX > 0) ? *sX - 1 : 0; break;                    // ◀︎
    case 'j' : *sY = (*sY < height) ? *sY + 1 : *sY; break;             // ▼
    case 'l' : *sX = (*sX < width) ? *sX + 1 : *sX; break;              // ▶︎
    case 'W' : *sY = (*sY > 0) ? *sY - (++draw) : 0; break;             // ▲  wasd draw
    case 'A' : *sX = (*sX > 0) ? *sX - (++draw) : 0; break;             // ◀︎
    case 'S' : *sY = (*sY < height) ? *sY + (++draw) : *sY; break;      // ▼
    case 'D' : *sX = (*sX < width) ? *sX + (++draw) : *sX; break;       // ▶︎
    case 'K' : *sY = (*sY > 0) ? *sY - (++draw) : 0; break;             // ▲  vi draw
    case 'H' : *sX = (*sX > 0) ? *sX - (++draw) : 0; break;             // ◀︎
    case 'J' : *sY = (*sY < height) ? *sY + (++draw) : *sY; break;      // ▼
    case 'L' : *sX = (*sX < width) ? *sX + (++draw) : *sX; break;       // ▶︎
    case 'r' : [[fallthrough]];                                         // --red    colors
    case 'g' : [[fallthrough]];                                         // --green
    case 'b' : [[fallthrough]];                                         // --blue
    case 'R' : [[fallthrough]];                                         // ++red
    case 'G' : [[fallthrough]];                                         // ++green
    case 'B' : setColor(color, &ev->ch); goto bypass;                   // ++blue
    case '1' : *c = &arr[0]; goto bypass;                               //    char selection
    case '2' : *c = &arr[1]; goto bypass;                               // ░
    case '3' : *c = &arr[2]; goto bypass;                               // ▒
    case '4' : *c = &arr[3]; goto bypass;                               // ▓
    case '5' : *c = &arr[4]; goto bypass;                               // █
    case '6' : *c = &arr[5]; goto bypass;                               // ▀
    case '7' : *c = &arr[6]; goto bypass;                               // ▔
    case '8' : *c = &arr[7]; goto bypass;                               // ▁
    case '9' : *c = &arr[8]; goto bypass;                               // ▄
    case '0' : *c = &arr[9]; goto bypass;                               // ◼
    case 'Q' : [[fallthrough]];                                         // tell clang fallthrough intentional
    case 'q' : *c != &arr[0] ? (*c -= 1) : (*c = &arr[9]); goto bypass; // choose block to left in palette
    case 'E' : [[fallthrough]];                                         //
    case 'e' : *c != &arr[9] ? (*c += 1) : (*c = &arr[0]); goto bypass; // choose block to right in palette
    case TB_KEY_MOUSE_RIGHT :                                           // movement without draw
      *sX = (uint16_t) ev->x;
      *sY = (uint16_t) ev->y;
      break; // draw to left, right, top, or lower boundary
    case TB_KEY_HOME : hLine(*sX, *sY, color->rgb, BLACK, **c, 0); goto bypass; // ◀︎
    case TB_KEY_END  : hLine(*sX, *sY, color->rgb, BLACK, **c, 1); goto bypass; // ▶︎
    case TB_KEY_PGUP : vLine(*sX, *sY, color->rgb, BLACK, **c, 0); goto bypass; // ▲
    case TB_KEY_PGDN : vLine(*sX, *sY, color->rgb, BLACK, **c, 1); goto bypass; // ▼
  }
  // set cursor position and draw
  tb_set_cursor(*sX, *sY);
  if (draw == 1) { tb_set_cell(*sX, *sY, **c, color->rgb, BLACK); }
bypass:; // jump if no draw required
}
// increment value of RGB color channels until wrap to 0
void setColor(Color *restrict color, uint32_t *restrict ch) {
  switch (*ch) {
    // decrease color
    case 'r' : color->r -= 8; break;
    case 'g' : color->g -= 8; break;
    case 'b' : color->b -= 8; break;
    // increase color
    case 'R' : color->r += 8; break;
    case 'G' : color->g += 8; break;
    case 'B' : color->b += 8; break;
    default  : break;
  }
}
// draw horizontal line
void hLine(uint16_t x, uint16_t y, uint32_t fgCol, uint32_t bgCol, wchar_t c, uint8_t dir) {
  const uint16_t width = (uint16_t) tb_width();
  if (dir == 1) {
    // draw line left to right
    for (; x < width; ++x) { tb_set_cell(x, y, c, fgCol, bgCol); }
  } else {
    // draw line right to left
    while (x--) { tb_set_cell(x, y, c, fgCol, bgCol); }
  }
}
// draw a vertical line
void vLine(uint16_t x, uint16_t y, uint32_t fgCol, uint32_t bgCol, wchar_t c, uint8_t dir) {
  const uint16_t height = (uint16_t) tb_height() - 1;
  if (dir == 1) {
    // draw line downward
    for (; y < height; ++y) { tb_set_cell(x, y, c, fgCol, bgCol); }
  } else {
    // draw line upward
    while (y--) { tb_set_cell(x, y, c, fgCol, bgCol); }
  }
}
// draw palette characters at screen bottom
void drawPalette(const wchar_t *restrict arr, const uint8_t len, const wchar_t *restrict c) {
  static uint32_t cfg = fg_UI;
  const uint16_t  y   = (uint16_t) tb_height() - 1;
  // draw UI bottom bar.  in case of mouse draw over UI
  hLine(0, y, bg_UI, BLACK, ' ', 1);
  // iterate over palette array.  dim unselected blocks
  for (uint16_t i = 0; i <= len; ++i) {
    cfg = ((arr[i] == *c) ? 0x10000000 | fg_UI : (uint32_t) (fg_UI * 0.5));
    tb_set_cell(i, y, arr[i], cfg, BLACK);
  }
}
// draw current color settings
void drawColorStatus(const Color *restrict color) {
  const uint16_t xloc = (uint16_t) (tb_width() * 0.5);
  const uint16_t yloc = (uint16_t) (tb_height() - 1);
  // hashtag shows current color
  tb_set_cell(xloc - 5, yloc, '#', 0x01000000 | color->rgb, BLACK);
  // print RGB hex values.  screen bottom, and centered
  for (uint8_t l = 0, j = 0; j < 3; l += 2, j++) {
    tb_printf(xloc - l, yloc, (uintattr_t) fg_UI, BLACK, "%02X", color->rgbArr[j]);
  }
}

