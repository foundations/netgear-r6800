CC = $(CROSS)gcc

CFLAGS  = -Os -s -Wall -Werror -fPIC -g -c

DESTLIB = $(TARGET_DIR)/lib
#DESTBIN = ../../target/usr/sbin

all: libb64.so
	@echo $@ done

libb64.so : base64.o
	$(CC) -Os -s -shared -Wl,-soname,libb64.so -o libb64.so base64.o

base64.o : base64.c
	$(CC) $(CFLAGS) -o $@ $^

install :
	cp -f libb64.so $(DESTLIB)

clean:
	rm -rf *~ *.o *.so
	rm -f $(DESTLIB)/libb64.so
