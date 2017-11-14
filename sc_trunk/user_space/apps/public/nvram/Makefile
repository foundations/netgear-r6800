CURR_DIR := $(shell pwd)
BUILD_DIR:=$(subst /user_space, /user_space,$(CURR_DIR))
BUILD_DIR:=$(word 1, $(BUILD_DIR))

include $(BUILD_DIR)/Rules.mak

#include ../../Rules.mak
CC=$(CROSS)gcc

CFLAGS  = -Os -s -Wall
CFLAGS += -Werror -I$(ROOT)/shared/
#CFLAGS += -DTEST
ifeq ($(MTK_NAND_FLASH_API),1)
CFLAGS += -DMTK_NAND_FLASH_API
LDFLAGS += 
endif

DESTLIB = $(TARGET_DIR)/lib
DESTBIN = $(BINDIR)

BINS = libscnvram.so libnvram.so nvram_dynamic libcms_msg.so

#all: libnvram.so nvram_static
all: $(BINS)

libcms_msg.so: sc_cms_msg.o
	$(CC) -Os -s -shared -Wl,-soname,libcms_msg.so -o libcms_msg.so sc_cms_msg.o

libscnvram.so : nvram.o nvram_lock.o
	$(CC) $(CFLAGS) $(LDFLAGS) -Os -s -shared -Wl,-soname,libscnvram.so -o libscnvram.so $^ 

libnvram.so : nvram_bcm.o nvram_lock.o
	$(CC) $(CFLAGS) -Os -s -shared -Wl,-soname,libnvram.so -o libnvram.so $^ 

nvram.o : nvram.c
	$(CC) $(CFLAGS) -Os -s -Wall -Werror -fPIC -g -c $^ -o $@
	
readycloud_nvram.o : nvram.c
	$(CC) -Os -s -Wall -Werror -fPIC -g -c $^ -o $@
	
nvram_bcm.o : nvram_bcm.c
	$(CC) $(CFLAGS) -Os -s -Wall -Werror -fPIC -g -c $^ -o $@

nvram_lock.o: nvram_lock.c
	$(CC) $(CFLAGS) -Os -s -g -Wall -fPIC -c $^ -o $@

sc_cms_msg.o : sc_cms_msg.c
	$(CC) $(CFLAGS) -Os -s -Wall -Werror -fPIC -g -c $^ -o $@

install :
	cp -f libnvram.so $(DESTLIB)	
	cp -f libscnvram.so $(DESTLIB)
	cp -f nvram $(DESTBIN)
	cp -f readycloud_nvram $(DESTBIN)
	cp -f libcms_msg.so $(DESTLIB)
	ln -sf libcms_msg.so $(DESTLIB)/libcms_util.so
	ln -sf libcms_msg.so $(DESTLIB)/libcms_boardctl.so

#
# nvram utility with static link
#
nvram_static: nvram_bin.c nvram.c
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ nvram_bin.c nvram.c
	cp $@ nvram
	cp $@ readycloud_nvram

#
#nvram utility with dynamic link
#
nvram_dynamic: nvram_bin.c
	$(CC) $(CFLAGS) -o $@ nvram_bin.c -L./ -lscnvram $(LDFLAGS) 
	cp $@ nvram
	cp $@ readycloud_nvram


clean:
	rm -rf *~ *.o *.so nvram readycloud_nvram $(BINS)

