#!/bin/bash
build_dir=build_sc
cd $build_dir

CPPFLAGS="-I." CFLAGS="-I`pwd`/../ -I. -I`pwd`/../../expat-2.0.1/lib/" LDFLAGS="-L`pwd`/../../target/usr/local/lib/" CC=$1 ../configure --target=$2uclibc --host=$2uclibc --build=i486-linux-gnu --enable-shared --prefix= \
		  --disable-python
