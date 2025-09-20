#include <locale.h>
#include <stdint.h>
#define TB_IMPL
#include "term.h"

int main( int argc, char **argv ) {
  // setup memory for event loop, color value, and drawing chars
  struct tb_event *restrict const ev = alloca( sizeof *ev );
  Layer *layer                       = alloca( sizeof *layer );
  Color *restrict color              = alloca( sizeof *color );
  color->rgb                         = 0x00c0c0c0;
  const Slots *restrict const slot   = alloca( sizeof *slot );
  *(Slots *restrict) slot            = (Slots) { {
               .space     = L'\U000000A0', // blank
               .shadL     = L'\U00002591', // ░
               .shadM     = L'\U00002592', // ▒
               .shadH     = L'\U00002593', // ▓
               .fullH     = L'\U00002588', // █
               .blkTp     = L'\U00002580', // ▀
               .blkHi     = L'\U00002594', // ▔
               .blkLo     = L'\U00002581', // ▁
               .blkBt     = L'\U00002584', // ▄
               .blkMd     = L'\U000025FC', // ◼
               .tallHalfL = L'\U0000258C', // ▌
               .tallThinL = L'\U0000258F', // ▏
               .smUpperR  = L'\U00002598', // ▘
               .smLoL     = L'\U00002596', // ▖
               .smLoR     = L'\U00002597', // ▗
               .smUpperL  = L'\U0000259D', // ▝
               .tallThinR = L'\U00002595', // ▕
               .tallHalfR = L'\U00002590'  // ▐
  } };
  // hold current slot selection
  memset( layer, 0, sizeof( *layer ) );
  setlocale( LC_ALL, "C.UTF-8" );
  // setup termbox2
  tb_init();
  tb_set_output_mode( TB_OUTPUT_TRUECOLOR );
  tb_set_input_mode( TB_INPUT_MOUSE );
  // input loop (esc exits)
  ev->key = 0;
  while ( ev->key != TB_KEY_CTRL_Q ) {
    // status x and y coordinates
    static uint8_t sX     = 0;
    static uint8_t sY     = 0;
    static uint8_t select = 4;
    // replacement modes
    // normal mode:   0
    // replace color: 1
    // replace char:  2
    static uint8_t rmode = 0;
    drawUI( layer, slot->arr, &select, color, &sX, &sY, &rmode );
    // wait for input, process input, only if found
    if ( tb_poll_event( ev ) == TB_OK ) {
      checkInput( ev, color, &rmode, &select, slot->arr, &sX, &sY, layer );
    }
  }
  // clean up terminal
  tb_shutdown();
  return 0;
}
