#include <Arduino.h>
#include <EEPROM.h>
#define SECTOR_START 255

static const unsigned char spaceman[5][6016]  = {
{
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x80,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x08,0x80,0x07,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x78,0x80,0x03,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xF0,0xC0,0x03,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xC0,0x01,0x06,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xF8,0x0F,0x03,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xFE,0x3F,0x06,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x80,0x07,0x78,0x0C,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xC0,0x03,0xE0,0x1C,0x80,0x03,0x00,0x00,0x00,0x00,0x00,0x00,0xE0,0x03,0xC0,0x19,0x80,0x03,0x00,0x00,0x00,0x00,0x00,0x00,0xF0,0x00,0x80,0x33,0x00,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x70,0x00,0x00,0x63,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x78,0x00,0x00,0x66,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x38,0x00,0x00,0xC6,0x40,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xCC,0x1F,0x00,0x8C,0xC1,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xFC,0x7F,0x00,0x8C,0xE1,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xFC,0xFF,0x00,0x0E,0x80,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xFC,
0xFF,0x01,0x0F,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xFC,0xFF,0x03,0x0E,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xFC,0xFF,0x07,0x0F,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xFC,0xFF,0x07,0x0F,0x00,0x00,0x00,0x00,0x00,0x00,0x40,0x00,0xF8,0xFF,0x07,0x0E,0x00,0x06,0x00,0x00,0x00,0x00,0xC0,0x00,0xF8,0xFF,0x07,0x06,0x00,
0x0F,0x00,0x00,0x00,0x00,0xC0,0x00,0xF8,0xFF,0x0F,0x06,0x00,0x1B,0x00,0x00,0x00,0x00,0x40,0x00,0xF0,0xFF,0x0F,0x1F,0x00,0x0F,0x00,0x00,0x00,0x00,0x00,0x00,0xE0,0xFF,0x87,0x7B,0x00,0x0E,0x03,0x00,0x00,0x00,0x00,0x00,0xE0,0xFF,0xC7,0xE1,0x00,0x00,0x03,0x00,0x00,0x00,0x00,0x00,0xC0,0xFF,0xF7,0x80,0x01,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0xFF,0x3F,0x06,0x03,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xFE,0x1F,0x1E,0x07,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xF0,0x03,0xB8,0x0F,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x03,0xE0,0x09,0x00,0x00,0x00,0x00,0x00,0x00,0x03,0x00,0x00,0x06,0xC0,0x0C,0x00,0x00,0x00,0x00,0x00,0x00,0x02,0x00,
0x00,0x0E,0x84,0x07,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x0E,0x0F,0x07,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x1C,0x9B,0x0F,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x1C,0xDB,0x0D,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x7C,0xEF,0x18,0x08,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xF8,0x66,0x18,
0x18,0x30,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xF8,0x04,0x18,0x30,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xF0,0x27,0x18,0x70,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x30,0xE0,0x63,0x7C,0x60,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x60,0x00,0xC3,0x67,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xC0,0x00,0xCE,0x33,0x00,0x00,0x00,0x00,
0x00,0x00,0x80,0x06,0xC0,0x00,0xFC,0x1F,0x06,0x00,0x00,0x00,0x00,0x00,0x00,0x07,0x80,0x01,0x78,0x0F,0x0C,0x00,0x00,0x00,0x00,0x00,0x00,0x06,0x00,0x03,0xE0,0x03,0x0C,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x03,0xC0,0x01,0x08,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x06,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x0C,0x08,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xC0,0x00,0x00,0x18,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xC0,0x00,0x00,0x10,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xC0,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x80,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00
/* (94 X 64 )*/

},{
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x08,0x80,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x78,0x00,0x0F,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xF0,0x00,0x07,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xC0,0x81,0x07,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xF8,0x0F,0x03,0x0C,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xFE,0x3F,0x06,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x80,0x07,0x78,0x0C,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xC0,0x03,0xE0,0x1C,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xE0,0x03,0xC0,0x19,0x00,0x07,0x00,0x00,0x00,0x00,0x00,0x00,0xF0,0x00,0x80,0x33,0x00,0x07,0x00,0x00,0x00,0x00,0x00,0x00,0x70,0x00,0x00,0x63,0x00,0x02,0x00,0x00,0x00,
0x00,0x00,0x00,0x78,0x00,0x00,0x66,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x38,0x00,0x00,0xC6,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xCC,0x1F,0x00,0x8C,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xFC,0x7F,0x00,0x8C,0xC1,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0xFC,0xFF,0x00,0x0E,0xC0,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0xFC,
0xFF,0x01,0x0F,0x00,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0xFC,0xFF,0x03,0x0E,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xFC,0xFF,0x07,0x0F,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xFC,0xFF,0x07,0x0F,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xF8,0xFF,0x07,0x0E,0x00,0x00,0x00,0x00,0x00,0x00,0xC0,0x00,0xF8,0xFF,0x07,0x06,0x00,
0x00,0x00,0x00,0x00,0x00,0x80,0x00,0xF8,0xFF,0x0F,0x06,0x00,0x1E,0x00,0x00,0x00,0x00,0x80,0x01,0xF0,0xFF,0x0F,0x1F,0x00,0x1E,0x00,0x00,0x00,0x00,0x80,0x00,0xE0,0xFF,0x87,0x7B,0x00,0x1E,0x00,0x00,0x00,0x00,0x00,0x00,0xE0,0xFF,0xC7,0xE1,0x00,0x1C,0x07,0x00,0x00,0x00,0x00,0x00,0xC0,0xFF,0xF7,0x80,0x01,0x00,0x07,0x00,0x00,
0x00,0x00,0x00,0x00,0xFF,0x3F,0x06,0x03,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xFE,0x1F,0x1E,0x07,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xF0,0x03,0xB8,0x0F,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x03,0xE0,0x09,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x06,0xC0,0x0C,0x00,0x00,0x00,0x00,0x00,0x00,0x06,0x00,
0x00,0x0E,0x84,0x07,0x00,0x00,0x00,0x00,0x00,0x00,0x06,0x00,0x00,0x0E,0x0F,0x07,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x1C,0x9B,0x0F,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x1C,0xDB,0x0D,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x7C,0xEF,0x18,0x08,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xF8,0x66,0x18,
0x18,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xF8,0x04,0x18,0x30,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xF0,0x27,0x18,0x70,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x30,0xE0,0x63,0x7C,0x60,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x60,0x00,0xC3,0x67,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xC0,0x00,0xCE,0x33,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0xC0,0x00,0xFC,0x1F,0x06,0x00,0x00,0x00,0x00,0x00,0x00,0x0D,0x80,0x01,0x78,0x0F,0x0C,0x00,0x00,0x00,0x00,0x00,0x00,0x0E,0x00,0x03,0xE0,0x03,0x0C,0x00,0x00,0x00,0x00,0x00,0x00,0x0C,0x00,0x03,0xC0,0x01,0x08,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x06,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x0C,0x08,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x18,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x10,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x80,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x80,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x03,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00
/* (94 X 64 )*/

},{
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x08,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x78,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xF0,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xC0,0x01,0x16,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xF8,0x0F,0x03,0x1C,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xFE,0x3F,0x06,0x0E,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x80,0x07,0x78,0x0C,0x1A,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xC0,0x03,0xE0,0x1C,0x10,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xE0,0x03,0xC0,0x19,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xF0,0x00,0x80,0x33,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x70,0x00,0x00,0x63,0x00,0x0C,0x00,0x00,0x00,
0x00,0x00,0x00,0x78,0x00,0x00,0x66,0x00,0x1C,0x00,0x00,0x00,0x00,0x00,0x00,0x38,0x00,0x00,0xC6,0x00,0x0C,0x00,0x00,0x00,0x00,0x00,0x00,0xCC,0x1F,0x00,0x8C,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xFC,0x7F,0x00,0x8C,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xFC,0xFF,0x00,0x0E,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xFC,
0xFF,0x01,0x0F,0x00,0x07,0x00,0x00,0x00,0x00,0x00,0x00,0xFC,0xFF,0x03,0x0E,0x00,0x07,0x00,0x00,0x00,0x00,0x00,0x00,0xFC,0xFF,0x07,0x0F,0x00,0x07,0x00,0x00,0x00,0x00,0x00,0x00,0xFC,0xFF,0x07,0x0F,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xF8,0xFF,0x07,0x0E,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xF8,0xFF,0x0F,0x06,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xF8,0xFF,0x0F,0x06,0x00,0x00,0x00,0x00,0x00,0x00,0x80,0x01,0xF0,0xFF,0x0F,0x1F,0x00,0x00,0x00,0x00,0x00,0x00,0xC0,0x03,0xE0,0xFF,0x87,0x7B,0x00,0x38,0x00,0x00,0x00,0x00,0x00,0x02,0xE0,0xFF,0xC7,0xE1,0x00,0x78,0x00,0x00,0x00,0x00,0x00,0x03,0xC0,0xFF,0xF7,0x80,0x01,0x6C,0x00,0x00,0x00,
0x00,0x80,0x01,0x00,0xFF,0x3F,0x06,0x03,0x78,0x0C,0x00,0x00,0x00,0x00,0x00,0x00,0xFE,0x1F,0x1E,0x07,0x10,0x0C,0x00,0x00,0x00,0x00,0x00,0x00,0xF0,0x03,0xB8,0x0F,0x00,0x08,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x03,0xE0,0x09,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x06,0xC0,0x0C,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x0E,0x84,0x07,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x0E,0x0F,0x07,0x00,0x00,0x00,0x00,0x00,0x00,0x18,0x00,0x00,0x1C,0x9B,0x0F,0x00,0x00,0x00,0x00,0x00,0x00,0x18,0x00,0x00,0x1C,0xDB,0x0D,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x7C,0xEF,0x18,0x08,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xF8,0x66,0x18,
0x18,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xF8,0x04,0x18,0x30,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xF0,0x27,0x18,0x70,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x30,0xE0,0x63,0x7C,0x60,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x60,0x00,0xC3,0x67,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xC0,0x00,0xCE,0x33,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0xC0,0x00,0xFC,0x1F,0x06,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x80,0x01,0x78,0x0F,0x0C,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x03,0xE0,0x03,0x0C,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x03,0xC0,0x01,0x08,0x00,0x00,0x00,0x00,0x00,0x00,0x38,0x00,0x06,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x38,
0x00,0x0C,0x08,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x18,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x10,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x02,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00
/* (94 X 64 )*/

},{
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x80,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x80,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01,0x06,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x04,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xF8,0x07,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xFE,0x3F,0x06,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x80,0x07,0x78,0x0C,0x18,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xC0,0x01,0xE0,0x18,0xF0,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xE0,0x01,0xC0,0x19,0x70,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xF0,0x01,0x80,0x33,0x78,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x71,0x00,0x00,0x63,0xC0,0x00,0x00,0x00,0x00,
0x00,0x00,0x80,0x79,0x00,0x00,0x66,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xC0,0x39,0x00,0x00,0xC6,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xC0,0x0C,0x00,0x00,0x8C,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0xC0,0xFC,0x0F,0x00,0x8C,0x03,0x70,0x00,0x00,0x00,0x00,0x00,0xC0,0xFC,0x3F,0x00,0x0F,0x03,0x70,0x00,0x00,0x00,0x00,0x00,0xC0,0xFC,
0x7F,0x00,0x0E,0x06,0x20,0x00,0x00,0x00,0x00,0x00,0xC0,0xFC,0xFF,0x00,0x0E,0x0C,0x00,0x00,0x00,0x00,0x00,0xC0,0xC0,0xFC,0xFF,0x01,0x0E,0x0C,0x00,0x00,0x00,0x00,0x00,0x80,0xC0,0xFC,0xFF,0x01,0x0E,0x18,0x08,0x00,0x00,0x00,0x00,0xC0,0xC0,0xF8,0xFF,0x03,0x0E,0x10,0x18,0x00,0x00,0x00,0x00,0x40,0x80,0xF9,0xFF,0x03,0x3E,0x00,
0x1C,0x00,0x00,0x00,0x00,0x00,0x00,0xF8,0xFF,0x03,0x7E,0x00,0x10,0x00,0x00,0x00,0x00,0x00,0x00,0xF0,0xFF,0x03,0x6F,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xE0,0xFF,0x83,0xDF,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xE0,0xFF,0xC3,0xF1,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xC0,0xFF,0xF3,0xE0,0x01,0x00,0x00,0x00,0x00,
0x00,0x00,0x0E,0x00,0xFF,0x3F,0x83,0x03,0x00,0x00,0x00,0x00,0x00,0x00,0x1B,0x00,0xFE,0x1F,0x0F,0x07,0xE0,0x01,0x00,0x00,0x00,0x00,0x1B,0x00,0xF0,0x03,0x9C,0x07,0x60,0x01,0x00,0x00,0x00,0x00,0x0E,0x00,0x00,0x02,0xF0,0x0D,0xE0,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x06,0xE0,0x0C,0xC0,0x21,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x0E,0xC0,0x07,0x00,0x20,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x0C,0x87,0x07,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x9C,0x8F,0x07,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x9C,0xCD,0x0D,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xB8,0xEF,0x18,0x00,0x00,0x00,0x00,0x00,0x00,0x60,0x00,0x00,0x78,0x77,0x18,
0x00,0x00,0x00,0x00,0x00,0x00,0x60,0x00,0x00,0x70,0x10,0x18,0x30,0x00,0x00,0x00,0x00,0x00,0x60,0x00,0x00,0xF0,0x17,0x18,0x30,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xE0,0x33,0x7C,0x60,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xE3,0x6F,0xC0,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xCE,0x73,0xC0,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0xC0,0x00,0xFC,0x39,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x80,0x01,0x70,0x0F,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x03,0xE0,0x03,0x0C,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x03,0xC0,0x01,0x18,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x06,0x00,0x00,0x10,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x0C,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x1C,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x80,0x00,0x18,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xC0,0x00,0x00,0x60,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x60,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00
/* (94 X 64 )*/

},{
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x06,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x0C,0x30,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x30,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xF8,0x07,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xFE,0x3F,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x80,0x07,0x78,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xC0,0x01,0xE0,0x18,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xE0,0x00,0xC0,0x19,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xF0,0x00,0x80,0x33,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xF0,0x00,0x00,0x63,0xC0,0x02,0x00,0x00,0x00,
0x00,0x00,0x00,0x78,0x00,0x00,0x66,0x80,0x03,0x00,0x00,0x00,0x00,0x00,0x00,0x38,0x00,0x00,0xC6,0xC0,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x3C,0x00,0x00,0x8F,0x41,0x03,0x00,0x00,0x00,0x00,0x00,0x40,0xFC,0x03,0x00,0x8F,0x03,0x02,0x00,0x00,0x00,0x00,0x00,0xC0,0xFC,0x0F,0x00,0x0F,0x03,0x00,0x00,0x00,0x00,0x00,0x00,0xC0,0xFC,
0x1F,0x00,0x0E,0x06,0x00,0x00,0x00,0x00,0x00,0x00,0xC0,0xFC,0x7F,0x00,0x0E,0x0C,0x80,0x01,0x00,0x00,0x00,0x00,0xC0,0xFC,0x7F,0x00,0x0E,0x0C,0x80,0x01,0x00,0x00,0x00,0x00,0xC0,0xFC,0xFF,0x00,0x0C,0x18,0x80,0x01,0x00,0x00,0x00,0x00,0xC0,0xF8,0xFF,0x01,0x3E,0x30,0x00,0x00,0x00,0x00,0x00,0x80,0x83,0xF9,0xFF,0x01,0x7E,0x70,
0x00,0x00,0x00,0x00,0x00,0x80,0x87,0xF9,0xFF,0x01,0xEE,0x60,0x00,0x00,0x00,0x00,0x00,0xC0,0x06,0xF3,0xFF,0x01,0xC7,0x00,0xE0,0x00,0x00,0x00,0x00,0x80,0x03,0xE3,0xFF,0x81,0x8F,0x01,0xE0,0x00,0x00,0x00,0x00,0x00,0x01,0xE6,0xFF,0xC1,0x1F,0x03,0xE0,0x00,0x00,0x00,0x00,0x00,0x00,0xC6,0xFF,0xF1,0x3C,0x06,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x0C,0xFF,0x3D,0x38,0x06,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x18,0xFE,0x1F,0x73,0x0C,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x38,0xF0,0x07,0xE3,0x18,0x00,0x00,0x00,0x00,0x00,0x00,0x38,0x30,0x00,0x06,0xE6,0x19,0x00,0x00,0x00,0x00,0x00,0x00,0x78,0x00,0x00,0x0C,0xCC,0x0F,0x00,0x07,0x00,0x00,0x00,0x00,0x6C,0x00,
0x00,0x0C,0xEC,0x07,0x00,0x0F,0x00,0x00,0x00,0x00,0x78,0x00,0x00,0x0C,0xB9,0x07,0x80,0x0D,0x00,0x00,0x00,0x00,0x30,0x00,0x00,0x98,0x1F,0x07,0x00,0x0F,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xD8,0xB6,0x07,0x00,0x06,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xF0,0xE6,0x0D,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xF0,0x07,0x0C,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xE0,0x03,0x0C,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xC0,0x03,0x0C,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x07,0x1C,0x60,0x00,0x00,0x00,0x00,0x00,0x80,0x03,0x00,0x00,0x1C,0x3F,0xC0,0x00,0x00,0x00,0x00,0x00,0x80,0x03,0x00,0x00,0xF8,0x33,0xC0,0x01,0x00,0x00,
0x00,0x00,0x00,0x03,0x00,0x00,0xE0,0x18,0x80,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xC0,0x0E,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x03,0xC0,0x07,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x03,0x80,0x01,0x10,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x06,0x00,0x00,0x30,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x0C,0x00,0x00,0x70,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x1C,0x00,0x00,0x20,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x18,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x30,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x60,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x80,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x80,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00
/* (94 X 64 )*/

}

};


void save_img(int index, const unsigned char * str){
  EEPROMClass MYROM_0(2*index + SECTOR_START);
  MYROM_0.begin(3008);
  for(int i = 0 ; i < 3008; i++){
    MYROM_0.write(i, str[i]);
  }
  MYROM_0.commit();
  MYROM_0.end();

  EEPROMClass MYROM_1(2*index + 1 + SECTOR_START);
  MYROM_1.begin(3008);
  for(int i = 0 ; i < 3008; i++){
    MYROM_1.write(i, str[ i + 3008]);
  }
  MYROM_1.commit();
  MYROM_1.end();
}




void setup(void) {
  Serial.begin(115200);
  int c = 8;
  save_img(c*5, spaceman[0]);
  save_img(c*5+1, spaceman[1]);
  save_img(c*5+2, spaceman[2]);
  save_img(c*5+3, spaceman[3]);
  save_img(c*5+4, spaceman[4]);
  
}

void loop(void) {
  
  
}