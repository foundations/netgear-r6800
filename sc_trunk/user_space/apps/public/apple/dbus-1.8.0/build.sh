#!/bin/bash
build_dir=build_sc
cd $build_dir

CFLAGS_VARS=" -I`pwd/`../ -I`pwd`/../../expat-2.0.1/lib/ ";

CPPFLAGS="$CFLAGS_VARS" CFLAGS="$CFLAGS_VARS" LDFLAGS="-L`pwd`/../../target/usr/local/lib/" CC=$1 ../configure --target=mipsel-linux-uclibc --host=mipsel-linux-uclibc --build=i486-linux-gnu --enable-shared --enable-static --disable-launchd --disable-systemd --prefix=
