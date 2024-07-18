NAME = chiplabfunc
obj_path = ./build/obj
build_path = ./build
inst_srcs = $(wildcard ./inst/*.S)

txt_path = $(build_path)/$(NAME).txt
bin_path = $(build_path)/$(NAME).bin
# data_path = $(build_path)/$(NAME).data
elf_path = $(build_path)/$(NAME).elf

all:
	mkdir -p ${obj_path}
	make ${txt_path} ${bin_path}


${txt_path}: ${elf_path}
	loongarch32r-linux-gnusf-objdump -alD ${elf_path} > ${txt_path}

${obj_path}/libinst.a: ${inst_srcs}
	make -C inst libinst.a

${obj_path}/start.o: ${obj_path}/start.s
	loongarch32r-linux-gnusf-as -mabi=ilp32 ${obj_path}/start.s -o ${obj_path}/start.o

${bin_path}: ${elf_path}
	loongarch32r-linux-gnusf-objcopy -O binary  ${elf_path} ${bin_path}
# ${data_path}: ${elf_path}
# 	loongarch32r-linux-gnusf-objcopy -O binary -j .data ${elf_path} ${data_path}

${elf_path}: bin.lds ${obj_path}/libinst.a ${obj_path}/start.o
	loongarch32r-linux-gnusf-ld ${obj_path}/start.o ${obj_path}/libinst.a -T bin.lds -o ${elf_path}

${obj_path}/start.s: start.S
	loongarch32r-linux-gnusf-gcc \
		-Iinclude -nostdinc -nostdlib -D_KERNEL -fno-builtin -D__loongarch32\
		-DMEMSTART=0x10000000 -DMEMSIZE=0x04000 -DCPU_COUNT_PER_US=1000     \
		-S start.S > ${obj_path}/start.s 

clean:
	rm -rf build
