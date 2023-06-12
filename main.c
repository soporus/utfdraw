#define TB_IMPL
#define TB_LIB_OPTS
#define TB_OPT_TRUECOLOR

#include "term.h"

int main( int argc, char **argv ) {

  struct tb_event *ev     = alloca( sizeof *ev );
  Color *color            = alloca( sizeof *color );
  color->rgb              = 0x00808080;
  const Slots *const slot = alloca( sizeof *slot );
  *(Slots *) slot         = ( Slots ) { {
              .space = L'\u00a0',  // spaceblock
              .shadL = L'\u2591',  // ░
              .shadM = L'\u2592',  // ▒
              .shadH = L'\u2593',  // ▓︎
              .fullH = L'\u2588',  // █
              .blkTp = L'\u2580',  // top block
              .blkHi = L'\u2594',  // high thin
              .blkLo = L'\u2581',  // low thin
              .blkBt = L'\u2584',  // bottom blo
              .blkMd = L'\u25FC',  // middle blo
  } };
  const blocks *const st  = &slot->stp;  // access union as struct
  const uint8_t slotSize  = sizeof( slot->arr ) / sizeof( slot->arr[0] ) - 1;
  const uint16_t *c       = &st->fullH;
  tb_init();
  tb_set_output_mode( TB_OUTPUT_TRUECOLOR );
  tb_set_input_mode( TB_INPUT_MOUSE );
  // input loop (esc exits)
  ev->key = 0;
  while ( ev->key != TB_KEY_ESC ) {
    // status x and y coordinates
    static uint8_t sX = 0;
    static uint8_t sY = 0;
    drawPalette( slot->arr, slotSize, c );
    tb_printf( tb_width() - 9, tb_height() - 1, fg_UI, bg_UI, "%d, %d", sX, sY );
    drawColorStatus( color, slot->arr );
    tb_present();
    // wait for input, proces input, only if found
    if ( tb_poll_event( ev ) == TB_OK ) { checkInput( ev, color, &c, st, &sX, &sY ); }
  }
  // clean up terminal
  tb_shutdown();
  return 0;
}

