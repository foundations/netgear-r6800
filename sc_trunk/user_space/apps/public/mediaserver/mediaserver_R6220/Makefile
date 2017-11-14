

CURR_DIR := $(shell pwd)
BUILD_DIR:=$(subst /user_space, /user_space,$(CURR_DIR))
BUILD_DIR:=$(word 1, $(BUILD_DIR))

include $(BUILD_DIR)/Rules.mak


#include ../../Rules.mak
#export _CROSS
export CROSS_COMPILE=$(CROSS)

#export FF_PLATFORM = x86
export FF_PLATFORM = mips
#export FF_PLATFORM = arm

FFMPEG_DIR=

ifeq ($(USE_MINIDLNA_VERSION),)
USE_MINIDLNA_VERSION=1.1.15
endif

ifeq ($(USE_MINIDLNA_VERSION),1.1.15)
	FFMPEG_DIR = library/ffmpeg-0.11.2
else 
ifeq ($(USE_MINIDLNA_VERSION),1.0.26)
	FFMPEG_DIR = library/ffmpeg-0.5.1
else
	FFMPEG_DIR = library/ffmpeg-0.10
endif
endif

ifeq ($(USE_MINIDLNA),1)
	SUBLIBS = $(FFMPEG_DIR)
	SUBLIBS += library/zlib-1.2.3
	SUBLIBS += library/jpeg-6b
	SUBLIBS += library/libogg-1.3.0/src
	SUBLIBS += library/libexif-0.6.20/libexif
	SUBLIBS += library/flac-1.2.1/src/libFLAC
	SUBLIBS += library/libid3tag-0.15.1b
	SUBLIBS += library/libvorbis-1.3.2/lib
	#built by apps/apple
	#SUBLIBS += ../apple/sqlite-3.6.16/build_sc/

ifeq ($(USE_MINIDLNA_VERSION),1.1.15)
	SUBDIRS += minidlna
else
	SUBDIRS += minidlna-$(USE_MINIDLNA_VERSION)
endif
else

SUBLIBS = library/ffmpeg-export-2008-01-17
SUBLIBS += library/zlib-1.2.3
SUBLIBS += library/sqlite-3.5.9
SUBLIBS += library/jpeg-6b
SUBLIBS	+= library/libdlna
SUBLIBS += library/giflib-4.1.4
SUBLIBS += library/libpng-1.2.18
ifneq ($(GPL),1)
SUBLIBS += library/libupnp-1.6.0_dlna
SUBLIBS += libs
SUBDIRS	= mediaserver_dlna
endif

endif
.PHONY:libs

all: libm-prepare ffmpeg-config libs dirs

ffmpeg-config:
	[ -f $(FFMPEG_DIR)/.sc_config ] || (cd $(FFMPEG_DIR); sh ./build.sh; touch .sc_config)

libm-prepare:
	find . -name Makefile | xargs touch

libs:${SUBLIBS}
	@for i in ${SUBLIBS} ;  do                                                    \
        echo "-----------------------------------------------------------------"; \
	echo -e "\033[;34m                         $$i                 \033[;0m"; \
	echo "-----------------------------------------------------------------"; \
	make -C $$i || exit 1 ;                                                   \
	done

dirs : ${SUBDIRS}
	@for i in ${SUBDIRS} ;  do                                                    \
	echo "-----------------------------------------------------------------"; \
	echo -e "\033[;34m                         $$i                 \033[;0m"; \
	echo "-----------------------------------------------------------------"; \
	make -C $$i || exit 1 ;                                                   \
	done

install:
	for i in ${SUBLIBS} ; do make -C $$i install || exit 1 ; done
	for i in ${SUBDIRS} ; do make -C $$i install || exit 1 ; done
	$(STRIP) $(FFMPEG_DIR)/libavutil/libavutil.so* $(FFMPEG_DIR)/libavformat/libavformat.so* $(FFMPEG_DIR)/libavcodec/libavcodec.so.*
	cp -a $(FFMPEG_DIR)/libavutil/libavutil.so* $(TARGET_DIR)/lib/
	cp -a $(FFMPEG_DIR)/libavformat/libavformat.so* $(TARGET_DIR)/lib/
	cp -a $(FFMPEG_DIR)/libavcodec/libavcodec.so.* $(TARGET_DIR)/lib/


clean:
	for i in ${SUBLIBS} ; do make -C $$i clean || exit 1 ; done
	for i in ${SUBDIRS} ; do make -C $$i clean || exit 1 ; done
	rm -f library/lib/*
	


#=====================================================================================
