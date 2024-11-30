# Flags for the compiler and linker
# Compiler and linker flags
CFLAGS = -fno-stack-protector -ffreestanding -m64 -nostdlib -c -I $(DIR_INCLUDE)/ -g
LDFLAGS = -m elf_x86_64 -T linker.ld


#Dirs
DIR_BUILD   := ../build/kernel
DIR_CODE    := src
DIR_INCLUDE := lib

#compilers and flags
NASM	   := nasm
NASM_FLAGS := -f elf64
GCC        := gcc
GCC_FLAGS  := -fno-stack-protector -ffreestanding -m64 -nostdlib -c -I $(DIR_INCLUDE)/ -g
LD		   := ld
LD_FLAGS   := -m elf_x86_64 -T linker.ld

#files
FILE_OUT  := kernel.elf

FILES_C_ALL := $(shell find $(DIR_CODE) -type f \( -name '*.c' -not -wholename '*/kernel.c' \))
FILES_C_ISR := $(shell find $(DIR_CODE) -name '*.isr.c')
FILES_C	    := $(DIR_CODE)/kernel.c $(filter-out $(FILES_C_ISR), $(FILES_C_ALL))

FILES_S 	:= $(shell find $(DIR_CODE) -name '*.s')

FILE_FONT   := ../tools/font.bin

FILE_FONT_O   := $(DIR_BUILD)/font.o
FILES_O_C	  := $(patsubst $(DIR_CODE)/%.c,$(DIR_BUILD)/%.o,$(FILES_C))
FILES_O_C_ISR := $(patsubst $(DIR_CODE)/%.c,$(DIR_BUILD)/%.o,$(FILES_C_ISR))
FILES_O_S     := $(patsubst $(DIR_CODE)/%.s,$(DIR_BUILD)/%.o,$(FILES_S))
FILES_O       := $(FILES_O_C) $(FILES_O_C_ISR) $(FILES_O_S) $(FILE_FONT_O)

BUILD_DIRS    := $(sort $(dir $(FILES_O))) 

.SILENT: $(FILE_OUT) clean

#LINK KERNEL
$(DIR_BUILD)/$(FILE_OUT): $(FILES_O) 
	$(LD) $(LD_FLAGS) -o $@ $^

#COMPILE ISR
$(FILES_O_C_ISR): $(DIR_BUILD)/%.o: $(DIR_CODE)/%.c | $(BUILD_DIRS)
	$(GCC) $(GCC_FLAGS) -mgeneral-regs-only -c $^ -o $@

#COMPILE NORMAL
$(FILES_O_C): $(DIR_BUILD)/%.o: $(DIR_CODE)/%.c | $(BUILD_DIRS)
	$(GCC) $(GCC_FLAGS) -c $^ -o $@

#COMPILE ASM
$(FILES_O_S): $(DIR_BUILD)/%.o: $(DIR_CODE)/%.s | $(BUILD_DIRS)
	$(NASM) $^ $(NASM_FLAGS) -o $@ 

#REPACKAGE FONT
$(FILE_FONT_O): $(FILE_FONT) | $(BUILD_DIRS)
	objcopy -I binary -O elf64-x86-64 --rename-section .data=.font $^ $@

$(BUILD_DIRS): 
	mkdir -p $@