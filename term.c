#include "term.h"

// inputs
void checkInput( struct tb_event *restrict ev, Color *restrict color, const uint16_t **restrict c,
                 const blocks *restrict const st, uint8_t *restrict sX, uint8_t *restrict sY ) {
  // movement - wasd: move cursor and paint. hjkl: move only
  uint8_t wasd = 0;
  uint8_t hjkl = 0;
  // save previous keypress to test
  static uint32_t keyTest = 0;
  // mouse movement and paint
  switch ( ev->key ) {
    case TB_KEY_MOUSE_LEFT :
      tb_set_cursor( ev->x, ev->y );
      tb_set_cell( ev->x, ev->y, **c, color->rgb, BLACK );
      *sX = (uint8_t) ev->x;
      *sY = (uint8_t) ev->y;
      goto bypass;
    // draw to boundary
    case TB_KEY_HOME : hLine( *sX, *sY, color->rgb, BLACK, **c, 0 ); goto bypass;
    case TB_KEY_END : hLine( *sX, *sY, color->rgb, BLACK, **c, 1 ); goto bypass;
    case TB_KEY_PGUP : vLine( *sX, *sY, color->rgb, BLACK, **c, 0 ); goto bypass;
    case TB_KEY_PGDN : vLine( *sX, *sY, color->rgb, BLACK, **c, 1 ); goto bypass;
  }
  // check keypress, do something
  switch ( ev->ch ) {
    // block select
    case '1' : *c = &st->space; goto bypass;
    case '2' : *c = &st->shadL; goto bypass;
    case '3' : *c = &st->shadM; goto bypass;
    case '4' : *c = &st->shadH; goto bypass;
    case '5' : *c = &st->fullH; goto bypass;
    case '6' : *c = &st->blkTp; goto bypass;
    case '7' : *c = &st->blkHi; goto bypass;
    case '8' : *c = &st->blkLo; goto bypass;
    case '9' : *c = &st->blkBt; goto bypass;
    case '0' : *c = &st->blkMd; goto bypass;
    // colors
    case 'z' : setColor( color, &ev->ch ); goto bypass;
    case 'x' : setColor( color, &ev->ch ); goto bypass;
    case 'c' : setColor( color, &ev->ch ); goto bypass;
    // directional draw
    case 'w' : *sY = ( *sY > 0 ) ? *sY - ( ++wasd ) : 0; break;
    case 'a' : *sX = ( *sX > 0 ) ? *sX - ( ++wasd ) : 0; break;
    case 's' : *sY = ( *sY < tb_height() - 2 ) ? *sY + ( ++wasd ) : *sY; break;
    case 'd' : *sX = ( *sX < tb_width() - 1 ) ? *sX + ( ++wasd ) : *sX; break;
    // directional no draw
    case 'k' : *sY = ( *sY > 0 ) ? *sY - ( ++hjkl ) : 0; break;
    case 'h' : *sX = ( *sX > 0 ) ? *sX - ( ++hjkl ) : 0; break;
    case 'j' : *sY = ( *sY < tb_height() - 2 ) ? *sY + ( ++hjkl ) : *sY; break;
    case 'l' : *sX = ( *sX < tb_width() - 1 ) ? *sX + ( ++hjkl ) : *sX; break;
  }
  tb_set_cursor( *sX, *sY );
  if ( wasd == 1 ) { tb_set_cell( *sX, *sY, **c, color->rgb, BLACK ); }
bypass:
  keyTest = ev->ch;
}

// increment value of RGB color channels until wrap to 0
void setColor( Color *restrict color, uint32_t *restrict c ) {
  switch ( *c ) {
    case 'z' : color->r += 8; break;
    case 'x' : color->g += 8; break;
    case 'c' : color->b += 8; break;
    default : break;
  }
}
// draw a horizontal line
void hLine( uint8_t x, uint16_t y, uint32_t fgCol, uint32_t bgCol, uint16_t c, uint8_t dir ) {
  const uint16_t width = (uint16_t) tb_width();
  if ( dir == 1 ) {
    for ( ; x < width; ++x ) {
      tb_set_cell( x, y, c, fgCol, bgCol );
    }
  } else {
    while ( x-- ) {
      tb_set_cell( x, y, c, fgCol, bgCol );
    }
  }
}
// draw a vertical line
void vLine( uint16_t x, uint16_t y, uint32_t fgCol, uint32_t bgCol, uint16_t c, uint8_t dir ) {
  const uint16_t height = (uint16_t) tb_height();
  if ( dir == 1 ) {
    for ( ; y < height; ++y ) {
      tb_set_cell( x, y, c, fgCol, bgCol );
    }
  } else {
    while ( y-- ) {
      tb_set_cell( x, y, c, fgCol, bgCol );
    }
  }
}
// draw palette characters at screen bottom
void drawPalette( const uint16_t *restrict arr, const uint8_t len, const uint16_t *restrict c ) {
  hLine( 0, (uint16_t) tb_height() - 1, 0, bg_UI, ' ', 1 );
  const uint16_t y = (uint16_t) tb_height() - 1;
  uint32_t cfg     = fg_UI;
  uint32_t cbg     = bg_UI;
  for ( uint16_t i = 0; i <= len; ++i ) {
    cfg = ( ( arr[i] == *c ) ? WHITE : fg_UI );
    cbg = ( ( arr[i] == *c ) ? BLACK : bg_UI );
    tb_set_cell( i, y, arr[i], cfg, cbg );
  }
}
// draw current color settings
void drawColorStatus( const Color *restrict color, const uint16_t *restrict array ) {
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

