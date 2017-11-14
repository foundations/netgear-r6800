#include ../../Rules.mak

CURR_DIR := $(shell pwd)
BUILD_DIR:=$(subst /user_space, /user_space,$(CURR_DIR))
BUILD_DIR:=$(word 1, $(BUILD_DIR))

include $(BUILD_DIR)/Rules.mak


CC=$(CROSS)gcc
CFLAGS=-Wall -Werror -I$(SHARED_DIR)

all: bwping_ttl

bwping_ttl: bwping_ttl.o
	$(CC) $(CFLAGS) -o $@ $^	

clean:
	rm -rf bwping_ttl *.o
	rm -f $(BINDIR)/bwping_ttl 

install:
	cp -a bwping_ttl $(BINDIR)
