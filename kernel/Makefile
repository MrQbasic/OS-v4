BUILD_DIR := ../build/kernel

# Compiler and linker settings
CC = gcc
LD = ld

FONT   := ../tools/font.bin
FONT_O := $(BUILD_DIR)/font.o

# Flags for the compiler and linker
# Compiler and linker flags
CFLAGS = -fno-stack-protector -ffreestanding -m64 -nostdlib -c -I $(DIR_INCLUDE)/ -g
LDFLAGS = -m elf_x86_64 -T linker.ld



# Output filename
OUTPUT = $(BUILD_DIR)/kernel.elf

# Default target
all: $(OUTPUT)

# Compiling the kernel
$(OUTPUT): $(BUILD_DIR)/kernel.o $(FONT_O)
	$(LD) $(LDFLAGS) -o $@ $^

$(FONT_O): $(FONT)
	objcopy -I binary -O elf64-x86-64 --rename-section .data=.font $^ $@

# Compiling the object file
$(BUILD_DIR)/kernel.o: kernel.c
	$(CC) $(CFLAGS) -c $^ -o $@