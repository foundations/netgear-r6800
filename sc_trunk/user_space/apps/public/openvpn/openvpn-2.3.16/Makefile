CURR_DIR := $(shell pwd)
BUILD_DIR:=$(subst /user_space, /user_space,$(CURR_DIR))
BUILD_DIR:=$(word 1, $(BUILD_DIR))

include $(BUILD_DIR)/Rules.mak

PKG_NAME=openvpn-2.3.16.tar.gz
SVN_URL=http://sdcsvn.sdc.sercomm.com/svn/NTGR_TRUNK_CODE/trunk/warehouse/app
DL_TO=$(shell pwd)/dl
DECOMP_TO=$(shell pwd)
SRC=src

all: touch_makefile
	$(MAKE) -C $(SRC)
touch_makefile:
	find . -name Makefile | xargs touch
	
clean:
	$(MAKE) -C $(SRC) clean
install:
	$(MAKE) -C $(SRC) install
	
