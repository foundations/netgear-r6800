
CURR_DIR := $(shell pwd)
BUILD_DIR:=$(subst /user_space, /user_space,$(CURR_DIR))
BUILD_DIR:=$(word 1, $(BUILD_DIR))

include $(BUILD_DIR)/Rules.mak
DESTDIR=
KERNEL_HEADERS=-I/usr/src/linux/include

INSTALL=/usr/bin/install -c

prefix=/usr/local
exec_prefix=${prefix}
bindir=${exec_prefix}/bin
sbindir=${exec_prefix}/sbin
mandir=${prefix}/man
datarootdir=@datarootdir@
distdir = $(PACKAGE)-$(VERSION)

SUBDIRS=libbridge brctl

all:
	for x in $(SUBDIRS); do $(MAKE) $(MFLAGS) -C $$x ; done

clean:
	for x in $(SUBDIRS); do $(MAKE) $(MFLAGS) -C $$x clean ; done

distclean:	clean
	rm -f config.log
	rm -f config.cache
	rm -f config.status

maintainer-clean: distclean
	rm -f Makefile
	rm -f brctl/Makefile
	rm -f libbridge/Makefile
	rm -f doc/Makefile

install: brctl/brctl
	$(CROSS)strip $<
	cp $< $(BINDIR)
	#for x in $(SUBDIRS); do $(MAKE) $(MFLAGS) -C $$x install; done
# brcm begin
dynamic: all
	install -m 755 brctl/brctl $(INSTALL_DIR)/bin
	$(STRIP) $(INSTALL_DIR)/bin/brctl

static: all
	make -C brctl static
# brcm end	
