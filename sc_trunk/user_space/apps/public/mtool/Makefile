#
#       Makefile for Mtools
#
# check the Configure file for some examples of device-specific setups
# Berkeley flavors of Unix should include -DBSD in the CFLAGS.  Pick
# a lock method... either -DLOCKF, -DFLOCK, or -DFCNTL and put that
# string in the CFLAGS line below.

CURR_DIR := $(shell pwd)
BUILD_DIR:=$(subst /user_space, /user_space,$(CURR_DIR))
BUILD_DIR:=$(word 1, $(BUILD_DIR))

include $(BUILD_DIR)/Rules.mak

#include ../../Rules.mak

# User specified flags
USERCFLAGS = 
USERLDFLAGS =
USERLDLIBS =

MAKEINFO = makeinfo
TEXI2DVI = texi2dvi
TEXI2PDF = texi2pdf
TEXI2HTML = texi2html

DESTBIN = $(FS_DIR)/usr/sbin

BIN = mlabel

# do not edit below this line
# =============================================================================

SHELL = /bin/sh

top_srcdir=.
srcdir=.


prefix      = /usr
exec_prefix = ${prefix}
bindir      = ${exec_prefix}/sbin
infodir     = ${prefix}/info
mandir      = ${prefix}/man
sysconfdir  = ${prefix}/etc

#CROSS = mips-uclibc-
#CROSS = ../../tools/mipsel-linux-uclibc/bin/mips-uclibc-
CC = $(CROSS)gcc
LD = $(CROSS)ld

#CC         = gcc
CXX        = @CXX@
#MYCFLAGS   = -Os -Wall -s -Wstrict-prototypes -ansi -pedantic -Ilibevent
MYCFLAGS   = -Os -Wall -s
MYCXXFLAGS = @CXXFLAGS@
CPPFLAGS   = 
HOST_ID    = -DCPU_i686 -DVENDOR_pc -DOS_linux_gnu 
DEFS       = -DHAVE_CONFIG_H -DSYSCONFDIR=\"$(sysconfdir)\" $(HOST_ID)

LDFLAGS     = 
LIBS        = -lnsl 
SHLIB       = 
MACHDEPLIBS = 	
LN_S        = ln -s

INSTALL         = /usr/bin/install -c
INSTALL_PROGRAM = ${INSTALL}
INSTALL_DATA    = ${INSTALL} -m 644
INSTALL_INFO	= /sbin/install-info

.SUFFIXES:
.SUFFIXES: .o .c
.SUFFIXES: .o .c

MAN1 = floppyd.1 floppyd_installtest.1 mattrib.1 mbadblocks.1 mcat.1 mcd.1 \
mclasserase.1 mcopy.1 mdel.1 mdeltree.1 mdir.1 mdu.1 mformat.1  minfo.1 \
mkmanifest.1 mlabel.1 mmd.1 mmount.1 mmove.1 mpartition.1 \
mrd.1 mren.1 mshowfat.1 mtoolstest.1 mtools.1 mtype.1 mzip.1
MAN1EXT	= 1
MAN1DIR	= $(mandir)/man${MAN1EXT}
MAN5	= mtools.5
MAN5EXT	= 5
MAN5DIR	= $(mandir)/man${MAN5EXT}

OBJS1 = buffer.o codepage.o codepages.o config.o copyfile.o devices.o \
dirCache.o directory.o direntry.o expand.o fat.o fat_free.o file.o  \
file_name.o filter.o floppyd_io.o force_io.o hash.o init.o llong.o match.o \
mainloop.o mattrib.o mbadblocks.o mcat.o mcd.o mclasserase.o mcopy.o mdel.o \
mdir.o mdoctorfat.o mdu.o \
mformat.o minfo.o misc.o missFuncs.o mk_direntry.o mlabel.o mmd.o mmount.o \
mmove.o mpartition.o mshowfat.o mzip.o mtools.o patchlevel.o plain_io.o \
precmd.o privileges.o scsi.o signal.o stream.o streamcache.o subdir.o \
unixdir.o toupper.o tty.o vfat.o xdf_io.o

#OBJS2 = missFuncs.o mkmanifest.o misc.o patchlevel.o

#SRCS3 = floppyd.c

#OBJS4 = floppyd_installtest.o misc.o expand.o privileges.o

SRCS = buffer.c codepage.c codepages.c config.c copyfile.c devices.c \
dirCache.c directory.c direntry.c expand.c fat.c fat_free.c file.c file_name.c \
file_read.c filter.c floppyd_io.c force_io.c hash.c init.c match.c mainloop.c \
mattrib.c mbadblocks.c mcat.c mcd.c mclasserase.c mcopy.c mdel.c mdir.c \
mdu.c mdoctorfat.c mformat.c minfo.c misc.c \
missFuncs.c mk_direntry.c mlabel.c mmd.c mmount.c mmove.c mpartition.c \
mshowfat.c mzip.c mtools.c plain_io.c precmd.c privileges.c scsi.o \
signal.c stream.c streamcache.c subdir.c unixdir.c toupper.c tty.o vfat.c \
xdf_io.c mkmanifest.c

SRCS = mlabel.c

OBJS = direntry.o mainloop.o mk_direntry.o mlabel.o streamcache.o vfat.o


CFLAGS = $(CPPFLAGS) $(DEFS) $(MYCFLAGS) -fno-strict-aliasing -I.  -I. $(USERCFLAGS) 
CXXFLAGS  = $(CPPFLAGS) $(DEFS) $(MYCXXFLAGS) -I.  -I. $(USERCFLAGS) 
LINK      = $(CC) $(LDFLAGS) $(USERLDFLAGS) 
ALLLIBS   = $(USERLDLIBS) $(MACHDEPLIBS) $(SHLIB) $(LIBS)

all:    $(BIN)

%.o: %.c
	$(CC) $(CFLAGS) -c $<


$(BIN): $(OBJS1)

	$(LINK) $(OBJS1) -o $@ $(ALLLIBS)

install:
	cp -a $(BIN) $(BINDIR)
#	cp -a mtools.conf ../../target/usr/etc

clean:
	rm *.o mlabel;find . -type l -exec rm -f {} \;
