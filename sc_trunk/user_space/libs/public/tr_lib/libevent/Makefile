
CURR_DIR := $(shell pwd)
BUILD_DIR:=$(subst /user_space, /user_space,$(CURR_DIR))
BUILD_DIR:=$(word 1, $(BUILD_DIR))

include $(BUILD_DIR)/Rules.mak


#include ../../../Rules.mak

DESTLIB = $(ROOT)/target/lib

CFLAGS = -s -Os -fPIC
CFLAGS +=-I$(LIBS_DIR)/public/openssl-0.9.7e/include
CFLAGS +=-I$(LIBS_DIR)/public/tr_lib/include

LDFLAGS = -L$(LIBS_DIR)/public/openssl-0.9.7e
LDFLAGS += -L$(LIBS_DIR)/public/tr_lib/lib

all: config
ifeq ($(NEW_TRANSMISSION),1)
	cd libevent-2.0.21-stable && make && make install
else
	cd libevent-2.1.1-alpha && make && make install
endif

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
	cd libevent-2.0.21-stable && ./configure --host=mipsel-linux CC=$(CROSS)gcc --libdir=$(LIBS_DIR)/public/tr_lib/lib --includedir=$(LIBS_DIR)/public/tr_lib/include CFLAGS="$(CFLAGS)" LDFLAGS="$(LDFLAGS)"
else
	cd libevent-2.1.1-alpha && ./configure --disable-openssl --host=mips-linux CC=$(CROSS)gcc  --libdir=$(LIBS_DIR)/public/tr_lib/lib --includedir=$(LIBS_DIR)/public/tr_lib/include
endif

install:
ifeq ($(NEW_TRANSMISSION),1)
	$(STRIP) libevent-2.0.21-stable/.libs/libevent-2.0.so.5.1.9
	$(STRIP) libevent-2.0.21-stable/.libs/libevent_core-2.0.so.5.1.9 
	$(STRIP) libevent-2.0.21-stable/.libs/libevent_extra-2.0.so.5.1.9
	$(STRIP) libevent-2.0.21-stable/.libs/libevent_openssl-2.0.so.5.1.9 
	$(STRIP) libevent-2.0.21-stable/.libs/libevent_pthreads-2.0.so.5.1.9
	cp -a libevent-2.0.21-stable/.libs/*.so* $(DESTLIB)
else
	$(STRIP) libevent-2.1.1-alpha/.libs/libevent-2.1.so.1.0.0 libevent-2.1.1-alpha/.libs/libevent_core-2.1.so.1.0.0 libevent-2.1.1-alpha/.libs/libevent_extra-2.1.so.1.0.0 libevent-2.1.1-alpha/.libs/libevent_pthreads-2.1.so.1.0.0
	cp -a libevent-2.1.1-alpha/.libs/libevent-2.1.so.1* $(DESTLIB)
	cp -a libevent-2.1.1-alpha/.libs/libevent_core-2.1.so.1* $(DESTLIB)
	cp -a libevent-2.1.1-alpha/.libs/libevent_extra-2.1.so.1* $(DESTLIB)
	cp -a libevent-2.1.1-alpha/.libs/libevent_pthreads-2.1.so.1* $(DESTLIB)
endif

clean:
ifeq ($(NEW_TRANSMISSION),1)
	cd libevent-2.0.21-stable && make clean
else
	cd libevent-2.1.1-alpha && make clean
endif
