CURR_DIR := $(shell pwd)
BUILD_DIR:=$(subst /user_space, /user_space,$(CURR_DIR))
BUILD_DIR:=$(word 1, $(BUILD_DIR))

include $(BUILD_DIR)/Rules.mak
#include ../../../../Rules.mak

AR=$(CROSS)ar
CC=$(CROSS)gcc
LD=$(CROSS)ld

all: lib_napt.so

OBJS := common.o
OBJS += cpm.o
OBJS += cpt.o
OBJS += cdmz.o
OBJS += cnapt.o

CFLAGS += -Wall -Werror -I$(KERNEL_MODULE_DIR)/public/cnapt_multi/$(CNAPT_USE)/ -I$(KERNEL_DIR)/include -fPIC

lib_napt.so: $(OBJS)
	$(CC) -Os -s -shared -Wl,-soname,lib_napt.so -o $@ $(LIBS) $^

%.o:%.c
	$(CC) $(CFLAGS) -c $^

install:
	cp -a lib_napt.so $(ROOT)/target/lib/

clean:
	rm -f *.a *.o *.so
