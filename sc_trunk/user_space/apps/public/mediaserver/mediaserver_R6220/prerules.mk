#---------------------------------------------------
#                COMPILE OPTIONS
#---------------------------------------------------
#CROSS=$(_CROSS)
#ECHO_CMD         =  @echo $@;
#CROSS_COMPILE   = $(_CROSS)
CC              =  $(CROSS_COMPILE)gcc
BCC              =  $(CROSS_COMPILE)gcc 
CXX		=  $(ECHO_CMD)$(CROSS_COMPILE)g++
CPP             =  $(CROSS_COMPILE)cpp
LD              =  $(CROSS_COMPILE)ld
AS              =  $(CROSS_COMPILE)as
AR              =  $(CROSS_COMPILE)ar
STRIP           =  $(CROSS_COMPILE)strip
RANLIB          =  $(CROSS_COMPILE)ranlib
OBJCOPY         =  $(CROSS_COMPILE)objcopy
OBJDUMP         =  $(CROSS_COMPILE)objdump
export CC CPP LD AS AR STRIP RANLIB OBJCOPY OBJDUMP
export RAMDISK_DIR=$(ROOT)/target
export T_LIB=$(RAMDISK_DIR)/lib
export T_USBIN=$(RAMDISK_DIR)/etc
# We should warning all messages
#CFLAGS          :=  -Wall 

# We use this flags to build depence files which extend filename is ".d"
#CFLAGS          +=  -MD

#CFLAGS          +=  -Os -fomit-frame-pointer

CFLAGS += -Os -Wall -s -MMD -DSC_MUTIL_GROUP

#---------------------------------------------------
#                COMMON PATH
#---------------------------------------------------
COMM_LIB_PATH   = $(TOPDIR)/library/lib
COMM_INC_PATH   = $(TOPDIR)/library/include

#---------------------------------------------------
#                TARGET PATH
#---------------------------------------------------
