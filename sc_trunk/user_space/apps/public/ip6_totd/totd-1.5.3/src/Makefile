#
# Makefile for totd
#
# <$Id: Makefile.in,v 3.43 2005/01/31 11:55:14 dillema Exp $>
#

CURR_DIR := $(shell pwd)
BUILD_DIR:=$(subst /user_space, /user_space,$(CURR_DIR))
BUILD_DIR:=$(word 1, $(BUILD_DIR))

include $(BUILD_DIR)/Rules.mak

#include ../../Rules.mak
CROSS_PREFIX=$(CROSS)
CC = $(CROSS_PREFIX)gcc
LD = $(CROSS_PREFIX)ld
STRIP = $(CROSS_PREFIX)strip

# These use the standard autoconf variables, which by default are
# rooted in /home/disk2/eason/eason_disk3/R6950/sc_trunk/user_space/apps/public/ip6_totd/totd-1.5.3/src/../qqq

prefix=/home/disk2/eason/eason_disk3/R6950/sc_trunk/user_space/apps/public/ip6_totd/totd-1.5.3/src/../qqq
exec_prefix=${prefix}

INSTALLDIR = ${exec_prefix}/sbin
INSTALLLIB = ${exec_prefix}/lib
INSTALLMAN = ${prefix}/man
TOT_CONFIG_FILE=/etc/totd.conf

INSTALL = /usr/bin/install

CFLAGS  = -g -O2 -DHAVE_CONFIG_H  -Wall -DTOTCONF=\"$(TOT_CONFIG_FILE)\" -DUSE_INET6  $(INCLUDEPATH)

CFLAGS += -DIP6_6RD_CE -I$(SHARED_DIR)

ifeq ($(HTTPD_IPV6_SUPPORT),1)
CFLAGS += -DHTTPD_IPV6_SUPPORT
endif

# When debugging is enabled by --enable-malloc-debug flag to the configure
# script, the # substitution will contain the empty string, thus
# enabling the dbmalloc lines.  When the flag is not specified, the
# # will contain the string "#", thus commenting out the lines
# and disabling dbmalloc.

#LIBDEBUG = -ldbmalloc
#DEBUGINCLUDE= -I/usr/local/debug_include
#CFLAGS+= -DDBMALLOC

#LIBSWILL= -L./ -lswill
#SWILLINCLUDE= -I./SWILL-0.1/Include
#CFLAGS+= -DSWILL

# Similar as above for TCP debugging
#CFLAGS+= -DDBTCP

LDFLAGS =  
LDADD = -lnsl  $(LIBDEBUG) $(LIBSWILL)
CFLAGS +=   $(DEBUGINCLUDE) $(SWILLINCLUDE)

PROG = totd
MAN = totd.8
SRCS =  request.c response.c context.c ne_mesg.c conv_trick.c ev_tcp.c forward.c queue.c \
	read_config.c tcp_request.c tcp_response.c ev_dup.c list.c res_record.c udp_request.c \
	ev_timeout.c udp_response.c ev_signal.c ev_udp_in.c ne_io.c conv_scoped.c conv_stf.c \
	strlcpy.c strlcat.c daemon.c inet_aton.c html.c dnshj.c
INCLUDES = config.h macros.h protos.h totd.h tot_constants.h tot_types.h
#INCLUDES+= ./SWILL-0.1/Include/swill.h

OBJS+=$(SRCS:.c=.o)

SRCS+=  ${PROG}.c

all: $(PROG)

${PROG}:	${OBJS} ${INCLUDES} # libswill.a
	        ${CC} ${LDFLAGS} -o ${PROG} ${OBJS} ${LDADD}

install: all
	cp -a totd $(BINDIR)

clean:
	-rm -f *.o *.d totd
	-rm $(BINDIR)/totd

libswill.a:	
	-(cd SWILL-0.1/ && ./configure --prefix=/home/disk2/eason/eason_disk3/R6950/sc_trunk/user_space/apps/public/ip6_totd/totd-1.5.3/src/../qqq --enable-ip6 && make static)
	-cp SWILL-0.1/Source/SWILL/libswill.a ./
	-ranlib libswill.a

lint:
	lint ${SRCS}

depend:
	@(mkdep ${SRCS} || makedepend ${SRCS}) 2>/dev/null

#	-(cd SWILL-0.1; make clean)
	

distclean: clean
	rm -f config.cache config.status config.log .depend config.h
	cp Makefile.dummy Makefile

cvs: distclean
	cvs2cl -W 10000 -r -t -b -P
	cvs commit -m "End of my working day"

