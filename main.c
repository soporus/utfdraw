#define TB_IMPL
#define TB_OPT_TRUECOLOR

#include "term.h"

int main( int argc, char **argv ) {
	  // setup memory for event loop, color value, and drawing chars
  struct tb_event *ev     = alloca( sizeof *ev );
  Color *color            = alloca( sizeof *color );
  color->rgb              = 0x00c0c0c0;
  const Slots *const slot = alloca( sizeof *slot );
  *(Slots *) slot         = ( Slots ) { {
	 .space = L'\U000000A0',  // spaceblock
              .shadL = L'\U00002591',  // ░
              .shadM = L'\U00002592',  // ▒
              .shadH = L'\U00002593',  // ▓︎
              .fullH = L'\U00002588',  // █
              .blkTp = L'\U00002580',  // top block
              .blkHi = L'\U00002594',  // high thin
              .blkLo = L'\U00002581',  // low thin
              .blkBt = L'\U00002584',  // bottom block
              .blkMd = L'\U000025FC',  // middle block
  } };
  const blocks *const st  = &slot->stp;  // access union as struct
  const uint8_t slotSize  = sizeof( slot->arr ) / sizeof( slot->arr[0] ) - 1;
  const wchar_t *c       = &st->fullH;
  setlocale(LC_ALL,"C.UTF-8");

  // setup termbox2 
  tb_init();
  tb_set_output_mode( TB_OUTPUT_TRUECOLOR );
  tb_set_input_mode( TB_INPUT_MOUSE );
  // input loop (esc exits)
  ev->key = 0;
  while ( ev->key != TB_KEY_ESC ) {
    // status x and y coordinates
    static uint16_t sX = 0;
    static uint16_t sY = 0;
    drawPalette( slot->arr, slotSize, c, color );
    tb_printf( tb_width() - 9, tb_height() - 1, fg_UI, bg_UI, "%d, %d", sX, sY );
    drawColorStatus( color );
    tb_present();
    // wait for input, proces input, only if found
    if ( tb_poll_event( ev ) == TB_OK ) { checkInput( ev, color, &c, st, &sX, &sY ); }
  }
  // clean up terminal
  tb_shutdown();
  return 0;
}

