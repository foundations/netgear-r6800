#!/bin/bash

CPPFLAGS="-I." CFLAGS="-I."  CC=$1 ./configure --target=mipsel-linux-uclibc --host=mipsel-linux-uclibc --build=i486-linux-gnu --enable-shared --prefix= \
		  --disable-python
