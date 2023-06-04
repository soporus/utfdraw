#define TB_IMPL
#define TB_LIB_OPTS
#define TB_OPT_TRUECOLOR

#include "term.h"

int main( int argc, char **argv ) {

  const slots slot       = { {
            .space = L'\u00a0', // spaceblock
            .shadL = L'\u2591', // ░
            .shadM = L'\u2592', // ▒
            .shadH = L'\u2593', // ▓︎
            .fullH = L'\u2588', // █
            .blkTp = L'\u2580', // top block
            .blkHi = L'\u2594', // high thin
            .blkLo = L'\u2581', // low thin
            .blkBt = L'\u2584', // bottom blo
            .blkMd = L'\u25FC', // middle blo
  } };
  const blocks *const st = &slot.stp; // access union as struct
  const uint8_t slotSize = sizeof( slot.arr ) / sizeof( slot.arr[0] ) - 1;
  const uint16_t *c      = &st->fullH;
  struct tb_event *ev    = malloc( sizeof *ev );
  Color color            = { .rgb = 0x00808080 };

  tb_init();
  tb_set_output_mode( TB_OUTPUT_TRUECOLOR );
  tb_set_input_mode( TB_INPUT_MOUSE );
  drawPalette( slot.arr, slotSize, c );
  tb_present();
  // input loop (esc exits)
  while ( ev->key != TB_KEY_ESC ) {
    // save previous keypress to test
    static uint32_t keyTest = 0;
    // status x and y coordinates
    static uint8_t sX = 0;
    static uint8_t sY = 0;

    uint8_t wasd = 0;
    // listen (non blocking)
    tb_peek_event( ev, 17 );
    if ( ev->key == TB_KEY_MOUSE_LEFT ) {
      tb_set_cursor( ev->x, ev->y );
      tb_set_cell( ev->x, ev->y, *c, color.rgb, BLACK );
      sX = (uint8_t) ev->x;
      sY = (uint8_t) ev->y;
      goto draw;
    }
    // check only if a key was pressed
    if ( keyTest != ev->ch ) {
      switch ( ev->ch ) {
      // block select
      case '1': c = &st->space; goto draw;
      case '2': c = &st->shadL; goto draw;
      case '3': c = &st->shadM; goto draw;
      case '4': c = &st->shadH; goto draw;
      case '5': c = &st->fullH; goto draw;
      case '6': c = &st->blkTp; goto draw;
      case '7': c = &st->blkHi; goto draw;
      case '8': c = &st->blkLo; goto draw;
      case '9': c = &st->blkBt; goto draw;
      case '0': c = &st->blkMd; goto draw;
      // colors
      case 'z': setColor( &color, &ev->ch ); goto draw;
      case 'x': setColor( &color, &ev->ch ); goto draw;
      case 'c': setColor( &color, &ev->ch ); goto draw;
      // directional draw
      case 'w': sY = ( sY > 0 ) ? sY - ( ++wasd ) : 0; break;
      case 'a': sX = ( sX > 0 ) ? sX - ( ++wasd ) : 0; break;
      case 's': sY = ( sY < tb_height() - 2 ) ? sY + ( ++wasd ) : sY; break;
      case 'd': sX = ( sX < tb_width() - 1 ) ? sX + ( ++wasd ) : sX; break;
      }
      switch ( ev->key ) {
        // directional no draw
      case TB_KEY_ARROW_UP: sY = ( sY > 0 ) ? sY - ( ++wasd ) : 0; break;
      case TB_KEY_ARROW_LEFT: sX = ( sX > 0 ) ? sX - ( ++wasd ) : 0; break;
      case TB_KEY_ARROW_DOWN: sY = ( sY < tb_height() - 2 ) ? sY + ( ++wasd ) : sY; break;
      case TB_KEY_ARROW_RIGHT: sX = ( sX < tb_width() - 1 ) ? sX + ( ++wasd ) : sX; break;
      }
      if ( wasd == 1 ) {
        tb_set_cursor( sX, sY );
        tb_set_cell( sX, sY, *c, color.rgb, BLACK );
      }
    }
  draw:
    drawPalette( slot.arr, slotSize, c );
    tb_printf( tb_width() - 9, tb_height() - 1, fg_UI, bg_UI, "%d, %d", sX, sY );
    drawColorStatus( &color, slot.arr );
    tb_present();
    keyTest = ev->ch;
  }
  free( ev );
  // clean up terminal
  tb_shutdown();
  return 0;
}

