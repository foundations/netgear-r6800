CURR_DIR := $(shell pwd)
BUILD_DIR:=$(subst /user_space, /user_space,$(CURR_DIR))
BUILD_DIR:=$(word 1, $(BUILD_DIR))

include $(BUILD_DIR)/Rules.mak

CC = $(CROSS)gcc
STRIP=$(CROSS)strip
LDFLAGS=
CFLAGS += -Os -Wall -s -Werror -Wno-unused-but-set-variable
BIN =syslogd klogd

NETGEAR_LOG=1

CFLAGS += -I$(ROOT)/shared
#CFLAGS += -D_SC_DEBUG_ENABLE_
# Define _ROUTER_DEBUG_MODE_ to print all of the syslog message to log.htm
#CFLAGS += -D_ROUTER_DEBUG_MODE_

LIBS  = syslogd.c 
LIBS_klog = klogd.c 

ifeq ($(CUSTOMER_NAME),Amped)
CFLAGS += -DAMPED
endif
ifeq ($(CHIP_VENDOR),MTK)
ifneq (,$(findstring sdk5,$(NEEDED_SDKPATH)))
CFLAGS += -DMTK_SDK5000
endif
endif

ifeq ($(NETGEAR_LOG), 1)
CFLAGS += -DNETGEAR_LOG
LIBS += log_queue.c
endif

ifeq ($(DSL_WIZARD_LOG),1)
LIBS += dsl_log_queue.c
CFLAGS += -DDSL_WIZARD_LOG
CFLAGS += -I$(SC_LIBS_DIR)/include -I../nvram
LDFLAGS +=-L$(SC_LIBS_DIR)/libflash -lflash
LDFLAGS +=-L../nvram -lscnvram
endif

all: $(BIN) 

syslogd:$(LIBS)
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $(LIBS)
	$(STRIP) $@

klogd:$(LIBS_klog)
	$(CC) $(CFLAGS) -o $@ $(LIBS_klog)
	$(STRIP) $@

install:
	#rm -f klogd
	#ln -s syslogd klogd
	cp -a syslogd klogd $(ROOT)/target/sbin
clean:
	rm -f $(BIN) 
#	rm -f ../../target/sbin/syslogd  ../../target/sbin/klogd
