#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "lib/bootloader.h"
#include "lib/screen/framebuffer.h"
#include "lib/screen/consoleio.h"

void main(void) {
    // Ensure the bootloader actually understands our base revision (see spec).
    if (LIMINE_BASE_REVISION_SUPPORTED == false) {
        while(1);
    }

    //debug hook
    //for(uint64_t i=0; i<500*2000*2000; i++){}


    if(setupFramebuffer()){
        while(1);
    }
    screenPrint("/0/e");


    for(uint32_t i=0; i<5; i++){
        screenPrint("/rHELLO WORLD! /rHEX: /xD /rBIN: /bD /n/e", colorRED, colorBLUE, i, colorGREEN, i);
    }

    while(1);
}