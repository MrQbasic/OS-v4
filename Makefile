DIR_BUILD  := ./build
DIR_KERNEL := ./kernel 

IMG_FILE := ./build/OS-boot.img
KERNEL_BIN := $(DIR_BUILD)/kernel/kernel.elf


all: $(IMG_FILE)
FORCE: ;


$(IMG_FILE) : $(KERNEL_BIN) | FORCE
	@./mountImg.sh

	sudo mkdir -p $(DIR_BUILD)/mnt/boot/kernel
	sudo cp -f $^ $(DIR_BUILD)/mnt/boot/kernel

	@./umountImg.sh 2> /dev/zero


$(KERNEL_BIN): FORCE
	mkdir -p $(DIR_BUILD)/kernel
	@cd $(DIR_KERNEL) && make



run: $(IMG_FILE)
	rm -f log.txt
	clear
	qemu-system-x86_64 \
    -drive id=disk,file=$^,if=none,index=0,media=disk,format=raw \
    -device ahci,id=ahci \
    -device ide-hd,drive=disk,bus=ahci.0 \
    -cpu max \
    -D log.txt \
    -d int \
    -m 4G \
	-trace ahci* \
	-enable-kvm

debug: $(IMG_FILE)
	rm -f log.txt
	clear
	qemu-system-x86_64 \
    -drive id=disk,file=$^,if=none,index=0,media=disk,format=raw \
    -device ahci,id=ahci \
    -device ide-hd,drive=disk,bus=ahci.0 \
    -cpu max \
    -D log.txt \
    -d int \
    -m 4G \
	-trace *ahci* \
	-enable-kvm \
	-gdb tcp::1234

clean:
	$(info Removing all files including finished OS)
	./umountImg.sh 
	rm -rf $(DIR_BUILD)
	mkdir $(DIR_BUILD)
	./makeImg.sh