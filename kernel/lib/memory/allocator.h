#include "screen/consoleio.h"

void* kernelmem = (void*) 0xffffffff00000000;

int mem_init(){
    screenPrint("Setting up kernel memory @: /xQ/n/e", kernelmem);

}