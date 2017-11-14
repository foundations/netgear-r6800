CURR_DIR := $(shell pwd)
BUILD_DIR:=$(subst /user_space, /user_space,$(CURR_DIR))
BUILD_DIR:=$(word 1, $(BUILD_DIR))

include $(BUILD_DIR)/Rules.mak

CFLAGS  = -Os -s -Wall -Werror -MD
CFLAGS += -I$(SHARED_DIR) -I../include  -I$(APPS_DIR)/public/nvram
ifeq ($(BOOT_NAND_FLASH),1)
CFLAGS += -DBOOT_NAND_FLASH
endif

ifeq ($(MTK_NAND_FLASH_API),1)
CFLAGS += -DMTK_NAND_FLASH_API
endif

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

ifeq ($(MT_CODE), 1)
CFLAGS += -DMT_CODE
endif

ifeq ($(CUSTOMER_NAME),Amped)
CFLAGS += -DAMPED
endif

LIBS = -L$(APPS_DIR)/public/nvram -lscnvram
OBJS = flash.o

DESTLIB = $(ROOT)/target/lib

TARGET = libflash.so

all: $(TARGET)

libflash.so : $(OBJS)
	$(CC) -Os -s -shared -Wl,-soname,libflash.so -o $@ $(LIBS) $^

%.o : %.c
	$(CC) $(CFLAGS) -fPIC -c $< -o $@

install :
	cp -f $(TARGET) $(DESTLIB)

clean:
	rm -rf *~ *.[oad] *.so
	-rm -rf $(DESTLIB)/$(TARGET)

# abc.d <=> abc.o: abc.c abcd.c. run by obscure rule.
-include $(OBJS:.o=.d)

