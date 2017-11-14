#
#	TRUNK Makefile 
#	Usage:  make [TARGET] [ANNEX] [VER] [MODULE] [HW_ID] [UR2]
#
export ROOT=$(shell pwd)
BSP_DIR=$(ROOT)/bsp
-include Rules.mak
-include $(ROOT)/Builds/$(ENV_FILE)

all: prepare target lib kernel libi app appi prefs langpkg fs img
	@echo complete

export MT_CODE

# Prepare toolchain and tools
prepare:
	# check if project is using right sdk?
ifneq ($(SC_SDKPATH),$(NEEDED_SDKPATH))
	@echo project $(PROJECT) need sdk $(NEEDED_SDKPATH) but now sdk is using $(SC_SDKPATH)
	@echo change SC_SDKPATH=$(SC_SDKPATH) to SC_SDKPATH=$(NEEDED_SDKPATH)
	@exit 1
endif	

	# update bsp pointer
	rm -rf linux bsp
	ln -sf $(LINUX_DIR)/ linux
	ln -sf $(ROOT)/../$(SC_SDKPATH)/ bsp
	
	# run prepare in bsp for toolchain and anything related to bsp prepare 
	cd $(BSP_DIR) && make -f Platform.mak sdk_prepare
	
	# tools
	make -C $(IMAGE_DIR)/tools
	# check fakeroot exist or not?
	if [ $(firstword $(shell fakeroot -v) xxx) != "fakeroot" ]; then      \
	    echo "Need fakeroot support. Please install it" ; \
	    echo "You can get it from image/tools/src" ; \
	    exit 1;                     \
	else                            \
	    echo "fakeroot OK, but please use version >= 1.12.2.";         \
	    echo fakeroot is now using the one under;which fakeroot; \
	fi
	cd shared/ && rm -rf sc_project_defines.h && ln -sf $(PROJECT_DEFINES_H_FILE) sc_project_defines.h
	cd shared/ && rm -rf if_name.h && ln -sf $(IF_NAME_H_FILE) if_name.h

.PHONY: target
# decompress target.tgz
target:
	rm -Rf target/
	tar -zxvf target.tgz

# Compile and install uClib & toolchain lib
lib:
	#build uClib & lib in bsp
	cd $(BSP_DIR) && make -f Platform.mak sdk_lib


# Compile Kernel
kernel:
	#build linux kernel & modules in bsp
	cd $(BSP_DIR) && make -f Platform.mak sdk_kernel
	make -C kernel_module
	make -C kernel_module install

.PHONY: kernel_module
kernel_module:
	make -C kernel_module
	make -C kernel_module install

# Compile Applications
# todo,temp solution for svn checkout link issue
app:
	ln -sf ../../../../../../kernel_module/public/fw_modules/ipt_http_string.h $(APPS_DIR)/public/iptables/$(IPTABLES_USE)/extensions/ipt_http_string.h
	touch $(LIBS_DIR)/public/$(OPENSSL_USE)/Makefile
	touch $(APPS_DIR)/public/ez-ipupdate-3.0.11b8/Makefile
	touch $(LIBS_DIR)/public/flex-2.5.4/scan.c
	make -C user_space
	#build apps in bsp
	cd $(BSP_DIR) && make -f Platform.mak sdk_apps

# Install broadcom wlan tools and lib to target
libi:
	@echo libi
	
# Install Applications to target
appi:
	cd $(BSP_DIR) && make -f Platform.mak sdk_install
	make -C user_space install
	find $(ROOT)/target/ -name .svn  | xargs rm -rf

#
# Generate files in target/ before make squash image
# Currently we generate these files:
# svn.info/version/md5/currentsetting.htm/pid
#

#need not add WW in version for WW release
ifeq ($(CUSTOMER_NAME),Amped)
VERSION="$(VER) Build $(SUB_VER)"
ETCVERSION="$(VER) Build $(SUB_VER)"
else
ifeq ($(REGION),WW)
VERSION=V$(VER)_$(SUB_VER)
ETCVERSION=$(ANNEX)$(VER)_$(SUB_VER)
else
VERSION=V$(VER)_$(SUB_VER)$(REGION)
ETCVERSION=$(ANNEX)$(VER)_$(SUB_VER)$(REGION)
endif
endif
SVN_INFO=$(TARGET_DIR)/usr/etc/svn.info
CUR_SETTING=$(TARGET_DIR)/www.eng/currentsetting.htm
PID=$(IMAGE_DIR)/$(PROJECT)/$(BOARD_ID).pid
LANG_PID=$(IMAGE_DIR)/$(PROJECT)/$(BOARD_ID)_lang.pid
MAKE_PID=$(IMAGE_DIR)/tools/src/make_PID/make_PID
FW_INFO=$(IMAGE_DIR)/$(PROJECT)/$(BOARD_ID).info

prefs:
	# Generate version
	echo $(ETCVERSION) > $(TARGET_DIR)/usr/etc/version

	# Generate md5
	cd $(TARGET_DIR);\
	echo Firmware version: $(VERSION) > www.eng/md5;\
	find -type f -exec md5sum {} >> www.eng/md5 \;;\
	cd -;

	# Generate currentsetting.htm
	# For test, smartwizard CD request Model name is "WNR2000v2"
	# Now we don't have JWNR3200T CD, so just change this values as we think, maybe change them again when testing with new CD.
	echo "Firmware="$(VERSION)"@region#" > $(CUR_SETTING)
	echo "RegionTag=@model_name#_@region#" >>  $(CUR_SETTING)
	echo "Region=@region#"  >> $(CUR_SETTING)
	echo "Model=@model_name#"  >> $(CUR_SETTING)
	echo "InternetConnectionStatus=@if_wan_up#" >> $(CUR_SETTING)
	if [ "$(OPENDNS)" == "1" ]; then echo "ParentalControlSupported=1" >> $(CUR_SETTING); fi
ifeq ($(SOAP_V2),1)
	echo "SOAPVersion=2.00" >> $(CUR_SETTING)
else
	echo "SOAPVersion=1.13" >> $(CUR_SETTING)
endif
	echo "ReadyShareSupportedLevel=@readysharesupportlevel#" >> $(CUR_SETTING)
ifeq ($(XCLOUD),1)	
	echo "XCloudSupported=1" >> $(CUR_SETTING)
endif
	# Generate pid
ifeq ($(CUSTOMER_NAME),Amped)
	# for amped version style.
	$(MAKE_PID) $(HW_ID) $(ANNEX)1.1.$(VER) $(PRODUCT_ID) $(PID)
else
	$(MAKE_PID) $(HW_ID) $(ANNEX)$(VER) $(PRODUCT_ID) $(PID)
endif

	# Generate language pid
	$(MAKE_PID) $(HW_ID) $(ANNEX)$(GUI_VER) $(PRODUCT_ID) $(LANG_PID)

	# Generate fw info which will bind to trx file
	echo $(VERSION) > $(FW_INFO)
	echo $(REGION) >> $(FW_INFO)

langpkg:
	# Generate GUI language file.
ifneq ($(CUSTOMER_NAME),Amped)
ifeq ($(USE_JEFFS2_FS),0)
	./image/gen_lang.sh $(BOARD_ID) $(USE_JEFFS2_FS) || exit 1
else
	./image/gen_lang.sh $(BOARD_ID) $(USE_JEFFS2_FS) $(FLASH_NAND_BLOCK_SIZE) || exit 1
endif
endif

# Geneate filesystem image from target
FAKEROOT_SH=$(ROOT)/fakeroot.sh

fs:
	@echo -e "\\e[35mSC_TRUNK: make \\e[36mFS \\e[0m"	
	$(STRIP) -d target/lib/modules/*.ko
	
ifeq ($(FAILOVER),1)
	cd target/usr/sbin && ln -sf pppd pppd9 && ln -sf udhcpc udhcpc9
	ls target/usr/etc/ppp/ip-up9 # if this file not exist, ls exit with 1, Makefile stops.
	ls target/usr/etc/udhcpc.failover.script
endif

	#cat $(IMAGE_DIR)/tools/makenode.sh > $(FAKEROOT_SH)
	#chmod +x $(FAKEROOT_SH)
	#$(FAKEROOT_SH) $(TARGET_DIR)/dev $(TARGET_DIR)/usr/dev
	#rm -f $(FAKEROOT_SH)

	#fakeroot id && chown -R root:root target/
	echo 'tar xzf target_usr_dev.tgz && chown -R root:root target/ && cd $$1 && make -f Platform.mak sdk_fs' > $(FAKEROOT_SH)
	chmod +x $(FAKEROOT_SH)
	fakeroot $(FAKEROOT_SH) $(BSP_DIR)
	#fakeroot tar xzf targetdevice.tgz && cd $(BSP_DIR) && make -f Platform.mak sdk_fs
	#cd $(BSP_DIR) && make -f Platform.mak sdk_fs

# Generate image file
img:
	@echo -e "\\e[35mSC_TRUNK: make \\e[36mIMG \\e[0m"	
	cd $(BSP_DIR) && make -f Platform.mak sdk_image
ifeq ($(MT_CODE),1)
	cd $(IMAGE_DIR); ./bb_MT $(ROOT)/Builds/$(PROJECT)/$(PROJECT).mak
else
	cd $(IMAGE_DIR); ./bb $(ROOT)/Builds/$(PROJECT)/$(PROJECT).mak
endif
ifeq ($(BOOT_NAND_FLASH),1)
	cd $(IMAGE_DIR) ; ./bb_dual $(BOARD_ID)
endif
	@echo
	@echo -e "Done! Image $(BOARD_ID).img has been built in $(IMAGE_DIR)/$(BOARD_ID)/."

clean: kernelc appc imgc targetc

# Clean kernel
kernelc:
	cd $(BSP_DIR) && make -f Platform.mak sdk_clean
	make -C kernel_module clean

# Clean apps
appc:
	make -C user_space clean

IMAGE_FILES := linux_lsys.bin linux-lzma.trx linux.trx target.squashfs vmlinuz vmlinuz-lzma wnr2000v3.bin zImage
# Clean generated images
imgc:
	cd $(IMAGE_DIR) ; rm -f $(IMAGE_FILES)
	make -C $(IMAGE_DIR)/tools clean

# Clean target
targetc:
	rm -rf target

# HTTP Upgrade FLASH from telnet window
upgrade:
	cd $(IMAGE_DIR) ; ./upgrade_flash.sh

### make menuconfig
SCRIPTSDIR = $(ROOT)/scripts/kconfig
HOSTCC   = cc	
export HOSTCC SCRIPTSDIR

lxdialog:
	$(MAKE) -C $(SCRIPTSDIR)/lxdialog all

mconf: lxdialog 
	$(MAKE) -C $(SCRIPTSDIR) all

menuconfig: mconf
	python scripts/convert_config.py Builds/$(PROJECT)/$(PROJECT).mak
	cp -rf scripts/defconfig .config
	mkdir -p include
	$(SCRIPTSDIR)/mconf scripts/Config.in
	#rm scripts/defconfig_new
	python scripts/replace.py
	cp -rf scripts/defconfig_new Builds/$(PROJECT)/$(PROJECT).mak

