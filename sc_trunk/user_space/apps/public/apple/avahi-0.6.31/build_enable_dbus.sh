#!/bin/bash
build_dir=build_sc
cd $build_dir

export LIBDAEMON_CFLAGS="-I`pwd`/../../target/usr/local/include/libdaemon" 
export LIBDAEMON_LIBS="-L`pwd`/../../target/usr/local/lib -ldaemon" 


DBUS_CFLAGS="" DBUS_LIBS="" CC=$1 CFLAGS="-I`pwd`/../../target/lib/dbus-1.0/include/ -DHAVE_DBUS -I`pwd`/../../target/include/dbus-1.0/ -I`pwd`/../../target/usr/include -I`pwd`/../../target/usr/local/include -I`pwd`/../../target/usr/include/dbus-1.0/" LDFLAGS="-L`pwd`/../../target/lib -ldbus-1 -L`pwd`/../../target/usr/lib -L`pwd`/../../target/usr/local/lib" ../configure --target=mipsel-linux-uclibc --host=mipsel-linux-uclibc --build=i486-linux-gnu --prefix= --sbindir=/usr/sbin --sysconfdir=/etc --exec-prefix=/usr--sbindir=/usr/sbin --sysconfdir=/etc --datadir=/usr/share --mandir=/usr/man --exec-prefix=/usr\
	--enable-shared \
	--enable-static \
	--disable-glib \
	--disable-gobject \
	--disable-qt3 \
	--disable-qt4 \
	--disable-gtk \
	--disable-gtk3 \
	--enable-dbus \
	--with-xml=expat \
	--disable-dbm \
	--enable-gdbm \
	--enable-libdaemon \
	--disable-python \
	--disable-pygtk \
	--disable-python-dbus \
	--disable-mono \
	--disable-monodoc \
	--disable-doxygen-doc \
	--disable-doxygen-dot \
	--disable-doxygen-man \
	--disable-doxygen-rtf \
	--disable-doxygen-xml \
	--disable-doxygen-chm \
	--disable-doxygen-chi \
	--disable-doxygen-html \
	--disable-doxygen-ps \
	--disable-doxygen-pdf \
	--disable-xmltoman \
	--disable-manpages \
	--with-distro=none \
	--with-avahi-user=root \
	--with-avahi-group=root \
	--with-autoipd-user=root \
	--with-autoipd-group=root \
	--disable-stack-protector

