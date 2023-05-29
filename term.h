#include "termbox2.h"
#include <assert.h>

typedef struct {
  const uint16_t spaceblock ;
  const uint16_t shadeLo    ;
  const uint16_t shadeMed   ;
  const uint16_t shadeHi    ;
  const uint16_t full       ;
  const uint16_t blockTop   ;
  const uint16_t blockHiThin;
  const uint16_t blockLoThin;
  const uint16_t blockBtm   ;
  const uint16_t midBlock   ;
  const uint16_t tallHalfL  ;
  const uint16_t tallThinL  ;
  const uint16_t smUpperR   ;
  const uint16_t smLoL      ;
  const uint16_t smLoR      ;
  const uint16_t smUpperL   ;
  const uint16_t tallThinR  ;
  const uint16_t tallHalfR  ;
} const blockstruct;

typedef union {
  const blockstruct stp;
  const uint16_t arr[18];
} const slots;


typedef union {
  uint32_t rgb : 32;
  struct {
    uint8_t b: 8;
    uint8_t g: 8;
    uint8_t r: 8;
    uint8_t x: 8;
  } __attribute__((packed));
} Color;


/////////////////////////////////////////////////
// function declarations ////////////////////////

// uint32_t setColor(uint32_t *color, uint32_t ch);

void setColor(Color *color, uint32_t ch);

// draw a horizontal line
void hLine(uint16_t x, uint16_t y, uint32_t fgCol, uint32_t bgCol, uint16_t c);

// draw a vertical line
void vLine(uint16_t x, uint16_t y, uint32_t fgCol, uint32_t bgCol, uint16_t c); 

// draw palette characters at screen bottom
void drawPalette(const uint16_t array[],
    uint16_t len,
    const uint16_t * c);

// rand uint16_t in range
const uint32_t randRange(const uint32_t min, const uint32_t max);

// box with fill
void boxFill(
    uint16_t x,
    uint16_t y,
    uint16_t c,
    const uint16_t w,
    const uint16_t h,
    uint32_t colorfg,
    uint32_t colorbg
    ); 

// rand cell in bounds
void randCell(const uint16_t quant, const uint16_t size, uint16_t c);

// draw a line at random
void randLine(void);

