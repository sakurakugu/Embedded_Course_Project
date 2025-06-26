#pragma once

typedef struct 
{
    unsigned char Index[4];	
    char Msk[32];
}typFNT_GB16; 

typedef struct 
{
    unsigned char Index[4];	
    char Msk[72];
}typFNT_GB24; 

typedef struct 
{
    unsigned char Index[4];	
    char Msk[128];
}typFNT_GB32; 

extern const unsigned char asc2_1206[95][12];
extern const unsigned char asc2_1608[95][16];
extern const typFNT_GB16 tfont16[100];
extern const typFNT_GB24 tfont24[100];
extern const typFNT_GB32 tfont32[100];

extern const unsigned char gImage_bmp[];
extern const unsigned char gImage_zsc[];




