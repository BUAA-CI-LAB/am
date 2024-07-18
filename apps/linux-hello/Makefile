
BUILD_PATH = ./build

KERNEL_ENTRY_ADDRESS = a07c5c28 # loongarch32r-linux-gnusf-readelf -s vmlinux | grep kernel_entry

.DEFAULT_GOAL = all

CROSS_COMPILE = loongarch32r-linux-gnusf-
AS        = $(CROSS_COMPILE)gcc
CC        = $(CROSS_COMPILE)gcc
CXX       = $(CROSS_COMPILE)g++
LD        = $(CROSS_COMPILE)ld
OBJDUMP   = $(CROSS_COMPILE)objdump
OBJCOPY   = $(CROSS_COMPILE)objcopy
READELF   = $(CROSS_COMPILE)readelf

init:
	mkdir -p ${BUILD_PATH}

start: init
	$(CC) -DKERNEL_ENTRY_ADDRESS=0x${KERNEL_ENTRY_ADDRESS} -c start.S -o ${BUILD_PATH}/start.o
	$(OBJDUMP) -d ${BUILD_PATH}/start.o > ${BUILD_PATH}/start.s
	$(OBJCOPY) -O binary -j .text ${BUILD_PATH}/start.o ${BUILD_PATH}/start.bin
	rm $(BUILD_PATH)/start.o

linux: ./vmlinux
	$(OBJCOPY) -O binary ./vmlinux $(BUILD_PATH)/vmlinux.bin
	$(OBJDUMP) -d vmlinux > $(BUILD_PATH)/vmlinux.s

convert: linux_convert.c
	gcc linux_convert.c -o linux_convert


all:init start linux convert
	./linux_convert
	rm -f $(BUILD_PATH)/vmlinux.bin
	rm -f $(BUILD_PATH)/start.bin
	rm -f linux_convert


clean:
	rm -rf ${BUILD_PATH}

.PHONY: init start linux convert all clean