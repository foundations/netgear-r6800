#!/bin/bash
build_dir=build_sc
cd $build_dir

CC_VAR=/opt/buildroot-gcc463/usr/bin/mipsel-buildroot-linux-uclibc-gcc
#CC_VAR=/opt/buildroot-gcc342/bin/mipsel-linux-uclibc-gcc
CFLAGS_VARS= "-D_GNU_SOURCE ";

LDFLAGS_VARS=" ";

CC="$CC_VAR" CFLAGS="$CFLAGS_VARS" CPPFLAGS="$CFLAGS_VARS" LDFLAGS="$LDFLAGS_VARS" ../configure --target=mipsel-linux-uclibc --host=mipsel-linux-uclibc --build=i486-linux-gnu --prefix= --sbindir=/usr/sbin --sysconfdir=/etc --exec-prefix=/usr --sbindir=/usr/sbin --sysconfdir=/etc --datadir=/usr/share --mandir=/usr/man --exec-prefix=/usr\
	--with-ssl \
	--without-ca-bundle 
