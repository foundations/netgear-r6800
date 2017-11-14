#SC_BUILD
#Based on BSP Makefile's all targets and related to BSP
#sdk_prepare/sdk_lib/sdk_kernel/sdk_apps/sdk_install/sdk_image/sdk_clean
#will be excuted at different stages by root Makefile
#
-include Makefile
-include ../sc_trunk/Rules.mak

sdk_prepare: toolchain   
		ln -sf $(ROOT)/shared/if_name.h $(CONFIG_LINUXDIR)/include/sc/if_name_link_to_shared.h
		cd linux-2.6.36.x/drivers/net/wireless/mt_wifi_ap/; cp -af mt_wifi.bk mt_wifi.o 
		cd linux-2.6.36.x/net/nat/hw_nat/; cp -af hw_nat.bk hw_nat.o 
		cd linux-2.6.36.x/net/nat/foe_hook/; cp -af foe_hook.bk foe_hook.o 
		@echo sc_prepare complete

sdk_lib:  $(UCLIBC_PLUS_LIB_ONLY) $(UCLIBC_LIB_ONLY) lib_only
	# the mtk prebuilt uClibc not have __adjtimex which is needed by ntp
	# also not have getpt which is needed by utelnetd.
	sudo cp -rf uClibc-0.9.33.2/lib/libuClibc-0.9.33.2.so $(CROSS_TOOLCHAIN_LOCATION)/usr/mipsel-buildroot-linux-uclibc/sysroot/lib/libuClibc-0.9.33.2.so 	
	# some app like KC_PRINT complain need this file.
	cp -arf $(CROSS_TOOLCHAIN_LOCATION)/usr/mipsel-buildroot-linux-uclibc/lib/libgcc* $(TARGET_DIR)/lib/
	# XCLOUD need this file.
	cp $(CROSS_TOOLCHAIN_LOCATION)/usr/mipsel-buildroot-linux-uclibc/sysroot/usr/lib/libstdc++.so.6 $(TARGET_DIR)/lib/
	@echo q

sdk_dep: dep

sdk_kernel: linux modules
	# Install modules into filesystem
	if grep -q "CONFIG_MODULES=y" $(KERNEL_DIR)/.config ; then \
            make -C $(KERNEL_DIR) modules_install DEPMOD=/bin/true INSTALL_MOD_PATH=$(TARGET_DIR) ; \
	fi
	@echo linux kernel and modules done

sdk_apps: user_only
	make -C user/busybox 
	make -C user/busybox install
	@echo sdk_apps done

sdk_install: sc_if_name_sh romfs.subdirs modules_install
	@echo sdk_install done

sdk_fs: romfs.post
	@echo fs done

sdk_image: image
	@echo image done
	
toolchain:
	[ -d $(CROSS_DIR_DIR) ] || tar jxf $(ROOT)/toolchain/buildroot-gcc463_32bits.tar.bz2 -C /opt/

# no need enable it on this project since no ubifs needed.
# a fixup for sdk e2fsprogs which is for ubifs.
e2fsprogs:
	[ -f lib/e2fsprogs-1.41.3/lib/libe2p.a ] || \
	( cd lib/e2fsprogs-1.41.3; make clean; CC=$(CC) ./configure --host=mipsel-linux --enable-static --disable-shared )

#bsp' Makefile will generate kernel config
#We don't need this way after bsp is merged with sercomm
#we just usr a static $(SC_SDKPATH)_config instead
#so define a sc_config to cover over .config

sc_if_name_sh:
	cd $(SHARED_DIR) && sh sc_setconfig.sh if_name.h;\
	if [ -f sc_if_name_config.sh ]; then \
	   	chmod +x sc_if_name_config.sh; \
	   	cp -f sc_if_name_config.sh $(ROOT)/target/usr/sbin/sc_if_name_config.sh; \
	fi
	
sdk_clean:
	$(MAKE) clean

