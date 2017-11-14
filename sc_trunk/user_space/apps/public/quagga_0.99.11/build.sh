#!/bin/sh
#environment variable
export CC=/Platform/DG834V4/DGN2000/Source/tools/mips-linux-uclibc/bin/mips-uclibc-gcc
export CFLAGS="-Wall -s -Os"
#http://www.uclibc.org/FAQ.html
export ac_cv_func_malloc_0_nonnull=yes
export ac_cv_func_realloc_0_nonnull=yes
#config feature
./configure --disable-rtadv --enable-netlink --disable-ipv6 --disable-ospfd --disable-ospf6d --disable-ospfapi --disable-bgpd --disable-bgp-announce --disable-ospfclient --disable-dependency-tracking --host=mips-linux --enable-user=root --enable-group=root
