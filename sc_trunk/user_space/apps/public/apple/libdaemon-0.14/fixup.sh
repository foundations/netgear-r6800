#!/bin/bash
sed -i -e "s,^\(libdir\)=.*,\1=$1/usr/local/lib,g" $1/usr/local/lib/libdaemon.la
sed -i -e "s,^\(prefix\)=.*,\1=$1/usr/local,g" $1/usr/local/lib/pkgconfig/libdaemon.pc
