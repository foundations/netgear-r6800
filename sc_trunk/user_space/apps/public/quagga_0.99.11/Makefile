
CURR_DIR := $(shell pwd)
BUILD_DIR:=$(subst /user_space, /user_space,$(CURR_DIR))
BUILD_DIR:=$(word 1, $(BUILD_DIR))

include $(BUILD_DIR)/Rules.mak
CC=$(CROSS)gcc
CPP=$(CROSS)gcc -E
STRIP=$(CROSS)strip
all: 
	$(MAKE) -e -C lib
	$(MAKE) -e -C zebra
	$(MAKE) -e -C ripd
clean:
	$(MAKE) -C lib clean
	$(MAKE) -C ripd clean
	$(MAKE) -C zebra clean

install:
	cp -f ripd/.libs/ripd $(ROOT)/target/usr/sbin/
	cp -f zebra/.libs/zebra $(ROOT)/target/usr/sbin/
	cp -f lib/.libs/libzebra.so.0 $(ROOT)/target/lib/

