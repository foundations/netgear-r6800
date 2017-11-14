# DIR setting

# SDK PATH setting
export SC_SDKPATH=sdk5010

# TRUNK GENERAL DIR setting
# If not necessary, don't change this part.
export TARGET_DIR=$(ROOT)/target
export IMAGE_DIR=$(ROOT)/image
export SHARED_DIR=$(ROOT)/shared
export UI_DIR=$(ROOT)/UI
export THIRD_PARTY_DIR=$(ROOT)/third_party
export KERNEL_MODULE_DIR=$(ROOT)/kernel_module
export USER_SPACE_DIR=$(ROOT)/user_space
export FILES_DIR=$(USER_SPACE_DIR)/files
export APPS_DIR=$(USER_SPACE_DIR)/apps
export LIBS_DIR=$(USER_SPACE_DIR)/libs
export SC_LIBS_DIR=$(LIBS_DIR)/public/sc_libs
export BINDIR=$(TARGET_DIR)/usr/sbin
export LIBDIR=$(TARGET_DIR)/usr/lib/
export SDK_ADDON_DIR=$(ROOT)/sdk_addon/

# KERNEL DIR setting
# App's Makefile needs, different project can be difference.
LINUX_DIR=$(ROOT)/../$(SC_SDKPATH)/linux-2.6.36.x
export KERNEL_DIR=$(ROOT)/linux
export KERNEL_SRC=$(KERNEL_DIR)
export KERNEL_ARCH_INC=$(KERNEL_DIR)
export KERNEL_INC=$(KERNEL_DIR)/include
export KERNEL_DRIVERS_DIR=$(KERNEL_DIR)/drivers/net/raeth

# Platform DIR setting
export MTK_DIR_LINUX=$(ROOT)/../$(SC_SDKPATH)/linux-2.6.36.x
export MTK_DIR_USER=$(ROOT)/../$(SC_SDKPATH)/user
export MTK_DIR_GOAHEAD=$(ROOT)/../$(SC_SDKPATH)/user/goahead
export MTK_DIR_HTTPD=$(ROOT)/../$(SC_SDKPATH)/user/lighttpd-1.4.20
export MTK_DIR_CGI=$(ROOT)/../$(SC_SDKPATH)/user/lighttpd-1.4.20/cgi_src

export SQUASHDIR=$(ROOT)/bsp/src/router/squashfs
export COMPRESSDIR=$(ROOT)/bsp/src/router/compressed
export BCMROUTER_DIR=$(ROOT)/bsp/src/router
export BCMINSTALL_DIR=$(ROOT)/bsp/src/router/mipsel-uclibc/install

# filename setting
export ROOT_IMG :=target.squashfs

# platfrom setting

export CROSS_TOOLCHAIN_LOCATION=/opt/buildroot-gcc463_NPTLSHAREDLIBGCC/

export ARCH=mips
export CROSS=mipsel-linux-
export CROSS_COMPILE=$(CROSS)
export CC=$(CROSS)gcc
export CPP=$(CROSS)gcc -E
export CXX=$(CROSS)g++
export LD=$(CROSS)ld
export AR=$(CROSS)ar
export AS=$(CROSS)as
export RANLIB=$(CROSS)ranlib
export STRIP=$(CROSS)strip
CROSS_DIR = $(CROSS_TOOLCHAIN_LOCATION)/usr/bin/

export CROSS_DIR_DIR=$(CROSS_DIR)

TOOLS_DIR = $(ROOT)/tools/
export PATH := $(CROSS_DIR):$(TOOLS_DIR):$(PATH)

