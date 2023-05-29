#define TB_IMPL
#define TB_LIB_OPTS
#define TB_OPT_TRUECOLOR

#define UI_FG 0x808080
#define UI_BG 0x101010

#include "term.h"

int main(int argc, char **argv) {

  const slots slot = {
    {
      .spaceblock  = L'\u00a0',    // spaceblock
      .shadeLo     = L'\u2591',    // shade low
      .shadeMed    = L'\u2592',    // shade med
      .shadeHi     = L'\u2593',    // shade hi
      .full        = L'\u2588',    // full
      .blockTop    = L'\u2580',    // top block
      .blockHiThin = L'\u2594',    // high thin
      .blockLoThin = L'\u2581',    // low thin
      .blockBtm    = L'\u2584',    // bottom blo
      .midBlock    = L'\u25FC',    // middle blo
      .tallHalfL   = L'\u258C',    // tall half l
      .tallThinL   = L'\u258F',    // tall thin l
      .smUpperR    = L'\u2598',    // small upper
      .smLoL       = L'\u2596',    // small low l
      .smLoR       = L'\u2597',    // small low r
      .smUpperL    = L'\u259d',    // small upper
      .tallThinR   = L'\u2595',    // tall thin r
      .tallHalfR   = L'\u2590'     // tall half r
    }
  };
  const blockstruct* const st = &slot.stp; // access union as struct
  struct tb_event ev;
  const uint16_t* c = &st->full;
  const uint8_t slotSize = sizeof(slot.arr) / sizeof(slot.arr[0])-1;
  Color color = { .rgb = 0x00808080 };

  tb_init();
  tb_set_output_mode(TB_OUTPUT_TRUECOLOR);
  tb_set_input_mode( TB_INPUT_MOUSE);
  drawPalette(slot.arr, slotSize, c);
  tb_present();

  while (ev.key != TB_KEY_ESC) { // esc key exits
    static uint32_t keyTest = 0;
    static uint8_t  statX   = 0;
    static uint8_t  statY   = 0;

    tb_peek_event(&ev, 16);      // listen (non blocking)
    if (keyTest != ev.ch) {      // check only if a key was pressed
      switch (ev.ch) {           // then check which key to change char
        case '1': c =  &st->spaceblock;
                  goto draw;
        case '2': c =  &st->shadeLo;
                  goto draw;
        case '3': c =  &st->shadeMed;
                  goto draw;
        case '4': c =  &st->shadeHi;
                  goto draw;
        case '5': c =  &st->full;
                  goto draw;
        case '6': c =  &st->blockTop;
                  goto draw;
        case '7': c =  &st->blockHiThin;
                  goto draw;
        case '8': c =  &st->blockLoThin;
                  goto draw;
        case '9': c =  &st->blockBtm;
                  goto draw;
        case '0': c =  &st->midBlock;
                  goto draw;
        default : break;
      }
      switch (ev.ch) {
	      case 'w': statY = (statY > 0) ? statY - 1 : 0;
			tb_set_cursor(statX, statY);
			tb_set_cell(  statX, statY, *  c, color.rgb, 0x20000000);
			goto draw;
	      case 's': statY = (statY < tb_height()-2) ? statY + 1 : statY;
			tb_set_cursor(statX, statY);
			tb_set_cell(  statX, statY,   *c, color.rgb, 0x20000000);
			goto draw;
	      case 'a': statX = (statX > 0) ? statX - 1 : 0;
			tb_set_cursor(statX, statY);
			tb_set_cell(  statX,   statY, *c, color.rgb, 0x20000000);
			goto draw;
	      case 'd': statX = (statX < tb_width()-1) ? statX + 1 : statX;
			tb_set_cursor(statX, statY);
			tb_set_cell(  statX,   statY, *c, color.rgb, 0x20000000);
			goto draw;
      }
      switch (ev.ch) {
        case 'z' : setColor(&color, ev.ch);
                   goto draw;
        case 'x' : setColor(&color, ev.ch);
                   goto draw;
        case 'c' : setColor(&color, ev.ch);
                   goto draw;
        default  : break;
      }
    }
    if(ev.key == TB_KEY_MOUSE_LEFT) {
      tb_set_cursor(ev.x, ev.y);
      tb_set_cell(  ev.x, ev.y, *c, color.rgb, 0x20000000);
      statX =   (uint8_t) ev.x;
      statY =   (uint8_t) ev.y;
      goto draw;
    }
draw:
    drawPalette(slot.arr, slotSize, c);
    tb_printf(tb_width()-9, tb_height()-1, UI_FG, UI_BG, "%d, %d", statX, statY);

    for (uint8_t i = 1;  i < 5; i++) {
      tb_set_cell(tb_width()-23+i, tb_height()-1, slot.arr[i], color.rgb, UI_BG);
    }
    tb_printf(tb_width()-17, tb_height()-1, 0xc0c0c0, 0x401010, "%02X", color.r);
    tb_printf(tb_width()-15, tb_height()-1, 0xc0c0c0, 0x104010, "%02X", color.g);
    tb_printf(tb_width()-13, tb_height()-1, 0xc0c0c0, 0x101040, "%02X", color.b);
    
    tb_present();
    keyTest = ev.ch;
  }
  tb_shutdown();       // clean up so terminal isn't messed
  return 0;
}

