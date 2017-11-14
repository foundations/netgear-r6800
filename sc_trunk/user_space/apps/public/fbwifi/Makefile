CURR_DIR := $(shell pwd)
BUILD_DIR1:=$(subst /user_space, /user_space,$(CURR_DIR))
BUILD_DIR1:=$(word 1, $(BUILD_DIR1))

include $(BUILD_DIR1)/Rules.mak


STRIP = $(CROSS)strip --strip-unneeded --remove-section=.comment
INSTALL_DIR:=install -d -m0755

#DIRS := db-4.7.25.NC libgpg-error-1.10 libgcrypt-1.5.0  expat-2.0.1 gdbm-1.8.3 libdaemon-0.14 avahi-0.6.25
DIRS := libmicrohttpd-0.9.35
DIRS_SIMPLE := 

STAGING_DIR :=$(shell pwd)/target
DIR_BUILD := $(foreach DIR, $(DIRS), $(DIR)/.build)
BUILD_DIR := $(foreach DIR, $(DIRS), $(DIR)/build_sc)
#SSL_DIR := $(shell pwd)/../openssl-0.9.8y

TARGET = $(TARGET_DIR)
TARGET_LIBDIR = $(TARGET)/lib

#all:install_ssl $(DIR_BUILD) $(DIRS_SIMPLE) 
all:$(DIR_BUILD) $(DIRS_SIMPLE) 
#all:$(DIR_BUILD)
# NEED SSL, to build afppasswd
install_ssl:
	make -C $(SSL_DIR) INSTALLTOP=$(STAGING_DIR) OPENSSLDIR=$(STAGING_DIR) install_sw

$(DIRS_SIMPLE)::
	@for i in $(DIRS_SIMPLE) ; do echo -e "\\e[35mapps: make \\e[36m$$i \\e[0m"; $(MAKE) ARCH=$(ARCH) CROSS_COMPILE=$(CROSS_DIR)$(CROSS_COMPILE) -C $$i || exit 1; done


define BUILD_PACKAGE


$(1)/build_sc/config.status:
	if [ ! -e $(1)/build_sc ]; then mkdir $(1)/build_sc;fi
	cd $(1);sh build.sh

$(1)/.build: $(1)/build_sc/config.status
	make -C $(1)/build_sc
	make -C $(1)/build_sc DESTDIR=$(STAGING_DIR) install
	if [ -e $(1)/fixup.sh ]; then cd $(1);sh fixup.sh $(STAGING_DIR); fi
	touch $(1)/.build
endef

$(foreach DIR, $(DIRS), $(eval $(call BUILD_PACKAGE, $(DIR))))


clean:
	@for i in $(DIRS_SIMPLE) ; do echo -e "\\e[35mapps: clean \\e[36m$$i \\e[0m"; $(MAKE) -C $$i clean  || exit 1; done
	rm -rf $(STAGING_DIR) $(BUILD_DIR) $(DIR_BUILD)
#make -C $(SSL_DIR) INSTALLTOP=$(STAGING_DIR) OPENSSLDIR=$(STAGING_DIR) clean

.PHONY: clean install

install:
	cp -af target/usr/lib/libmicrohttpd.so* $(TARGET)/lib/


