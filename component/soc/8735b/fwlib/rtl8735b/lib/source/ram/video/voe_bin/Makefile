
.PHONY: all clean mksubdir build_lib post_process obj_copy

MKDIR= mkdir

ROOT_DIR= $(PWD)/..

ifeq ($(shell uname), Linux)
ASDK_DIR=/toolchain/asdk-8.4.0/linux/newlib
else
ASDK_DIR=/asdk-8.4.0/mingw32/newlib
endif

OBJ_DIR	= $(PWD)/cmsis/voe/source/obj
OUTPUT_DIR = $(PWD)/cmsis/voe/source/output
export ASDK_DIR ROOT_DIR OBJ_DIR

CC	= $(ASDK_DIR)/bin/arm-none-eabi-gcc  
AR		= $(ASDK_DIR)/bin/arm-none-eabi-ar
AS		= $(ASDK_DIR)/bin/arm-none-eabi-gcc
LD		= $(ASDK_DIR)/bin/arm-none-eabi-gcc 
NM		= $(ASDK_DIR)/bin/arm-none-eabi-nm
STRIP	= $(ASDK_DIR)/bin/arm-none-eabi-strip
OBJDUMP	= $(ASDK_DIR)/bin/arm-none-eabi-objdump
OBJCOPY	= $(ASDK_DIR)/bin/arm-none-eabi-objcopy
CC_SIZE	= $(ASDK_DIR)/bin/arm-none-eabi-size

export CC AR AS LD NM STRIP OBJDUMP OBJCOPY CC_SIZE


OUTLIB = 
all:
	$(OBJCOPY) -I binary -O elf32-littlearm -B arm voe.bin $(OBJCOPYDIR)/voe.o;
	$(OBJCOPY) -I binary -O elf32-littlearm -B arm voe_fcs.bin $(OBJCOPYDIR)/voe_fcs.o;
	$(OBJCOPY) -I binary -O elf32-littlearm -B arm fcs_data.bin $(OBJCOPYDIR)/fcs_data.o;
	#$(OBJCOPY) -I binary -O elf32-littlearm -B arm sensor.bin $(OBJCOPYDIR)/sensor.o;
	$(OBJCOPY) -I binary -O elf32-littlearm -B arm sensor_gc2053.bin $(OBJCOPYDIR)/sensor_gc2053.o;
	$(OBJCOPY) -I binary -O elf32-littlearm -B arm sensor_fixp.bin $(OBJCOPYDIR)/sensor_fixp.o;
	$(OBJCOPY) -I binary -O elf32-littlearm -B arm sensor_sc2310.bin $(OBJCOPYDIR)/sensor_sc2310.o;
	$(OBJCOPY) -I binary -O elf32-littlearm -B arm iq.bin $(OBJCOPYDIR)/iq.o;

	

