CROSS_COMPILE := $(LA32RTC_HOME)/loongarch32r-linux-gnusf-2022-05-20/bin/loongarch32r-linux-gnusf-
COMMON_FLAGS  := -fno-pic 
COMMON_FLAGS  += -I$(LA32RTC_HOME)/loongarch32r-linux-gnusf-2022-05-20/lib/gcc/loongarch32r-linux-gnusf/8.3.0/include
COMMON_FLAGS  += -I$(LA32RTC_HOME)/system_newlib/loongarch32-unknown-elf/include
COMMON_FLAGS  += -I$(LA32RTC_HOME)/loongarch32r-linux-gnusf-2022-05-20/lib/gcc/loongarch32r-linux-gnusf/8.3.0/include-fixed
CFLAGS        += $(COMMON_FLAGS) -nostdlib -nostdinc -static -fno-builtin
ASFLAGS       += $(COMMON_FLAGS) -O0
LDFLAGS       += 

# all of the commend start with -m means Machine-Dependent Options