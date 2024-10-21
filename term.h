#include "termbox2/termbox2.h"

#define fg_UI  0x00808080
#define bg_UI  0x00101010
#define BLACK  TB_HI_BLACK // use termbox definition of black for portability
#define WHITE  0x00F0F0F0  // off-white
#define fg_RGB 0x00C0C0C0  // RGB status foreground color
#define bg_R   0x00401010  // RGB status foreground color
#define bg_G   0x00104010  // RGB status foreground color
#define bg_B   0x00101040  // RGB status foreground color
#define UI     0
#define CANVAS 1

typedef struct {
  wchar_t space;
  wchar_t shadL;
  wchar_t shadM;
  wchar_t shadH;
  wchar_t fullH;
  wchar_t blkTp;
  wchar_t blkHi;
  wchar_t blkLo;
  wchar_t blkBt;
  wchar_t blkMd;
} blocks;

typedef union {
  blocks  stp;
  wchar_t arr[10];
} Slots;

typedef union {
  uint32_t rgb : 32;
  uint8_t  rgbArr[4];
  struct {
    uint8_t b : 8;
    uint8_t g : 8;
    uint8_t r : 8;
    uint8_t x : 8;
  };
} Color;

typedef union {
  uint32_t cell : 32;
  struct {
    uint32_t rgb   : 24;
    uint8_t  block : 8;
  };
} Pixel;

// print custom canvas buffer
void printBuffer(Pixel buffer[256][256], const wchar_t *restrict arr);

// inputs
void checkInput(struct tb_event *restrict ev, Color *restrict color, uint8_t *select,
                const wchar_t *restrict arr, uint8_t *restrict sX, uint8_t *restrict sY,
                Pixel buffer[256][256]);

// increment value of RGB color channels until wrap to 0
void setColor(Color *restrict color, uint32_t *restrict ch);

// draw a horizontal line
void hLine(uint8_t layer, Pixel buffer[256][256], uint8_t x, uint8_t y, uint32_t fgCol, uint32_t bgCol,
           uint8_t select, const wchar_t *restrict arr, uint8_t dir);

// draw a vertical line
void vLine(uint8_t layer, Pixel buffer[256][256], uint8_t x, uint8_t y, uint32_t fgCol, uint32_t bgCol,
           uint8_t select, const wchar_t *restrict arr, uint8_t dir);

// draw palette characters at screen bottom
void drawPalette(Pixel buffer[256][256], const wchar_t *restrict arr, const uint8_t len,
                 const uint8_t *restrict select);

// draw current color settings
void drawColorStatus(const Color *restrict color);

