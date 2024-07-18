include $(AM_HOME)/am/arch/isa/la32r.mk

AM_SRCS := eula/isa/la32r/trm.c \
           eula/common/mainargs.S \
           eula/common/uartlite.c \
           driver/confreg/timer.c \
           eula/isa/la32r/boot/start.S

CFLAGS  += -I$(AM_HOME)/am/src/eula/include -DISA_H=\"mmio.h\" -DBOARD_H=\"eula.h\"

ASFLAGS += -DMAINARGS=\"$(mainargs)\"
.PHONY: $(AM_HOME)/am/src/eula/common/mainargs.S
# -L searchdir : Add path searchdir to the list of paths that ld will search for archive libraries and ld control scripts.
# -T scriptfile :  Use scriptfile as the linker script. 
LDFLAGS += -L $(AM_HOME)/am/src/eula/ldscript
LDFLAGS += -T $(AM_HOME)/am/src/eula/isa/la32r/boot/loader.ld

std_convert:
	$(MAKE) -C $(AM_HOME)/tools

image: std_convert
	@echo + LD "->" $(BINARY_REL).elf
	@$(LD) $(LDFLAGS) --gc-sections -o $(BINARY).elf --start-group $(LINK_FILES) --end-group
	@$(OBJDUMP) -d $(BINARY).elf > $(BINARY).txt
	@echo + OBJCOPY "->" $(BINARY_REL).bin
	@$(OBJCOPY) -S --set-section-flags .bss=alloc,contents -O binary $(BINARY).elf $(BINARY).bin
	@echo + std_convert "->" $(BINARY_REL).vlog
	@$(AM_HOME)/tools/std_convert 0x1c000000 $(BINARY).bin $(BINARY).vlog

# ld:
# --gc-sections : aim to decrease bin size, see https://www.cnblogs.com/zqb-all/p/6128673.html
# --start-group & --end-group :  resolving circular dependences between several libraries, https://stackoverflow.com/questions/5651869

# objcopy:
#  -S --strip-all : Do not copy relocation and symbol information from the source file.
#  -O bfdname --output-target=bfdname : Write the output file using the object format bfdname.
# --set-section-flags sectionpattern=flags : Set the flags for any sections matching sectionpattern
