#-----------------------------------------------------
#TOPDIR  :=  $(shell /bin/pwd)
#CC=$(CROSS_PREFIX)gcc
#LD=$(CROSS_PREFIX)ld
#STRIP=$(CROSS_PREFIX)strip
#-----------------------------------------------------
localedir=
CFLAGS += -fPIC -I. -I../include -DLOCALEDIR=\"$(localedir)\" -DHAVE_CONFIG_H -DLIBDIR=\"\" -DUSE_DOS
ARFLAGS=rc
OBJS=iconv.o
LIBCHARSET_OBJECTS = ../libcharset/lib/localcharset.o

all: $(OBJS) $(LIBCHARSET_OBJECTS)
	$(CC) $(CFLAGS) -shared -Wl,-soname,libiconv.so.0 -o libiconv.so.0.0 $(OBJS) $(LIBCHARSET_OBJECTS)
	$(STRIP) libiconv.so.0.0
	ln -sf libiconv.so.0.0 libiconv.so.0
	ln -sf libiconv.so.0 libiconv.so
#	cp -afv libiconv* $(INSTALLDIR)/usr/lib/
install:
	mkdir -p $(T_LIB)
	cp -pdR $(COMM_LIB_PATH)/libiconv* $(T_LIB)

iconv.o: iconv.c
	$(CC) -c $(CFLAGS) iconv.c

clean:
	@echo Cleaning up
	rm -f *.[od]
	rm -f ../libcharset/lib/localcharset.[od]
	rm -f libiconv*
	rm -f $(COMM_LIB_PATH)/libiconv*

#-----------------------------------------------------
#include $(TOPDIR)/rules.mk
#-----------------------------------------------------
