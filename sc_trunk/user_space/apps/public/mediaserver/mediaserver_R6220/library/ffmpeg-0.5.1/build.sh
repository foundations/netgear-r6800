#!/bin/bash
#build_dir=build_sc
#cd $build_dir
#CC=/opt/buildroot-gcc463/usr/bin/mipsel-linux-gcc ./configure --enable-shared --enable-gpl --arch=mips --enable-cross-compile --disable-muxers --disable-encoders --disable-filters --disable-vhook --extra-cflags=-fPIC --disable-zlib --disable-debug --enable-small --disable-ffmpeg --disable-ffplay --disable-ffserver --disable-swscale --disable-postproc --disable-avfilter --disable-outdevs --disable-indevs --disable-encoders --disable-muxers --disable-decoders --enable-decoder=aac --enable-decoder=aac_latm --enable-decoder=ac3 --enable-decoder=aasc --enable-decoder=avs --enable-decoder=h264 --enable-decoder=mp2 --enable-decoder=mp3 --enable-decoder=mpeg1video --enable-decoder=mpeg2video --enable-decoder=mpegvideo --enable-decoder=mpeg4 --enable-decoder=msmpeg4v1 --enable-decoder=msmpeg4v2 --enable-decoder=msmpeg4v3 --enable-decoder=wmv1 --enable-decoder=wmv2 --enable-decoder=wmv3
local_path=`pwd`
echo "path=$local_path"
cat config.mak | sed "s|SRC_PATH=.*$|SRC_PATH=\"$local_path\"|" | sed "s|SRC_PATH_BARE=.*$|SRC_PATH_BARE=$local_path|" | sed "s|BUILD_ROOT=.*$|BUILD_ROOT=\"$local_path\"|" > tmp1
mv tmp1 config.mak
