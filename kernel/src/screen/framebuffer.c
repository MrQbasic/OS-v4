#include "screen/framebuffer.h"

int maxcharX;
int maxcharY;

extern unsigned char _font_start[];


struct limine_framebuffer_request framebuffer_request;
struct limine_framebuffer *framebuffer;
volatile uint32_t *fb_prt;
 

void getSizeChars(int* maxX, int* maxY){
    *maxX=maxcharX;
    *maxY=maxcharY;
}


int setupFramebuffer(struct limine_framebuffer_request fb_req){
    framebuffer_request = fb_req;
    //get the Framebuffer from the bootloader
    if (framebuffer_request.response == 0 || framebuffer_request.response->framebuffer_count < 1) {
        return 1;
    }
    framebuffer = framebuffer_request.response->framebuffers[0];
    fb_prt = framebuffer->address;

    maxcharX = framebuffer->width / 6 - 1;
    maxcharY = framebuffer->height / 14 - 1;

    return 0;
}

void clearFrameBuffer(uint32_t color){
    for(int x=0; x<framebuffer->width; x++){
        for(int y=0; y<framebuffer->height; y++){
            fb_prt[x+y*framebuffer->width] = color;
        }
    }
}

void putChar(char c, int cx, int cy, uint32_t color){
    c = c % 200;
    for(int x=0; x<6; x++){
        for(int y=0; y<14; y++){
            int bitmapIndex = ((int)c) * 14 + y;
            int fb_x = x + cx*6;
            int fb_y = y + cy*14;
            fb_prt[fb_x + fb_y*framebuffer->width] = color * ((_font_start[bitmapIndex] >> (8-x)) & 1);    
        }
    }
}


void scrollScreen(int n){
    for(int i=n; i<framebuffer->height; i++){
        for(int j=0; j<framebuffer->width; j++){
            fb_prt[j+(i-n)*framebuffer->width] = fb_prt[j+i*framebuffer->width]; 
        }
    }
}