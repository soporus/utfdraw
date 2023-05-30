#include "term.h"

// increment value of RGB color channels until wrap to 0
void setColor( Color *color, uint32_t ch ) {
  switch ( ch ) {
  case 'z': color->r += 8; break;
  case 'x': color->g += 8; break;
  case 'c': color->b += 8; break;
  default: break;
  }
}

// draw palette characters at screen bottom
void drawPalette( const uint16_t arr[],
                  const uint16_t len,
                  const uint16_t *c ) {
  hLine( 0, (uint16_t) tb_height() - 1, 0, 0x101010, ' ' );
  const uint16_t y = (uint16_t) tb_height() - 1;
  uint32_t cfg     = 0x20000000;
  uint32_t cbg     = 0x40C0FF;
  for ( uint16_t i = 0; i < len; ++i ) {
    cfg = ( ( arr[i] == *c ) ? 0xF0F0F0 : 0x808080 );
    cbg = ( ( arr[i] == *c ) ? 0x20000000 : 0x101010 );
    tb_set_cell( i, y, arr[i], cfg, cbg );
  }
}

// random integer within range given
const uint32_t randRange( const uint32_t min, const uint32_t max ) {
  return min + rand() / ( RAND_MAX / ( max - min + 1 ) + 1 );
}

// draw a horizontal line
void hLine(
    uint16_t x, uint16_t y, uint32_t fgCol, uint32_t bgCol, uint16_t c ) {
  for ( ; x < tb_width(); ++x ) {
    tb_set_cell( x, y, c, fgCol, bgCol );
  }
}

// draw a vertical line
void vLine(
    uint16_t x, uint16_t y, uint32_t fgCol, uint32_t bgCol, uint16_t c ) {
  for ( ; y < tb_height(); ++y ) {
    tb_set_cell( x, y, ' ', fgCol, bgCol );
  }
}

