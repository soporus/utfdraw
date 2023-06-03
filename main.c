#define TB_IMPL
#define TB_LIB_OPTS
#define TB_OPT_TRUECOLOR

#include "term.h"

int main( int argc, char **argv ) {

  const slots slot       = { {
            .spaceblock  = L'\u00a0', // spaceblock
            .shadeLo     = L'\u2591', // ░
            .shadeMed    = L'\u2592', // ▒
            .shadeHi     = L'\u2593', // ▓︎
            .full        = L'\u2588', // █
            .blockTop    = L'\u2580', // top block
            .blockHiThin = L'\u2594', // high thin
            .blockLoThin = L'\u2581', // low thin
            .blockBtm    = L'\u2584', // bottom blo
            .midBlock    = L'\u25FC', // middle blo
            .tallHalfL   = L'\u258C', // tall half l
            .tallThinL   = L'\u258F', // tall thin l
            .smUpperR    = L'\u2598', // small upper
            .smLoL       = L'\u2596', // small low l
            .smLoR       = L'\u2597', // small low r
            .smUpperL    = L'\u259d', // small upper
            .tallThinR   = L'\u2595', // tall thin r
            .tallHalfR   = L'\u2590'  // tall half r
  } };
  const blocks *const st = &slot.stp; // access union as struct
  const uint8_t slotSize = sizeof( slot.arr ) / sizeof( slot.arr[0] ) - 1;
  const uint16_t *c      = &st->full;
  struct tb_event ev;
  Color color = { .rgb = 0x00808080 };

  tb_init();
  tb_set_output_mode( TB_OUTPUT_TRUECOLOR );
  tb_set_input_mode( TB_INPUT_MOUSE );
  drawPalette( slot.arr, slotSize, c );
  tb_present();
  // input loop (esc exits)
  while ( ev.key != TB_KEY_ESC ) {
    // save previous keypress to test
    static uint32_t keyTest = 0;
    static uint8_t sX       = 0;
    static uint8_t sY       = 0;

    uint8_t wasd = 0;
    // listen (non blocking)
    tb_peek_event( &ev, 17 );
    if ( ev.key == TB_KEY_MOUSE_LEFT ) {
      tb_set_cursor( ev.x, ev.y );
      tb_set_cell( ev.x, ev.y, *c, color.rgb, BLACK );
      sX = (uint8_t) ev.x;
      sY = (uint8_t) ev.y;
      goto draw;
    }
    // check only if a key was pressed
    if ( keyTest != ev.ch ) {
      switch ( ev.ch ) {
      // block select
      case '1': c = &st->spaceblock; goto draw;
      case '2': c = &st->shadeLo; goto draw;
      case '3': c = &st->shadeMed; goto draw;
      case '4': c = &st->shadeHi; goto draw;
      case '5': c = &st->full; goto draw;
      case '6': c = &st->blockTop; goto draw;
      case '7': c = &st->blockHiThin; goto draw;
      case '8': c = &st->blockLoThin; goto draw;
      case '9': c = &st->blockBtm; goto draw;
      case '0': c = &st->midBlock; goto draw;
      // colors
      case 'z': setColor( &color, ev.ch ); goto draw;
      case 'x': setColor( &color, ev.ch ); goto draw;
      case 'c': setColor( &color, ev.ch ); goto draw;
      // directional
      case 'w': sY = ( sY > 0 ) ? sY - ( ++wasd ) : 0; break;
      case 'a': sX = ( sX > 0 ) ? sX - ( ++wasd ) : 0; break;
      case 's': sY = ( sY < tb_height() - 2 ) ? sY + ( ++wasd ) : sY; break;
      case 'd': sX = ( sX < tb_width() - 1 ) ? sX + ( ++wasd ) : sX; break;
      default: break;
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
    keyTest = ev.ch;
  }
  // clean up terminal
  tb_shutdown();
  return 0;
}

