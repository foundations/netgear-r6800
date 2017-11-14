#!/bin/bash
build_dir=build_sc
cd $build_dir

CPPFLAGS="-I`pwd`/../../../../../libs/public/tr_lib/zlib/zlib-1.2.3/include/ " CFLAGS="-I`pwd`/../../../../../libs/public/tr_lib/zlib/zlib-1.2.3/include/ -I`pwd`/../  -I`pwd`/../../expat-2.0.1/lib/" LDFLAGS=" -L`pwd`/../../../../../libs/public/tr_lib/zlib/zlib-1.2.3/ -lz -L`pwd`/../../target/usr/local/lib/" CC=$1 ../configure --target=mipsel-linux-uclibc --host=mipsel-linux-uclibc --build=i486-linux-gnu --enable-shared --enable-static --prefix= \
  --with-c14n=yes \
  --with-catalog=no \
  --with-debug=yes \
  --with-docbook=no \
  --with-fexceptions=no \
  --with-ftp=no \
  --with-history=no \
  --with-html=yes \
  --with-http=no \
  --with-iconv=no \
  --with-icu=no \
  --with-iso8859x=yes \
  --with-legacy=no \
  --with-mem-debug=no  \
  --with-minimum=yes  \
  --with-output=yes \
  --with-pattern=no \
  --with-push=no \
  --with-python=no \
  --with-reader=yes \
  --with-readline=no \
  --with-regexps=no \
  --with-run-debug=no \
  --with-sax1=yes \
  --with-schemas=no \
  --with-schematron=no \
  --with-threads=yes \
  --with-thread-alloc=no \
  --with-tree=yes \
  --with-valid=yes \
  --with-writer=yes \
  --with-xinclude=yes \
  --with-xpath=yes \
  --with-xptr=yes \
  --with-modules=yes \
  --with-zlib=yes \
  --with-lzma=no \
  --with-coverage=no   
