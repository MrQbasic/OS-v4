#pragma once

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

#include "bootloader.h"
#include "screen/framebuffer.h"
#include "screen/consoleio.h"
#include "memory/allocator.h"

void main(void) {
    // Ensure the bootloader actually understands our base revision (see spec).
    if (LIMINE_BASE_REVISION_SUPPORTED == false) {
        while(1);
    }

    //debug hook
    //for(uint64_t i=0; i<500*2000*2000; i++){}

    if(setupFramebuffer(framebuffer_request)){
        while(1);
    }
    screenPrint("/0---KERNEL OS-v4---/n/e");

    mem_init();


    while(1);
}