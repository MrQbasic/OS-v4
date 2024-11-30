#pragma once
#include <stdint.h>
#include <limine.h>

extern int maxcharX;
extern int maxcharY;

//returns 0 if ok
int setupFramebuffer(struct limine_framebuffer_request fb_req);


//clears the entrie framebuffer with one color
void clearFrameBuffer(uint32_t color);


void putChar(char c, int cx, int cy, uint32_t color);


void scrollScreen(int n);


void getSizeChars(int* maxX, int* maxY);