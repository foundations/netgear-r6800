# Makefile for hdparm


CURR_DIR := $(shell pwd)
BUILD_DIR:=$(subst /user_space, /user_space,$(CURR_DIR))
BUILD_DIR:=$(word 1, $(BUILD_DIR))

include $(BUILD_DIR)/Rules.mak

#include ../../Rules.mak

# DESTDIR is for non root installs (eg packages, NFS) only!
DESTDIR = $(BINDIR)


CC = $(CROSS)gcc
STRIP = $(CROSS)strip

CFLAGS := -O2 -W -Wall -Wbad-function-cast -Wcast-align -Wpointer-arith -Wcast-qual -Wshadow -Wstrict-prototypes -Wmissing-prototypes -Wmissing-declarations -fkeep-inline-functions -Wwrite-strings -Waggregate-return -Wnested-externs -Wtrigraphs $(CFLAGS)

LDFLAGS = -s
#LDFLAGS = -s -static
INSTALL = install
INSTALL_DATA = $(INSTALL) -m 644
INSTALL_DIR = $(INSTALL) -m 755 -d
INSTALL_PROGRAM = $(INSTALL)

OBJS = hdparm.o identify.o sgio.o sysfs.o geom.o fallocate.o fibmap.o fwdownload.o dvdspeed.o wdidle3.o

all: hdparm

hdparm: hdparm.h sgio.h $(OBJS)
	$(CC) $(LDFLAGS) -o hdparm $(OBJS)
	$(STRIP) hdparm

hdparm.o:	hdparm.h sgio.h

identify.o:	hdparm.h

dvdspeed.o:     dvdspeed.c

sgio.o: sgio.c sgio.h hdparm.h

fwdownload.o: fwdownload.c sgio.h hdparm.h

install: all hdparm.8
	$(STRIP) hdparm
	cp -a  hdparm $(DESTDIR)

clean:
	-rm -f hdparm $(OBJS) core 2>/dev/null

