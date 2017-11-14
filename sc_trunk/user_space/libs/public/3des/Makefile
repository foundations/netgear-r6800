
CC = $(CROSS)gcc
STRIP = $(CROSS)strip

CFLAGS  = -Os -s -Wall -Werror -fPIC -g -c

DESTLIB = $(TARGET_DIR)/lib
#DESTBIN = ../../target/usr/sbin

all: lib3des.so
	@echo $@ done

lib3des.so : 3des.o
	$(CC) -Os -s -shared -Wl,-soname,lib3des.so -o lib3des.so 3des.o

3des.o : 3des.c
	$(CC) $(CFLAGS) -o $@ $^

install :
	cp -f lib3des.so $(DESTLIB)

clean:
	rm -rf *~ *.o *.so

GPL:
	mkdir -p $(GPL_TARGET_DIR)
	cp -f  $(BIN) $(GPL_TARGET_DIR)/
	cp -f Makefile $(GPL_TARGET_DIR)/
