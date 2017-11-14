CURR_DIR := $(shell pwd)
BUILD_DIR:=$(subst /user_space, /user_space,$(CURR_DIR))
BUILD_DIR:=$(word 1, $(BUILD_DIR))

include $(BUILD_DIR)/Rules.mak

CC=$(CROSS)gcc
#CC=gcc
BIN=smtpc
CFLAGS = -Wall -Werror -Os -s
CFLAGS += -I$(SHARED_DIR)
CFLAGS += -DBOARD_ID=\"$(BOARD_ID)\"

ifeq ($(SMTP_SSL),1)
CFLAGS += -DSMTP_SSL
CFLAGS += -I$(LIBS_DIR)/public/$(OPENSSL_USE)/include
LDFLAGS +=-L$(LIBS_DIR)/public/$(OPENSSL_USE) -lssl -lcrypto
CFLAGS += -I../nvram
LIBS +=  -L../nvram -lscnvram
endif

ifeq ($(NTGR_CLOUD),1)
CFLAGS += -DNTGR_CLOUD
endif

$(BIN) : smtpc.c
	$(CC) $(CFLAGS) $(LDFLAGS) $(LIBS) -o $@ $^
clean:
	rm -f $(BIN)
install:
	cp -a smtpc $(BINDIR)
