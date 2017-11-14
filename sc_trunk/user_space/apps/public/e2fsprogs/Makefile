#include ../../Rules.mak

CURR_DIR := $(shell pwd)
BUILD_DIR:=$(subst /user_space, /user_space,$(CURR_DIR))
BUILD_DIR:=$(word 1, $(BUILD_DIR))

include $(BUILD_DIR)/Rules.mak


DESTLIB = $(ROOT)/target/lib
DESTBIN = $(ROOT)/target/usr/sbin
SOURCELIB = $(ROOT)/rtl819x/toolchain/rsdk-1.3.6-5281-EB-2.6.30-0.9.30/lib
all: config
	cd e2fsprogs-1.41.3 && make	
config:
	@(if test -f h_config; then \
		echo "has configuration" ;   \
        else \
		make configure; \
		rm -rf h_config; \
		touch h_config; \
	fi)
configure:
	cd e2fsprogs-1.41.3 && ./configure --disable-resizer --disable-imager --disable-testio-debug --with-libiconv-prefix=$(LIBS_DIR)/public/libiconv.1.8 --host=mips-linux CC=$(CROSS)gcc
install:
	#ln -f ./e2fsprogs-1.41.3/misc/tune2fs ./e2fsprogs-1.41.3/misc/e2label
	#$(STRIP) ./e2fsprogs-1.41.3/misc/e2label
	#cp -a ./e2fsprogs-1.41.3/misc/e2label $(DESTBIN)/e2label
	$(STRIP) ./e2fsprogs-1.41.3/misc/blkid
	cp -a ./e2fsprogs-1.41.3/misc/blkid $(DESTBIN)/blkid
clean:
	cd e2fsprogs-1.41.3 && make clean
	rm -rf ./misc/e2label 
