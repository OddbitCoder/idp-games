#include <stdlib.h>
#include "../common/avdc.h"
#include "../common/utils.h"
#include "../common/gdp.h"
#include "align.h"

void avdc_write_at_pointer_display_off(uint16_t addr, uint8_t chr, uint8_t attr) {
    //avdc_wait_access();
    // CPU checks RDFLG status bit to assure that any delayed commands have been completed.
    avdc_wait_ready();
    // CPU writes addr into pointer registers.
    AVDC_CMD = AVDC_CMD_SET_PTR_REG;
    AVDC_INIT = LO(addr);
    AVDC_INIT = HI(addr);
    // CPU loads data to be written to display memory into the interface latch.
    AVDC_CHR = chr;
    AVDC_ATTR = attr;
    // CPU issues 'write at pointer' command. AVDC generates control signals and outputs specified addr to perform requested operation. Data is copied from the interface latch into the memory. AVDC sets RDFLG status to indicate that the write is completed.
    AVDC_CMD = AVDC_CMD_WRITE_AT_PTR;
}

void avdc_define_glyphs(avdc_glyph_defs *glyph_defs) {
    // reset AVDC
    avdc_wait_access();
    avdc_wait_ready();
    AVDC_CMD = AVDC_CMD_RESET;
    // in the trace, reset is called twice with a "wait" between the two calls
    while ((AVDC_ACCESS & AVDC_ACCESS_FLAG) != 0);
    AVDC_CMD = AVDC_CMD_RESET;
    // set common text attributes
    //AVDC_COMMON_TXT_ATTR = mode == AVDC_MODE_80 ? 0x65 : (mode == AVDC_MODE_132 ? 0xC4 : custom_txt_attr_reg);
    // wait for V blanking
    while ((AVDC_GDP_STATUS & 2) != 2);
    while ((AVDC_GDP_STATUS & 2) == 2);
    // set Screen Start 2
    AVDC_SCREEN_START_2_LOWER = 0;
    AVDC_SCREEN_START_2_UPPER = 0;
    // write to init registers
    //AVDC_CMD = AVDC_CMD_SET_REG_0;
    //for (uint8_t i = 0; i < 10; i++) {
    //    AVDC_INIT = init_str[i];
    //}
    // for some reason, set Screen Start 2 again
    AVDC_SCREEN_START_2_LOWER = 0;
    AVDC_SCREEN_START_2_UPPER = 0;
    // define glyphs
    if (glyph_defs != NULL) {
        for (uint8_t i = 0; i < glyph_defs->count; i++) {
            avdc_glyph_def *def = &glyph_defs->items[i];
            uint16_t addr = 8 * 1024 + def->char_code * 16;
            for (uint8_t i = 0; i < 16; i++) {
                avdc_write_at_pointer_display_off(addr++, def->char_data[i], 0);
            }
        }
    }
    // display on (turned off by reset)
    AVDC_CMD = AVDC_CMD_DISPLAY_ON;
    // wrap-up 
    avdc_cursor_off();
    //avdc_purge();
}

avdc_glyph_defs glyph_defs = {
    10, { 
        { 'A', "\x00\x50\x10\x10\x50\x54\x54\x54\x54\x54\x54\x50\x10\x10\x50\x00" },
        { 'B', "\x00\x55\x11\x11\x55\x55\x05\x05\x05\x05\x55\x55\x11\x11\x55\x00" },
        { 'C', "\x00\x01\x01\x01\x55\x11\x15\x15\x15\x15\x11\x55\x01\x01\x01\x00" },
        { 'D', "\x00\x00\x54\x14\x55\x14\x54\x14\x14\x54\x14\x55\x14\x54\x00\x00" },
        { 'E', "\x00\x00\x40\x54\x54\x51\x55\x55\x55\x55\x51\x54\x54\x40\x00\x00" },
        { 'F', "\x00\x00\x05\x55\x41\x50\x50\x50\x50\x50\x50\x41\x55\x05\x00\x00" },
        { 'G', "\x00\x00\x40\x55\x54\x05\x45\x45\x45\x45\x05\x54\x55\x40\x00\x00" },
        { 'H', "\x00\x00\x05\x15\x15\x50\x14\x14\x14\x14\x50\x15\x15\x05\x00\x00" },
        { 'I', "\x00\x54\x54\x54\x40\x50\x05\x44\x44\x05\x50\x40\x54\x54\x54\x00" },
        { 'J', "\x00\x05\x05\x05\x00\x55\x51\x44\x44\x51\x55\x00\x05\x05\x05\x00" }
    }
};

int main() {
	gdp_init();

    avdc_init_ex(AVDC_MODE_132, 0, NULL);
    avdc_define_glyphs(&glyph_defs);
	
    //void avdc_write_glyphs_at_pointer_pos(uint8_t row, uint8_t col, uint8_t glyph_count, uint8_t *glyphs, uint8_t attr) 
    avdc_write_glyphs_at_pointer_pos(0, 0, 10, "ABCDEFGHIJ", 0);

	while (!kbhit());

	avdc_done();
	gdp_done();

	return 0;
}