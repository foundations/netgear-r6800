
CURR_DIR := $(shell pwd)
BUILD_DIR:=$(subst /kernel_module, /kernel_module,$(CURR_DIR))
BUILD_DIR:=$(word 1, $(BUILD_DIR))

include $(BUILD_DIR)/Rules.mak


GPL             :=
PRIVATE         :=
SUBDIRS         :=

##------------------------------------
ifeq ($(BASIC),1)
GPL             += fw_modules
##repeater don't need spi firewall
ifeq ($(WIFI_REPEATER_APCLI), 0)
GPL         	+= spi_firewall_modules
endif
endif

#------------------------------------
SUBDIRS += kernel_upgrade
SUBDIRS += $(GPL)

$(SUBDIRS)::
	@for i in $(SUBDIRS) ; do echo -e "\\e[35mkernel_module: make \\e[36m$$i \\e[0m"; $(MAKE) ARCH=$(ARCH) CROSS_COMPILE=$(CROSS_DIR)$(CROSS_COMPILE)  -C $$i || exit 1; done


all:    $(SUBDIRS)

clean:
	@for i in $(SUBDIRS) ; do echo -e "\\e[35mkernel_module: clean \\e[36m$$i \\e[0m"; $(MAKE) -C $$i clean  || exit 1; done

install:
	@for i in $(SUBDIRS) ; do echo -e "\\e[35mkernel_module: install \\e[36m$$i \\e[0m"; $(MAKE) -C $$i install  || exit 1; done


