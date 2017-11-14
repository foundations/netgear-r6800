# Makefile.in generated automatically by automake 1.5 from Makefile.am.

# Copyright 1994, 1995, 1996, 1997, 1998, 1999, 2000, 2001
# Free Software Foundation, Inc.
# This Makefile.in is free software; the Free Software Foundation
# gives unlimited permission to copy and/or distribute it,
# with or without modifications, as long as this notice is preserved.

# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY, to the extent permitted by law; without
# even the implied warranty of MERCHANTABILITY or FITNESS FOR A
# PARTICULAR PURPOSE.



SHELL = /bin/sh

srcdir = .
top_srcdir = ..

prefix = /home/disk2/eason/eason_disk3/R6950_1042/sc_trunk/user_space/apps/public/wol/src/wol-0.5.1
exec_prefix = ${prefix}

bindir = ${exec_prefix}/bin
sbindir = ${exec_prefix}/sbin
libexecdir = ${exec_prefix}/libexec
datadir = ${prefix}/share
sysconfdir = ${prefix}/etc
sharedstatedir = ${prefix}/com
localstatedir = ${prefix}/var
libdir = ${exec_prefix}/lib
infodir = ${prefix}/share/info
mandir = ${prefix}/share/man
includedir = ${prefix}/include
oldincludedir = /usr/include
pkgdatadir = $(datadir)/wol
pkglibdir = $(libdir)/wol
pkgincludedir = $(includedir)/wol
top_builddir = ..

ACLOCAL = ${SHELL} /home/disk2/eason/eason_disk3/R6950_1042/sc_trunk/user_space/apps/public/wol/src/wol-0.5.1/missing --run aclocal
AUTOCONF = ${SHELL} /home/disk2/eason/eason_disk3/R6950_1042/sc_trunk/user_space/apps/public/wol/src/wol-0.5.1/missing --run autoconf
AUTOMAKE = ${SHELL} /home/disk2/eason/eason_disk3/R6950_1042/sc_trunk/user_space/apps/public/wol/src/wol-0.5.1/missing --run automake
AUTOHEADER = ${SHELL} /home/disk2/eason/eason_disk3/R6950_1042/sc_trunk/user_space/apps/public/wol/src/wol-0.5.1/missing --run autoheader

INSTALL = /usr/bin/install -c
INSTALL_PROGRAM = ${INSTALL}
INSTALL_DATA = ${INSTALL} -m 644
INSTALL_SCRIPT = ${INSTALL}
INSTALL_HEADER = $(INSTALL_DATA)
transform = s,x,x,
NORMAL_INSTALL = :
PRE_INSTALL = :
POST_INSTALL = :
NORMAL_UNINSTALL = :
PRE_UNINSTALL = :
POST_UNINSTALL = :
host_alias = mips-linux
host_triplet = mips-mips-linux-gnu
AMTAR = ${SHELL} /home/disk2/eason/eason_disk3/R6950_1042/sc_trunk/user_space/apps/public/wol/src/wol-0.5.1/missing --run tar
AWK = gawk
BUILD_INCLUDED_LIBINTL = no
CATALOGS = 
CATOBJEXT = 
CC = mipsel-linux-gcc
DATADIRNAME = share
DEPDIR = .deps
EXEEXT = 
EXTRA_LIBS = 
GENCAT = gencat
GLIBC21 = yes
GMOFILES =  de.gmo
GMSGFMT = 
INSTALL_STRIP_PROGRAM = ${SHELL} $(install_sh) -c -s
INSTOBJEXT = .mo
INTLBISON = bison
INTLLIBS = 
INTLOBJS = 
INTL_LIBTOOL_SUFFIX_PREFIX = 
LIBICONV = 
LN_S = ln -s
MKINSTALLDIRS = ./mkinstalldirs
MSGFMT = 
OBJEXT = o
PACKAGE = wol
POFILES =  de.po
POSUB = 
RANLIB = mipsel-linux-ranlib
USE_INCLUDED_LIBINTL = no
USE_NLS = no
VERSION = 0.5.1
am__include = include
am__quote = 
install_sh = /home/disk2/eason/eason_disk3/R6950_1042/sc_trunk/user_space/apps/public/wol/src/wol-0.5.1/install-sh

noinst_LIBRARIES = libwrappers.a
libwrappers_a_SOURCES = wrappers.h getopt.h getopt1.c getopt.c usleep.c \
usleep.h xmalloc.c xmalloc.h

MAINTAINERCLEANFILES = Makefile.in
subdir = lib
mkinstalldirs = $(SHELL) $(top_srcdir)/mkinstalldirs
CONFIG_HEADER = $(top_builddir)/config.h
CONFIG_CLEAN_FILES =
LIBRARIES = $(noinst_LIBRARIES)

libwrappers_a_AR = $(AR) cru
libwrappers_a_LIBADD =
am_libwrappers_a_OBJECTS = getopt1.$(OBJEXT) getopt.$(OBJEXT) \
	usleep.$(OBJEXT) xmalloc.$(OBJEXT)
libwrappers_a_OBJECTS = $(am_libwrappers_a_OBJECTS)

DEFS = -DHAVE_CONFIG_H
DEFAULT_INCLUDES =  -I. -I$(srcdir) -I$(top_builddir)
CPPFLAGS = 
LDFLAGS = -L/home/disk2/eason/eason_disk3/R6950_1042/sc_trunk/user_space/apps/public/nvram -lscnvram
LIBS = 
depcomp =
COMPILE = $(CC) $(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES) $(AM_CPPFLAGS) \
	$(CPPFLAGS) $(AM_CFLAGS) $(CFLAGS)
CCLD = $(CC)
LINK = $(CCLD) $(AM_CFLAGS) $(CFLAGS) $(AM_LDFLAGS) $(LDFLAGS) -o $@
CFLAGS = -I/home/disk2/eason/eason_disk3/R6950_1042/sc_trunk/user_space/apps/public/nvram -W -Wall -Wpointer-arith -Wimplicit-function-declaration
DIST_SOURCES = $(libwrappers_a_SOURCES)
DIST_COMMON = Makefile.am Makefile.in
SOURCES = $(libwrappers_a_SOURCES)

all: all-am

.SUFFIXES:
.SUFFIXES: .c .o .obj
$(srcdir)/Makefile.in:  Makefile.am  $(top_srcdir)/configure.ac $(ACLOCAL_M4)
	cd $(top_srcdir) && \
	  $(AUTOMAKE) --gnu  --ignore-deps lib/Makefile
Makefile:  $(srcdir)/Makefile.in  $(top_builddir)/config.status
	cd $(top_builddir) && \
	  CONFIG_HEADERS= CONFIG_LINKS= \
	  CONFIG_FILES=$(subdir)/$@ $(SHELL) ./config.status

AR = ar

clean-noinstLIBRARIES:
	-test -z "$(noinst_LIBRARIES)" || rm -f $(noinst_LIBRARIES)
libwrappers.a: $(libwrappers_a_OBJECTS) $(libwrappers_a_DEPENDENCIES) 
	-rm -f libwrappers.a
	$(libwrappers_a_AR) libwrappers.a $(libwrappers_a_OBJECTS) $(libwrappers_a_LIBADD)
	$(RANLIB) libwrappers.a

mostlyclean-compile:
	-rm -f *.$(OBJEXT) core *.core

distclean-compile:
	-rm -f *.tab.c

.c.o:
	$(COMPILE) -c `test -f $< || echo '$(srcdir)/'`$<

.c.obj:
	$(COMPILE) -c `cygpath -w $<`
uninstall-info-am:

tags: TAGS

ID: $(HEADERS) $(SOURCES) $(LISP) $(TAGS_FILES)
	list='$(SOURCES) $(HEADERS) $(TAGS_FILES)'; \
	unique=`for i in $$list; do \
	    if test -f "$$i"; then echo $$i; else echo $(srcdir)/$$i; fi; \
	  done | \
	  $(AWK) '    { files[$$0] = 1; } \
	       END { for (i in files) print i; }'`; \
	mkid -fID $$unique $(LISP)

TAGS:  $(HEADERS) $(SOURCES)  $(TAGS_DEPENDENCIES) \
		$(TAGS_FILES) $(LISP)
	tags=; \
	here=`pwd`; \
	list='$(SOURCES) $(HEADERS) $(TAGS_FILES)'; \
	unique=`for i in $$list; do \
	    if test -f "$$i"; then echo $$i; else echo $(srcdir)/$$i; fi; \
	  done | \
	  $(AWK) '    { files[$$0] = 1; } \
	       END { for (i in files) print i; }'`; \
	test -z "$(ETAGS_ARGS)$$unique$(LISP)$$tags" \
	  || etags $(ETAGS_ARGS) $$tags  $$unique $(LISP)

GTAGS:
	here=`CDPATH=: && cd $(top_builddir) && pwd` \
	  && cd $(top_srcdir) \
	  && gtags -i $(GTAGS_ARGS) $$here

distclean-tags:
	-rm -f TAGS ID GTAGS GRTAGS GSYMS GPATH

DISTFILES = $(DIST_COMMON) $(DIST_SOURCES) $(TEXINFOS) $(EXTRA_DIST)

top_distdir = ..
distdir = $(top_distdir)/$(PACKAGE)-$(VERSION)

distdir: $(DISTFILES)
	@for file in $(DISTFILES); do \
	  if test -f $$file; then d=.; else d=$(srcdir); fi; \
	  dir=`echo "$$file" | sed -e 's,/[^/]*$$,,'`; \
	  if test "$$dir" != "$$file" && test "$$dir" != "."; then \
	    $(mkinstalldirs) "$(distdir)/$$dir"; \
	  fi; \
	  if test -d $$d/$$file; then \
	    cp -pR $$d/$$file $(distdir) \
	    || exit 1; \
	  else \
	    test -f $(distdir)/$$file \
	    || cp -p $$d/$$file $(distdir)/$$file \
	    || exit 1; \
	  fi; \
	done
check-am: all-am
check: check-am
all-am: Makefile $(LIBRARIES)

installdirs:

install: install-am
install-exec: install-exec-am
install-data: install-data-am
uninstall: uninstall-am

install-am: all-am
	@$(MAKE) $(AM_MAKEFLAGS) install-exec-am install-data-am

installcheck: installcheck-am
install-strip:
	$(MAKE) $(AM_MAKEFLAGS) INSTALL_PROGRAM="$(INSTALL_STRIP_PROGRAM)" \
	  `test -z '$(STRIP)' || \
	    echo "INSTALL_PROGRAM_ENV=STRIPPROG='$(STRIP)'"` install
mostlyclean-generic:

clean-generic:

distclean-generic:
	-rm -f Makefile $(CONFIG_CLEAN_FILES) stamp-h stamp-h[0-9]*

maintainer-clean-generic:
	@echo "This command is intended for maintainers to use"
	@echo "it deletes files that may require special tools to rebuild."
	-test -z "$(MAINTAINERCLEANFILES)" || rm -f $(MAINTAINERCLEANFILES)
clean: clean-am

clean-am: clean-generic clean-noinstLIBRARIES mostlyclean-am

distclean: distclean-am

distclean-am: clean-am distclean-compile distclean-generic \
	distclean-tags

dvi: dvi-am

dvi-am:

info: info-am

info-am:

install-data-am:

install-exec-am:

install-info: install-info-am

install-man:

installcheck-am:

maintainer-clean: maintainer-clean-am

maintainer-clean-am: distclean-am maintainer-clean-generic

mostlyclean: mostlyclean-am

mostlyclean-am: mostlyclean-compile mostlyclean-generic

uninstall-am: uninstall-info-am

.PHONY: GTAGS all all-am check check-am clean clean-generic \
	clean-noinstLIBRARIES distclean distclean-compile \
	distclean-generic distclean-tags distdir dvi dvi-am info \
	info-am install install-am install-data install-data-am \
	install-exec install-exec-am install-info install-info-am \
	install-man install-strip installcheck installcheck-am \
	installdirs maintainer-clean maintainer-clean-generic \
	mostlyclean mostlyclean-compile mostlyclean-generic tags \
	uninstall uninstall-am uninstall-info-am

# Tell versions [3.59,3.63) of GNU make to not export all variables.
# Otherwise a system limit (for SysV at least) may be exceeded.
.NOEXPORT:
