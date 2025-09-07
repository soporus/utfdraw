#include "term.h"
#define UP    TB_KEY_ARROW_UP
#define LEFT  TB_KEY_ARROW_LEFT
#define DOWN  TB_KEY_ARROW_DOWN
#define RIGHT TB_KEY_ARROW_RIGHT

// -----------------------------------------------------------
// TODO:  Reorganize source tree with bin and source directory
// -----------------------------------------------------------

// print custom canvas buffer
inline static void printBuffer( Layer *restrict layer, const uint16_t *restrict arr ) {
  for ( uint8_t x = 0; ( x < bwidth ); x++ ) {
    for ( uint8_t y = 0; ( y < bheight ); y++ ) {
      // In TrueColor mode 0x000000 is Default color, usually off-white
      // Use 0x20000000 for TrueColor black, defined as TB_HI_BLACK
      tb_set_cell( x, y, arr[ layer->canvas[ x ][ y ].block ],
                   // swap to TB_HI_BLACK color if 0x000000
                   ( layer->canvas[ x ][ y ].rgb == 0x0000000 ) ? TB_HI_BLACK : layer->canvas[ x ][ y ].rgb,
                   BLACK );
    }
  }
}
// print custom canvas file
static void printFileUTF( Layer *restrict layer ) {
  FILE *f = fopen( "drawing.utf", "w" );
  if ( f == NULL ) { return; }
  fwrite( layer, sizeof( uint32_t ), bwidth * bheight, f );
  fclose( f );
}
// print custom canvas file
static void openFileUTF( Layer *restrict layer ) {
  FILE *f = fopen( "drawing.utf", "r" );
  if ( f == NULL ) { return; }
  fread( &layer->canvas[ 0 ][ 0 ], 4, bwidth * bheight, f );
  fclose( f );
}
// inputs
[[gnu::hot]]
inline void checkInput( struct tb_event *restrict ev, Color *restrict color, uint8_t *select,
                        const uint16_t *restrict arr, uint8_t *restrict sX, uint8_t *restrict sY,
                        Layer *restrict layer ) {
  // flag when a screen draw is needed
  uint8_t draw = 0;
  // finds the bottom edge of canvas.  If UI overlaps canvas or not.
  uint8_t palOffset = ( tb_height() > bheight ) ? 1 : 2;
  // setup boundaries for movement, and check within boundary after resize
  const uint8_t width  = ( tb_width() < bwidth ) ? (uint8_t) ( tb_width() - 1 ) : bwidth - 1;
  const uint8_t height = ( tb_height() < bheight ) ? (uint8_t) ( tb_height() - 2 ) : bheight - palOffset;
  *sX                  = ( *sX < width ) ? *sX : width;
  *sY                  = ( *sY < height ) ? *sY : height;
  // check if input is normal character or jump to check for special keys
  if ( ev->ch > 0 ) {
    switch ( ev->ch ) {
      // movement only, but must draw to paint cursor position
      case 'k' : [[fallthrough]];                                          // ▲
      case 'w' : *sY = ( *sY > 0 ) ? *sY - 1 : height; break;              // ▲
      case 'h' : [[fallthrough]];                                          // ◀︎
      case 'a' : *sX = ( *sX > 0 ) ? *sX - 1 : width; break;               // ◀︎
      case 'j' : [[fallthrough]];                                          // ▼
      case 's' : *sY = ( *sY < height ) ? *sY + 1 : 0; break;              // ▼
      case 'd' : [[fallthrough]];                                          // ▶︎
      case 'l' : *sX = ( *sX < width ) ? *sX + 1 : 0; break;               // ▶︎
      case 'W' : *sY = ( *sY > 0 ) ? *sY - ( ++draw ) : height; break;     // ▲  wasd draw
      case 'A' : *sX = ( *sX > 0 ) ? *sX - ( ++draw ) : width; break;      // ◀︎
      case 'S' : *sY = ( *sY < height ) ? *sY + ( ++draw ) : 0; break;     // ▼
      case 'D' : *sX = ( *sX < width ) ? *sX + ( ++draw ) : 0; break;      // ▶︎
      case 'K' : *sY = ( *sY > 0 ) ? *sY - ( ++draw ) : height; break;     // ▲  vi draw
      case 'H' : *sX = ( *sX > 0 ) ? *sX - ( ++draw ) : width; break;      // ◀︎
      case 'J' : *sY = ( *sY < height ) ? *sY + ( ++draw ) : 0; break;     // ▼
      case 'L' : *sX = ( *sX < width ) ? *sX + ( ++draw ) : 0; break;      // ▶︎
      case 'r' : [[fallthrough]];                                          // --red    colors
      case 'g' : [[fallthrough]];                                          // --green
      case 'b' : [[fallthrough]];                                          // --blue
      case 'R' : [[fallthrough]];                                          // ++red
      case 'G' : [[fallthrough]];                                          // ++green
      case 'B' : setColor( color, &ev->ch ); break;                        // ++blue
      case '0' : *select = 0; break;                                       // ░  char selection
      case '1' : *select = 1; break;                                       // ▒
      case '2' : *select = 2; break;                                       // ▓
      case '3' : *select = 3; break;                                       // █
      case '4' : *select = 4; break;                                       // ▀
      case '5' : *select = 5; break;                                       // ▔
      case '6' : *select = 6; break;                                       // ▁
      case '7' : *select = 7; break;                                       // ▄
      case '8' : *select = 8; break;                                       // ◼
      case '9' : *select = 9; break;                                       // ' '
      case 'Q' : [[fallthrough]];                                          // tell clang fallthrough intentional
      case 'q' : *select != 0 ? ( *select -= 1 ) : ( *select = 9 ); break; // choose block to left in palette
      case 'E' : [[fallthrough]];
      case 'e' : *select != 9 ? ( *select += 1 ) : ( *select = 0 ); break; // choose block to right in palette
      case '_' : [[fallthrough]];
      case '-' : setColor( color, &ev->ch ); break;
      case '=' : [[fallthrough]];
      case '+' : setColor( color, &ev->ch ); break;
      // draw to left, right, top, or lower boundary
      case 'Y' : *select = layer->canvas[ *sX ][ *sY ].block;         // choose block and fallthrough to
      case 'y' : color->rgb = layer->canvas[ *sX ][ *sY ].rgb; break; // choose color
    }
  } else {
    switch ( ev->key ) {
      // movement and paints character
      case TB_KEY_MOUSE_LEFT :
        *sX = ( ev->x < width ) ? (uint8_t) ev->x : width;
        *sY = ( ev->y < height ) ? (uint8_t) ev->y : height;
        ++draw;
        break;
      case TB_KEY_MOUSE_RIGHT : // movement without draw
        *sX = ( ev->x < width ) ? (uint8_t) ev->x : width;
        *sY = ( ev->y < height ) ? (uint8_t) ev->y : height;
        break;
      // movement only, but must draw to paint cursor position
      case UP   : *sY = ( *sY > 0 ) ? *sY - 1 : height; break; // ▲
      case LEFT : *sX = ( *sX > 0 ) ? *sX - 1 : width; break;  // ◀︎
      case DOWN : *sY = ( *sY < height ) ? *sY + 1 : 0; break; // ▼
      case RIGHT :
        *sX = ( *sX < width ) ? *sX + 1 : 0;
        break; // ▶︎
      // draw lines to edge of canvas
      case TB_KEY_HOME : hLine( CANVAS, layer, *sX, *sY, color->rgb, BLACK, *select, arr, 0 ); break; // ◀︎
      case TB_KEY_END  : hLine( CANVAS, layer, *sX, *sY, color->rgb, BLACK, *select, arr, 1 ); break; // ▶︎
      case TB_KEY_PGUP : vLine( CANVAS, layer, *sX, *sY, color->rgb, BLACK, *select, arr, 0 ); break; // ▲
      case TB_KEY_PGDN :
        vLine( CANVAS, layer, *sX, *sY, color->rgb, BLACK, *select, arr, 1 );
        break; // ▼
      // write buffer to file
      case TB_KEY_CTRL_P : printFileUTF( layer ); break;
      // open file to buffer
      case TB_KEY_CTRL_O : openFileUTF( layer ); break;
    }
  }
  // set cursor position and draw
  tb_set_cursor( *sX, *sY );
  if ( draw == 1 ) {
    layer->canvas[ *sX ][ *sY ].cell  = color->rgb;
    layer->canvas[ *sX ][ *sY ].block = *select;
  }
  // print the draw layer
  printBuffer( layer, arr );
}

// find highest value of RGB channels
static const uint8_t max3( const Color *restrict color ) {
  return color->r > color->g ? ( color->r > color->b ? color->r : color->b ) :
                               ( color->g > color->b ? color->g : color->b );
}
// find lowest value of RGB channels
static const uint8_t min3( const Color *restrict color ) {
  return color->r < color->g ? ( color->r < color->b ? color->r : color->b ) :
                               ( color->g < color->b ? color->g : color->b );
}
// increment value of RGB color channels until wrap to 0
static void setColor( Color *restrict color, uint32_t *restrict ch ) {
  uint8_t *limit = alloca( sizeof *limit );
  switch ( *ch ) {
    // decrease color
    case 'r' : color->r -= 5; break;
    case 'g' : color->g -= 5; break;
    case 'b' : color->b -= 5; break;
    // increase color
    case 'R' : color->r += 5; break;
    case 'G' : color->g += 5; break;
    case 'B' : color->b += 5; break;
    // adjust brightness
    // preserve color balance.  find max and min value, as relative floor and ceiling
    case '_' : [[fallthrough]];
    case '-' :
      *limit   = min3( color );
      color->r = ( color->r >= 5 + ( color->r - *limit ) ) ? color->r - 5 : 0 + ( color->r - *limit );
      color->g = ( color->g >= 5 + ( color->g - *limit ) ) ? color->g - 5 : 0 + ( color->g - *limit );
      color->b = ( color->b >= 5 + ( color->b - *limit ) ) ? color->b - 5 : 0 + ( color->b - *limit );
      break;
    case '=' : [[fallthrough]];
    case '+' :
      *limit   = max3( color );
      color->r = ( color->r <= 250 - ( *limit - color->r ) ) ? color->r + 5 : 255 - ( *limit - color->r );
      color->g = ( color->g <= 250 - ( *limit - color->g ) ) ? color->g + 5 : 255 - ( *limit - color->g );
      color->b = ( color->b <= 250 - ( *limit - color->b ) ) ? color->b + 5 : 255 - ( *limit - color->b );
      break;
    default : break;
  }
}
// draw horizontal line
static inline void hLine( uint8_t which, Layer *restrict layer, uint16_t x, uint16_t y, uint32_t fgCol,
                          uint32_t bgCol, uint8_t sel, const uint16_t *restrict arr, uint8_t dir ) {
  uint16_t width = 0;
  if ( which == CANVAS ) { width = ( tb_width() < bwidth ) ? (uint16_t) tb_width() : bwidth; }
  if ( which == UI ) { width = (uint16_t) tb_width(); }
  if ( dir == 1 ) {
    // draw line left to right
    for ( ; x < width; ++x ) {
      // draw to buffer
      if ( which == CANVAS ) {
        layer->canvas[ x ][ y ].cell  = fgCol; // instead of .rgb bitfield, just write 32bit to 32bit
        layer->canvas[ x ][ y ].block = sel; // -since this bitfield will overwrite and isn't a weird 24bit size
      }
      // draw to terminal only  (UI elements)
      tb_set_cell( x, y, arr[ sel ], fgCol, bgCol );
    }
  } else {
    // draw line right to left
    while ( x-- ) {
      // draw to buffer
      if ( which == CANVAS ) {
        layer->canvas[ x ][ y ].cell  = fgCol;
        layer->canvas[ x ][ y ].block = sel;
      }
      tb_set_cell( x, y, arr[ sel ], fgCol, bgCol );
    }
  }
}
// draw a vertical line
static inline void vLine( uint8_t which, Layer *restrict layer, uint16_t x, uint16_t y, uint32_t fgCol,
                          uint32_t bgCol, uint8_t sel, const uint16_t *restrict arr, uint8_t dir ) {
  const uint8_t height = ( tb_height() < bheight ) ? (uint8_t) tb_height() : bheight - 1;
  if ( dir == 1 ) {
    // draw line top to bottom
    for ( ; y < height; ++y ) {
      // draw to buffer
      if ( which == CANVAS ) {
        layer->canvas[ x ][ y ].cell  = fgCol;
        layer->canvas[ x ][ y ].block = sel;
      }
      // draw to terminal only  (UI elements)
      tb_set_cell( x, y, arr[ sel ], fgCol, bgCol );
    }
  } else {
    // draw line bottom to top
    while ( y-- ) {
      // draw to buffer
      if ( which == CANVAS ) {
        layer->canvas[ x ][ y ].cell  = fgCol;
        layer->canvas[ x ][ y ].block = sel;
      }
      // draw to terminal only  (UI elements)
      tb_set_cell( x, y, arr[ sel ], fgCol, bgCol );
    }
  }
}
// draw palette characters at screen bottom
// TODO: stop drawing to camvas buffer, move to UI buffer
static inline void drawPalette( Layer *restrict layer, const uint16_t *restrict arr, const uint8_t len,
                                const uint8_t *restrict select ) {
  static uint32_t cfg = fg_UI;
  const uint8_t   y   = (uint8_t) tb_height() - 1;
  // iterate over palette array.  dim unselected blocks
  for ( uint8_t i = 0; i < len; ++i ) {
    cfg = ( ( arr[ i ] == arr[ *select ] ) ? 0x10000000 | fg_UI : (uint32_t) ( fg_UI * 0.5 ) );
    tb_set_cell( i, y, arr[ i ], cfg, BLACK );
  }
}
// draw current color settings
static inline void drawColorStatus( const Color *restrict color ) {
  const uint8_t xloc = ( tb_width() > bwidth ) ? (uint8_t) ( tb_width() * 0.5 ) : (uint8_t) ( bwidth * 0.5 );
  const uint8_t yloc = (uint8_t) ( tb_height() - 1 );
  // hashtag shows current color
  if ( color->rgb != 0x000000 ) {
    tb_set_cell( xloc - 5, yloc, '#', 0x01000000 | color->rgb, BLACK );
  } else {
    tb_set_cell( xloc - 5, yloc, '#', 0x01000000 | TB_HI_BLACK, BLACK );
  }
  // print RGB hex values.  screen bottom, and centered
  for ( uint8_t l = 0, j = 0; j < 3; l += 2, j++ ) {
    tb_printf( xloc - l, yloc, (uintattr_t) fg_UI, BLACK, "%02X", color->rgbArr[ j ] );
  }
}
// draws current cursor location in bottom right corner
static void drawXYStatus( Layer *restrict layer, const uint16_t *restrict arr, const uint8_t *restrict sX,
                          const uint8_t *restrict sY ) {
  tb_printf( tb_width() - 9, tb_height() - 1, (uintattr_t) fg_UI, BLACK, "%d, %d", *sX, *sY );
}
// TODO: create UI buffer and call all UI functions in drawUI() function
inline void drawUI( Layer *restrict layer, const uint16_t *restrict arr, const uint8_t *restrict select,
                    const Color *restrict color, const uint8_t *restrict sX, const uint8_t *restrict sY ) {
  const uint8_t  h = (uint8_t) tb_height() - 1;
  const uint16_t w = (uint16_t) tb_width();
  // fill for off-canvas area.
  // canvas is fixed-width, so terminal could be larger than canvas
  if ( w > bwidth ) {
    for ( uint16_t f = ( w - bwidth ); f > 0; f-- ) { vLine( UI, layer, w - f, h, GUTTER, BLACK, 6, arr, 0 ); }
  }
  // fill for area below canvas
  if ( h > bheight ) {
    for ( uint16_t f = ( h - bheight ); f > 0; f-- ) { hLine( UI, layer, 0, h - f, GUTTER, BLACK, 6, arr, 1 ); }
  }
  // draw UI bottom bar.  in case of mouse draw over UI
  hLine( UI, layer, 0, h, BLACK, BLACK, 0, arr, 1 );
  drawPalette( layer, arr, palSize, select );
  drawXYStatus( layer, arr, sX, sY );
  drawColorStatus( color );
  tb_present();
}
// TODO: implement fill tool
/* span filling method
//It's possible to optimize things further by working primarily with spans, a row with constant y. The first
published complete example works on the following basic principle.[1]

    Starting with a seed point, fill left and right. Keep track of the leftmost filled point lx and rightmost
filled point rx. This defines the span. Scan from lx to rx above and below the seed point, searching for new
seed points to continue with.

As an optimisation, the scan algorithm does not need restart from every seed point, but only those at the start
of the next span. Using a stack explores spans depth first, whilst a queue explores spans breadth first.

Over time, the following optimizations were realized:

    When a new scan would be entirely within a grandparent span, it would certainly only find filled pixels, and
so wouldn't need queueing.[1][6][3] Further, when a new scan overlaps a grandparent span, only the overhangs
(U-turns and W-turns) need to be scanned.[3] It's possible to fill while scanning for seeds [6]

The final, combined-scan-and-fill span filler was then published in 1990. In pseudo-code form:

*/
// pseudo code implementation:
/* fn fill(x, y):
    if not Inside(x, y) then return
    let s = new empty queue or stack
    Add (x, x, y, 1) to s
    Add (x, x, y - 1, -1) to s
    while s is not empty:
        Remove an (x1, x2, y, dy) from s
        let x = x1
        if Inside(x, y):
            while Inside(x - 1, y):
                Set(x - 1, y)
                x = x - 1
            if x < x1:
                Add (x, x1 - 1, y - dy, -dy) to s
        while x1 <= x2:
            while Inside(x1, y):
                Set(x1, y)
                x1 = x1 + 1
            if x1 > x:
                Add (x, x1 - 1, y + dy, dy) to s
            if x1 - 1 > x2:
                Add (x2 + 1, x1 - 1, y - dy, -dy) to s
            x1 = x1 + 1
            while x1 < x2 and not Inside(x1, y):
                x1 = x1 + 1
            x = x1
 */
/*
static void fill( Layer *restrict layer, const uint8_t *restrict sX, const uint8_t *restrict sY,
                  const uint8_t *restrict select, const Color *restrict color, const uint16_t *restrict arr ) {
  Layer *layer2 = alloca( sizeof *layer );
}
*/

