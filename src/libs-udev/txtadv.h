#ifndef __TXTADV_H__
#define __TXTADV_H__

#include <stdint.h>
#include "files.h"

typedef struct {       
	uint16_t addr;
	uint16_t len;
} ta_voc_word;

typedef enum {
	TA_MODE_TERM,
	TA_MODE_AVDC
	//TA_MODE_GDP
} ta_mode;

void ta_set_mode(ta_mode mode);

void ta_user_input(char *buffer, const char *prev, uint8_t txt_len, bool prompt);

#endif