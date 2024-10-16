#define _XOPEN_SOURCE
#define _DEFAULT_SOURCE
#define TB_IMPL
#define TB_OPT_TRUECOLOR
#include "term.h"
#include <locale.h>

int main(int argc, char **argv) {
  // setup memory for event loop, color value, and drawing chars
  struct tb_event *ev              = alloca(sizeof *ev);
  Color *restrict color            = alloca(sizeof *color);
  color->rgb                       = 0x00c0c0c0;
  const Slots *restrict const slot = alloca(sizeof *slot);
  *(Slots *restrict) slot          = (Slots) {{
             .space = L'\U000000A0', // blank
             .shadL = L'\U00002591', // ░
             .shadM = L'\U00002592', // ▒
             .shadH = L'\U00002593', // ▓
             .fullH = L'\U00002588', // █
             .blkTp = L'\U00002580', // ▀
             .blkHi = L'\U00002594', // ▔
             .blkLo = L'\U00002581', // ▁
             .blkBt = L'\U00002584', // ▄
             .blkMd = L'\U000025FC', // ◼
  }};
  const blocks *restrict const st  = &slot->stp; // access union as struct
  const uint8_t  slotSize          = sizeof(slot->arr) / sizeof(slot->arr[0]) - 1;
  const wchar_t *c                 = &st->fullH;
  setlocale(LC_ALL, "C.UTF-8");
  // setup termbox2
  tb_init();
  tb_set_output_mode(TB_OUTPUT_TRUECOLOR);
  tb_set_input_mode(TB_INPUT_MOUSE);
  // input loop (esc exits)
  ev->key = 0;
  while (ev->key != TB_KEY_ESC) {
    // status x and y coordinates
    static uint16_t sX = 0;
    static uint16_t sY = 0;
    drawPalette(slot->arr, slotSize, c);
    tb_printf(tb_width() - 9, tb_height() - 1, (uintattr_t) fg_UI, BLACK, "%d, %d", sX, sY);
    drawColorStatus(color);
    tb_present();
    // wait for input, process input, only if found
    if (tb_poll_event(ev) == TB_OK) { checkInput(ev, color, &c, slot->arr, &sX, &sY); }
  }
  // clean up terminal
  tb_shutdown();
  return 0;
}
