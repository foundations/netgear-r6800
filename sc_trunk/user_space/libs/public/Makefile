

CURR_DIR := $(shell pwd)
BUILD_DIR:=$(subst /user_space, /user_space,$(CURR_DIR))
BUILD_DIR:=$(word 1, $(BUILD_DIR))

include $(BUILD_DIR)/Rules.mak


GPL             :=
SUBDIRS         :=

##------------------------------------
ifeq ($(BASIC),1)
#  build nvram first before libs build 
GPL 		+= cyassl-2.2.0
GPL 		+= $(OPENSSL_USE)
##Build tr_lib's curl for NETGEAR DDNS also for itunes server#
GPL         	+= tr_lib
GPL             += libixml

endif

ifeq ($(USB),1)
GPL += libiconv.1.8
GPL += fuse.2.7.4
endif

ifeq ($(HIDDEN_PASSWORD),1)
GPL             += base64
GPL             += 3des
endif

ifeq ($(IPV6),1)
GPL += flex-2.5.4
endif

ifeq ($(OPENVPN),1)
GPL += lzo
endif

#------------------------------------
SUBDIRS += $(APPS_DIR)/public/nvram sc_libs/liblink/ sc_libs/libflash/
SUBDIRS += $(GPL)

$(SUBDIRS)::
	@for i in $(SUBDIRS) ; do echo -e "\\e[35mlibs: make \\e[36m$$i \\e[0m"; $(MAKE) ARCH=$(ARCH) CROSS_COMPILE=$(CROSS_DIR)$(CROSS_COMPILE)  -C $$i || exit 1; done


all:    $(SUBDIRS)

clean:
	@for i in $(SUBDIRS) ; do echo -e "\\e[35mlibs: clean \\e[36m$$i \\e[0m"; $(MAKE) -C $$i clean  || exit 1; done

install:
	@for i in $(SUBDIRS) ; do echo -e "\\e[35mlibs: install \\e[36m$$i \\e[0m"; $(MAKE) -C $$i install  || exit 1; done

