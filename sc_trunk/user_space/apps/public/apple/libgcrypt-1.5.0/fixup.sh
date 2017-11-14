#!/bin/bash
sed -i -e "s,^\(libdir\)=.*,\1=$1/usr/lib,g" $1/usr/lib/libgcrypt.la
