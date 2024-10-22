#include "term.h"
#define UP    TB_KEY_ARROW_UP
#define LEFT  TB_KEY_ARROW_LEFT
#define DOWN  TB_KEY_ARROW_DOWN
#define RIGHT TB_KEY_ARROW_RIGHT
// print custom canvas buffer
void printBuffer(Pixel buffer[bwidth][bheight], const uint16_t *restrict arr) {
  for (uint8_t x = 0; (x < bwidth); x++) {
    for (uint8_t y = 0; (y < bheight); y++) {
      tb_set_cell(x, y, arr[buffer[x][y].block], buffer[x][y].rgb, BLACK);
    }
  }
}
// print custom canvas file
void printFile(Pixel buffer[bwidth][bheight], const uint16_t *restrict arr) {
  uint16_t temp[bwidth][bheight];
  FILE    *f = fopen("drawing.txt", "w");
  // TODO: bug - saved array is rotated clockwise
  // TODO: write open file function
  for (uint8_t x = 0; (x < bwidth); x++) {
    for (uint8_t y = 0; (y < bheight); y++) {
      temp[x][y] = arr[buffer[x][y].block];
      if (y == bheight - 1) { temp[x][y] = 10; } // insert newline \n at the end of each row
    }
  }
  fwrite(temp, sizeof(uint16_t), bwidth * bheight, f);
  fclose(f);
}
// inputs
void checkInput(struct tb_event *restrict ev, Color *restrict color, uint8_t *select,
                const uint16_t *restrict arr, uint8_t *restrict sX, uint8_t *restrict sY,
                Pixel buffer[bwidth][bheight]) {
  const uint32_t key = (ev->ch > 0) ? ev->ch : (uint32_t) ev->key;
  // flag when a draw is needed
  uint8_t draw = 0;
  // setup boundaries for movement, and check within boundary after resize
  const uint8_t width  = (tb_width() < bwidth) ? (uint8_t) (tb_width() - 1) : bwidth - 1;
  const uint8_t height = (tb_height() < bheight) ? (uint8_t) (tb_height() - 2) : bheight - 2;
  // check we are within boundary
  *sX = (*sX < bwidth) ? *sX : bwidth - 1;
  *sY = (*sY < bheight) ? *sY : bheight - 2;
  switch (key) {
    // movement and paints character
    case TB_KEY_MOUSE_LEFT :
      *sX = (ev->x < bwidth) ? (uint8_t) ev->x : bwidth - 1;
      *sY = (ev->y < bheight) ? (uint8_t) ev->y : bheight - 2;
      ++draw;
      break;
      // movement only, but must draw to paint cursor position
    case UP    : [[fallthrough]];                                  // ▲
    case 'k'   : [[fallthrough]];                                  // ▲
    case 'w'   : *sY = (*sY > 0) ? *sY - 1 : height; break;        // ▲
    case LEFT  : [[fallthrough]];                                  // ◀︎
    case 'h'   : [[fallthrough]];                                  // ◀︎
    case 'a'   : *sX = (*sX > 0) ? *sX - 1 : width; break;         // ◀︎
    case DOWN  : [[fallthrough]];                                  // ▼
    case 'j'   : [[fallthrough]];                                  // ▼
    case 's'   : *sY = (*sY < height) ? *sY + 1 : 0; break;        // ▼
    case RIGHT : [[fallthrough]];                                  // ▶︎
    case 'd'   : [[fallthrough]];                                  // ▶︎
    case 'l'   : *sX = (*sX < width) ? *sX + 1 : 0; break;         // ▶︎
    case 'W'   : *sY = (*sY > 0) ? *sY - (++draw) : height; break; // ▲  wasd draw
    case 'A'   : *sX = (*sX > 0) ? *sX - (++draw) : width; break;  // ◀︎
    case 'S'   : *sY = (*sY < height) ? *sY + (++draw) : 0; break; // ▼
    case 'D'   : *sX = (*sX < width) ? *sX + (++draw) : 0; break;  // ▶︎
    case 'K'   : *sY = (*sY > 0) ? *sY - (++draw) : height; break; // ▲  vi draw
    case 'H'   : *sX = (*sX > 0) ? *sX - (++draw) : width; break;  // ◀︎
    case 'J'   : *sY = (*sY < height) ? *sY + (++draw) : 0; break; // ▼
    case 'L'   : *sX = (*sX < width) ? *sX + (++draw) : 0; break;  // ▶︎
    case 'r'   : [[fallthrough]];                                  // --red    colors
    case 'g'   : [[fallthrough]];                                  // --green
    case 'b'   : [[fallthrough]];                                  // --blue
    case 'R'   : [[fallthrough]];                                  // ++red
    case 'G'   : [[fallthrough]];                                  // ++green
    case 'B'   : setColor(color, &ev->ch); goto bypass;            // ++blue
    case '1'   : *select = 0; goto bypass;                         //    char selection
    case '2'   : *select = 1; goto bypass;                         // ░
    case '3'   : *select = 2; goto bypass;                         // ▒
    case '4'   : *select = 3; goto bypass;                         // ▓
    case '5'   : *select = 4; goto bypass;                         // █
    case '6'   : *select = 5; goto bypass;                         // ▀
    case '7'   : *select = 6; goto bypass;                         // ▔
    case '8'   : *select = 7; goto bypass;                         // ▁
    case '9'   : *select = 8; goto bypass;                         // ▄
    case '0'   : *select = 9; goto bypass;                         // ◼
    case 'Q'   : [[fallthrough]];                                  // tell clang fallthrough intentional
    case 'q'   : *select != 0 ? (*select -= 1) : (*select = 9); goto bypass; // choose block to left in palette
    case 'E'   : [[fallthrough]];                                            //
    case 'e'   : *select != 9 ? (*select += 1) : (*select = 0); goto bypass;   // choose block to right in palette
    case TB_KEY_MOUSE_RIGHT :                                                // movement without draw
      *sX = (ev->x < bwidth) ? (uint8_t) ev->x : bwidth - 1;
      *sY = (ev->y < bheight) ? (uint8_t) ev->y : bheight - 2;
      break; // draw to left, right, top, or lower boundary
    case TB_KEY_HOME : hLine(CANVAS, buffer, *sX, *sY, color->rgb, BLACK, *select, arr, 0); goto bypass; // ◀︎
    case TB_KEY_END  : hLine(CANVAS, buffer, *sX, *sY, color->rgb, BLACK, *select, arr, 1); goto bypass; // ▶︎
    case TB_KEY_PGUP : vLine(CANVAS, buffer, *sX, *sY, color->rgb, BLACK, *select, arr, 0); goto bypass; // ▲
    case TB_KEY_PGDN : vLine(CANVAS, buffer, *sX, *sY, color->rgb, BLACK, *select, arr, 1); goto bypass; // ▼
    case 'p'         : printFile(buffer, arr);
  }
  // set cursor position and draw
  tb_set_cursor(*sX, *sY);
  if (draw == 1) {
    buffer[*sX][*sY].rgb   = color->rgb;
    buffer[*sX][*sY].block = *select;
    printBuffer(buffer, arr);
  }
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
void hLine(uint8_t layer, Pixel buffer[bwidth][bheight], uint8_t x, uint8_t y, uint32_t fgCol, uint32_t bgCol,
           uint8_t select, const uint16_t *restrict arr, uint8_t dir) {
  const uint8_t width = (tb_width() < bwidth) ? (uint8_t) tb_width() : bwidth;
  if (dir == 1) {
    // draw line left to right
    for (; x < width; ++x) {
      // draw to buffer
      if (layer == 1) {
        buffer[x][y].rgb   = fgCol;
        buffer[x][y].block = select;
      }
      // draw to terminal only  (UI elements)
      tb_set_cell(x, y, arr[select], fgCol, bgCol);
    }
  } else {
    // draw line right to left
    while (x--) {
      // draw to buffer
      if (layer == 1) {
        buffer[x][y].rgb   = fgCol;
        buffer[x][y].block = select;
      }
      tb_set_cell(x, y, arr[select], fgCol, bgCol);
    }
  }
}
// draw a vertical line
void vLine(uint8_t layer, Pixel buffer[bwidth][bheight], uint8_t x, uint8_t y, uint32_t fgCol, uint32_t bgCol,
           uint8_t select, const uint16_t *restrict arr, uint8_t dir) {
  const uint8_t height = (tb_height() < bheight) ? (uint8_t) tb_height() : bheight - 1;
  if (dir == 1) {
    // draw line left to right
    for (; y < height; ++y) {
      // draw to buffer
      if (layer == 1) {
        buffer[x][y].rgb   = fgCol;
        buffer[x][y].block = select;
      }
      // draw to terminal only  (UI elements)
      tb_set_cell(x, y, arr[select], fgCol, bgCol);
    }
  } else {
    // draw line right to left
    while (y--) {
      // draw to buffer
      if (layer == 1) {
        buffer[x][y].rgb   = fgCol;
        buffer[x][y].block = select;
      }
      // draw to terminal only  (UI elements)
      tb_set_cell(x, y, arr[select], fgCol, bgCol);
    }
  }
}
// draw palette characters at screen bottom
void drawPalette(Pixel buffer[bwidth][bheight], const uint16_t *restrict arr, const uint8_t len,
                 const uint8_t *restrict select) {
  static uint32_t cfg = fg_UI;
  const uint8_t   y   = (tb_height() < bheight) ? (uint8_t) tb_height() - 1 : bheight - 1;
  // draw UI bottom bar.  in case of mouse draw over UI
  hLine(UI, buffer, 0, y, BLACK, BLACK, *select, arr, 1);
  // iterate over palette array.  dim unselected blocks
  for (uint8_t i = 0; i <= len; ++i) {
    cfg = ((arr[i] == arr[*select]) ? 0x10000000 | fg_UI : (uint32_t) (fg_UI * 0.5));
    tb_set_cell(i, y, arr[i], cfg, BLACK);
  }
}
// draw current color settings
void drawColorStatus(const Color *restrict color) {
  const uint8_t xloc = (tb_width() < bwidth) ? (uint8_t) (tb_width() * 0.5) : (uint8_t) (bwidth * 0.5);
  const uint8_t yloc = (tb_height() < bheight) ? (uint8_t) (tb_height() - 1) : bheight - 1;
  // hashtag shows current color
  tb_set_cell(xloc - 5, yloc, '#', 0x01000000 | color->rgb, BLACK);
  // print RGB hex values.  screen bottom, and centered
  for (uint8_t l = 0, j = 0; j < 3; l += 2, j++) {
    tb_printf(xloc - l, yloc, (uintattr_t) fg_UI, BLACK, "%02X", color->rgbArr[j]);
  }
}

