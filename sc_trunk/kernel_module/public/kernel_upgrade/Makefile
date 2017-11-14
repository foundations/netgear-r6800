#module makefile for 2.6 kernel
#

obj-m := kernel_upgrade.o
PWD := $(shell pwd)
KVER := 2.6

#EXTRA_CFLAGS += -Werror 
EXTRA_CFLAGS += -I$(SHARED_DIR) -I$(KERNEL_ARCH_INC)
#_ZIP_3G_ Will be define by Kbuild, refer KERNEL_SRC/Makefile
#EXTRA_CFLAGS += -D_ZIP_3G_			
EXTRA_CFLAGS += -DMARK_UPGRADE

ifeq ($(ZIP_IMG),1)
EXTRA_CFLAGS += -D_ZIP_IMG_
endif

.PHONY : all install clean

all:
	make -C $(KERNEL_DIR) M=$(PWD) modules V=1 CROSS_COMPILE=$(CROSS_COMPILE)
	$(STRIP) -d kernel_upgrade.ko

clean:
	rm -rf ./*.o  *.mod.c *.ko .*.cmd .tmp_versions Module.symvers

install:all
	cp kernel_upgrade.ko $(ROOT)/target/lib/modules/kernel_upgrade.ko
