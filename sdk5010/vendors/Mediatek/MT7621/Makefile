
include $(ROOTDIR)/../sc_trunk/Rules.mak

KERNELZ = $(IMAGEDIR)/zImage
RAMDISK  = $(IMAGEDIR)/rootfs.bin
# SC_BUILD
IMAGE = $(IMAGEDIR)/uImage.bin
ROMFS_DIR = $(ROOTDIR)/../sc_trunk/target/
SC_IMG_DIR = $(ROOTDIR)/../sc_trunk/image/R6950
LIBGCC = $(shell $(CROSS_COMPILE)gcc -print-file-name=libgcc_s.so.1)

RAMDISK_SIZE  := $(shell echo $(CONFIG_BLK_DEV_RAM_SIZE))
MKIMAGE_DIR = $(ROOTDIR)/user/mkimage
CUR_DIR = $(shell pwd)
COMP = lzma

# SC_BUILD: 
ROMFS_DIRS = \
	tmp \
	bin \
	home \
	usr/etc_ro usr/etc_ro/web usr/etc_ro/ppp usr/etc_ro/ppp/peers usr/etc_ro/ppp/plugins usr/etc_ro/ppp/3g \
	usr/etc_ro/wlan usr/etc_ro/Wireless usr/etc_ro/xml usr/etc_ro/usb \
	usr/etc_ro/lighttpd \
	usr/etc_ro/linuxigd usr/etc_ro/Wireless/RT2860AP usr/etc_ro/Wireless/RT61AP usr/etc_ro/Wireless/iNIC \
	usr/etc_ro/Wireless/WIFI3 \
	lib lib/modules lib/ipsec  \
	proc \
	sbin \
	sys \
	media \
	usr usr/bin usr/sbin usr/codepages 

clean:
	rm -f mkimage

romfs: 
	[ -d $(ROMFSDIR)/$$i ] || mkdir -p $(ROMFSDIR)
	for i in $(ROMFS_DIRS); do \
		[ -d $(ROMFSDIR)/$$i ] || mkdir -p $(ROMFSDIR)/$$i; \
	done
	#generate device node
#ifneq	($(LINUXDIR),linux-2.4.x)
#	@if [ \! -d $(ROMFSDIR)/dev ];then mkdir -p $(ROMFSDIR)/dev;fi;
#	./makedevlinks $(ROMFSDIR)
#endif
	#$(ROMFSINST) /usr/etc_ro/rcS
	#$(ROMFSINST) /usr/etc_ro/inittab
	#$(ROMFSINST) /usr/etc_ro/motd
	#$(ROMFSINST) /usr/etc/fstab
 
 # SC_BUILD: these are for wireless, need double check after wireless driver is built in
 
 	#$(ROMFSINST) /usr/etc_ro/wlan/MT7620_AP_2T2R-4L_V15.BIN.PR
 	#$(ROMFSINST) /usr/etc_ro/wlan/MT7620_AP_2T2R-4L_V15.BIN.CE_RU
 
 	#$(ROMFSINST) /usr/etc_ro/Wireless/iNIC/MT7610E-V10-20130102-FEM.bin.PR
 	#$(ROMFSINST) /usr/etc_ro/Wireless/iNIC/MT7610E-V10-20130102-FEM.bin.CE_RU
 
 	#cp -f SingleSKU.dat.MT7620 $(ROMFSDIR)/usr/etc_ro/Wireless/RT2860AP/SingleSKU.dat.MT7620
 	#cp -f SingleSKU.dat.MT7610E $(ROMFSDIR)/usr/etc_ro/Wireless/iNIC/SingleSKU.dat.MT7610E
 
	$(ROMFSINST) /usr/etc_ro/Wireless/RT2860AP/RT2860_default_novlan
	$(ROMFSINST) /usr/etc_ro/Wireless/RT2860AP/RT2860_default_vlan
	$(ROMFSINST) /usr/etc_ro/Wireless/iNIC/RT2860AP.dat
 	#for sigma test
	$(ROMFSINST) -e CONFIG_RALINKAPP_HOTSPOT /usr/etc_ro/Wireless/RT2860AP/RT2860_def.dat
 # SC_BUILD for wireless end
  
#	$(ROMFSINST) /etc_ro/rept_test.sh
#	$(ROMFSINST) /etc_ro/rept_dbdc_test.sh

	rm -f "MT7612E_EEPROM_layout_20131121_2G_ePAeLNA_TXTC_off.bin" \
		"MT7612E_EEPROM_layout_20131121_5G_ePAeLNA_TXTC_off.bin" \
		"MT7612E_EEPROM_layout_20131121_2G_ePAeLNAcoclock_TXTC_off.bin" \
		"MT7612E_EEPROM_layout_20131121_5G_ePAeLNAcoclock_TXTC_off.bin" \
		"MT7612E_EEPROM_layout_20131121_2G_iPAeLNA_wTSSI default slope offset.bin" \
		"MT7612E_EEPROM_layout_20131121_5G_iPAeLNA_wTSSI default slope offset.bin" \
		"MT7612E_EEPROM_layout_20131121_2G_iPAeLNAcoclock_wTSSI default slope offset.bin" \
		"MT7612E_EEPROM_layout_20131121_5G_iPAeLNAcoclock_wTSSI default slope offset.bin" \
		"MT7612E_EEPROM_layout_20131121_2G_iPAiLNA_wTSSI default slope offset.bin" \
		"MT7612E_EEPROM_layout_20131121_2G_iPAiLNAcoclock_wTSSI default slope offset.bin"
ifneq	($(findstring 7612, $(CONFIG_RT_FIRST_CARD)$(CONFIG_RT_SECOND_CARD)),)
	ln -s "MT7612E_EEPROM_layout_20131121_2G5G_ePAeLNA_TXTC_off.bin" \
		"MT7612E_EEPROM_layout_20131121_2G_ePAeLNA_TXTC_off.bin"
	ln -s "MT7612E_EEPROM_layout_20131121_2G5G_ePAeLNA_TXTC_off.bin" \
		"MT7612E_EEPROM_layout_20131121_5G_ePAeLNA_TXTC_off.bin"
	ln -s "MT7612E_EEPROM_layout_20131121_2G5G_ePAeLNAcoclock_TXTC_off.bin" \
		"MT7612E_EEPROM_layout_20131121_2G_ePAeLNAcoclock_TXTC_off.bin"
	ln -s "MT7612E_EEPROM_layout_20131121_2G5G_ePAeLNAcoclock_TXTC_off.bin" \
		"MT7612E_EEPROM_layout_20131121_5G_ePAeLNAcoclock_TXTC_off.bin"
	ln -s "MT7612E_EEPROM_layout_20131121_2G5G_iPAeLNA_wTSSI default slope offset.bin" \
		"MT7612E_EEPROM_layout_20131121_2G_iPAeLNA_wTSSI default slope offset.bin"
	ln -s "MT7612E_EEPROM_layout_20131121_2G5G_iPAeLNA_wTSSI default slope offset.bin" \
		"MT7612E_EEPROM_layout_20131121_5G_iPAeLNA_wTSSI default slope offset.bin"
	ln -s "MT7612E_EEPROM_layout_20131121_2G5G_iPAeLNAcoclock_wTSSI default slope offset.bin" \
		"MT7612E_EEPROM_layout_20131121_2G_iPAeLNAcoclock_wTSSI default slope offset.bin"
	ln -s "MT7612E_EEPROM_layout_20131121_2G5G_iPAeLNAcoclock_wTSSI default slope offset.bin" \
		"MT7612E_EEPROM_layout_20131121_5G_iPAeLNAcoclock_wTSSI default slope offset.bin"
	ln -s "MT7612E_EEPROM_layout_20131121_5G_iPAiLNA_wTSSI default slope offset.bin" \
		"MT7612E_EEPROM_layout_20131121_2G_iPAiLNA_wTSSI default slope offset.bin"
	ln -s "MT7612E_EEPROM_layout_20131121_5G_iPAiLNAcoclock_wTSSI default slope offset.bin" \
		"MT7612E_EEPROM_layout_20131121_2G_iPAiLNAcoclock_wTSSI default slope offset.bin"
endif
	rm -f *_EEPROM.bin
ifeq	($(CONFIG_CO_CLOCK_SUPPORT), y)
# Fisrt wifi interface - 2.4G
ifneq	($(CONFIG_RT_FIRST_CARD),)
	rm -f MT$(shell echo $(CONFIG_RT_FIRST_CARD) | tr a-z A-Z)_EEPROM.bin
ifeq	($(CONFIG_FIRST_CARD_EXTERNAL_PA),)
ifeq	($(CONFIG_FIRST_CARD_EXTERNAL_LNA),)
	# iPA/iLNA
	ln -s "MT$(shell echo $(CONFIG_RT_FIRST_CARD) | tr a-z A-Z)_EEPROM_layout_20131121_2G_iPAiLNAcoclock_wTSSI default slope offset.bin" \
		MT$(shell echo $(CONFIG_RT_FIRST_CARD) | tr a-z A-Z)_EEPROM.bin
else
	# iPA/eLNA
	ln -s "MT$(shell echo $(CONFIG_RT_FIRST_CARD) | tr a-z A-Z)_EEPROM_layout_20131121_2G_iPAeLNAcoclock_wTSSI default slope offset.bin" \
		MT$(shell echo $(CONFIG_RT_FIRST_CARD) | tr a-z A-Z)_EEPROM.bin
endif
else
	# ePA/eLNA, ePA/iLNA
	ln -s "MT$(shell echo $(CONFIG_RT_FIRST_CARD) | tr a-z A-Z)_EEPROM_layout_20131121_2G_ePAeLNAcoclock_TXTC_off.bin" \
		MT$(shell echo $(CONFIG_RT_FIRST_CARD) | tr a-z A-Z)_EEPROM.bin
endif
endif
# Second wifi interface - 5G
ifneq	($(CONFIG_RT_SECOND_CARD),)
	rm -f MT$(shell echo $(CONFIG_RT_SECOND_CARD) | tr a-z A-Z)_EEPROM.bin
ifeq	($(CONFIG_SECOND_CARD_EXTERNAL_PA),)
ifeq	($(CONFIG_SECOND_CARD_EXTERNAL_LNA),)
	# iPA/iLNA
	ln -s "MT$(shell echo $(CONFIG_RT_SECOND_CARD) | tr a-z A-Z)_EEPROM_layout_20131121_5G_iPAiLNAcoclock_wTSSI default slope offset.bin" \
		MT$(shell echo $(CONFIG_RT_SECOND_CARD) | tr a-z A-Z)_EEPROM.bin
else
	# iPA/eLNA
	ln -s "MT$(shell echo $(CONFIG_RT_SECOND_CARD) | tr a-z A-Z)_EEPROM_layout_20131121_5G_iPAeLNAcoclock_wTSSI default slope offset.bin" \
		MT$(shell echo $(CONFIG_RT_SECOND_CARD) | tr a-z A-Z)_EEPROM.bin
endif
else
	# ePA/eLNA, ePA/iLNA
	ln -s "MT$(shell echo $(CONFIG_RT_SECOND_CARD) | tr a-z A-Z)_EEPROM_layout_20131121_5G_ePAeLNAcoclock_TXTC_off.bin" \
		MT$(shell echo $(CONFIG_RT_SECOND_CARD) | tr a-z A-Z)_EEPROM.bin
endif
endif
else # non co-Clock
# Fisrt wifi interface - 2.4G
ifneq	($(CONFIG_RT_FIRST_CARD),)
	rm -f MT$(shell echo $(CONFIG_RT_FIRST_CARD) | tr a-z A-Z)_EEPROM.bin
ifeq	($(CONFIG_FIRST_CARD_EXTERNAL_PA),)
ifeq	($(CONFIG_FIRST_CARD_EXTERNAL_LNA),)
	# iPA/iLNA
	ln -s "MT$(shell echo $(CONFIG_RT_FIRST_CARD) | tr a-z A-Z)_EEPROM_layout_20131121_2G_iPAiLNA_wTSSI default slope offset.bin" \
		MT$(shell echo $(CONFIG_RT_FIRST_CARD) | tr a-z A-Z)_EEPROM.bin
else
	# iPA/eLNA
	ln -s "MT$(shell echo $(CONFIG_RT_FIRST_CARD) | tr a-z A-Z)_EEPROM_layout_20131121_2G_iPAeLNA_wTSSI default slope offset.bin" \
		MT$(shell echo $(CONFIG_RT_FIRST_CARD) | tr a-z A-Z)_EEPROM.bin
endif
else
	# ePA/eLNA, ePA/iLNA
	ln -s "MT$(shell echo $(CONFIG_RT_FIRST_CARD) | tr a-z A-Z)_EEPROM_layout_20131121_2G_ePAeLNA_TXTC_off.bin" \
		MT$(shell echo $(CONFIG_RT_FIRST_CARD) | tr a-z A-Z)_EEPROM.bin
endif
endif
# Second wifi interface - 5G
ifneq	($(CONFIG_RT_SECOND_CARD),)
	rm -f MT$(shell echo $(CONFIG_RT_SECOND_CARD) | tr a-z A-Z)_EEPROM.bin
ifeq	($(CONFIG_SECOND_CARD_EXTERNAL_PA),)
ifeq	($(CONFIG_SECOND_CARD_EXTERNAL_LNA),)
	# iPA/iLNA
	ln -s "MT$(shell echo $(CONFIG_RT_SECOND_CARD) | tr a-z A-Z)_EEPROM_layout_20131121_5G_iPAiLNA_wTSSI default slope offset.bin" \
		MT$(shell echo $(CONFIG_RT_SECOND_CARD) | tr a-z A-Z)_EEPROM.bin
else
	# iPA/eLNA
	ln -s "MT$(shell echo $(CONFIG_RT_SECOND_CARD) | tr a-z A-Z)_EEPROM_layout_20131121_5G_iPAeLNA_wTSSI default slope offset.bin" \
		MT$(shell echo $(CONFIG_RT_SECOND_CARD) | tr a-z A-Z)_EEPROM.bin
endif
else
	# ePA/eLNA, ePA/iLNA
	ln -s "MT$(shell echo $(CONFIG_RT_SECOND_CARD) | tr a-z A-Z)_EEPROM_layout_20131121_5G_ePAeLNA_TXTC_off.bin" \
		MT$(shell echo $(CONFIG_RT_SECOND_CARD) | tr a-z A-Z)_EEPROM.bin
endif
endif
endif

	#rm -rf MT7603E_EEPROM.bin MT7612E_EEPROM.bin
	#cp MT7603E_HW_20140812.bin MT7603E_EEPROM.bin
	#cp MT7612E_HW_20140812.bin MT7612E_EEPROM.bin 

	rm -rf MT7603E.bin MT7612E_EEPROM.bin MT7612E_EEPROM.bin.11ac MT7612E_EEPROM.bin.11n
	cp MT7603E_HW_20140826.bin MT7603E.bin
	cp MT7612E_HW_20140915.bin MT7612E_EEPROM.bin.11ac
	cp MT7612E_HW_20140918_WNDR3700v5.bin MT7612E_EEPROM.bin.11n

	#if [ -f MT$(shell echo $(CONFIG_RT_FIRST_CARD) | tr a-z A-Z)_EEPROM.bin ]; then \
	#	$(ROMFSINST) /usr/etc_ro/wlan/MT$(shell echo $(CONFIG_RT_FIRST_CARD) | tr a-z A-Z)_EEPROM.bin; \
	#fi
	if [ -f MT$(shell echo $(CONFIG_RT_FIRST_CARD) | tr a-z A-Z).bin ]; then \
		$(ROMFSINST) /usr/etc_ro/wlan/MT$(shell echo $(CONFIG_RT_FIRST_CARD) | tr a-z A-Z).bin; \
	fi
	
	if [ -f MT$(shell echo $(CONFIG_RT_SECOND_CARD) | tr a-z A-Z)_EEPROM.bin.11ac ]; then \
		$(ROMFSINST) /usr/etc_ro/wlan/MT$(shell echo $(CONFIG_RT_SECOND_CARD) | tr a-z A-Z)_EEPROM.bin.11ac; \
		$(ROMFSINST) /usr/etc_ro/wlan/MT$(shell echo $(CONFIG_RT_SECOND_CARD) | tr a-z A-Z)_EEPROM.bin.11n; \
	fi

	#cp -f $(SC_2G_SINGLESKU)  $(ROMFSDIR)/usr/etc_ro/Wireless/RT2860AP/SingleSKU.dat.MT7620
	#cp -f $(SC_5G_SINGLESKU)  $(ROMFSDIR)/usr/etc_ro/Wireless/iNIC/SingleSKU.dat.MT7610E
	mkdir -p $(ROMFSDIR)/usr/etc_ro/Wireless/singlesku_dat/
	cp -af $(SC_SINGLE_SKU_DIR)/* $(ROMFSDIR)/usr/etc_ro/Wireless/singlesku_dat/

romfs.post:: 
	[ -d $(IMAGEDIR) ] || mkdir -p $(IMAGEDIR)
	
	#Squash File System
	 rm -f $(RAMDISK)  
ifeq	($(LINUXDIR),linux-2.4.x)
	$(CONFIG_CROSS_COMPILER_PATH)/mksquashfs_lzma-3.0 $(ROMFSDIR) $(RAMDISK)
else
	#linux2.6.x support squashfs with lzma
ifeq	($(CONFIG_BLK_DEV_INITRD),)

ifeq	($(LINUXDIR),linux-2.6.21.x)
	$(CONFIG_CROSS_COMPILER_PATH)/mksquashfs_lzma-3.2 $(ROMFSDIR) $(RAMDISK)
endif
ifeq	($(LINUXDIR),linux-2.6.36MT.x)
	$(CONFIG_CROSS_COMPILER_PATH)/mksquashfs_lzma-4.2 $(ROMFSDIR) $(RAMDISK)
endif
# SC_BUILD
ifeq	($(LINUXDIR),linux-2.6.36.x)
	# clean up
	rm -rf `find $(ROMFSDIR) -name .svn`
# sync device smp.sh to sdk
	[ -f $(ROMFSDIR)/usr/etc/mtk/mt7621_smp.sh ] && cp -rf $(ROMFSDIR)/usr/etc/mtk/mt7621_smp.sh $(ROMFSDIR)/usr/sbin/smp.sh

	@echo ROMFSDIR=$(ROMFSDIR) 
	@echo SC_IMG_DIR=$(SC_IMG_DIR)
	@echo ROOTDIR=$(ROOTDIR)
	$(ROMFSDIR)/../image/tools/src/squashfs4.2/squashfs-tools/mksquashfs $(ROMFSDIR) $(RAMDISK)
endif
endif
endif

ifeq	($(CONFIG_BLK_DEV_INITRD),y)
ifeq	($(LINUXDIR),linux-2.4.x)
	gzip -9 -f $(RAMDISK)
	cp $(RAMDISK).gz $(ROOTDIR)/$(LINUXDIR)/arch/mips/ramdisk/ramdisk.gz 2>/dev/null
endif
endif


mkimage: $(MKIMAGE_DIR)/crc32.c $(MKIMAGE_DIR)/mkimage.c 
	$(HOSTCC) $(HOST_CFLAGS) -DUSE_HOSTCC -o $@ -I$(MKIMAGE_DIR)/include $^ 

image: mkimage
	$(CROSS_COMPILE)objcopy -O binary -R .note -R .comment -S $(ROOTDIR)/$(LINUXDIR)/vmlinux  $(KERNELZ)
	cd $(IMAGEDIR) ; rm -f $(KERNELZ).*; $(COMP) -9 -f -S .$(COMP) $(KERNELZ)

ifeq	($(CONFIG_BLK_DEV_INITRD),)
ifeq	($(CONFIG_ROOTFS_IN_FLASH_NO_PADDING),)
	@#============================================================
	@# uboot hdr(64B) + mtdblock0(192KB) + mtdblock1(64KB) + 
	@# mtdblock2(64KB) + mtdblock3 + mtdblock4 + mtdblock5
	@# 
	@# Padding size of mtdblock3 is equal to "MTD_KRN_PART_SIZE 
	@# - 64B - the size of compressed kernel image"
	@#
	@# In Other Words:
	@# MTD Kernel Partition Size is MUST bigger than 
	@# "Original Kernel Image Size + 64B"
	@#============================================================
	#========<< This is README not Warning Message>>==============
	# Please make sure that original kernel image size can't 
	# bigger or equal to Kernel Partition Size. 
	#
	# For Example:
	# 
	# Original kernel image size is 563421 Bytes(0x898DD Bytes).
	# Flash sector size is 0x10000, so MTD kernel 
	# partition size MUST bigger than 0x90000.
	#
	# #make menuconfig
	#
	# Kernel/Library/Defaults Selection  --->
	#   [*] Customize Kernel Settings (NEW)
	#      Machine selection  --->
	#        (90000) MTD Kernel Partition Size (Unit:Bytes)
	#=============================================================
	# Original Kernel Image Size
	  @wc -c $(KERNELZ).$(COMP)
	# Padded Kernel Image Size 
ifeq    ($(findstring 0x, $(CONFIG_MTD_KERNEL_PART_SIZ)),0x)
	  @SIZE=`wc -c $(KERNELZ).$(COMP) | awk '{ print $$1 }'` ; \
	   MTD_KRN_PART_SIZE=`printf "%d" $(CONFIG_MTD_KERNEL_PART_SIZ)` ; \
	   PAD=`expr $$MTD_KRN_PART_SIZE - 64 - $$SIZE` ; \
	   dd if=/dev/zero count=1 bs=$$PAD 2> /dev/null | tr \\000 \\377 >> $(KERNELZ).$(COMP)
else
	  @SIZE=`wc -c $(KERNELZ).$(COMP) | awk '{ print $$1 }'` ; \
	   MTD_KRN_PART_SIZE=`printf "%d" 0x$(CONFIG_MTD_KERNEL_PART_SIZ)` ; \
	   PAD=`expr $$MTD_KRN_PART_SIZE - 64 - $$SIZE` ; \
	   dd if=/dev/zero count=1 bs=$$PAD 2> /dev/null | tr \\000 \\377 >> $(KERNELZ).$(COMP)
endif
endif
	@wc -c $(KERNELZ).$(COMP)
	# Original RootFs Size 
	  @du -sb $(ROMFSDIR)
	# Compressed RootFs Size 
	  @wc -c $(RAMDISK)
	# Padded Kernel Image + Compressed Rootfs Size
# SC_BUILD
#  @cat $(RAMDISK) >> $(KERNELZ).$(COMP) 
	  @wc -c $(KERNELZ).$(COMP)
	#===========================================
endif

ifeq	($(CONFIG_ROOTFS_IN_FLASH_NO_PADDING),y)
	@cd $(IMAGEDIR) ; \
	ISIZE=`wc -c $(KERNELZ).$(COMP) | awk '{print $$1}'` ; \
	RSIZE=`wc -c $(RAMDISK) | awk '{print $$1}'` ; \
	KRN_SIZE=`expr $$ISIZE - $$RSIZE + 64` ; \
	ENTRY=`readelf -h $(ROOTDIR)/$(LINUXDIR)/vmlinux | grep "Entry" | awk '{print $$4}'` ; \
	$(CUR_DIR)/mkimage -A mips -O linux -T kernel -C $(COMP) -a 80001000 -e $$ENTRY -k $$KRN_SIZE -n "Linux Kernel Image" -d $(KERNELZ).$(COMP) $(IMAGE)
else
ifneq    ($(CONFIG_KERNEL_START_ADDR),)
	@cd $(IMAGEDIR) ; $(CUR_DIR)/mkimage -A mips -O linux -T kernel -C $(COMP) -a $(CONFIG_KERNEL_START_ADDR) -e $(shell readelf -h $(ROOTDIR)/$(LINUXDIR)/vmlinux | grep "Entry" | awk '{print $$4}') -n "Linux Kernel Image"  -d $(KERNELZ).$(COMP) $(IMAGE)
else
	@cd $(IMAGEDIR) ; $(CUR_DIR)/mkimage -A mips -O linux -T kernel -C $(COMP) -a 80001000 -e $(shell readelf -h $(ROOTDIR)/$(LINUXDIR)/vmlinux | grep "Entry" | awk '{print $$4}') -n "Linux Kernel Image"  -d $(KERNELZ).$(COMP) $(IMAGE)
endif
endif
# SC_BUILD
	cp $(IMAGE) $(SC_IMG_DIR)
	cp $(RAMDISK) $(SC_IMG_DIR)
