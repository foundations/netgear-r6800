#include ../../../Rules.mak


CURR_DIR := $(shell pwd)
BUILD_DIR:=$(subst /user_space, /user_space,$(CURR_DIR))
BUILD_DIR:=$(word 1, $(BUILD_DIR))

include $(BUILD_DIR)/Rules.mak

DESTLIB = $(ROOT)/target/lib

all: config
	cd zlib-1.2.3 && make
	$(STRIP) zlib-1.2.3/libz.so.1.2.3
	cp -a zlib-1.2.3/libz.so* $(LIBS_DIR)/public/tr_lib/lib
	cd zlib-1.2.3;cp -af zlib.h zconf.h $(LIBS_DIR)/public/tr_lib/include
		
config:
	@(if test -f h_config; then \
		echo "has configuration" ;   \
        else \
		make configure; \
		rm -rf h_config; \
		touch h_config; \
	fi)

configure:
ifeq ($(NEW_TRANSMISSION),1)
	cd zlib-1.2.3 && host=mipsel-linux CC=$(CROSS)gcc ./configure -s --prefix=$(LIBS_DIR)/public/tr_lib --libdir=$(LIBS_DIR)/public/tr_lib/lib --includedir=$(LIBS_DIR)/tr_lib/include
else
	cd zlib-1.2.3 && ./configure -s --host=mips-linux CC=$(CROSS)gcc --libdir=$(LIBS_DIR)/tr_lib/lib --includedir=$(LIBS_DIR)/tr_lib/include
endif

install:
	$(STRIP) zlib-1.2.3/libz.so.1.2.3
	cp -a zlib-1.2.3/libz.so* $(DESTLIB)

clean:
	cd zlib-1.2.3 && make clean
	
