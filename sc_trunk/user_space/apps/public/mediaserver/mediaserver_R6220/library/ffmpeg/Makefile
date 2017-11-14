
CURR_DIR := $(shell pwd)
BUILD_DIR:=$(subst /user_space, /user_space,$(CURR_DIR))
BUILD_DIR:=$(word 1, $(BUILD_DIR))

include $(BUILD_DIR)/Rules.mak

DESTLIB = $(ROOT)/target/lib
DESTBIN = $(ROOT)/target/usr/sbin

LDFLAGS += -L$(APPS_DIR)/public/mediaserver/$(MEDIASERVER_USE)/library/zlib-1.2.3 -L$(LIBS_DIR)/public/libiconv.1.8/lib
CFLAGS += -I$(APPS_DIR)/public/mediaserver/$(MEDIASERVER_USE)/library/zlib-1.2.3 -I$(LIBS_DIR)/public/libiconv.1.8/include/

LDFLAGS += -L$(LIBS_DIR)/public/libiconv.1.8/lib
CFLAGS += -I$(LIBS_DIR)/public/libiconv.1.8/include/


all: config
	cd ffmpeg-3.0.1 && make	
config:
	@(if test -f h_config; then \
		echo "has configuration" ;   \
        else \
		make configure; \
		rm -rf h_config; \
		touch h_config; \
	fi)
configure:
	cd ffmpeg-3.0.1 && ./configure  --enable-cross-compile --enable-shared --enable-zlib --arch=mips --disable-stripping --target-os=linux --cc=$(CROSS)gcc --extra-cflags="$(CFLAGS)" --extra-ldflags="$(LDFLAGS)" --disable-debug --enable-small --disable-doc --disable-ffmpeg --disable-ffplay --disable-ffserver --enable-swresample --enable-swscale --disable-postproc --disable-avdevice --disable-avfilter --disable-ffprobe --disable-outdevs --disable-indevs --disable-encoders --disable-muxers --disable-decoders --enable-decoder=aac --enable-decoder=aac_latm --enable-decoder=ac3 --enable-decoder=aasc --enable-decoder=avs --enable-decoder=h264 --enable-decoder=mp2 --enable-decoder=mp3 --enable-decoder=mpeg1video --enable-decoder=mpeg2video --enable-decoder=dnxhd --enable-decoder=mpeg2_qsv --enable-decoder=mpeg2_mmal --enable-decoder=mpeg2_crystalhd  --enable-decoder=mpegvideo --enable-decoder=mpeg4_vdpau --enable-decoder=mpeg4_mmal --enable-decoder=mpeg4_crystalhd --enable-decoder=mpeg4 --enable-decoder=msmpeg4v1 --enable-decoder=msmpeg4v2 --enable-decoder=msmpeg4v3 --enable-decoder=msmpeg4_crystalhd  --enable-decoder=wmv1 --enable-decoder=wmv2 --enable-decoder=wmv3
	#cd ffmpeg-3.0.1 && ./configure  --enable-cross-compile --enable-shared --enable-zlib --arch=arm --disable-stripping --target-os=linux --cc=$(CROSS)gcc --extra-cflags="$(CFLAGS)" --extra-ldflags="$(LDFLAGS)" --disable-debug --enable-small --disable-doc --disable-ffmpeg --disable-ffplay --disable-ffserver --disable-swresample --disable-swscale --disable-postproc --disable-avdevice --disable-avfilter --disable-ffprobe --disable-outdevs --disable-indevs --disable-encoders --disable-muxers --disable-decoders --enable-decoder=aac --enable-decoder=aac_latm --enable-decoder=ac3 --enable-decoder=aasc --enable-decoder=avs --enable-decoder=h264 --enable-decoder=mp2 --enable-decoder=mp3 --enable-decoder=mpeg1video --enable-decoder=mpeg2video --enable-decoder=dnxhd --enable-decoder=mpeg2_qsv --enable-decoder=mpeg2_mmal --enable-decoder=mpeg2_crystalhd  --enable-decoder=mpegvideo --enable-decoder=mpeg4_vdpau --enable-decoder=mpeg4_mmal --enable-decoder=mpeg4_crystalhd --enable-decoder=mpeg4 --enable-decoder=msmpeg4v1 --enable-decoder=msmpeg4v2 --enable-decoder=msmpeg4v3 --enable-decoder=msmpeg4_crystalhd  --enable-decoder=wmv1 --enable-decoder=wmv2 --enable-decoder=wmv3
	#cd ffmpeg-3.0.1 && ./configure  --target-os=linux --enable-shared --enable-gpl --arch=arm --disable-stripping --cc=$(CROSS)gcc --enable-cross-compile --disable-muxers --disable-encoders --disable-filters --extra-cflags=-fPIC

install:		


clean:
	#cd ffmpeg-3.0.1 && make clean

