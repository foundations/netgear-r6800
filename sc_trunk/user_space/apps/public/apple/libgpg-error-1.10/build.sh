#!/bin/bash

build_dir=build_sc
cd $build_dir
CC=$1 ../configure --host=mipsel-linux-uclibc --build=i486-linux-gnu prefix=/usr --enable-shared --enable-static --disable-rpath
