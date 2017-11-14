#include ../../Rules.mak

CURR_DIR := $(shell pwd)
BUILD_DIR:=$(subst /user_space, /user_space,$(CURR_DIR))
BUILD_DIR:=$(word 1, $(BUILD_DIR))

include $(BUILD_DIR)/Rules.mak


SUBDIRS := src
SUBDIRS += daemon

all: $(SUBDIRS)
	@for i in ${SUBDIRS} ;	do make -C $$i || exit 1 ; done

install:
	@for i in ${SUBDIRS} ;	do make -C $$i install || exit 1 ; done 

clean:
	for i in ${SUBDIRS} ;	do make -C $$i clean || exit 1 ; done
