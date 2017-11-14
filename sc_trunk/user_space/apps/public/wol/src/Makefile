CURR_DIR := $(shell pwd)
BUILD_DIR:=$(subst /user_space, /user_space,$(CURR_DIR))
BUILD_DIR:=$(word 1, $(BUILD_DIR))

include $(BUILD_DIR)/Rules.mak


CFLAGS +=-I$(APPS_DIR)/public/nvram
LDFLAGS +="-L$(APPS_DIR)/public/nvram -lscnvram" 
all: config
	cd wol-0.5.1 && make	
config:
	@(if test -f h_config; then \
		echo "has configuration" ;   \
        else \
		make configure; \
		rm -rf h_config; \
		touch h_config; \
	fi)
configure:
	cd wol-0.5.1 && ./configure --disable-nls --prefix=$(APPS_DIR)/public/wol/src/wol-0.5.1 --host=mips-linux CC=$(CROSS)gcc CFLAGS=$(CFLAGS) LDFLAGS=$(LDFLAGS)
install:
	cp  ./wol-0.5.1/src/wol $(TARGET_DIR)/usr/sbin/
clean:
	-rm ./h_config
	cd  wol-0.5.1 && make clean
