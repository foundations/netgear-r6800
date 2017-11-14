#!/bin/bash
sed -i -e "s,^\(libdir\)=.*,\1=$1/usr/local/lib,g" $1/usr/local/lib/libgdbm.la
