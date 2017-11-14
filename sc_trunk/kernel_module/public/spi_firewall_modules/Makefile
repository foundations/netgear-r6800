


 
ifndef PATCHLEVEL

CURR_DIR := $(shell pwd)
BUILD_DIR:=$(subst /kernel_module, /kernel_module,$(CURR_DIR))
BUILD_DIR:=$(word 1, $(BUILD_DIR))

include $(BUILD_DIR)/Rules.mak

else
include $(ROOT)/Rules.mak
endif 

CROSS_COMPILE=$(CROSS)

KERNELDIR=$(KERNEL_DIR)
SUBDIR = $(shell pwd)


OBJS = common.ko firewall_block.ko \
		psd_and_special_udp_dos.ko tcp_syn_dos.ko clamp_total_session_for_one_src.ko \
		xt_ct_dir.ko

obj-m:= common.o firewall_block.o \
		psd_and_special_udp_dos.o tcp_syn_dos.o clamp_total_session_for_one_src.o \
		xt_ct_dir.o 

ifeq ($(CUSTOMER_NAME),Amped)
obj-m +=  xt_sclimit.o 
OBJS += xt_sclimit.ko 
endif

EXTRA_CFLAGS += -Werror -I$(SHARED_DIR) -I$(KERNEL_ARCH_INC)

all:modules

modules:
	$(MAKE) -C $(KERNELDIR) SUBDIRS=$(SUBDIR) modules V=1 CROSS_COMPILE=$(CROSS_COMPILE)
	$(STRIP) -d $(OBJS)

clean:
	rm -rf $(OBJS) *.o *.ko *.cmd .*.cmd *.mod.c
	rm -rf .tmp_versions Module.symvers

install:
	cp -a $(OBJS)  $(ROOT)/target/lib/modules
	

