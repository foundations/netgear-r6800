# Path to parent kernel include files directory

#KERNEL_INCLUDE=$(PROJ_DIR_KERNEL_SRC)/include
#KERNEL_INCLUDE = ../../kernel/src/include

#LIBC_INCLUDE=../../$(PROJ_DIR_TOOL_CHAIN_INCLUDE)

#DESTDIR=
#SBINDIR=/usr/sbin
#CONFDIR=/etc/iproute2
#DOCDIR=/usr/share/doc/iproute2
#MANDIR=/usr/share/man

# Path to db_185.h include
#DBM_INCLUDE:=/usr/include

DEFINES= -DRESOLVE_HOSTNAMES

#options if you have a bind>=4.9.4 libresolv (or, maybe, glibc)
LDLIBS=-lresolv
ADDLIB=

#options for decnet
ADDLIB+=dnet_ntop.o dnet_pton.o

#options for ipx
ADDLIB+=ipx_ntop.o ipx_pton.o

#CC = gcc
#HOSTCC = gcc

CURR_DIR := $(shell pwd)
BUILD_DIR:=$(subst /user_space, /user_space,$(CURR_DIR))
BUILD_DIR:=$(word 1, $(BUILD_DIR))

include $(BUILD_DIR)/Rules.mak
#include ../../Rules.mak
CC = $(CROSS)gcc
#CC = mips-uclibc-gcc
CCOPTS = -D_GNU_SOURCE -Wstrict-prototypes -Wall  -D_SC_BUILD_
#CCOPTS = -D_GNU_SOURCE -O2 -Wstrict-prototypes -Wall -Werror
CFLAGS = $(CCOPTS) -I../include $(DEFINES)

KERNEL_INCLUDE = $(KERNEL_INC)
#LIBC_INCLUDE= $(ROOT)tools/mips-linux-uclibc/include/

YACCFLAGS = -d -t -v

LDLIBS += -L../lib -lnetlink -lutil

#SUBDIRS=lib ip tc misc netem genl
SUBDIRS=lib ip tc 

LIBNETLINK=../lib/libnetlink.a ../lib/libutil.a

#all: # Config
all: 	
#	@echo "--------------------------------------------"
#	@echo "   make all"
#	@echo "--------------------------------------------"	
	@set -e; \
	for i in $(SUBDIRS); \
	do $(MAKE) -C $$i; done 
#	do \
#	echo "--------------------------------------------" ;\
#	echo "   make subdir- $$i" ;\
#	echo "--------------------------------------------"	; \
#	$(MAKE) -C $$i; done

Config:
	sh configure $(KERNEL_INCLUDE)



install:
	$(STRIP) tc/tc
	cp tc/tc $(BINDIR)
	$(STRIP) ip/ip
	cp ip/ip $(BINDIR)
	if [ -e $(TARGET_DIR)/usr/etc/iproute2 ]; then \
	rm -rf $(TARGET_DIR)/usr/etc/iproute2;\
	fi
	cp -rf etc/iproute2  $(TARGET_DIR)/usr/etc/iproute2
	



XXXXXinstall: all   #original
	install -m 0755 -d $(DESTDIR)$(SBINDIR)
	install -m 0755 -d $(DESTDIR)$(CONFDIR)
	install -m 0755 -d $(DESTDIR)$(DOCDIR)/examples
	install -m 0755 -d $(DESTDIR)$(DOCDIR)/examples/diffserv
	install -m 0644 README.iproute2+tc $(shell find examples -maxdepth 1 -type f) \
		$(DESTDIR)$(DOCDIR)/examples
	install -m 0644 $(shell find examples/diffserv -maxdepth 1 -type f) \
		$(DESTDIR)$(DOCDIR)/examples/diffserv
	@for i in $(SUBDIRS) doc; do $(MAKE) -C $$i install; done
	install -m 0644 $(shell find etc/iproute2 -maxdepth 1 -type f) $(DESTDIR)$(CONFDIR)
	install -m 0755 -d $(DESTDIR)$(MANDIR)/man8
	install -m 0644 $(shell find man/man8 -maxdepth 1 -type f) $(DESTDIR)$(MANDIR)/man8
	ln -sf tc-pbfifo.8  $(DESTDIR)$(MANDIR)/man8/tc-bfifo.8
	ln -sf tc-pbfifo.8  $(DESTDIR)$(MANDIR)/man8/tc-pfifo.8
	install -m 0755 -d $(DESTDIR)$(MANDIR)/man3
	install -m 0644 $(shell find man/man3 -maxdepth 1 -type f) $(DESTDIR)$(MANDIR)/man3

clean:
	rm -f cscope.*
	@for i in $(SUBDIRS) doc; \
	do $(MAKE) $(MFLAGS) -C $$i clean; done

clobber: clean
	rm -f Config

distclean: clobber

cscope:
	cscope -b -q -R -Iinclude -sip -slib -smisc -snetem -stc

.EXPORT_ALL_VARIABLES:
