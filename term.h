#define TB_OPT_ATTR_W 32
#include "termbox2/termbox2.h"

#define fg_UI   0x00808080
#define bg_UI   0x00101010
#define BLACK   TB_HI_BLACK // use termbox definition of black for portability
#define WHITE   0x00F0F0F0  // off-white
#define fg_RGB  0x00C0C0C0  // RGB status foreground color
#define GUTTER  0x00202020  // right gutter foreground color
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

// current foreground color: 32bit value
// leftmost bits enable features like blink.
// 0x20000000 is true black
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

// TODO: add --flag for user defined width / height
typedef struct {
  Pixel canvas[ bwidth ][ bheight ];
} Layer;

typedef struct {
  Pixel canvas[ bwidth ][ 1 ];
} UIBottom;

// print custom canvas buffer
static void printBuffer(Layer *restrict layer, const uint16_t *restrict arr);

static void printFileUTF(Layer *restrict layer);

static void openFileUTF(Layer *restrict layer);

// find the highest of 3 integers
// find which RGB channel is highest value
uint8_t max3(const Color *restrict color);

// find which RGB channels is lowest value
uint8_t min3(const Color *restrict color);

// inputs
[[gnu::hot]]
void checkInput(struct tb_event *restrict ev, Color *restrict color, uint8_t *select,
                const uint16_t *restrict arr, uint8_t *restrict sX, uint8_t *restrict sY,
                Layer *restrict layer);

// increment value of RGB color channels until wrap to 0
static void setColor(Color *restrict color, uint32_t *restrict ch);

// draw a horizontal line
static void hLine(uint8_t which, Layer *restrict layer, uint16_t x, uint16_t y, uint32_t fgCol, uint32_t bgCol,
                  uint8_t sel, const uint16_t *restrict arr, uint8_t dir);

// draw a vertical line
static void vLine(uint8_t which, Layer *restrict layer, uint16_t x, uint16_t y, uint32_t fgCol, uint32_t bgCol,
                  uint8_t sel, const uint16_t *restrict arr, uint8_t dir);

// draw palette characters at screen bottom
static void drawPalette(Layer *restrict layer, const uint16_t *restrict arr, const uint8_t len,
                        const uint8_t *restrict select);

// draw current color settings
static void drawColorStatus(const Color *restrict color);

// draws current cursor location in bottom right corner
static void drawXYStatus(Layer *restrict layer, const uint16_t *restrict arr, const uint8_t *restrict sX,
                         const uint8_t *restrict sY);
void        drawUI(Layer *restrict layer, const uint16_t *restrict arr, const uint8_t len,
                   const uint8_t *restrict select, const Color *restrict color, const uint8_t *restrict sX,
                   const uint8_t *restrict sY);

