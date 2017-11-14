#
# Simple build file for now
#
# License:  GNU General Public License, Version 2.
#


CURR_DIR := $(shell pwd)
BUILD_DIR:=$(subst /user_space, /user_space,$(CURR_DIR))
BUILD_DIR:=$(word 1, $(BUILD_DIR))

include $(BUILD_DIR)/Rules.mak

#include ../../Rules.mak
CC=$(CROSS)gcc
STRIP=$(CROSS)strip

all: reaim

ifdef NetBSD
FW_FLAGS="-DUSE_IPF"
endif

reaim: reaim.c
	$(CC) -o reaim reaim.c -Os -s -Wall $(FW_FLAGS)
	$(STRIP) reaim

clean:
	-rm -f reaim

install: reaim

	cp reaim $(BINDIR)
