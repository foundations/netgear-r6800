#!/bin/bash
sed -i -e "s,^\(libdir\)=.*,\1=$1/usr,g" $1/usr/lib/libdb-4.7.la
