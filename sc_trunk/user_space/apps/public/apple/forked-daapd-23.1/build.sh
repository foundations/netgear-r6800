#!/bin/bash
build_dir=build_sc
cd $build_dir

FFMPEG_DIR="`pwd`/../../../mediaserver/$2/library/ffmpeg-0.11.2"

#CC_VAR=/opt/buildroot-gcc463/usr/bin/arm-linux-gcc
CC_VAR=$1
CFLAGS_VARS= "-D_GNU_SOURCE ";
CFLAGS_VARS+="-I`pwd`/../../avl-0.3.5/ -I`pwd`/../../mxml-2.8/ ";
CFLAGS_VARS+="-I`pwd`/../../avahi-0.6.31/ -I`pwd`/../../libxml2-2.9.1/include/ ";
CFLAGS_VARS+="-I$FFMPEG_DIR/ -I$FFMPEG_DIR/libavutil/ -I$FFMPEG_DIR/libavformat/ -I$FFMPEG_DIR/libavcodec/ ";
CFLAGS_VARS+="-I`pwd`/../../../../../libs/public/tr_lib/zlib/zlib-1.2.3/include/ -I`pwd`/../../../../../libs/public/tr_lib/zlib/zlib-1.2.3/ ";
#CFLAGS_VARS+="-I`pwd`/../../libunistring-0.9.5/build_sc/lib/ -I`pwd`/../../libunistring-0.9.5/lib/ ";
CFLAGS_VARS+="-I`pwd`/../../target/include -I`pwd`/../../target/usr/include -I`pwd`/../../target/usr/local/include -I`pwd`/../../target/usr/include/dbus-1.0/ ";
#CFLAGS_VARS+="-I`pwd`/../../sqlite-3.6.22 -I`pwd` ";
CFLAGS_VARS+="-I`pwd`/../../sqlite-autoconf-3130000 -I`pwd` ";
#CFLAGS_VARS+="-I/opt/mips-linux-uclibc-0.9.33/usr/include/ "; #new lib

LDFLAGS_VARS="-L`pwd`/../../avl-0.3.5/ -L`pwd`/../../mxml-2.8/ ";
LDFLAGS_VARS+="-L$FFMPEG_DIR/libavcodec/ -L$FFMPEG_DIR/libavutil/ -L$FFMPEG_DIR/libavformat/ -L$FFMPEG_DIR/libswscale/ ";
LDFLAGS_VARS+="-lm -ldl -lpthread -lavl -lmxml -lxml2 -lasound ";
LDFLAGS_VARS+=" `pwd`/../../target/lib/libdbus-1.so ";
LDFLAGS_VARS+=" `pwd`/../../target/usr/lib/libavahi-common.so ";
LDFLAGS_VARS+=" `pwd`/../../target/usr/lib/libavahi-client.so ";
LDFLAGS_VARS+="-lavutil -lavformat -lavcodec -lswscale ";
#LDFLAGS_VARS+="-L`pwd`/../../sqlite-3.6.22/ -lsqlite3 -L`pwd`/../../../zlib-1.2.3/ -lz ";
LDFLAGS_VARS+="-L`pwd`/../../sqlite-autoconf-3130000/build_sc/.libs/ -lsqlite3 -L`pwd`/../../../../../libs/public/tr_lib/zlib/zlib-1.2.3/ -lz ";
LDFLAGS_VARS+="-L`pwd`../../../libiconv.1.8/lib/ ";
LDFLAGS_VARS+="-lantlr3c -lconfuse -levent -lplist -lunistring -lgcrypt -lgpg-error ";
LDFLAGS_VARS+="-L`pwd`/../../target/lib -L`pwd`/../../target/usr/lib -L`pwd`/../../target/usr/local/lib ";
#LDFLAGS_VARS+="-Wl,-rpath=/lib_new/ -Wl,-rpath=/lib/ "; #run time
#LDFLAGS_VARS+="-L/opt/mips-linux-uclibc-0.9.33/usr/lib/ -Wl,-rpath-link=/lib/ "; #link time
#LDFLAGS_VARS+="-nostdlib -fno-builtin "; #link time
#LDFLAGS_VARS+="-Wl,--dynamic-linker=/opt/mips-linux-uclibc-0.9.33/lib/ld-uClibc-0.9.33.2.so /opt/mips-linux-uclibc-0.9.33/lib/ld-uClibc-0.9.33.2.so /opt/mips-linux-uclibc-0.9.33/lib/libpthread-0.9.33.2.so /opt/mips-linux-uclibc-0.9.33/lib/libc.so.0 "; 
#workaround to remove unused link
LINKS="../../target/usr/lib/libavahi-common.la "
LINKS+="../../target/usr/lib/libavahi-core.la "
LINKS+="../../target/usr/lib/libavahi-client.la "
LINKS+="../../target/lib/libplist.la "
LINKS+="../../target/lib/libplist++.la "

for file in $LINKS
do
	cat $file | sed "s/ \/usr\/lib\/libavahi-common.la//" | sed "s/ \/lib\/libdbus-1.la//" | sed "s/ \/lib\/libxml2.la//" > tmp1 
	mv tmp1 $file
done

ANTLR="`pwd`/../../antlr3/antlr3" CC="$CC_VAR" GPG_ERROR_CONFIG="`pwd`/../../target/usr/bin/gpg-error-config" GPG_ERROR_LIBS=" -L`pwd`/../target/usr/lib -l lgpg-error" LIBGCRYPT_CONFIG="`pwd`/../../target/usr/bin/libgcrypt-config" LIBGCRYPT_LIBS=" -L`pwd`/../target/usr/lib -l lgcrypt" CFLAGS="$CFLAGS_VARS" CPPFLAGS="$CFLAGS_VARS" LDFLAGS="$LDFLAGS_VARS" ../configure --target=mipsel-linux-uclibc --host=mipsel-linux-uclibc --build=i486-linux-gnu --prefix= --sbindir=/usr/sbin --sysconfdir=/etc --exec-prefix=/usr --sbindir=/usr/sbin --sysconfdir=/etc --datadir=/usr/share --mandir=/usr/man --exec-prefix=/usr\
	--enable-shared \
	--enable-static \
	--enable-itunes 
#	--enable-flac \
#	--enable-musepack \
#	--with-libunistring-prefix=/home/peter/R6200v3/apps/apple/target/
