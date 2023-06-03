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
// draw a horizontal line
void hLine( uint16_t x, uint16_t y, uint32_t fgCol, uint32_t bgCol, uint16_t c ) {
  for ( ; x < tb_width(); ++x ) {
    tb_set_cell( x, y, c, fgCol, bgCol );
  }
}
// draw a vertical line
void vLine( uint16_t x, uint16_t y, uint32_t fgCol, uint32_t bgCol, uint16_t c ) {
  for ( ; y < tb_height(); ++y ) {
    tb_set_cell( x, y, ' ', fgCol, bgCol );
  }
}
// draw palette characters at screen bottom
void drawPalette( const uint16_t arr[], const uint16_t len, const uint16_t *c ) {
  hLine( 0, (uint16_t) tb_height() - 1, 0, bg_UI, ' ' );
  const uint16_t y = (uint16_t) tb_height() - 1;
  uint32_t cfg     = fg_UI;
  uint32_t cbg     = bg_UI;
  for ( uint16_t i = 0; i < len; ++i ) {
    cfg = ( ( arr[i] == *c ) ? 0xF0F0F0 : fg_UI );
    cbg = ( ( arr[i] == *c ) ? BLACK : bg_UI );
    tb_set_cell( i, y, arr[i], cfg, cbg );
  }
}
// draw current color settings
void drawColorStatus( const Color *color, const uint16_t array[] ) {
  for ( uint8_t i = 1; i < 5; i++ ) {
    tb_set_cell( tb_width() - 23 + i, tb_height() - 1, array[i], color->rgb, bg_UI );
  }
  tb_printf( tb_width() - 17, tb_height() - 1, fg_RGB, bg_R, "%02X", color->r );
  tb_printf( tb_width() - 15, tb_height() - 1, fg_RGB, bg_G, "%02X", color->g );
  tb_printf( tb_width() - 13, tb_height() - 1, fg_RGB, bg_B, "%02X", color->b );
}
// random integer within range given
const uint32_t randRange( const uint32_t min, const uint32_t max ) {
  return min + rand() / ( RAND_MAX / ( max - min + 1 ) + 1 );
}

