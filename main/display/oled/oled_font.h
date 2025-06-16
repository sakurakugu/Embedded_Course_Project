#ifndef __OLEDFONT_H
#define __OLEDFONT_H 	   

#include <cstdint>

typedef struct
{
    uint8_t value[4];   //UTF-8
	uint8_t data[24];   //
}HZZM_12;

extern const uint8_t chinese_12_len;
extern uint8_t  Hzk[][32];
extern const uint8_t  F6x8[][6];
extern const uint8_t  F8X16[];
extern const HZZM_12 chinese_12[5];

extern const uint8_t  BMP1[];

#endif


