
CURR_DIR := $(shell pwd)
BUILD_DIR:=$(subst /apps, /apps,$(CURR_DIR))
BUILD_DIR:=$(word 1, $(BUILD_DIR))

#include $(BUILD_DIR)/Rules.mak

#TOPDIR=../../../../../
#include $(TOPDIR)Rules.mak

CC=$(CROSS)gcc
STRIP=$(CROSS)strip
LD=$(CROSS)ld
INSTALL = install
LN = ln
RM =rm -f

CFLAGS+=-DHAVE_CONFIG_H -Ilibusb -I./ 
LIBUSB=libusb.a
LIBUSB_VERS=1.0.9
LIBUSB_SHARED=libusb.so.$(LIBUSB_VERS)

OBJS := libusb/core.o libusb/descriptor.o libusb/io.o libusb/sync.o libusb/os/linux_usbfs.o libusb/os/threads_posix.o

all: $(LIBUSB_SHARED)

$(OBJS): %.o : %.c
	$(CC) $(CFLAGS) -c $< -o $@
	$(STRIP) -x -R .note -R .comment $*.o

$(LIBUSB_SHARED) : $(LIBUSB)
	$(LD) $(LDFLAGS) -soname=$(LIBUSB_SHARED) \
		-o $(LIBUSB_SHARED) --whole-archive $(LIBUSB) \
		--no-whole-archive \
		-lc -lpthread;
#	$(INSTALL) -m 644 $(LIBUSB_SHARED) $(TOPDIR)sdk4210/lib/lib
#	$(LN) -sf $(TOPDIR)sdk4210/lib/lib/$(LIBUSB_SHARED) $(TOPDIR)sdk4210/lib/lib/libusb.so

$(LIBUSB) ar-target: $(OBJS)
	$(AR) $(ARFLAGS) $(LIBUSB) $(OBJS)
#	$(INSTALL) -m 644 $(LIBUSB) $(TOPDIR)sdk4210/lib/lib
	
#install:
#	$(INSTALL) -d $(TOPDIR)target/lib
#	$(RM) $(TOPDIR)target/lib/$(LIBUSB)
#	$(INSTALL) -m 644 $(LIBUSB) $(TOPDIR)target/lib
#	$(INSTALL) -m 644 $(LIBUSB_SHARED) $(TOPDIR)target/lib
#	$(LN) -sf $(TOPDIR)/lib/$(LIBUSB_SHARED) $(TOPDIR)target/lib/libusb.so

$(OBJS): Makefile

clean:
	$(RM) *.[oa] $(LIBUSB_SHARED)* libusb/*.o libusb/os/*.o

