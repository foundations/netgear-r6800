#!/bin/bash
build_dir=build_sc
cd $build_dir
CC=$1 CFLAGS=-I`pwd`/../../target/usr/include LDFLAGS=-L`pwd`/../../target/usr/lib ../configure --host=mipsel-linux-uclibc --build=i486-linux-gnu prefix=/usr --enable-shared --enable-static --disable-asm --with-gpg-error-prefix=`pwd`/../../target/usr --enable-digests="md5 rmd160 sha1 sha256 sha512" --enable-ciphers="arcfour des aes cast5" --enable-pubkey-ciphers="rsa dsa"
