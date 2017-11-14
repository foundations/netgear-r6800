
CURR_DIR := $(shell pwd)
BUILD_DIR:=$(subst /user_space, /user_space,$(CURR_DIR))
BUILD_DIR:=$(word 1, $(BUILD_DIR))

include $(BUILD_DIR)/Rules.mak

PROG        = usb_modeswitch
VERS        = 2.2.0
CCFLAGS     = -I./libusb-1.0.9/libusb  
RM          = /bin/rm -f
OBJS        = usb_modeswitch.c
DFLAGS      = -L./libusb-1.0.9/
LIBUSBPATH  = libusb-1.0.9

LIBUSB_OLD_PATH = libusb-0.1.12

BUILD_TYPE=embedded

ifeq ($(BUILD_TYPE), embedded)
CONFIGURE = ./configure --host=mips-linux CC=$(CROSS)gcc CXX=$(CROSS)g++
INSTALLDIR = $(ROOT)/target/bin/

else
	CONFIGURE = ./configure
endif	

.PHONY:     clean
all:        libusb $(PROG)

oldlibusb:
	if test -e $(LIBUSB_OLD_PATH)/Makefile; then \
	$(MAKE) -C $(LIBUSB_OLD_PATH); \
	else \
	cd $(LIBUSB_OLD_PATH); \
    $(CONFIGURE); \
	$(MAKE); \
	fi
	cp -rf $(LIBUSB_OLD_PATH)/.libs/libusb.a $(LIBUSB_OLD_PATH)

libusb:
	cd $(LIBUSBPATH); \
	$(MAKE); \
	
$(PROG):    $(OBJS)
	      $(CC) $(CCFLAGS) $(DFLAGS) -s -o $@ $^ -lusb -lpthread
	      $(STRIP) $(PROG)

clean:
	      $(RM) -rf ./usb_modeswitch
	      if test -e $(LIBUSBPATH)/Makefile; then \
	      $(MAKE) -C $(LIBUSBPATH) clean; \
	      fi
	      -$(RM) -rf $(LIBUSBPATH)/libusb.a
	      -$(RM) -rf $(LIBUSBPATH)/.deps/
		  if test -e $(LIBUSB_OLD_PATH)/Makefile; then \
	      $(MAKE) -C $(LIBUSB_OLD_PATH) clean; \
	      fi
		  -$(RM)	$(LIBUSB_OLD_PATH)/Makefile  	
	      -$(RM) -rf $(LIBUSB_OLD_PATH)/libusb.a
	      -$(RM) -rf $(LIBUSB_OLD_PATH)/.deps/




install: 
	install $(PROG) $(INSTALLDIR)

.PHONY:     clean install
