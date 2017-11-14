
CURR_DIR := $(shell pwd)
BUILD_DIR:=$(subst /user_space, /user_space,$(CURR_DIR))
BUILD_DIR:=$(word 1, $(BUILD_DIR))

include $(BUILD_DIR)/Rules.mak


#include ../../../Rules.mak

DESTLIB = $(ROOT)/target/lib
DESTBIN = $(ROOT)/target/usr/sbin

SSL_DIR := $(LIBS_DIR)/public/$(OPENSSL_USE)/
#SSL_DIR := $(APPS_DIR)/openssl-1.0.1g/
STAGING_DIR :=$(LIBS_DIR)/public/tr_lib/
#STAGING_DIR :=$(LIBS_DIR)/public/tr_lib/curl/staging/
CURL_DIR := curl-7.36.0

all: install_ssl config
	cd $(CURL_DIR) && make # && make install

install_ssl:
	[ -f $(STAGING_DIR)/lib/libcrypto.so.0.9.8 ] || (rm -f $(SSL_DIR)/*.pc; make -C $(SSL_DIR) INSTALLTOP=$(STAGING_DIR) OPENSSLDIR=$(STAGING_DIR) install_sw)

config:
	@(if test -f h_config; then \
		echo "has configuration" ;   \
        else \
		make configure; \
		rm -rf h_config; \
		touch h_config; \
	fi)

#configure:
#	cd $(CURL_DIR) && ./configure --host=mipsel-linux CC=$(CROSS)gcc --with-ssl=$(STAGING_DIR) --with-zlib=$(STAGING_DIR)/lib/  --libdir=$(STAGING_DIR)/lib/ --includedir=$(LIBS_DIR)/public/tr_lib/include --bindir=$(CURR_DIR)/aaa/ --mandir=$(CURR_DIR)/aaa/
configure:
	cd $(CURL_DIR); ./configure --host=$(LINUX_PLATFORM) CC=$(CROSS)gcc PKG_CONFIG_PATH=$(STAGING_DIR) \
			--without-cyassl --with-ssl=$(STAGING_DIR) --libdir=$(STAGING_DIR)/lib/ --includedir=$(STAGING_DIR)/include \
			-enable-shared=yes -enable-static=yes --prefix=$(CURR_DIR)/aaa/ 

install:
	$(STRIP) $(CURL_DIR)/lib/.libs/libcurl.so.*
	$(STRIP) $(CURL_DIR)/src/.libs/curl
	cp -rf $(CURL_DIR)/lib/.libs/libcurl.so* $(DESTLIB)
	cp -rf $(CURL_DIR)/src/.libs/curl $(DESTBIN)

clean:
	rm -f h_config
	cd $(CURL_DIR) && make clean
