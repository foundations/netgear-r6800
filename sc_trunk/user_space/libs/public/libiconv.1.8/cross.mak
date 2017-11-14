

CURR_DIR := $(shell pwd)
BUILD_DIR:=$(subst /user_space, /user_space,$(CURR_DIR))
BUILD_DIR:=$(word 1, $(BUILD_DIR))

include $(BUILD_DIR)/Rules.mak


#include ../../Rules.mak

CROSS_COMPILE  = $(CROSS)
AS		= $(CROSS_COMPILE)as
LD		= $(CROSS_COMPILE)ld
CC		= $(CROSS_COMPILE)gcc
# CONFIG_MIPS_BRCM Begin Broadcom changed code.
CPP		= $(CROSS_COMPILE)g++
# CPP           = $(CC) -E
# CONFIG_MIPS_BRCM End Broadcom changed code.
AR		= $(CROSS_COMPILE)ar
RANLIB		= $(CROSS_COMPILE)ranlib
NM		= $(CROSS_COMPILE)nm
STRIP		= $(CROSS_COMPILE)strip
OBJCOPY		= $(CROSS_COMPILE)objcopy
OBJDUMP		= $(CROSS_COMPILE)objdump 
#TARGET_PREFIX	= ../../../tools/mipsel-linux-uclibc/bin

export AS LD CC CPP AR NM STRIP OBJCOPY OBJDUMP TARGET_PREFIX RANLIB
