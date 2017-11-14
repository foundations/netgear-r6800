#include ../../../Rules.mak

CURR_DIR := $(shell pwd)
BUILD_DIR:=$(subst /user_space, /user_space,$(CURR_DIR))
BUILD_DIR:=$(word 1, $(BUILD_DIR))

include $(BUILD_DIR)/Rules.mak


CC = $(CROSS)gcc
STRIP=$(CROSS)strip
CFLAGS += -Os -Wall -s 
BIN = wol_daemon

CFLAGS += -I$(ROOT)/shared
CFLAGS += -I../../nvram -L../../nvram -lscnvram
#CFLAGS += -D_SC_DEBUG_ENABLE_
# Define _ROUTER_DEBUG_MODE_ to print all of the syslog message to log.htm
#CFLAGS += -D_ROUTER_DEBUG_MODE_
SRC:=wol_daemon.c

all: $(BIN)

.PHONY: all

wol_daemon:wol_daemon.c
	$(CC) $(CFLAGS) -o $@ $<
	$(STRIP) $@

install:
	cp -a $(BIN) $(TARGET_DIR)/sbin
clean:
	rm -f $(BIN)
