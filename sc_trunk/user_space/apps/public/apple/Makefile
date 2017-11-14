#include ../../Rules.mak

CURR_DIR := $(shell pwd)
BUILD_DIR1:=$(subst /user_space, /user_space,$(CURR_DIR))
BUILD_DIR2:=$(word 1, $(BUILD_DIR1))

include $(BUILD_DIR2)/Rules.mak


STRIP = $(CROSS)strip --strip-unneeded --remove-section=.comment
INSTALL_DIR:=install -d -m0755

DIRS=
DIRS_BASIC=
FFMPEG_DIR=../mediaserver/$(MEDIASERVER_USE)/library/ffmpeg-0.11.2/
AVAHI_DIR=avahi-0.6.31
FORKED_DAAPD_DIR=forked-daapd-23.1
NETATALK_DIR=netatalk-2.2.5
ifeq ($(ITUNES_SERVER), 1)
DIRS += sqlite-autoconf-3130000 alsa-lib-1.0.26 libxml2-2.9.1 libplist-1.11 libantlr3c-3.2 confuse-2.6 libevent-2.0.22-stable libunistring-0.9.3 db-4.7.25.NC libgpg-error-1.10 libgcrypt-1.5.0 $(NETATALK_DIR) expat-2.0.1 dbus-1.8.0 gdbm-1.8.3 libdaemon-0.14 $(AVAHI_DIR) $(FORKED_DAAPD_DIR)
DIRS_BASIC += avl-0.3.5 mxml-2.8 $(FFMPEG_DIR)
else
DIRS += sqlite-autoconf-3130000 db-4.7.25.NC libgpg-error-1.10 libgcrypt-1.5.0 $(NETATALK_DIR) expat-2.0.1 gdbm-1.8.3 libdaemon-0.14 $(AVAHI_DIR)
endif

STAGING_DIR :=$(shell pwd)/target
DIR_BUILD := $(foreach DIR, $(DIRS), $(DIR)/.build)
BUILD_DIR := $(foreach DIR, $(DIRS), $(DIR)/build_sc)
SSL_DIR := $(LIBS_DIR)/public/$(OPENSSL_USE)

TARGET = $(TARGET_DIR)
TARGET_LIBDIR = $(TARGET)/lib

all:install_ssl preconfig_avahi basic_lib $(DIR_BUILD)
	@echo "!!!!!!!ITUNES_SERVER=$(ITUNES_SERVER) TIME_MACHINE=$(TIME_MACHINE) DIR_BUILD=$(DIR_BUILD)"
#all:$(DIR_BUILD)
# NEED SSL, to build afppasswd
preconfig_avahi:
	rm -f $(AVAHI_DIR)/build.sh
ifeq ($(ITUNES_SERVER), 1)
	ln -s build_enable_dbus.sh $(AVAHI_DIR)/build.sh 
else
	ln -s build_disable_dbus.sh $(AVAHI_DIR)/build.sh 
endif


install_ssl:
	make -C $(SSL_DIR) INSTALLTOP=$(STAGING_DIR) OPENSSLDIR=$(STAGING_DIR) install_sw

basic_lib:
	@for i in $(DIRS_BASIC) ; do echo -e "\\e[35mapps: make \\e[36m$$i \\e[0m"; $(MAKE) ARCH=$(ARCH) CROSS_COMPILE=$(CROSS_DIR)$(CROSS_COMPILE) -C $$i || exit 1; done


define BUILD_PACKAGE


$(1)/build_sc/config.status:
	@echo "-----------sc-----"$1"--------"$(CC)"---------------"
	if [ ! -e $(1)/build_sc ]; then mkdir $(1)/build_sc;fi
	if [ $(1) == $(FORKED_DAAPD_DIR) ]; then \
		cd $(1);sh build.sh $(CC) $(MEDIASERVER_USE);\
	else\
		cd $(1);sh build.sh $(CC) $(CROSS);\
	fi

$(1)/.build: $(1)/build_sc/config.status
	@echo "-----------build-----"$1"-----------------------"
	make -C $(1)/build_sc
	make -C $(1)/build_sc DESTDIR=$(STAGING_DIR) install
	if [ -e $(1)/fixup.sh ]; then cd $(1);sh fixup.sh $(STAGING_DIR); fi
	touch $(1)/.build
endef

$(foreach DIR, $(DIRS), $(eval $(call BUILD_PACKAGE, $(DIR))))


clean:
	@for i in $(DIRS_BASIC) ; do echo -e "\\e[35mapps: clean \\e[36m$$i \\e[0m"; $(MAKE) -C $$i clean  || exit 1; done
	rm -rf $(STAGING_DIR) $(BUILD_DIR) $(DIR_BUILD)
	#echo "dir=$(DIR_BUILD) $(BUILD_DIR) $(STAGING_DIR) itunes-server=$(ITUNES_SERVER)"
	-rm $(TARGET)/usr/sbin/afpd
	-rm $(TARGET)/usr/sbin/afppasswd 
	-rm $(TARGET)/usr/sbin/avahi-autoipd
	-rm $(TARGET_LIBDIR)/libdb*.so
	-rm $(TARGET_LIBDIR)/libgpg-error.so.*
	-rm $(TARGET_LIBDIR)/libgcrypt.so.*
	-rm $(TARGET_LIBDIR)/libexpat.so*
	-rm $(TARGET_LIBDIR)/libgdbm.so.*
	-rm $(TARGET_LIBDIR)/libdaemon.so.*
	-rm $(TARGET_LIBDIR)/libavahi-{common,core}.so.*
	-rm $(TARGET)/usr/sbin/avahi-daemon 
	-rm $(TARGET)/usr/sbin/avahi-dnsconfd
	-rm $(TARGET_LIBDIR)/uams_*.so
	-rm $(TARGET)/bin/cnid_dbd
	-rm $(TARGET)/bin/cnid_metad
	-rm $(TARGET_LIBDIR)/libantlr3c.so	
	-rm $(TARGET_LIBDIR)/libasound.so*	
	-rm $(TARGET_LIBDIR)/libconfuse.so*	
	-rm $(TARGET_LIBDIR)/libdbus-1.so*	
	-rm $(TARGET_LIBDIR)/libevent*.so*	
	-rm $(TARGET_LIBDIR)/libplist.so*	
	-rm $(TARGET_LIBDIR)/libunistring.so*
	-rm $(TARGET_LIBDIR)/libxml2.so*	
	-rm $(TARGET_LIBDIR)/libsqlite3.so* 
	-rm $(TARGET_LIBDIR)/libavahi-client.so.* 
	-rm $(TARGET_LIBDIR)/forked-daapd-sqlext.so 
	-rm $(TARGET_LIBDIR)/libswscale.so*
	-rm $(TARGET_LIBDIR)/libavutil.so.* 
	-rm $(TARGET_LIBDIR)/libavformat.so.*
	-rm $(TARGET_LIBDIR)/libavcodec.so.*
	-rm $(TARGET)/usr/sbin/forked-daapd 
	-rm $(TARGET)/bin/dbus-daemon 

.PHONY: clean install

install:
	$(INSTALL_DIR) $(TARGET)/usr/sbin/
	$(INSTALL_DIR) $(TARGET)/usr/etc/netatalk/
	$(INSTALL_DIR) $(TARGET_LIBDIR)
	$(INSTALL_DIR) $(TARGET)/usr/etc/avahi/
	$(INSTALL_DIR) $(TARGET)/usr/etc/avahi/services

	$(STRIP) $(STAGING_DIR)/usr/lib/libdb*.so
	$(STRIP) $(STAGING_DIR)/usr/lib/libgpg-error.so.*
	$(STRIP) $(STAGING_DIR)/usr/lib/libgcrypt.so.*
	$(STRIP) $(STAGING_DIR)/usr/sbin/mipsel-linux-uclibc-afpd
	$(STRIP) $(STAGING_DIR)/usr/sbin/mipsel-linux-uclibc-afpd \
		$(NETATALK_DIR)/build_sc/bin/afppasswd/afppasswd \
		$(STAGING_DIR)/usr/sbin/mipsel-linux-uclibc-cnid_dbd \
		$(STAGING_DIR)/usr/sbin/mipsel-linux-uclibc-cnid_metad \
		$(STAGING_DIR)/etc/netatalk/uams/*.so

	$(STRIP) $(STAGING_DIR)/usr/local/lib/libexpat.so*
	$(STRIP) $(STAGING_DIR)/usr/local/lib/libgdbm.so*
	$(STRIP) $(STAGING_DIR)/usr/local/lib/libdaemon.so*
	$(STRIP) $(STAGING_DIR)/usr/lib/libavahi-{common,core}.so*
	$(STRIP) $(STAGING_DIR)/usr/sbin/avahi-autoipd \
		$(STAGING_DIR)/usr/sbin/avahi-daemon \
		$(STAGING_DIR)/usr/sbin/avahi-dnsconfd

ifeq ($(ITUNES_SERVER),1)
	@for i in $(DIRS_BASIC) ; do echo -e "\\e[35mapps: install\\e[36m$$i \\e[0m";  $(MAKE) -C $$i install  || exit 1; done
	$(STRIP) $(STAGING_DIR)/lib/libantlr3c.so
	$(STRIP) $(STAGING_DIR)/lib/libasound.so*
	$(STRIP) $(STAGING_DIR)/lib/libconfuse.so*
	$(STRIP) $(STAGING_DIR)/lib/libdbus-1.so*
	$(STRIP) $(STAGING_DIR)/lib/libevent*.so*
	$(STRIP) $(STAGING_DIR)/lib/libplist.so*
	$(STRIP) $(STAGING_DIR)/lib/libunistring.so*
	$(STRIP) $(STAGING_DIR)/lib/libxml2.so*
	$(STRIP) $(STAGING_DIR)/lib/libsqlite3.so*
	$(STRIP) $(STAGING_DIR)/usr/lib/libavahi-client.so*
	$(STRIP) $(STAGING_DIR)/usr/lib/forked-daapd/forked-daapd-sqlext.so 
	$(STRIP) $(FFMPEG_DIR)/libswscale/libswscale.so* $(FFMPEG_DIR)/libavutil/libavutil.so* $(FFMPEG_DIR)/libavformat/libavformat.so* $(FFMPEG_DIR)/libavcodec/libavcodec.so.*
	$(STRIP) $(STAGING_DIR)/usr/sbin/forked-daapd
	$(STRIP) $(STAGING_DIR)/bin/dbus-daemon

	cp -a $(STAGING_DIR)/lib/libantlr3c.so	$(TARGET_LIBDIR)
	cp -a $(STAGING_DIR)/lib/libasound.so*	$(TARGET_LIBDIR)
	cp -a $(STAGING_DIR)/lib/libconfuse.so*	$(TARGET_LIBDIR)
	cp -a $(STAGING_DIR)/lib/libdbus-1.so*	$(TARGET_LIBDIR)
	cp -a $(STAGING_DIR)/lib/libevent*.so*	$(TARGET_LIBDIR)
	cp -a $(STAGING_DIR)/lib/libplist.so*	$(TARGET_LIBDIR)
	cp -a $(STAGING_DIR)/lib/libunistring.so*	$(TARGET_LIBDIR)
	cp -a $(STAGING_DIR)/lib/libxml2.so*	$(TARGET_LIBDIR)
	cp -a $(STAGING_DIR)/lib/libsqlite3.so* $(TARGET_LIBDIR)
	cp -a $(STAGING_DIR)/usr/lib/libavahi-client.so.* $(TARGET_LIBDIR)
	cp -a $(STAGING_DIR)/usr/lib/forked-daapd/forked-daapd-sqlext.so $(TARGET_LIBDIR)
	cp -a $(FFMPEG_DIR)/libswscale/libswscale.so* $(TARGET_LIBDIR)
	cp -a $(FFMPEG_DIR)/libavutil/libavutil.so.* $(TARGET_LIBDIR)
	cp -a $(FFMPEG_DIR)/libavformat/libavformat.so.* $(TARGET_LIBDIR)
	cp -a $(FFMPEG_DIR)/libavcodec/libavcodec.so.* $(TARGET_LIBDIR)
	cp -a $(STAGING_DIR)/usr/sbin/forked-daapd $(TARGET)/usr/sbin/
	cp -a $(STAGING_DIR)/bin/dbus-daemon $(TARGET)/usr/sbin/
endif

	cp -a $(STAGING_DIR)/usr/local/lib/libexpat.so* $(TARGET_LIBDIR)
	cp -a $(STAGING_DIR)/usr/local/lib/libgdbm.so.* $(TARGET_LIBDIR)
	cp -a $(STAGING_DIR)/usr/local/lib/libdaemon.so.* $(TARGET_LIBDIR)
	cp -a $(STAGING_DIR)/usr/lib/libdb*.so $(TARGET_LIBDIR)
	cp -a $(STAGING_DIR)/usr/lib/libgpg-error.so.* $(TARGET_LIBDIR)
	cp -a $(STAGING_DIR)/usr/lib/libgcrypt.so.* $(TARGET_LIBDIR)
	cp -a $(STAGING_DIR)/usr/lib/libavahi-{common,core}.so.* $(TARGET_LIBDIR)

	cp -a $(STAGING_DIR)/usr/sbin/mipsel-linux-uclibc-afpd $(TARGET)/usr/sbin/afpd
	cp -a $(NETATALK_DIR)/build_sc/bin/afppasswd/afppasswd  $(TARGET)/usr/sbin/afppasswd
	cp -a $(STAGING_DIR)/usr/sbin/mipsel-linux-uclibc-cnid_dbd $(TARGET)/usr/sbin/cnid_dbd
	cp -a $(STAGING_DIR)/usr/sbin/mipsel-linux-uclibc-cnid_metad $(TARGET)/usr/sbin/cnid_metad 
	cp -a $(STAGING_DIR)/etc/netatalk/uams/*.so $(TARGET)/lib
	#ln -s uams_dhx2_passwd.so $(TARGET)/lib/uams_dhx2.so
	cp $(NETATALK_DIR)/AppleVolumes.default $(TARGET)/usr/etc/netatalk/
	cp $(NETATALK_DIR)/AppleVolumes.system $(TARGET)/usr/etc/netatalk/
	cp $(NETATALK_DIR)/afpd.conf $(TARGET)/usr/etc/netatalk/
	cp $(NETATALK_DIR)/afppasswd $(TARGET)/usr/etc/netatalk/
	chmod 644 $(TARGET)/usr/etc/netatalk/AppleVolumes.default $(TARGET)/usr/etc/netatalk/AppleVolumes.system

	cp -a $(STAGING_DIR)/etc/avahi/avahi-autoipd.action $(TARGET)/usr/etc/avahi/
	cp -a $(STAGING_DIR)/usr/sbin/avahi-autoipd $(TARGET)/usr/sbin/
	cp -a $(STAGING_DIR)/usr/sbin/avahi-daemon $(TARGET)/usr/sbin/

	cp $(AVAHI_DIR)/avahi-daemon.conf $(TARGET)/usr/etc/avahi/
	cp $(AVAHI_DIR)/service-http $(TARGET)/usr/etc/avahi/
	#cp $(AVAHI_DIR)/service-http $(TARGET)/usr/etc/avahi/services/http.service
	cp $(AVAHI_DIR)/afpd.service $(TARGET)/usr/etc/avahi/services/afpd.service
	cp $(AVAHI_DIR)/smbd.service $(TARGET)/usr/etc/avahi/services/smbd.service
	cp $(AVAHI_DIR)/adisk.service $(TARGET)/usr/etc/avahi/services/adisk.service

	cp $(STAGING_DIR)/etc/avahi/avahi-dnsconfd.action $(TARGET)/usr/etc/avahi/
	cp $(STAGING_DIR)/usr/sbin/avahi-dnsconfd $(TARGET)/usr/sbin/

