#pragma once
#include <stdint.h>
#include <stdarg.h>

#include "framebuffer.h"

#define colorRED      0xFFFF0000
#define colorGREEN    0xFF00FF00
#define colorBLUE     0xFF0000FF
#define colorWHITE    0xFFFFFFFF


#define SCREEN_DEFAULT_COLOR 0x0F


void screenUp();

void screenNl();

void screenSpace();

void screenSetChar(char c, int pos);

void screenPrintChar(char c);

void screenPrintChars(char *c);

void screenPrintX(uint64_t inp, uint8_t bits);

void screenPrintB(uint64_t inp, uint8_t bits);

void screenClear();

void screenCursorMove(int x, int y);

void screenCursorSet(int x, int y);

void screenCursorGet(int* x, int*y);

//commands:
//  /e  ->  end of string
//  /n  ->  new line
//  /0  ->  clear screen
//  /xB ->  print hex byte  (8-bit)
//  /xW ->  print hex word  (16-bit)
//  /xD ->  print hex dword (32-bit)
//  /xQ ->  print hex qword (64-bit)
//  /bB ->  print bin byte  (8-bit)
//  /bW ->  print bin word  (16-bit)
//  /bD ->  print bin dword (32-bit)
//  /bQ ->  print bin qword (64-bit)
//  /c  ->  print char[]    (ends with/e)
//  /r  ->  change Color    (32-bit)
void screenPrint(const char* fmt,...);


#define SCREEN_MEMORY_DUMP_BYTES_PER_ROW 8
void screenMemoryDump(uint64_t start, uint64_t size);

