#!/bin/bash
build_dir=build_sc
cd $build_dir
#CC=/opt/toolchains/uclibc-crosstools-gcc-4.4.2-1-with-ftw/usr/bin/mips-linux-uclibc-gcc ../dist/configure --target=mips-linux-uclibc --host=mips-linux-uclibc --build=i486-linux-gnu --prefix=/usr --enable-shared --enable-static --disable-java --disable-cxx --with-mutex=UNIX/fcntl --disable-tcl --disable-rpc --enable-compat185 --enable-debug --enable-statistics --enable-replication --enable-cryptography
CC=$1 ../dist/configure --target=mipsel-linux-uclibc --host=mipsel-linux-uclibc --build=i486-linux-gnu --prefix=/usr --enable-shared --enable-static --disable-java --disable-cxx --with-mutex=UNIX/fcntl --disable-tcl --disable-rpc --enable-compat185 --enable-debug --enable-statistics --enable-replication --enable-cryptography
