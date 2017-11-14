#!/bin/bash
build_dir=build_sc
cd $build_dir
CC=$1 ../configure --target=mipsel-linux-uclibc --host=mipsel-linux-uclibc --build=i486-linux-gnu
