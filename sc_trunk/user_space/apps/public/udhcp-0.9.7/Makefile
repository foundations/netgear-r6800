# udhcp makefile

prefix=$(TI_FILESYSTEM)
SBINDIR=/sbin
USRSBINDIR=${prefix}/sbin
USRBINDIR=${prefix}/bin
USRSHAREDIR=${prefix}/share

# Uncomment this to get a shared binary. Call as udhcpd for the server,
# and udhcpc for the client
COMBINED_BINARY=1

# Uncomment this for extra output and to compile with debugging symbols
DEBUG=1

# Uncomment this to output messages to syslog, otherwise, messages go to stdout
CFLAGS += -s -Wall
#CFLAGS += -Os -s -Wall
CFLAGS += -Werror

CURR_DIR := $(shell pwd)
BUILD_DIR:=$(subst /user_space, /user_space,$(CURR_DIR))
BUILD_DIR:=$(word 1, $(BUILD_DIR))

include $(BUILD_DIR)/Rules.mak

#include ../../Rules.mak
#CROSS_COMPILE=mips-uclibc-
CROSS_COMPILE=$(CROSS)
CC = $(CROSS_COMPILE)gcc
LD = $(CROSS_COMPILE)gcc
INSTALL = install

VER := 0.9.7

OBJS_SHARED = options.o socket.o packet.o pidfile.o
DHCPD_OBJS = dhcpd.o arpping.o files.o leases.o serverpacket.o
DHCPC_OBJS = dhcpc.o clientpacket.o script.o
SHARED_LIBS = -L$(APPS_DIR)/public/nvram/ -lscnvram

ifdef COMBINED_BINARY
EXEC1 = udhcpd
OBJS1 = $(DHCPD_OBJS) $(DHCPC_OBJS) $(OBJS_SHARED) frontend.o
CFLAGS += -DCOMBINED_BINARY
else
EXEC1 = udhcpd
OBJS1 = $(DHCPD_OBJS) $(OBJS_SHARED)

EXEC2 = udhcpc
OBJS2 = $(DHCPC_OBJS) $(OBJS_SHARED)
endif

EXEC3 = dumpleases
OBJS3 = dumpleases.o

BOOT_PROGRAMS = udhcpc
DAEMONS = udhcpd
COMMANDS = dumpleases

ifdef SYSLOG
CFLAGS += -DSYSLOG
endif

ifeq ($(TR069),1)
CFLAGS+= -DTR069
endif

ifeq ($(IPV6),1)
CFLAGS += -DIPV6
endif

CFLAGS += -W -Wall -Wstrict-prototypes -DVERSION='"$(VER)"'
ifdef DEBUG
#CFLAGS += -g -DDEBUG
CFLAGS += -D_SC_DEBUG_ENABLE_
else
CFLAGS += -fomit-frame-pointer
#CFLAGS += -Os -fomit-frame-pointer
STRIP_FLAG=-s
endif

CFLAGS +=-DRONSCODE
CFLAGS +=-I$(SHARED_DIR)
CFLAGS +=-I$(APPS_DIR)/public/nvram/
CFLAGS +=-DCHECK_DOMAIN

ifeq ($(DHCP_OPTION),1)
CFLAGS += -DDHCP_OPTION
endif

all: $(EXEC1) $(EXEC2) $(EXEC3)

$(OBJS1) $(OBJS2) $(OBJS3): *.h Makefile
$(EXEC1) $(EXEC2) $(EXEC3): Makefile

.c.o:
	$(CC) -c $(CFLAGS) $<

$(EXEC1): $(OBJS1)
	$(LD) $(STRIP_FLAG) $(OBJS1) $(SHARED_LIBS) -o $(EXEC1)

$(EXEC2): $(OBJS2)
	$(LD) $(STRIP_FLAG) $(OBJS2) -o $(EXEC2)

$(EXEC3): $(OBJS3)
	$(LD) $(STRIP_FLAG) $(OBJS3) -o $(EXEC3)

install: 
	rm -f udhcpc
	ln -s udhcpd udhcpc
	cp -a udhcpd $(BINDIR) 
	cp -a udhcpc $(BINDIR)	
	cp -a dumpleases $(BINDIR)
clean:
	rm -f udhcpd udhcpc dumpleases *.o core


