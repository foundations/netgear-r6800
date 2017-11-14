cmd_libbb/xatonum.o := "/opt/buildroot-gcc463_NPTLSHAREDLIBGCC/usr/bin"/mipsel-linux-gcc -Wp,-MD,libbb/.xatonum.o.d   -std=gnu99 -Iinclude -Ilibbb  -include include/autoconf.h -D_GNU_SOURCE -DNDEBUG -D_LARGEFILE_SOURCE -D_LARGEFILE64_SOURCE -D_FILE_OFFSET_BITS=64 -D"BB_VER=KBUILD_STR(1.12.1)" -DBB_BT=AUTOCONF_TIMESTAMP -I/home/disk2/eason/eason_disk3/R6950_1042/sdk5010  -Wall -Wshadow -Wwrite-strings -Wundef -Wstrict-prototypes -Wunused -Wunused-parameter -Wmissing-prototypes -Wmissing-declarations -Wdeclaration-after-statement -Wold-style-definition -fno-builtin-strlen -finline-limit=0 -fomit-frame-pointer -ffunction-sections -fdata-sections -fno-guess-branch-probability -funsigned-char -static-libgcc -falign-functions=1 -falign-jumps=1 -falign-labels=1 -falign-loops=1 -Os    -D"KBUILD_STR(s)=\#s" -D"KBUILD_BASENAME=KBUILD_STR(xatonum)"  -D"KBUILD_MODNAME=KBUILD_STR(xatonum)" -c -o libbb/xatonum.o libbb/xatonum.c

deps_libbb/xatonum.o := \
  libbb/xatonum.c \
  include/libbb.h \
    $(wildcard include/config/selinux.h) \
    $(wildcard include/config/locale/support.h) \
    $(wildcard include/config/use/bb/pwd/grp.h) \
    $(wildcard include/config/feature/shadowpasswds.h) \
    $(wildcard include/config/use/bb/shadow.h) \
    $(wildcard include/config/lfs.h) \
    $(wildcard include/config/feature/buffers/go/on/stack.h) \
    $(wildcard include/config/buffer.h) \
    $(wildcard include/config/ubuffer.h) \
    $(wildcard include/config/feature/buffers/go/in/bss.h) \
    $(wildcard include/config/inux.h) \
    $(wildcard include/config/feature/ipv6.h) \
    $(wildcard include/config/feature/check/names.h) \
    $(wildcard include/config/feature/prefer/applets.h) \
    $(wildcard include/config/busybox/exec/path.h) \
    $(wildcard include/config/getopt/long.h) \
    $(wildcard include/config/feature/pidfile.h) \
    $(wildcard include/config/feature/syslog.h) \
    $(wildcard include/config/feature/individual.h) \
    $(wildcard include/config/o.h) \
    $(wildcard include/config/ntf.h) \
    $(wildcard include/config/t.h) \
    $(wildcard include/config/l.h) \
    $(wildcard include/config/wn.h) \
    $(wildcard include/config/.h) \
    $(wildcard include/config/ktop.h) \
    $(wildcard include/config/route.h) \
    $(wildcard include/config/feature/hwib.h) \
    $(wildcard include/config/debug/crond/option.h) \
    $(wildcard include/config/use/bb/crypt.h) \
    $(wildcard include/config/ioctl/hex2str/error.h) \
    $(wildcard include/config/feature/editing.h) \
    $(wildcard include/config/feature/editing/history.h) \
    $(wildcard include/config/ture/editing/savehistory.h) \
    $(wildcard include/config/feature/editing/savehistory.h) \
    $(wildcard include/config/feature/tab/completion.h) \
    $(wildcard include/config/feature/username/completion.h) \
    $(wildcard include/config/feature/editing/vi.h) \
    $(wildcard include/config/feature/topmem.h) \
    $(wildcard include/config/feature/top/smp/process.h) \
    $(wildcard include/config/pgrep.h) \
    $(wildcard include/config/pkill.h) \
    $(wildcard include/config/pidof.h) \
    $(wildcard include/config/feature/devfs.h) \
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
  /opt/buildroot-gcc463_NPTLSHAREDLIBGCC/usr/mipsel-buildroot-linux-uclibc/sysroot/usr/include/ctype.h \
  /opt/buildroot-gcc463_NPTLSHAREDLIBGCC/usr/mipsel-buildroot-linux-uclibc/sysroot/usr/include/bits/uClibc_touplow.h \
  /opt/buildroot-gcc463_NPTLSHAREDLIBGCC/usr/mipsel-buildroot-linux-uclibc/sysroot/usr/include/dirent.h \
  /opt/buildroot-gcc463_NPTLSHAREDLIBGCC/usr/mipsel-buildroot-linux-uclibc/sysroot/usr/include/bits/dirent.h \
  /opt/buildroot-gcc463_NPTLSHAREDLIBGCC/usr/mipsel-buildroot-linux-uclibc/sysroot/usr/include/errno.h \
  /opt/buildroot-gcc463_NPTLSHAREDLIBGCC/usr/mipsel-buildroot-linux-uclibc/sysroot/usr/include/bits/errno.h \
  /opt/buildroot-gcc463_NPTLSHAREDLIBGCC/usr/mipsel-buildroot-linux-uclibc/sysroot/usr/include/linux/errno.h \
  /opt/buildroot-gcc463_NPTLSHAREDLIBGCC/usr/mipsel-buildroot-linux-uclibc/sysroot/usr/include/asm/errno.h \
  /opt/buildroot-gcc463_NPTLSHAREDLIBGCC/usr/mipsel-buildroot-linux-uclibc/sysroot/usr/include/asm-generic/errno-base.h \
  /opt/buildroot-gcc463_NPTLSHAREDLIBGCC/usr/mipsel-buildroot-linux-uclibc/sysroot/usr/include/sys/syscall.h \
  /opt/buildroot-gcc463_NPTLSHAREDLIBGCC/usr/mipsel-buildroot-linux-uclibc/sysroot/usr/include/bits/sysnum.h \
  /opt/buildroot-gcc463_NPTLSHAREDLIBGCC/usr/mipsel-buildroot-linux-uclibc/sysroot/usr/include/fcntl.h \
  /opt/buildroot-gcc463_NPTLSHAREDLIBGCC/usr/mipsel-buildroot-linux-uclibc/sysroot/usr/include/bits/fcntl.h \
  /opt/buildroot-gcc463_NPTLSHAREDLIBGCC/usr/mipsel-buildroot-linux-uclibc/sysroot/usr/include/sys/stat.h \
  /opt/buildroot-gcc463_NPTLSHAREDLIBGCC/usr/mipsel-buildroot-linux-uclibc/sysroot/usr/include/bits/stat.h \
  /opt/buildroot-gcc463_NPTLSHAREDLIBGCC/usr/mipsel-buildroot-linux-uclibc/sysroot/usr/include/inttypes.h \
  /opt/buildroot-gcc463_NPTLSHAREDLIBGCC/usr/mipsel-buildroot-linux-uclibc/sysroot/usr/include/netdb.h \
    $(wildcard include/config/3/ascii/rules.h) \
  /opt/buildroot-gcc463_NPTLSHAREDLIBGCC/usr/mipsel-buildroot-linux-uclibc/sysroot/usr/include/rpc/netdb.h \
  /opt/buildroot-gcc463_NPTLSHAREDLIBGCC/usr/mipsel-buildroot-linux-uclibc/sysroot/usr/include/bits/siginfo.h \
  /opt/buildroot-gcc463_NPTLSHAREDLIBGCC/usr/mipsel-buildroot-linux-uclibc/sysroot/usr/include/bits/netdb.h \
  /opt/buildroot-gcc463_NPTLSHAREDLIBGCC/usr/mipsel-buildroot-linux-uclibc/sysroot/usr/include/setjmp.h \
  /opt/buildroot-gcc463_NPTLSHAREDLIBGCC/usr/mipsel-buildroot-linux-uclibc/sysroot/usr/include/bits/setjmp.h \
  /opt/buildroot-gcc463_NPTLSHAREDLIBGCC/usr/mipsel-buildroot-linux-uclibc/sysroot/usr/include/signal.h \
  /opt/buildroot-gcc463_NPTLSHAREDLIBGCC/usr/mipsel-buildroot-linux-uclibc/sysroot/usr/include/bits/signum.h \
  /opt/buildroot-gcc463_NPTLSHAREDLIBGCC/usr/mipsel-buildroot-linux-uclibc/sysroot/usr/include/bits/sigaction.h \
  /opt/buildroot-gcc463_NPTLSHAREDLIBGCC/usr/mipsel-buildroot-linux-uclibc/sysroot/usr/include/bits/sigcontext.h \
  /opt/buildroot-gcc463_NPTLSHAREDLIBGCC/usr/mipsel-buildroot-linux-uclibc/sysroot/usr/include/bits/sigstack.h \
  /opt/buildroot-gcc463_NPTLSHAREDLIBGCC/usr/mipsel-buildroot-linux-uclibc/sysroot/usr/include/sys/ucontext.h \
  /opt/buildroot-gcc463_NPTLSHAREDLIBGCC/usr/mipsel-buildroot-linux-uclibc/sysroot/usr/include/bits/sigthread.h \
  /opt/buildroot-gcc463_NPTLSHAREDLIBGCC/usr/mipsel-buildroot-linux-uclibc/sysroot/usr/include/stdio.h \
  /opt/buildroot-gcc463_NPTLSHAREDLIBGCC/usr/mipsel-buildroot-linux-uclibc/sysroot/usr/include/bits/uClibc_stdio.h \
    $(wildcard include/config/io/futexes//.h) \
  /opt/buildroot-gcc463_NPTLSHAREDLIBGCC/usr/mipsel-buildroot-linux-uclibc/sysroot/usr/include/wchar.h \
  /opt/buildroot-gcc463_NPTLSHAREDLIBGCC/usr/mipsel-buildroot-linux-uclibc/sysroot/usr/include/bits/uClibc_mutex.h \
  /opt/buildroot-gcc463_NPTLSHAREDLIBGCC/usr/mipsel-buildroot-linux-uclibc/sysroot/usr/include/pthread.h \
  /opt/buildroot-gcc463_NPTLSHAREDLIBGCC/usr/mipsel-buildroot-linux-uclibc/sysroot/usr/include/sched.h \
  /opt/buildroot-gcc463_NPTLSHAREDLIBGCC/usr/mipsel-buildroot-linux-uclibc/sysroot/usr/include/bits/sched.h \
  /opt/buildroot-gcc463_NPTLSHAREDLIBGCC/usr/mipsel-buildroot-linux-uclibc/sysroot/usr/include/bits/uClibc_clk_tck.h \
  /opt/buildroot-gcc463_NPTLSHAREDLIBGCC/usr/mipsel-buildroot-linux-uclibc/sysroot/usr/include/bits/uClibc_pthread.h \
  /opt/buildroot-gcc463_NPTLSHAREDLIBGCC/usr/lib/gcc/mipsel-buildroot-linux-uclibc/4.6.3/include/stdarg.h \
  /opt/buildroot-gcc463_NPTLSHAREDLIBGCC/usr/mipsel-buildroot-linux-uclibc/sysroot/usr/include/stdlib.h \
  /opt/buildroot-gcc463_NPTLSHAREDLIBGCC/usr/mipsel-buildroot-linux-uclibc/sysroot/usr/include/bits/waitflags.h \
  /opt/buildroot-gcc463_NPTLSHAREDLIBGCC/usr/mipsel-buildroot-linux-uclibc/sysroot/usr/include/bits/waitstatus.h \
  /opt/buildroot-gcc463_NPTLSHAREDLIBGCC/usr/mipsel-buildroot-linux-uclibc/sysroot/usr/include/alloca.h \
  /opt/buildroot-gcc463_NPTLSHAREDLIBGCC/usr/mipsel-buildroot-linux-uclibc/sysroot/usr/include/string.h \
  /opt/buildroot-gcc463_NPTLSHAREDLIBGCC/usr/mipsel-buildroot-linux-uclibc/sysroot/usr/include/sys/poll.h \
  /opt/buildroot-gcc463_NPTLSHAREDLIBGCC/usr/mipsel-buildroot-linux-uclibc/sysroot/usr/include/bits/poll.h \
  /opt/buildroot-gcc463_NPTLSHAREDLIBGCC/usr/mipsel-buildroot-linux-uclibc/sysroot/usr/include/sys/mman.h \
  /opt/buildroot-gcc463_NPTLSHAREDLIBGCC/usr/mipsel-buildroot-linux-uclibc/sysroot/usr/include/bits/mman.h \
  /opt/buildroot-gcc463_NPTLSHAREDLIBGCC/usr/mipsel-buildroot-linux-uclibc/sysroot/usr/include/sys/time.h \
  /opt/buildroot-gcc463_NPTLSHAREDLIBGCC/usr/mipsel-buildroot-linux-uclibc/sysroot/usr/include/sys/wait.h \
  /opt/buildroot-gcc463_NPTLSHAREDLIBGCC/usr/mipsel-buildroot-linux-uclibc/sysroot/usr/include/sys/resource.h \
  /opt/buildroot-gcc463_NPTLSHAREDLIBGCC/usr/mipsel-buildroot-linux-uclibc/sysroot/usr/include/bits/resource.h \
  /opt/buildroot-gcc463_NPTLSHAREDLIBGCC/usr/mipsel-buildroot-linux-uclibc/sysroot/usr/include/termios.h \
  /opt/buildroot-gcc463_NPTLSHAREDLIBGCC/usr/mipsel-buildroot-linux-uclibc/sysroot/usr/include/bits/termios.h \
  /opt/buildroot-gcc463_NPTLSHAREDLIBGCC/usr/mipsel-buildroot-linux-uclibc/sysroot/usr/include/unistd.h \
  /opt/buildroot-gcc463_NPTLSHAREDLIBGCC/usr/mipsel-buildroot-linux-uclibc/sysroot/usr/include/bits/posix_opt.h \
  /opt/buildroot-gcc463_NPTLSHAREDLIBGCC/usr/mipsel-buildroot-linux-uclibc/sysroot/usr/include/bits/environments.h \
  /opt/buildroot-gcc463_NPTLSHAREDLIBGCC/usr/mipsel-buildroot-linux-uclibc/sysroot/usr/include/bits/confname.h \
  /opt/buildroot-gcc463_NPTLSHAREDLIBGCC/usr/mipsel-buildroot-linux-uclibc/sysroot/usr/include/bits/getopt.h \
  /opt/buildroot-gcc463_NPTLSHAREDLIBGCC/usr/mipsel-buildroot-linux-uclibc/sysroot/usr/include/utime.h \
  /opt/buildroot-gcc463_NPTLSHAREDLIBGCC/usr/mipsel-buildroot-linux-uclibc/sysroot/usr/include/sys/param.h \
  /opt/buildroot-gcc463_NPTLSHAREDLIBGCC/usr/mipsel-buildroot-linux-uclibc/sysroot/usr/include/linux/param.h \
  /opt/buildroot-gcc463_NPTLSHAREDLIBGCC/usr/mipsel-buildroot-linux-uclibc/sysroot/usr/include/asm/param.h \
  /opt/buildroot-gcc463_NPTLSHAREDLIBGCC/usr/mipsel-buildroot-linux-uclibc/sysroot/usr/include/asm-generic/param.h \
  /opt/buildroot-gcc463_NPTLSHAREDLIBGCC/usr/mipsel-buildroot-linux-uclibc/sysroot/usr/include/mntent.h \
  /opt/buildroot-gcc463_NPTLSHAREDLIBGCC/usr/mipsel-buildroot-linux-uclibc/sysroot/usr/include/paths.h \
  /opt/buildroot-gcc463_NPTLSHAREDLIBGCC/usr/mipsel-buildroot-linux-uclibc/sysroot/usr/include/sys/statfs.h \
  /opt/buildroot-gcc463_NPTLSHAREDLIBGCC/usr/mipsel-buildroot-linux-uclibc/sysroot/usr/include/bits/statfs.h \
  /opt/buildroot-gcc463_NPTLSHAREDLIBGCC/usr/mipsel-buildroot-linux-uclibc/sysroot/usr/include/pwd.h \
  /opt/buildroot-gcc463_NPTLSHAREDLIBGCC/usr/mipsel-buildroot-linux-uclibc/sysroot/usr/include/grp.h \
  include/xatonum.h \
  libbb/xatonum_template.c \

libbb/xatonum.o: $(deps_libbb/xatonum.o)

$(deps_libbb/xatonum.o):
