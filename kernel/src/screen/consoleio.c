#include "screen/consoleio.h"

int cur_X, cur_Y;
uint32_t textCol = colorWHITE;

void screenClear(){
    cur_X = 0;
    cur_Y = 0;
    clearFrameBuffer(0);
}


// 0 -> Normal
// 1 -> printDisables (only function screenPrint)
void screenSetPrintStatus(uint8_t status);


void screenCursorMove(int x, int y){
    cur_X += cur_X < 0 ? 0 : x;
    cur_Y += cur_Y < 0 ? 0 : y;
    if(cur_X > maxcharX) cur_X = maxcharX;
    if(cur_Y > maxcharY) cur_Y = maxcharY;
}

void screenCursorSet(int x, int y){
    cur_X = x;
    cur_Y = y;
    if(cur_X < 0) cur_X = 0;
    if(cur_Y < 0) cur_Y = 0;
    if(cur_X > maxcharX) cur_X = maxcharX;
    if(cur_Y > maxcharY) cur_Y = maxcharY;
}

void screenUp(){
    scrollScreen(14);
}

void screenNl(){
    cur_X = 0;
    cur_Y++;
    if(cur_Y > maxcharY){
        cur_Y--;
        screenUp();
    }
}

void screenPrintChar(char c){
    putChar(c, cur_X, cur_Y, textCol);
    cur_X++;
    if(cur_X > maxcharX){
        cur_X = 0; 
        cur_Y++;
    }
    if(cur_Y > maxcharY){
        cur_Y--;
        screenUp();
    }
}

void screenPrintChars(char *c){
    int i = 0;
    while(1){
        if(c[i] == '/'){
            i++;
            switch(c[i]){
                case 'e':
                    return;
                case 'n':
                    screenNl();
                    i++;
                    continue; 
            }
        }else{
            screenPrintChar(c[i]);
        }
        i++;
    }
}

char hex_digits[] = "0123456789ABCDEF"; 
char hex_prefix[] = "0x/e";

void screenPrintX(uint64_t inp, uint8_t bits){
    screenPrintChars(hex_prefix);
    for(int i=(bits-4); i>=0; i=i-4){
        uint64_t offset = (inp >> i) & 0xF;
        screenPrintChar(hex_digits[offset]);
    }
}

char bin_digits[] = "01";
char bin_prefix[] = "0b/e";

void screenPrintB(uint64_t inp, uint8_t bits){
    screenPrintChars(bin_prefix);
    for(int i=(bits-1); i>=0; i--){
        uint64_t offset = (inp >> i) & 1;
        screenPrintChar(bin_digits[offset]);
    }
}



uint8_t printStatus = 0;

void screenPrint(const char* fmt, ...){
    if(printStatus == 1) return;
    va_list args;
    va_start(args,fmt);
    while(1){
        if(*fmt == '/'){
            fmt++;
            if(*fmt == 'n'){
                screenNl();
            }else if(*fmt == '0'){
                screenClear();
            }else if(*fmt == 'e'){
                va_end(args);
                return;
            }else if(*fmt == 'x'){

                fmt++;
                switch(*fmt){
                case 'B':
                    screenPrintX((uint8_t)  va_arg(args, uint32_t), 8);  break;
                case 'W':
                    screenPrintX((uint16_t) va_arg(args, uint32_t), 16); break;
                case 'D':
                    screenPrintX((uint32_t) va_arg(args, uint32_t), 32); break;
                case 'Q': 
                    screenPrintX((uint64_t) va_arg(args, uint64_t), 64); break;
                }
            }else if(*fmt == 'b'){
                fmt++;

                switch(*fmt){
                case 'B':
                    screenPrintB((uint8_t)  va_arg(args, uint32_t), 8);  break;
                case 'W':
                    screenPrintB((uint16_t) va_arg(args, uint32_t), 16); break;
                case 'D':
                    screenPrintB((uint32_t) va_arg(args, uint32_t), 32); break;
                case 'Q':
                    screenPrintB((uint64_t) va_arg(args, uint64_t), 64); break;
                }
                
            }else if(*fmt == 'c'){
                screenPrintChars(va_arg(args, char*));
            }else if(*fmt == 'r'){
                textCol = va_arg(args, uint32_t);
            }
        }else{
            screenPrintChar(*fmt);
        }
        fmt++;
    }
}

void screenSetPrintStatus(uint8_t status){
    printStatus = status;
}

void screenMemoryDump(uint64_t start, uint64_t size){
    size = size & 0xFFFF;
    for(int i=0; i < size; i += SCREEN_MEMORY_DUMP_BYTES_PER_ROW){
        screenPrint("/xW: /e",i);
        for(int j=0; j<SCREEN_MEMORY_DUMP_BYTES_PER_ROW; j++){
            uint8_t* b = (uint8_t*) start + i + j;
            screenPrint("/xB /e ", *b);
        }
        screenNl();
    }
}

