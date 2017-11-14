CURR_DIR := $(shell pwd)
BUILD_DIR:=$(subst /user_space, /user_space,$(CURR_DIR))
BUILD_DIR:=$(word 1, $(BUILD_DIR))

include $(BUILD_DIR)/Rules.mak

CFLAGS  = -s -Wall -MD
#CFLAGS  = -Os -s -Wall -Werror -MD
CFLAGS += -I$(SHARED_DIR) -I../include
#CFLAGS += -I$(ROOT)/bsp/src/include
CFLAGS += -I$(KERNEL_DRIVERS_DIR)/
CFLAGS += -I$(APPS_DIR)/public/nvram
#CFLAGS += -D_SC_DEBUG_ENABLE_

LDFLAGS += -L$(APPS_DIR)/public/nvram -lscnvram

ifeq ($(CHIP_VENDOR),BCM)
CFLAGS += -DBCM_BSP
else
ifeq ($(CHIP_VENDOR),RTL)
CFLAGS += -DRTL_BSP
else 
ifeq ($(CHIP_VENDOR),MTK)
CFLAGS += -DMTK_BSP
endif
endif
endif

ifeq ($(EXTERNAL_SWITCH),1)
CFLAGS += -DEXTERNAL_SWITCH
endif

ifeq ($(ADSL),1)
CFLAGS += -DADSL
OBJS += lib_adslinfo.o
endif

ifeq ($(DUAL_WAN),1)
CFLAGS += -DDUAL_WAN
endif

ifeq ($(MTCODE),1)
CFLAGS += -DMT_CODE
endif

DESTLIB = $(ROOT)/target/lib
#OBJS = link.o lib_adslinfo.o basic.o
OBJS += link.o
OBJS += basic.o

TARGET = liblink.so

all: $(TARGET)

liblink.so : $(OBJS)
	$(CC) -Os -s $(LDFLAGS) -shared -Wl,-soname,liblink.so -o $@ $(LIBS) $^
test: main.o 
	$(CC) -Os -s  -o $@ $(LIBS) $^

main.o: main.c lib_adslinfo.c

%.o : %.c
	echo $(CFLAGS)
	$(CC) $(CFLAGS) $(LDFLAGS) -fPIC -c $< -o $@

install :
	cp -f $(TARGET) $(DESTLIB)

clean:
	rm -rf *~ *.[oad] *.so
	-rm -rf $(DESTLIB)/$(TARGET)
# abc.d <=> abc.o: abc.c abcd.c. run by obscure rule.
-include $(OBJS:.o=.d)

