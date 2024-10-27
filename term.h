#include "termbox2/termbox2.h"

#define fg_UI   0x00808080
#define bg_UI   0x00101010
#define BLACK   TB_HI_BLACK // use termbox definition of black for portability
#define WHITE   0x00F0F0F0  // off-white
#define fg_RGB  0x00C0C0C0  // RGB status foreground color
#define bg_R    0x00401010  // RGB status foreground color
#define bg_G    0x00104010  // RGB status foreground color
#define bg_B    0x00101040  // RGB status foreground color
#define UI      0           // layer for UI elements
#define CANVAS  1           // layer for drawing buffer
#define bwidth  100         // drawing buffer width
#define bheight 71          // drawing buffer height
#define palSize 10          // size of the block character palette array

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
  blocks   stp;
  uint16_t arr[ 10 ];
} Slots;

typedef union {
  uint32_t rgb : 32;
  uint8_t  rgbArr[ 4 ];
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

typedef struct {
  Pixel canvas[ bwidth ][ bheight ];
  Pixel ui[ bwidth ];
} Layer;

// print custom canvas buffer
static void printBuffer(Pixel buffer[ bwidth ][ bheight ], const uint16_t *restrict arr);

static void printFileUTF(Pixel buffer[ bwidth ][ bheight ]);

static void openFileUTF(Pixel buffer[ bwidth ][ bheight ]);

// inputs
void checkInput(struct tb_event *restrict ev, Color *restrict color, uint8_t *select,
                const uint16_t *restrict arr, uint8_t *restrict sX, uint8_t *restrict sY, Layer *layer);

// increment value of RGB color channels until wrap to 0
static void setColor(Color *restrict color, uint32_t *restrict ch);

// draw a horizontal line
static void hLine(uint8_t which, Layer *layer, uint8_t x, uint8_t y, uint32_t fgCol, uint32_t bgCol,
                  uint8_t select, const uint16_t *restrict arr, uint8_t dir);

// draw a vertical line
static void vLine(uint8_t which, Layer *layer, uint8_t x, uint8_t y, uint32_t fgCol, uint32_t bgCol,
                  uint8_t select, const uint16_t *restrict arr, uint8_t dir);

// draw palette characters at screen bottom
static void drawPalette(Layer *layer, const uint16_t *restrict arr, const uint8_t len,
                        const uint8_t *restrict select);

// draw current color settings
static void drawColorStatus(const Color *restrict color);

// draw current cursor position
static void drawXYStatus(const uint8_t *restrict sX, const uint8_t *restrict sY);

void drawUI(Layer *layer, const uint16_t *restrict arr, const uint8_t len, const uint8_t *restrict select,
            const Color *restrict color, const uint8_t *restrict sX, const uint8_t *restrict sY);
/*
// open canvas file
void openFile(Pixel buffer[ bwidth ][ bheight ], const uint16_t *restrict arr);

// print custom canvas file
void printFile(Pixel buffer[ bwidth ][ bheight ], const uint16_t *restrict arr);
*/

