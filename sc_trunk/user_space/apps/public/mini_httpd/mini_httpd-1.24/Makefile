# Makefile for mini_httpd

# CONFIGURE: If you are using a SystemV-based operating system, such as
# Solaris, you will need to uncomment this definition.
#SYSV_LIBS =    -lnsl -lsocket

# CONFIGURE: Some systems don't need -lcrypt, and indeed they get an
# error if you try to link with it.  If you get an error about libcrypt
# not found, try commenting out this definition.
CRYPT_LIB =    #-lcrypt

# CONFIGURE: If you want to compile in support for https, uncomment these
# definitions.  You will need to have already built OpenSSL, available at
# http://www.openssl.org/  Make sure the SSL_TREE definition points to the
# tree with your OpenSSL installation - depending on how you installed it,
# it may be in /usr/local instead of /usr/local/ssl.

CURR_DIR := $(shell pwd)
BUILD_DIR:=$(subst /user_space, /user_space,$(CURR_DIR))
BUILD_DIR:=$(word 1, $(BUILD_DIR))

include $(BUILD_DIR)/Rules.mak

#include ../../Rules.mak
ifeq ($(HTTPS),1)
SSL_TREE =      $(LIBS_DIR)/public/$(OPENSSL_USE)
SSL_INC =       -I${SSL_TREE}/include/
SSL_INC +=       -I${SSL_TREE}/
SSL_DEFS =      -DUSE_SSL
SSL_LIBS =	-L${SSL_TREE}/ -lssl -lcrypto
endif

#SSL_TREE =	/usr/local/ssl
#SSL_DEFS =	-DUSE_SSL
#SSL_INC =	-I$(SSL_TREE)/include
#SSL_LIBS =	-L$(SSL_TREE)/lib -lssl -lcrypto


BINDIR =	/usr/local/sbin
MANDIR =	/usr/local/man
CC =		$(CROSS)gcc
CDEFS =		$(SSL_DEFS) $(SSL_INC)
CFLAGS =	-Os  ${CDEFS}  -Wall -Werror
#CFLAGS =	-O $(CDEFS) -ansi -pedantic -U__STRICT_ANSI__ -Wall -Wpointer-arith -Wshadow -Wcast-qual -Wcast-align -Wstrict-prototypes -Wmissing-prototypes -Wmissing-declarations -Wredundant-decls -Wno-long-long

CFLAGS +=	-I$(SHARED_DIR) -I../../nvram
ifeq ($(USB),1)
CFLAGS += -DUSB
CFLAGS += -D_FILE_OFFSET_BITS=64
endif

ifeq ($(VLAN_SUPPORT),1)
CFLAGS += -DVLAN_SUPPORT
endif

ifeq ($(IPMAC),1)
CFLAGS += -DIPMAC
endif

ifeq ($(IP_ASSIGN_CHK),1)
CFLAGS += -DIP_ASSIGN_CHK
endif

ifeq ($(3G_FEATURE),1)
CFLAGS += -D_3G_FEATURE_
endif

ifeq ($(GUEST_MANAGEMENT),1)
CFLAGS += -DGUEST_MANAGEMENT
endif

ifeq ($(OPENVPN),1)
CFLAGS += -DOPEN_VPN
endif

ifeq ($(XCLOUD),1)
CFLAGS += -DXCLOUD
endif

ifeq ($(HTTPD_IPV6_SUPPORT),1)
CFLAGS += -DHTTPD_IPV6_SUPPORT
endif

CFLAGS += -DSMART_LOGOUT

CFLAGS += -DSC_BUILD

LDFLAGS =	-s
LDFLAGS +=      -L../../nvram -lscnvram
LDLIBS =	 $(SSL_LIBS) $(SYSV_LIBS) #$(CRYPT_LIB)

SETUPWIZARD=1
ifeq ($(SETUPWIZARD), 1)
        CFLAGS += -DSUPPORT_SETUPWIZARD
endif

ifeq ($(WIFI_AP), 1)
	CFLAGS += -DWIFI_AP
endif

ifeq ($(MIIICASA), 1)
	CFLAGS += -DMIIICASA
endif

ifeq ($(PNPX), 1)
	CFLAGS += -DPNPX
endif

LDLIBS += -L../../nvram -lscnvram
CFLAGS += -I../../rc/include -I../../nvram
#all:		mini_httpd #htpasswd

mini_httpd:	mini_httpd.o match.o tdate_parse.o sc_guest_management.o sc_usb.o sc_util.o
	$(CC) $(LDFLAGS) mini_httpd.o match.o tdate_parse.o  sc_guest_management.o sc_usb.o sc_util.o $(LDLIBS) -o mini_httpd

install:mini_httpd
	cp -f mini_httpd $(TARGET_DIR)/usr/sbin/
ifeq ($(HTTPS),1)
	cp -f mini_httpd.pem $(TARGET_DIR)/usr/etc
endif
mini_httpd.o:	mini_httpd.c version.h port.h match.h tdate_parse.h mime_encodings.h mime_types.h
	$(CC) $(CFLAGS) -c mini_httpd.c

match.o:	match.c match.h
	$(CC) $(CFLAGS) -c match.c

tdate_parse.o:	tdate_parse.c tdate_parse.h
	$(CC) $(CFLAGS) -c tdate_parse.c

sc_guest_management.o:	sc_guest_management.c sc_guest_management.h
	${CC} ${CFLAGS} -c sc_guest_management.c
sc_usb.o:	sc_usb.c sc_usb.h
	${CC} ${CFLAGS} -c sc_usb.c
sc_util.o:	sc_util.c sc_util.h
	${CC} ${CFLAGS} -c sc_util.c

mime_encodings.h: mime_encodings.txt
	rm -f mime_encodings.h
	sed < mime_encodings.txt > mime_encodings.h \
	  -e 's/#.*//' -e 's/[ 	]*$$//' -e '/^$$/d' \
	  -e 's/[ 	][ 	]*/", 0, "/' -e 's/^/{ "/' -e 's/$$/", 0 },/'

mime_types.h: mime_types.txt
	rm -f mime_types.h
	sed < mime_types.txt > mime_types.h \
	  -e 's/#.*//' -e 's/[ 	]*$$//' -e '/^$$/d' \
	  -e 's/[ 	][ 	]*/", 0, "/' -e 's/^/{ "/' -e 's/$$/", 0 },/'


htpasswd:	htpasswd.o
	$(CC) $(LDFLAGS) htpasswd.o $(CRYPT_LIB) -o htpasswd

htpasswd.o:	htpasswd.c
	$(CC) $(CFLAGS) -c htpasswd.c


cert:		mini_httpd.pem
mini_httpd.pem:	mini_httpd.cnf
	openssl req -new -x509 -days 3650 -nodes -config mini_httpd.cnf -out mini_httpd.pem -keyout mini_httpd.pem
	openssl x509 -subject -dates -fingerprint -noout -in mini_httpd.pem
	chmod 600 mini_httpd.pem


#install:	all
#	rm -f $(BINDIR)/mini_httpd $(BINDIR)/htpasswd
#	-mkdir -p $(BINDIR)
#	cp mini_httpd htpasswd $(BINDIR)
#	rm -f $(MANDIR)/man8/mini_httpd.8 $(MANDIR)/man1/htpasswd.1
#	-mkdir -p $(MANDIR)/man8
#	cp mini_httpd.8 $(MANDIR)/man8
#	-mkdir -p $(MANDIR)/man1
#	cp htpasswd.1 $(MANDIR)/man1

clean:
	rm -f mini_httpd mime_encodings.h mime_types.h htpasswd mini_httpd.rnd *.o core core.* *.core

tar:
	@name=`sed -n -e '/#define SERVER_SOFTWARE /!d' -e 's,.*mini_httpd/,mini_httpd-,' -e 's, .*,,p' version.h` ; \
	  rm -rf $$name ; \
	  mkdir $$name ; \
	  tar cf - `cat FILES` | ( cd $$name ; tar xfBp - ) ; \
	  chmod 644 $$name/Makefile $$name/mime_encodings.txt $$name/mime_types.txt ; \
	  chmod 755 $$name/contrib $$name/contrib/redhat-rpm ; \
	  tar cf $$name.tar $$name ; \
	  rm -rf $$name ; \
	  gzip $$name.tar
