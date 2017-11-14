cmd_libbb/perror_nomsg.o := "/opt/buildroot-gcc463_NPTLSHAREDLIBGCC/usr/bin"/mipsel-linux-gcc -Wp,-MD,libbb/.perror_nomsg.o.d   -std=gnu99 -Iinclude -Ilibbb  -include include/autoconf.h -D_GNU_SOURCE -DNDEBUG -D_LARGEFILE_SOURCE -D_LARGEFILE64_SOURCE -D_FILE_OFFSET_BITS=64 -D"BB_VER=KBUILD_STR(1.12.1)" -DBB_BT=AUTOCONF_TIMESTAMP -I/home/disk2/eason/eason_disk3/R6950_1042/sdk5010  -Wall -Wshadow -Wwrite-strings -Wundef -Wstrict-prototypes -Wunused -Wunused-parameter -Wmissing-prototypes -Wmissing-declarations -Wdeclaration-after-statement -Wold-style-definition -fno-builtin-strlen -finline-limit=0 -fomit-frame-pointer -ffunction-sections -fdata-sections -fno-guess-branch-probability -funsigned-char -static-libgcc -falign-functions=1 -falign-jumps=1 -falign-labels=1 -falign-loops=1 -Os    -D"KBUILD_STR(s)=\#s" -D"KBUILD_BASENAME=KBUILD_STR(perror_nomsg)"  -D"KBUILD_MODNAME=KBUILD_STR(perror_nomsg)" -c -o libbb/perror_nomsg.o libbb/perror_nomsg.c

deps_libbb/perror_nomsg.o := \
  libbb/perror_nomsg.c \
  include/platform.h \
    $(wildcard include/config/werror.h) \
    $(wildcard include/config///.h) \
    $(wildcard include/config/nommu.h) \
    $(wildcard include/config//nommu.h) \
    $(wildcard include/config//mmu.h) \
  /opt/buildroot-gcc463_NPTLSHAREDLIBGCC/usr/mipsel-buildroot-linux-uclibc/sysroot/usr/include/byteswap.h \
  /opt/buildroot-gcc463_NPTLSHAREDLIBGCC/usr/mipsel-buildroot-linux-uclibc/sysroot/usr/include/bits/byteswap.h \
  /opt/buildroot-gcc463_NPTLSHAREDLIBGCC/usr/mipsel-buildroot-linux-uclibc/sysroot/usr/include/bits/byteswap-common.h \
  /opt/buildroot-gcc463_NPTLSHAREDLIBGCC/usr/mipsel-buildroot-linux-uclibc/sysroot/usr/include/endian.h \
    $(wildcard include/config/.h) \
  /opt/buildroot-gcc463_NPTLSHAREDLIBGCC/usr/mipsel-buildroot-linux-uclibc/sysroot/usr/include/features.h \
    $(wildcard include/config/c99.h) \
    $(wildcard include/config/c95.h) \
    $(wildcard include/config/ix.h) \
    $(wildcard include/config/ix2.h) \
    $(wildcard include/config/ix199309.h) \
    $(wildcard include/config/ix199506.h) \
    $(wildcard include/config/en.h) \
    $(wildcard include/config/en/extended.h) \
    $(wildcard include/config/x98.h) \
    $(wildcard include/config/en2k.h) \
    $(wildcard include/config/en2k8.h) \
    $(wildcard include/config/gefile.h) \
    $(wildcard include/config/gefile64.h) \
    $(wildcard include/config/e/offset64.h) \
    $(wildcard include/config/d.h) \
    $(wildcard include/config/c.h) \
    $(wildcard include/config/ile.h) \
    $(wildcard include/config/ntrant.h) \
    $(wildcard include/config/tify/level.h) \
    $(wildcard include/config/i.h) \
    $(wildcard include/config/ern/inlines.h) \
    $(wildcard include/config/ix/implicitly.h) \
  /opt/buildroot-gcc463_NPTLSHAREDLIBGCC/usr/mipsel-buildroot-linux-uclibc/sysroot/usr/include/bits/uClibc_config.h \
    $(wildcard include/config/mips/o32/abi//.h) \
    $(wildcard include/config/mips/n32/abi//.h) \
    $(wildcard include/config/mips/n64/abi//.h) \
    $(wildcard include/config/mips/isa/1//.h) \
    $(wildcard include/config/mips/isa/2//.h) \
    $(wildcard include/config/mips/isa/3//.h) \
    $(wildcard include/config/mips/isa/4//.h) \
    $(wildcard include/config/mips/isa/mips32//.h) \
    $(wildcard include/config/mips/isa/mips32r2//.h) \
    $(wildcard include/config/mips/isa/mips64//.h) \
    $(wildcard include/config//.h) \
    $(wildcard include/config/link//.h) \
  /opt/buildroot-gcc463_NPTLSHAREDLIBGCC/usr/mipsel-buildroot-linux-uclibc/sysroot/usr/include/sys/cdefs.h \
    $(wildcard include/config/espaces.h) \
  /opt/buildroot-gcc463_NPTLSHAREDLIBGCC/usr/mipsel-buildroot-linux-uclibc/sysroot/usr/include/bits/endian.h \
  /opt/buildroot-gcc463_NPTLSHAREDLIBGCC/usr/mipsel-buildroot-linux-uclibc/sysroot/usr/include/arpa/inet.h \
  /opt/buildroot-gcc463_NPTLSHAREDLIBGCC/usr/mipsel-buildroot-linux-uclibc/sysroot/usr/include/netinet/in.h \
  /opt/buildroot-gcc463_NPTLSHAREDLIBGCC/usr/lib/gcc/mipsel-buildroot-linux-uclibc/4.6.3/include/stdint.h \
  /opt/buildroot-gcc463_NPTLSHAREDLIBGCC/usr/mipsel-buildroot-linux-uclibc/sysroot/usr/include/stdint.h \
  /opt/buildroot-gcc463_NPTLSHAREDLIBGCC/usr/mipsel-buildroot-linux-uclibc/sysroot/usr/include/bits/wchar.h \
  /opt/buildroot-gcc463_NPTLSHAREDLIBGCC/usr/mipsel-buildroot-linux-uclibc/sysroot/usr/include/bits/wordsize.h \
  /opt/buildroot-gcc463_NPTLSHAREDLIBGCC/usr/mipsel-buildroot-linux-uclibc/sysroot/usr/include/sys/socket.h \
  /opt/buildroot-gcc463_NPTLSHAREDLIBGCC/usr/mipsel-buildroot-linux-uclibc/sysroot/usr/include/sys/uio.h \
  /opt/buildroot-gcc463_NPTLSHAREDLIBGCC/usr/mipsel-buildroot-linux-uclibc/sysroot/usr/include/sys/types.h \
  /opt/buildroot-gcc463_NPTLSHAREDLIBGCC/usr/mipsel-buildroot-linux-uclibc/sysroot/usr/include/bits/types.h \
  /opt/buildroot-gcc463_NPTLSHAREDLIBGCC/usr/lib/gcc/mipsel-buildroot-linux-uclibc/4.6.3/include/stddef.h \
  /opt/buildroot-gcc463_NPTLSHAREDLIBGCC/usr/mipsel-buildroot-linux-uclibc/sysroot/usr/include/bits/typesizes.h \
  /opt/buildroot-gcc463_NPTLSHAREDLIBGCC/usr/mipsel-buildroot-linux-uclibc/sysroot/usr/include/bits/pthreadtypes.h \
  /opt/buildroot-gcc463_NPTLSHAREDLIBGCC/usr/mipsel-buildroot-linux-uclibc/sysroot/usr/include/sgidefs.h \
  /opt/buildroot-gcc463_NPTLSHAREDLIBGCC/usr/mipsel-buildroot-linux-uclibc/sysroot/usr/include/time.h \
  /opt/buildroot-gcc463_NPTLSHAREDLIBGCC/usr/mipsel-buildroot-linux-uclibc/sysroot/usr/include/sys/select.h \
  /opt/buildroot-gcc463_NPTLSHAREDLIBGCC/usr/mipsel-buildroot-linux-uclibc/sysroot/usr/include/bits/select.h \
  /opt/buildroot-gcc463_NPTLSHAREDLIBGCC/usr/mipsel-buildroot-linux-uclibc/sysroot/usr/include/bits/sigset.h \
  /opt/buildroot-gcc463_NPTLSHAREDLIBGCC/usr/mipsel-buildroot-linux-uclibc/sysroot/usr/include/bits/time.h \
  /opt/buildroot-gcc463_NPTLSHAREDLIBGCC/usr/mipsel-buildroot-linux-uclibc/sysroot/usr/include/sys/sysmacros.h \
  /opt/buildroot-gcc463_NPTLSHAREDLIBGCC/usr/mipsel-buildroot-linux-uclibc/sysroot/usr/include/bits/uio.h \
  /opt/buildroot-gcc463_NPTLSHAREDLIBGCC/usr/mipsel-buildroot-linux-uclibc/sysroot/usr/include/bits/socket.h \
  /opt/buildroot-gcc463_NPTLSHAREDLIBGCC/usr/lib/gcc/mipsel-buildroot-linux-uclibc/4.6.3/include-fixed/limits.h \
  /opt/buildroot-gcc463_NPTLSHAREDLIBGCC/usr/lib/gcc/mipsel-buildroot-linux-uclibc/4.6.3/include-fixed/syslimits.h \
  /opt/buildroot-gcc463_NPTLSHAREDLIBGCC/usr/mipsel-buildroot-linux-uclibc/sysroot/usr/include/limits.h \
  /opt/buildroot-gcc463_NPTLSHAREDLIBGCC/usr/mipsel-buildroot-linux-uclibc/sysroot/usr/include/bits/posix1_lim.h \
  /opt/buildroot-gcc463_NPTLSHAREDLIBGCC/usr/mipsel-buildroot-linux-uclibc/sysroot/usr/include/bits/local_lim.h \
  /opt/buildroot-gcc463_NPTLSHAREDLIBGCC/usr/mipsel-buildroot-linux-uclibc/sysroot/usr/include/linux/limits.h \
  /opt/buildroot-gcc463_NPTLSHAREDLIBGCC/usr/mipsel-buildroot-linux-uclibc/sysroot/usr/include/bits/uClibc_local_lim.h \
  /opt/buildroot-gcc463_NPTLSHAREDLIBGCC/usr/mipsel-buildroot-linux-uclibc/sysroot/usr/include/bits/posix2_lim.h \
  /opt/buildroot-gcc463_NPTLSHAREDLIBGCC/usr/mipsel-buildroot-linux-uclibc/sysroot/usr/include/bits/xopen_lim.h \
  /opt/buildroot-gcc463_NPTLSHAREDLIBGCC/usr/mipsel-buildroot-linux-uclibc/sysroot/usr/include/bits/stdio_lim.h \
  /opt/buildroot-gcc463_NPTLSHAREDLIBGCC/usr/mipsel-buildroot-linux-uclibc/sysroot/usr/include/bits/sockaddr.h \
  /opt/buildroot-gcc463_NPTLSHAREDLIBGCC/usr/mipsel-buildroot-linux-uclibc/sysroot/usr/include/asm/socket.h \
  /opt/buildroot-gcc463_NPTLSHAREDLIBGCC/usr/mipsel-buildroot-linux-uclibc/sysroot/usr/include/asm/sockios.h \
  /opt/buildroot-gcc463_NPTLSHAREDLIBGCC/usr/mipsel-buildroot-linux-uclibc/sysroot/usr/include/asm/ioctl.h \
  /opt/buildroot-gcc463_NPTLSHAREDLIBGCC/usr/mipsel-buildroot-linux-uclibc/sysroot/usr/include/asm-generic/ioctl.h \
  /opt/buildroot-gcc463_NPTLSHAREDLIBGCC/usr/mipsel-buildroot-linux-uclibc/sysroot/usr/include/bits/in.h \
  /opt/buildroot-gcc463_NPTLSHAREDLIBGCC/usr/lib/gcc/mipsel-buildroot-linux-uclibc/4.6.3/include/stdbool.h \
  /opt/buildroot-gcc463_NPTLSHAREDLIBGCC/usr/mipsel-buildroot-linux-uclibc/sysroot/usr/include/sys/mount.h \
  /opt/buildroot-gcc463_NPTLSHAREDLIBGCC/usr/mipsel-buildroot-linux-uclibc/sysroot/usr/include/sys/ioctl.h \
  /opt/buildroot-gcc463_NPTLSHAREDLIBGCC/usr/mipsel-buildroot-linux-uclibc/sysroot/usr/include/bits/ioctls.h \
  /opt/buildroot-gcc463_NPTLSHAREDLIBGCC/usr/mipsel-buildroot-linux-uclibc/sysroot/usr/include/asm/ioctls.h \
  /opt/buildroot-gcc463_NPTLSHAREDLIBGCC/usr/mipsel-buildroot-linux-uclibc/sysroot/usr/include/bits/ioctl-types.h \
  /opt/buildroot-gcc463_NPTLSHAREDLIBGCC/usr/mipsel-buildroot-linux-uclibc/sysroot/usr/include/sys/ttydefaults.h \

libbb/perror_nomsg.o: $(deps_libbb/perror_nomsg.o)

$(deps_libbb/perror_nomsg.o):
