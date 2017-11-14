#!/bin/bash
build_dir=build_sc
cd $build_dir

CPPFLAGS="" CFLAGS=" -DSQLITE_ENABLE_UNLOCK_NOTIFY -DHAVE_POSIX_FALLOCATE=0 -I`pwd`/../../expat-2.0.1/lib/" LDFLAGS="-L`pwd`/../../target/usr/local/lib/ -ldl -lpthread " CC=$1 ../configure --target=mipsel-linux-uclibc --host=mipsel-linux-uclibc --build=i486-linux-gnu --enable-shared --enable-static --prefix= --enable-threadsafe --enable-load-extension --disable-tcl
