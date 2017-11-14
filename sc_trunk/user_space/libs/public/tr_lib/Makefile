#Makefile for librarys

#-----------------------------------------------------

CURR_DIR := $(shell pwd)
BUILD_DIR:=$(subst /user_space, /user_space,$(CURR_DIR))
BUILD_DIR:=$(word 1, $(BUILD_DIR))

include $(BUILD_DIR)/Rules.mak


#include ../../Rules.mak
#-----------------------------------------------------



#-------------------------------------------------------#
#                       SUBLIBS                         #
#-------------------------------------------------------#
SUBLIBS  =
#NETGEAR DDNS need curl
SUBLIBS	 +=  curl
#fbwifi need curl
SUBLIBS	 +=  zlib
ifeq ($(TRANSMISSION), 1)
#SUBLIBS	 +=  zlib
SUBLIBS  +=  libevent
endif
#-------------------------------------------------------#
#                       TARGETS                         #
#-------------------------------------------------------#

all: libs
	@echo All LIBS are OK!


libs : ${SUBLIBS}
	@for i in ${SUBLIBS} ;  do                                                    \
            echo "-----------------------------------------------------------------"; \
            echo -e "\033[;34m                         $$i                 \033[;0m"; \
            echo "-----------------------------------------------------------------"; \
            make -C $$i || exit 1 ;                                         \
        done

install:
	@for i in ${SUBLIBS} ;  do                                                    \
            echo "-----------------------------------------------------------------"; \
            echo -e "\033[;35m                         $$i                 \033[;0m"; \
            echo "-----------------------------------------------------------------"; \
            make -C $$i install || exit 1 ;                                   \
        done

clean:
	for i in ${SUBLIBS} ; do make -C $$i clean || exit 1 ; done
	cd include && rm -rf *
	cd lib && rm -rf *
