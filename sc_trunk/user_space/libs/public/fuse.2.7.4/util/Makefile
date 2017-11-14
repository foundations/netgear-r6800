
CC = $(CROSS_PREFIX)gcc
LD = $(CROSS_PREFIX)ld
STRIP = $(CROSS_PREFIX)strip

LDFLAGS  := -Wall -W -Wno-sign-compare -Wstrict-prototypes \
	-Wmissing-declarations -Wwrite-strings \
	-g -O2 -fno-strict-aliasing -pthread

CFLAGS := -DHAVE_CONFIG_H -I. -I. -I../include -I../lib \
	-D_FILE_OFFSET_BITS=64 -D_REENTRANT \
	-Wall -W -Wno-sign-compare -Wstrict-prototypes \
	-Wmissing-declarations -Wwrite-strings -g -O2 -fno-strict-aliasing
 
SRC  := fusermount.c  mount.fuse.c  ulockmgr_server.c

OBJS := $(patsubst %.c,%.o,$(SRC))

BIN  := fusermount mount.fuse ulockmgr_server

all: $(BIN)

fusermount: fusermount.o ../lib/mount_util.o
	$(CC) $(CFLAGS) -o $@ $^
	$(STRIP) $@

mount.fuse: mount.fuse.o
	$(CC) $(LDFLAGS) -o $@ $^
	$(STRIP) $@
	
ulockmgr_server: ulockmgr_server.o
	$(CC) $(LDFLAGS) -o $@ $^
	$(STRIP) $@

install:
	-cp -a $(BIN) $(TARGET_DIR)/usr/sbin/

clean:
	rm -f *.o $(BIN) *.d

%.d: %.c
	@$(CC) $(CFLAGS) -MM $< > $@
	@sed -i "1s/^$*.o:/$*.o $*.d: Makefile /g;" $@

-include $(SRC:.c=.d)
