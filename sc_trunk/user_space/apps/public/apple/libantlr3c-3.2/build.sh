#!/bin/bash
build_dir=build_sc
cd $build_dir

CPPFLAGS="-I`pwd`/../ -I`pwd`/../include/ " CFLAGS="-I`pwd`/../ -I`pwd`/../include/  -I`pwd`/../../expat-2.0.1/lib/" LDFLAGS="-L`pwd`/../../target/usr/local/lib/" CC=$1 ../configure --target=mipsel-linux-uclibc --host=mipsel-linux-uclibc --build=i486-linux-gnu --enable-shared --enable-static --prefix=
