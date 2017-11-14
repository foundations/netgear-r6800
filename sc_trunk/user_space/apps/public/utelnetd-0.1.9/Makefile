# Makefile for utelnetd
# 
# Configure this with the following environment variables:
#

CURR_DIR := $(shell pwd)
BUILD_DIR:=$(subst /user_space, /user_space,$(CURR_DIR))
BUILD_DIR:=$(word 1, $(BUILD_DIR))

include $(BUILD_DIR)/Rules.mak


#include ../../Rules.mak
# where to install
INSTDIR		:= $(prefix)/bin/

# GNU target string 
# CROSS		:=

# where to find login program
ifneq ("", "$(BSD)")
LOGIN		:= /usr/bin/login
else
LOGIN		:= /bin/login
endif

ifneq ("", "$(BSD)")
CORE		:= utelnetd.core
else
CORE		:= core
endif

# nothing to configure below this line... ---8<---8<---8<---

PROGS     = utelnetd

INSTMODE  = 0755
INSTOWNER = root
INSTGROUP = root

CC = $(CROSS)gcc
OBJS = utelnetd.o

INSTALL   = install

CFLAGS	 += -I. -pipe -DSHELLPATH=\"$(LOGIN)\" -Wall -Werror -D_GNU_SOURCE

ifneq ("","$(DEBUG)")
CFLAGS   += -DDEBUG -g -Os
STRIP	  = \#
else
CFLAGS	 += -fomit-frame-pointer
STRIP	  = $(CROSS)strip 
endif

ifeq ("1", "$(BSD)")
CFLAGS   += -DBSD
endif


all: $(PROGS)

$(PROGS): $(OBJS)
	$(CC) $(LDFLAGS) $^ $(LDLIBS) -o $@
	$(STRIP) --remove-section=.comment --remove-section=.note $@

.PHONY: install
install: $(PROGS)
	cp utelnetd $(BINDIR)

.PHONY: clean
clean:
	rm -f $(PROGS) *.o $(CORE)
