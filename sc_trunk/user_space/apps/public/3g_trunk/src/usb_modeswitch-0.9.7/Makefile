CURR_DIR := $(shell pwd)
BUILD_DIR:=$(subst /user_space, /user_space,$(CURR_DIR))
BUILD_DIR:=$(word 1, $(BUILD_DIR))

include $(BUILD_DIR)/Rules.mak


PROG        = usb_modeswitch
VERS        = 0.9.7
CCFLAGS     = -I./libusb-0.1.12  
RM          = /bin/rm -f
OBJS        = usb_modeswitch.c
DFLAGS      = -L./libusb-0.1.12/
LIBUSBPATH  = libusb-0.1.12

#ifeq ($(BUILD_TYPE), embedded)
CONFIGURE = ./configure --host=mips-linux CC=$(CROSS)gcc CXX=$(CROSS)g++
INSTALLDIR = $(ROOT)/target/bin/

#else
#	CONFIGURE = ./configure
#endif	


.PHONY:     clean
all:        libusb $(PROG)

libusb:

	if test -e $(LIBUSBPATH)/Makefile; then \
	$(MAKE) -C $(LIBUSBPATH); \
	else \
	cd $(LIBUSBPATH); \
    $(CONFIGURE); \
	$(MAKE); \
	fi
	cp -rf $(LIBUSBPATH)/.libs/libusb.a $(LIBUSBPATH)

$(PROG):    $(OBJS)
	      $(CC) $(CCFLAGS) $(DFLAGS) -s -o $@ $^ -lusb
	      $(STRIP) $(PROG)

clean:
	      $(RM) -rf ./usb_modeswitch
	      if test -e $(LIBUSBPATH)/Makefile; then \
	      $(MAKE) -C $(LIBUSBPATH) clean; \
	      $(RM) -rf $(LIBUSBPATH)/Makefile; \
	      fi
	      $(RM) -rf $(LIBUSBPATH)/libusb.a
	      $(RM) -rf $(LIBUSBPATH)/.deps/

install: 
	install $(PROG) $(INSTALLDIR)

.PHONY:     clean install
