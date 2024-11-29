DIR_BUILD := ./build

IMG_FILE := ./build/OS-boot.img

#create img file


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

fresh:
	$(info Removing all files including finished OS)
	rm -rf $(IMG_FILE)