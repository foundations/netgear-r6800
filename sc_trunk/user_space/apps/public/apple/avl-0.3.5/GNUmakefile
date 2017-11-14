# You may select DEPTHs (no indexing), COUNTs (slower) or both (more memory).
#CPPFLAGS += -DAVL_COUNT -DAVL_DEPTH

TARGET_DIR = $(ROOT)/target
LN ?= ln
INSTALL ?= /usr/bin/install
LDCONFIG ?= /sbin/ldconfig

# Some suggestions: (-mcpu= generates i386 compatible code)
CFLAGS ?= -O2 -pipe -Wall -Werror
CFLAGS += -fPIC
#CFLAGS = -O2 -fomit-frame-pointer -pipe -march=i586 -Wall -g
#CFLAGS = -O6 -fomit-frame-pointer -pipe -march=i586 -Wall -ansi -pedantic
#CFLAGS = -O6 -fomit-frame-pointer -pipe -march=i686 -Wall -ansi -pedantic
#CFLAGS = -O6 -march=k6 -fforce-mem -fforce-addr -pipe
#CFLAGS = -g -fomit-frame-pointer -pipe -march=i686 -Wall -ansi -pedantic
#CFLAGS = -g -pg -a -pipe -march=i686 -Wall
#LDFLAGS = -s

prefix ?= /usr/local
libdir ?= $(prefix)/lib
includedir ?= $(prefix)/include
includedir ?= /usr/include

PROGRAMS = avlsort setdiff
LIBRARY = libavl.so.1.5

all: $(LIBRARY)

test: $(PROGRAMS)

setdiff: setdiff.o avl.o
	$(CC) $(LDFLAGS) $^ -o $@ $(LIBS)

avlsort: avlsort.o avl.o
	$(CC) $(LDFLAGS) $^ -o $@ $(LIBS)

$(LIBRARY): avl.o
	$(CC) -shared -Wl,-soname,libavl.so.1 $^ -o $@ -lc
	$(LN) -s $@ libavl.so

clean:
	$(RM) *.o $(PROGRAMS) libavl.*

install: all
#	$(INSTALL) -d $(DESTDIR)$(libdir)
#	$(INSTALL) -d $(DESTDIR)$(includedir)
#	$(INSTALL) -m 644 avl.h $(DESTDIR)$(includedir)
	cp -a $(LIBRARY) $(TARGET_DIR)/lib/
	for i in $(LIBRARY); do\
		$(LN) -sf $$i $(TARGET_DIR)/lib/$${i%.*};\
		$(LN) -sf $${i%.*} $(TARGET_DIR)/lib/$${i%.*.*};\
	done
	#-$(LDCONFIG)

.PHONY: clean install all
.PRECIOUS: %.h %.c
