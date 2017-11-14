#!/bin/bash
build_dir=build_sc
cd $build_dir

CPPFLAGS="-I`pwd`/../../../../../libs/public/libiconv.1.8/include/ -I`pwd`/../../libxml2-2.9.1/include/ " CFLAGS="-I`pwd`/../../../../../libs/public/libiconv.1.8/include/ -I`pwd`/../../libxml2-2.9.1/include/ -I`pwd`/../  -I`pwd`/../../expat-2.0.1/lib/" LDFLAGS="-static-libstdc++ -Wl,--as-needed -L`pwd`/../../../../../libs/public/libiconv.1.8/lib/ -L`pwd`/../../target/lib -lxml2 -L`pwd`/../../target/usr/local/lib/" CC=$1 ../configure --target=mipsel-linux-uclibc --host=mipsel-linux-uclibc --build=i486-linux-gnu --enable-shared --enable-static --prefix=  --without-cython
