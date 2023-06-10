#include "termbox2.h"

#define fg_UI  0x808080
#define bg_UI  0x101010
#define BLACK  0x20000000  // true color black
#define WHITE  0xF0F0F0    // off-white
#define fg_RGB 0xC0C0C0    // RGB status foreground color
#define bg_R   0x401010    // RGB status foreground color
#define bg_G   0x104010    // RGB status foreground color
#define bg_B   0x101040    // RGB status foreground color

typedef struct {
  uint16_t space;
  uint16_t shadL;
  uint16_t shadM;
  uint16_t shadH;
  uint16_t fullH;
  uint16_t blkTp;
  uint16_t blkHi;
  uint16_t blkLo;
  uint16_t blkBt;
  uint16_t blkMd;
} blocks;

typedef union {
  blocks stp;
  uint16_t arr[10];
} Slots;

typedef union {
  uint32_t rgb : 32;
  struct {
    uint8_t b : 8;
    uint8_t g : 8;
    uint8_t r : 8;
    uint8_t x : 8;
  };
} Color;

// inputs
void checkInput( struct tb_event *ev, Color *color, const uint16_t **c, const blocks *const st,
                 uint8_t *sX, uint8_t *sY );

// increment value of RGB color channels until wrap to 0
void setColor( Color *restrict color, uint32_t *restrict c );

// draw a horizontal line
void hLine( uint16_t x, uint16_t y, uint32_t fgCol, uint32_t bgCol, uint16_t c, uint8_t dir );

// draw a vertical line
void vLine( uint16_t x, uint16_t y, uint32_t fgCol, uint32_t bgCol, uint16_t c, uint8_t dir );

// draw palette characters at screen bottom
void drawPalette( const uint16_t *restrict array, const uint8_t len, const uint16_t *restrict c );

// draw current color settings
void drawColorStatus( const Color *restrict color, const uint16_t *array );

// rand uint16_t in range
const uint32_t randRange( const uint32_t min, const uint32_t max );

