CURR_DIR := $(shell pwd)
BUILD_DIR:=$(subst /user_space, /user_space,$(CURR_DIR))
BUILD_DIR:=$(word 1, $(BUILD_DIR))

include $(BUILD_DIR)/Rules.mak

DESTLIB = $(ROOT)/target/lib
DESTBIN = $(ROOT)/target/usr/sbin
FFMPEG_DIR=$(APPS_DIR)/public/mediaserver/$(MEDIASERVER_USE)/library/ffmpeg-0.11.2

CFLAGS = -s -Os -fPIC
CFLAGS += -I$(APPS_DIR)/public/mediaserver/$(MEDIASERVER_USE)/library/include/ -I$(FFMPEG_DIR)/include/ -I$(FFMPEG_DIR)/
CFLAGS += -I$(FFMPEG_DIR)/libavformat/
#CFLAGS += -I$(FFMPEG_DIR)/libavutil/
CFLAGS += -I$(FFMPEG_DIR)/libavcodec/
CFLAGS += -I$(APPS_DIR)/public/mediaserver/$(MEDIASERVER_USE)/library/zlib-1.2.3
CFLAGS += -I$(APPS_DIR)/public/mediaserver/$(MEDIASERVER_USE)/library/libid3tag-0.15.1b/
CFLAGS += -I$(APPS_DIR)/public/mediaserver/$(MEDIASERVER_USE)/library/libvorbis-1.3.2/include/
CFLAGS += -I$(APPS_DIR)/public/mediaserver/$(MEDIASERVER_USE)/library/flac-1.2.1/include/  
CFLAGS += -I$(APPS_DIR)/public/mediaserver/$(MEDIASERVER_USE)/library/libogg-1.3.0/include/
CFLAGS += -I$(APPS_DIR)/public/mediaserver/$(MEDIASERVER_USE)/library/libexif-0.6.20/
CFLAGS += -I$(LIBS_DIR)/public/libiconv.1.8/include/
#CFLAGS += -I/opt/toolchains/crosstools-arm-gcc-4.6-linux-3.4-uclibc-0.9.32-binutils-2.21-NPTL/usr/arm-unknown-linux-uclibcgnueabi/sysroot/usr/include
CFLAGS += -I$(KERNEL_INC)


LDFLAGS =  -L$(APPS_DIR)/public/mediaserver/$(MEDIASERVER_USE)/library/lib/
LDFLAGS += -L$(FFMPEG_DIR)/libavformat/
LDFLAGS += -L$(FFMPEG_DIR)/libavutil/
LDFLAGS += -L$(FFMPEG_DIR)/libavcodec/
LDFLAGS += -L$(APPS_DIR)/public/mediaserver/$(MEDIASERVER_USE)/library/libexif-0.6.20/libexif/.libs/
LDFLAGS += -L$(APPS_DIR)/public/mediaserver/$(MEDIASERVER_USE)/library/zlib-1.2.3/
LDFLAGS += -L$(APPS_DIR)/public/mediaserver/$(MEDIASERVER_USE)/library/libid3tag-0.15.1b/.libs/
LDFLAGS += -L$(APPS_DIR)/public/mediaserver/$(MEDIASERVER_USE)/library/libogg-1.3.0/src/.libs/
LDFLAGS += -L$(APPS_DIR)/public/mediaserver/$(MEDIASERVER_USE)/library/flac-1.2.1/src/libFLAC/.libs
LDFLAGS += -L$(APPS_DIR)/public/mediaserver/$(MEDIASERVER_USE)/library/libvorbis-1.3.2/lib/.libs
LDFLAGS += -L$(LIBS_DIR)/public/libiconv.1.8/lib
LDFLAGS += -L$(APPS_DIR)/public/apple/sqlite-autoconf-3130000/build_sc/.libs/

all: config
	cd minidlna-1.1.5 && make	
config:
	@(if test -f h_config; then \
		echo "has configuration" ;   \
        else \
		make configure; \
		rm -rf h_config; \
		touch h_config; \
	fi)
configure:
	cd minidlna-1.1.5 && ./configure --enable-tivo --host=mipsel-linux CC=$(CROSS)gcc CFLAGS="$(CFLAGS)" LDFLAGS="$(LDFLAGS)" LIBS="-lz -lexif -ljpeg -lsqlite3 -lavformat -lavcodec -lavutil -lid3tag -lFLAC -logg -lvorbis -liconv"

install:	
	$(STRIP) minidlna-1.1.5/minidlnad
	cp -af minidlna-1.1.5/minidlnad $(ROOT)/target/usr/sbin/minidlna
	$(INSTALL) --mode=0644 minidlna.conf $(ROOT)/usr/etc/

clean:
	cd minidlna-1.1.5 && make clean
