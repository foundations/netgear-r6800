#!/bin/bash
sed -i -e "s,^\(libdir\)=.*,\1=$1/usr/local,g" $1/usr/local/lib/libexpat.la
