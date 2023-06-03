#include "termbox2.h"

#define fg_UI  0x808080
#define bg_UI  0x101010
#define BLACK  0x20000000 // true color black
#define WHITE  0xF0F0F0   // off-white
#define fg_RGB 0xC0C0C0   // RGB status foreground color
#define bg_R   0x401010   // RGB status foreground color
#define bg_G   0x104010   // RGB status foreground color
#define bg_B   0x101040   // RGB status foreground color

typedef struct {
  const uint16_t spaceblock;
  const uint16_t shadeLo;
  const uint16_t shadeMed;
  const uint16_t shadeHi;
  const uint16_t full;
  const uint16_t blockTop;
  const uint16_t blockHiThin;
  const uint16_t blockLoThin;
  const uint16_t blockBtm;
  const uint16_t midBlock;
  const uint16_t tallHalfL;
  const uint16_t tallThinL;
  const uint16_t smUpperR;
  const uint16_t smLoL;
  const uint16_t smLoR;
  const uint16_t smUpperL;
  const uint16_t tallThinR;
  const uint16_t tallHalfR;
} const blocks;

typedef union {
  const blocks stp;
  const uint16_t arr[18];
} const slots;

typedef union {
  uint32_t rgb : 32;
  struct {
    uint8_t b : 8;
    uint8_t g : 8;
    uint8_t r : 8;
    uint8_t x : 8;
  };
} Color;

// increment value of RGB color channels until wrap to 0
void setColor( Color *color, uint32_t ch );

// draw a horizontal line
void hLine( uint16_t x, uint16_t y, uint32_t fgCol, uint32_t bgCol, uint16_t c );

// draw a vertical line
void vLine( uint16_t x, uint16_t y, uint32_t fgCol, uint32_t bgCol, uint16_t c );

// draw palette characters at screen bottom
void drawPalette( const uint16_t array[], uint16_t len, const uint16_t *c );

// draw current color settings
void drawColorStatus( const Color *color, const uint16_t array[] );

// rand uint16_t in range
const uint32_t randRange( const uint32_t min, const uint32_t max );

