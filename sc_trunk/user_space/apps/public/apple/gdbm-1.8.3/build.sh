#!/bin/bash
build_dir=build_sc
cd $build_dir
CC=$1 ../configure --target=mipsel-linux-uclibc --host=mipsel-linux --build=i486-linux-gnu --enable-shared --enable-static
echo "INSTALL_ROOT := \$(DESTDIR)" >> Makefile
echo "BINGRP := `id -u`" >> Makefile
echo "BINOWN := `id -g`" >> Makefile
