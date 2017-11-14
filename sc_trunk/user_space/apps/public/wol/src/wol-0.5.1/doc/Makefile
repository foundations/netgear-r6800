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

info_TEXINFOS = wol.texi
MAINTAINERCLEANFILES = Makefile.in texinfo.tex
subdir = doc
mkinstalldirs = $(SHELL) $(top_srcdir)/mkinstalldirs
CONFIG_HEADER = $(top_builddir)/config.h
CONFIG_CLEAN_FILES =
depcomp =
DIST_SOURCES =
INFO_DEPS = wol.info
DVIS = wol.dvi
TEXINFOS = wol.texi
DIST_COMMON = Makefile.am Makefile.in texinfo.tex
all: all-am

.SUFFIXES:
.SUFFIXES: .dvi .info .ps .texi
$(srcdir)/Makefile.in:  Makefile.am  $(top_srcdir)/configure.ac $(ACLOCAL_M4)
	cd $(top_srcdir) && \
	  $(AUTOMAKE) --gnu  --ignore-deps doc/Makefile
Makefile:  $(srcdir)/Makefile.in  $(top_builddir)/config.status
	cd $(top_builddir) && \
	  CONFIG_HEADERS= CONFIG_LINKS= \
	  CONFIG_FILES=$(subdir)/$@ $(SHELL) ./config.status

wol.info: wol.texi
wol.dvi: wol.texi

.texi.info:
	@cd $(srcdir) && rm -f $@ $@-[0-9] $@-[0-9][0-9]
	cd $(srcdir) \
	  && $(MAKEINFO) $(AM_MAKEINFOFLAGS) $(MAKEINFOFLAGS) \
	       `echo $< | sed 's,.*/,,'`

.texi.dvi:
	TEXINPUTS=$(srcdir):$$TEXINPUTS \
	MAKEINFO='$(MAKEINFO) $(AM_MAKEINFOFLAGS) $(MAKEINFOFLAGS) -I $(srcdir)' \
	$(TEXI2DVI) $<

.texi:
	@cd $(srcdir) && rm -f $@ $@-[0-9] $@-[0-9][0-9]
	cd $(srcdir) \
	  && $(MAKEINFO) $(AM_MAKEINFOFLAGS) $(MAKEINFOFLAGS) \
	       `echo $< | sed 's,.*/,,'`

MAKEINFO = ${SHELL} /home/disk2/eason/eason_disk3/R6950_1042/sc_trunk/user_space/apps/public/wol/src/wol-0.5.1/missing --run makeinfo
TEXI2DVI = texi2dvi
DVIPS = dvips
.dvi.ps:
	$(DVIPS) $< -o $@

uninstall-info-am:
	$(PRE_UNINSTALL)
	@if (install-info --version && \
	     install-info --version | fgrep -i -v debian) >/dev/null 2>&1; then \
	  list='$(INFO_DEPS)'; \
	  for file in $$list; do \
	    echo " install-info --info-dir=$(DESTDIR)$(infodir) --remove $(DESTDIR)$(infodir)/$$file"; \
	    install-info --info-dir=$(DESTDIR)$(infodir) --remove $(DESTDIR)$(infodir)/$$file; \
	  done; \
	else :; fi
	@$(NORMAL_UNINSTALL)
	@list='$(INFO_DEPS)'; \
	for file in $$list; do \
	  (if cd $(DESTDIR)$(infodir); then \
	     echo " rm -f $$file $$file-[0-9] $$file-[0-9][0-9])"; \
	     rm -f $$file $$file-[0-9] $$file-[0-9][0-9]; \
	   else :; fi); \
	done

dist-info: $(INFO_DEPS)
	list='$(INFO_DEPS)'; \
	for base in $$list; do \
	  d=$(srcdir); \
	  for file in `CDPATH=: && cd $$d && eval echo $$base*`; do \
	    test -f $(distdir)/$$file \
	    || cp -p $$d/$$file $(distdir)/$$file; \
	  done; \
	done

mostlyclean-aminfo:
	-rm -f wol.aux wol.cp wol.dvi wol.fn wol.ky wol.log wol.pg wol.ps wol.toc \
	  wol.tp wol.vr

maintainer-clean-aminfo:
	cd $(srcdir) && \
	for i in $(INFO_DEPS); do \
	  rm -f $$i; \
	  if test "`echo $$i-[0-9]*`" != "$$i-[0-9]*"; then \
	    rm -f $$i-[0-9]*; \
	  fi; \
	done
tags: TAGS
TAGS:


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
	$(MAKE) $(AM_MAKEFLAGS) \
	  top_distdir="${top_distdir}" distdir="$(distdir)" \
	  dist-info
check-am: all-am
check: check-am
all-am: Makefile $(INFO_DEPS)

installdirs:
	$(mkinstalldirs) $(DESTDIR)$(infodir)

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

clean-am: clean-generic mostlyclean-am

distclean: distclean-am

distclean-am: clean-am distclean-generic

dvi: dvi-am

dvi-am: $(DVIS)

info: info-am

info-am: $(INFO_DEPS)

install-data-am: install-info-am

install-exec-am:

install-info: install-info-am

install-info-am: $(INFO_DEPS)
	@$(NORMAL_INSTALL)
	$(mkinstalldirs) $(DESTDIR)$(infodir)
	@list='$(INFO_DEPS)'; \
	for file in $$list; do \
	  d=$(srcdir); \
	  for ifile in `CDPATH=: && cd $$d && echo $$file $$file-[0-9] $$file-[0-9][0-9]`; do \
	    if test -f $$d/$$ifile; then \
	      echo " $(INSTALL_DATA) $$d/$$ifile $(DESTDIR)$(infodir)/$$ifile"; \
	      $(INSTALL_DATA) $$d/$$ifile $(DESTDIR)$(infodir)/$$ifile; \
	    else : ; fi; \
	  done; \
	done
	@$(POST_INSTALL)
	@if (install-info --version && \
	     install-info --version | fgrep -i -v debian) >/dev/null 2>&1; then \
	  list='$(INFO_DEPS)'; \
	  for file in $$list; do \
	    echo " install-info --info-dir=$(DESTDIR)$(infodir) $(DESTDIR)$(infodir)/$$file";\
	    install-info --info-dir=$(DESTDIR)$(infodir) $(DESTDIR)$(infodir)/$$file || :;\
	  done; \
	else : ; fi
install-man:

installcheck-am:

maintainer-clean: maintainer-clean-am

maintainer-clean-am: distclean-am maintainer-clean-aminfo \
	maintainer-clean-generic

mostlyclean: mostlyclean-am

mostlyclean-am: mostlyclean-aminfo mostlyclean-generic

uninstall-am: uninstall-info-am

.PHONY: all all-am check check-am clean clean-generic dist-info \
	distclean distclean-generic distdir dvi dvi-am info info-am \
	install install-am install-data install-data-am install-exec \
	install-exec-am install-info install-info-am install-man \
	install-strip installcheck installcheck-am installdirs \
	maintainer-clean maintainer-clean-aminfo \
	maintainer-clean-generic mostlyclean mostlyclean-aminfo \
	mostlyclean-generic uninstall uninstall-am uninstall-info-am

# Tell versions [3.59,3.63) of GNU make to not export all variables.
# Otherwise a system limit (for SysV at least) may be exceeded.
.NOEXPORT:
