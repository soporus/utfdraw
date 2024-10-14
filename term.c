#include "term.h"

// inputs
void checkInput( struct tb_event *restrict ev, Color *restrict color, const wchar_t **restrict c,
                 const blocks *restrict const st, uint16_t *restrict sX, uint16_t *restrict sY ) {
  // flag when a draw is needed
  uint8_t draw = 0;
  // stores previous keypress persistently
  static uint32_t keyTest = 0;
  // because of the different key types, we use 2 switches.
  // first switch cases jump over the next switch, since additional tests are not required
  // -----------------------
  // mouse movement and paint
  switch ( ev->key ) {
    // movement and paints character
    case TB_KEY_MOUSE_LEFT :
      *sX = (uint16_t) ev->x;
      *sY = (uint16_t) ev->y;
      ++draw;
      goto draw;
    // movement.  does not paint character, but we need to paint cursor with draw
    case TB_KEY_MOUSE_RIGHT :
      *sX = (uint16_t) ev->x;
      *sY = (uint16_t) ev->y;
      goto draw;
    // movement.  does not draw a character, but we must call draw later to paint cursor position
    case TB_KEY_ARROW_LEFT : *sX = ( *sX > 0 ) ? *sX - 1 : 0; goto draw;
    case TB_KEY_ARROW_UP : *sY = ( *sY > 0 ) ? *sY - 1 : 0; goto draw;
    case TB_KEY_ARROW_DOWN : *sY = ( *sY < tb_height() - 2 ) ? *sY + 1 : *sY; goto draw;
    case TB_KEY_ARROW_RIGHT : *sX = ( *sX < tb_width() - 1 ) ? *sX + 1 : *sX; goto draw;
    // draw to boundary
    case TB_KEY_HOME : hLine( *sX, *sY, color->rgb, BLACK, **c, 0 ); goto bypass;
    case TB_KEY_END : hLine( *sX, *sY, color->rgb, BLACK, **c, 1 ); goto bypass;
    case TB_KEY_PGUP : vLine( *sX, *sY, color->rgb, BLACK, **c, 0 ); goto bypass;
    case TB_KEY_PGDN : vLine( *sX, *sY, color->rgb, BLACK, **c, 1 ); goto bypass;
    default : break;
  }
  // check keypress, do something
  switch ( ev->ch ) {
    // movement
    case 'w' : *sY = ( *sY > 0 ) ? *sY - 1 : 0; break;
    case 'a' : *sX = ( *sX > 0 ) ? *sX - 1 : 0; break;
    case 's' : *sY = ( *sY < tb_height() - 2 ) ? *sY + 1 : *sY; break;
    case 'd' : *sX = ( *sX < tb_width() - 1 ) ? *sX + 1 : *sX; break;
    // vi style movement
    case 'k' : *sY = ( *sY > 0 ) ? *sY - 1 : 0; break;
    case 'h' : *sX = ( *sX > 0 ) ? *sX - 1 : 0; break;
    case 'j' : *sY = ( *sY < tb_height() - 2 ) ? *sY + 1 : *sY; break;
    case 'l' : *sX = ( *sX < tb_width() - 1 ) ? *sX + 1 : *sX; break;
    // movement and draw
    case 'W' : *sY = ( *sY > 0 ) ? *sY - ( ++draw ) : 0; break;                  // up
    case 'A' : *sX = ( *sX > 0 ) ? *sX - ( ++draw ) : 0; break;                  // left
    case 'S' : *sY = ( *sY < tb_height() - 2 ) ? *sY + ( ++draw ) : *sY; break;  // down
    case 'D' :
      *sX = ( *sX < tb_width() - 1 ) ? *sX + ( ++draw ) : *sX;
      break;  // right
    // vi style movement and draw
    case 'K' : *sY = ( *sY > 0 ) ? *sY - ( ++draw ) : 0; break;                  // up
    case 'H' : *sX = ( *sX > 0 ) ? *sX - ( ++draw ) : 0; break;                  // left
    case 'J' : *sY = ( *sY < tb_height() - 2 ) ? *sY + ( ++draw ) : *sY; break;  // down
    case 'L' :
      *sX = ( *sX < tb_width() - 1 ) ? *sX + ( ++draw ) : *sX;
      break;  // right
    // character select
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
    // lowercase will decrease respective color
    case 'r' : setColor( color, &ev->ch ); goto bypass;
    case 'g' : setColor( color, &ev->ch ); goto bypass;
    case 'b' : setColor( color, &ev->ch ); goto bypass;
    // uppercase will increase respective color
    case 'R' : setColor( color, &ev->ch ); goto bypass;
    case 'G' : setColor( color, &ev->ch ); goto bypass;
    case 'B' : setColor( color, &ev->ch ); goto bypass;
  }
// we jump here if no further checks needed and ready to draw.  left-click draw for example
// set cursor position, write character, paint fg color and bg color (always black for now)
draw:
  tb_set_cursor( *sX, *sY );
  if ( draw == 1 ) { tb_set_cell( *sX, *sY, **c, color->rgb, BLACK ); }

// jump here when no further checks needed and no draw required. arrow keys or char select
// store more recent key pressed
bypass:
  keyTest = ev->ch;
}

// increment value of RGB color channels until wrap to 0
void setColor( Color *restrict color, uint32_t *restrict ch ) {
  switch ( *ch ) {
    // decrease color
    case 'r' : color->r -= 8; break;
    case 'g' : color->g -= 8; break;
    case 'b' : color->b -= 8; break;
    // increase color
    case 'R' : color->r += 8; break;
    case 'G' : color->g += 8; break;
    case 'B' : color->b += 8; break;
    default : break;
  }
}
// draw a horizontal line
void hLine( uint16_t x, uint16_t y, uint32_t fgCol, uint32_t bgCol, wchar_t c, uint8_t dir ) {
  const uint16_t width = (uint16_t) tb_width();
  // draw line from left to right
  if ( dir == 1 ) {
    for ( ; x < width; ++x ) {
      tb_set_cell( x, y, c, fgCol, bgCol );
    }
  // draw line from right to left
  } else {
    while ( x-- ) {
      tb_set_cell( x, y, c, fgCol, bgCol );
    }
  }
}
// draw a vertical line
void vLine( uint16_t x, uint16_t y, uint32_t fgCol, uint32_t bgCol, wchar_t c, uint8_t dir ) {
  const uint16_t height = (uint16_t) tb_height() - 1;
  // draw line downward 
  if ( dir == 1 ) {
    for ( ; y < height; ++y ) {
      tb_set_cell( x, y, c, fgCol, bgCol );
    }
  // draw line upward  
  } else {
    while ( y-- ) {
      tb_set_cell( x, y, c, fgCol, bgCol );
    }
  }
}
// draw palette characters at screen bottom
void drawPalette( const wchar_t *restrict arr, const uint8_t len, const wchar_t *restrict c ) {
  hLine( 0, (uint16_t) tb_height() - 1, bg_UI, BLACK, ' ', 1 );
  const uint16_t y    = (uint16_t) tb_height() - 1;
  static uint32_t cfg = fg_UI;
  // iterate over palette array.  dim unselected blocks
  for ( uint16_t i = 0; i <= len; ++i ) {
    cfg = ( ( arr[i] == *c ) ? 0x10000000 | fg_UI : (uint32_t) ( fg_UI * 0.5 ) );
    tb_set_cell( i, y, arr[i], cfg, BLACK );
  }
}
// draw current color settings
void drawColorStatus( const Color *restrict color ) {
  const uint16_t xplacement = (uint16_t) ( tb_width() * 0.5 );
  const uint16_t yplacement = (uint16_t) ( tb_height() - 1 );
  // hashtag shows current color
  tb_set_cell( xplacement - 5, yplacement, '#', 0x01000000 | color->rgb, BLACK );
  // print RGB hex values.  screen bottom, and centered
  tb_printf( xplacement - 4, yplacement, (uintattr_t) fg_UI, BLACK, "%02X", color->r );
  tb_printf( xplacement - 2, yplacement, (uintattr_t) fg_UI, BLACK, "%02X", color->g );
  tb_printf( xplacement, yplacement, (uintattr_t) fg_UI, BLACK, "%02X", color->b );
}
