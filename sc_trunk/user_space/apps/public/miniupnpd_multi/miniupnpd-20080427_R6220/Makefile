# $Id: Makefile.linux,v 1.38 2008/03/09 11:32:45 nanard Exp $
# MiniUPnP project
# http://miniupnp.free.fr/
# Author : Thomas Bernard
# for use with GNU Make
# To install use :
# $ PREFIX=/dummyinstalldir make -f Makefile.linux install
# or :
# $ INSTALLPREFIX=/usr/local make -f Makefile.linux install
# or :
# $ make -f Makefile.linux install
#
#CFLAGS = -Wall -O -D_GNU_SOURCE -g -DDEBUG

CURR_DIR := $(shell pwd)
BUILD_DIR:=$(subst /user_space, /user_space,$(CURR_DIR))
BUILD_DIR:=$(word 1, $(BUILD_DIR))

include $(BUILD_DIR)/Rules.mak

#include ../../Rules.mak

IPTABLE_DIR =$(APPS_DIR)/public/iptables/$(IPTABLES_USE)/

PWD=$(shell pwd)

CFLAGS = -Wall -Os -MD -D_GNU_SOURCE
CFLAGS += -I$(IPTABLE_DIR)include -L$(IPTABLE_DIR)libiptc
CFLAGS += -Werror
#CFLAGS += -DUPNP_DEBUG
CFLAGS += -I../../nvram -I$(SC_LIBS_DIR)/include -L../../nvram -L$(SC_LIBS_DIR)/libflash -L$(SC_LIBS_DIR)/libscmisc
CFLAGS += -I$(SC_LIBS_DIR)/include
CFLAGS += -I$(SHARED_DIR)
ifneq (,$(findstring sdk5,$(NEEDED_SDKPATH)))
CFLAGS += -I$(PWD) -I$(PWD)/wsc
else
CFLAGS += -I$(KERNEL_INC) -I$(PWD) -I$(PWD)/wsc
endif
#CFLAGS += -DBOARD_ID=\"$(BOARD_ID)\"

#LIBS = -liptc
LIBS = $(IPTABLE_DIR)/libiptc/.libs/libip4tc.a
LIBS += ../../nvram/libscnvram.so
LIBS += $(SC_LIBS_DIR)/liblink/liblink.so
LIBS += $(SC_LIBS_DIR)/libflash/libflash.so
LIBS += $(SC_LIBS_DIR)/libscm_wl/libscm_wl.so
LIBS += $(SC_LIBS_DIR)/libscmisc/libscmisc.so
LIBS += $(SC_LIBS_DIR)/libupgrade/libupgrade.a

#ifeq ($(PROJECT),DG834NUv2)
#CFLAGS += -DMODULE_NAME='"DG834Nv2"'
#else
#ifeq ($(PROJECT),DG834NUBv2)
#CFLAGS += -DMODULE_NAME='"DG834NBv2"'
#else
#CFLAGS += -DMODULE_NAME='"$(PROJECT)"'
#endif
#endif
CFLAGS += -DVER='"$(VER)"'

ifeq ($(CONENAT),1)
CFLAGS += -DCONENAT
CFLAGS += -I../../cnapt_multi/$(CNAPT_USE)/user/lib/
LIBS += ../../cnapt_multi/$(CNAPT_USE)/user/lib/lib_napt.so
endif

ifeq ($(CHIP_VENDOR),BCM)
CFLAGS += -DBCM_BSP
else
ifeq ($(CHIP_VENDOR),RTL)
CFLAGS += -DRTL_BSP
else
ifeq ($(CHIP_VENDOR),MTK)
CFLAGS += -DMTK_BSP
endif
endif
endif

ifeq ($(CUSTOMER_NAME),Amped)
CFLAGS += -DAMPED
endif

ifeq ($(PNPX),1)
CFLAGS += -DENABLE_PNPX
endif

ifeq ($(ANNEX),B)
CFLAGS+= -DANNEXB
endif

CC=$(CROSS)gcc
STRIP=$(CROSS)strip
RM = rm -f
INSTALL = install

INSTALLPREFIX ?= $(PREFIX)/usr
SBININSTALLDIR = $(INSTALLPREFIX)/sbin
ETCINSTALLDIR = $(PREFIX)/etc/miniupnpd

BASEOBJS = upnpgetnvram.o upnpaction.o
BASEOBJS += miniupnpd.o upnphttp.o upnpdescgen.o upnpsoap.o \
           	upnpreplyparse.o minixml.o \
		   	upnpredirect.o getifaddr.o daemonize.o upnpglobalvars.o \
           	options.o upnppermissions.o minissdp.o natpmp.o \
           	upnpevents.o
LNXOBJS = linux/getifstats.o
#WSCOBJS = wsc/wsc_main.o wsc/wsc_common.o wsc/wsc_ioctl.o wsc/wsc_netlink.o wsc/wsc_upnp_device.o wsc/wsc_msg.o
NETFILTEROBJS = netfilter/iptcrdr.o

ALLOBJS = $(BASEOBJS) $(LNXOBJS) $(NETFILTEROBJS) $(WSCOBJS)

TESTUPNPDESCGENOBJS = testupnpdescgen.o upnpdescgen.o upnpaction.o

EXECUTABLES = miniupnpd testupnpdescgen testgetifstats \
              testupnppermissions miniupnpdctl

.PHONY:	all clean install depend genuuid

all:	$(EXECUTABLES)

clean:
	$(RM) $(ALLOBJS)
	$(RM) $(EXECUTABLES)
	$(RM) testupnpdescgen.o testgetifstats.o
	$(RM) testupnppermissions.o
	$(RM) miniupnpdctl.o
	find . -name "*.[oad]" | xargs $(RM)

install: miniupnpd
	$(STRIP) -s miniupnpd
	cp -f miniupnpd $(BINDIR)

#install:	miniupnpd genuuid
#	$(INSTALL) -d $(SBININSTALLDIR)
#	$(INSTALL) miniupnpd $(SBININSTALLDIR)
#	$(INSTALL) -d $(ETCINSTALLDIR)
#	$(INSTALL) netfilter/iptables_init.sh $(ETCINSTALLDIR)
#	$(INSTALL) netfilter/iptables_removeall.sh $(ETCINSTALLDIR)
#	$(INSTALL) --mode=0644 miniupnpd.conf $(ETCINSTALLDIR)
#	$(INSTALL) -d $(PREFIX)/etc/init.d
#	$(INSTALL) linux/miniupnpd.init.d.script $(PREFIX)/etc/init.d/miniupnpd

# genuuid is using the uuidgen CLI tool which is part of libuuid
# from the e2fsprogs
genuuid:
	sed -i -e "s/^uuid=[-0-9a-f]*/uuid=`(genuuid||uuidgen) 2>/dev/null`/" miniupnpd.conf

miniupnpd:	$(BASEOBJS) $(LNXOBJS) $(NETFILTEROBJS) $(LIBS) $(WSCOBJS)

testupnpdescgen:	$(TESTUPNPDESCGENOBJS) $(LIBS)

testgetifstats:	testgetifstats.o linux/getifstats.o

testupnppermissions:	testupnppermissions.o upnppermissions.o

miniupnpdctl:	miniupnpdctl.o


# abc.d <=> abc.o: abc.c abcd.c. run by obscure rule.
-include $(ALLOBJS:.o=.d)
