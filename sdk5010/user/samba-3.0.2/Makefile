include ../../../sc_trunk/Rules.mak

all: samba

DIR_SOURCE=.
TARGET_DIR=../../../sc_trunk/target
STRIP=$(CROSS)strip

samba:
	make -C $(DIR_SOURCE)/source
	cp $(DIR_SOURCE)/source/bin/smbd $(TARGET_DIR)/usr/sbin/smbd
	$(STRIP) $(TARGET_DIR)/usr/sbin/smbd
	cp $(DIR_SOURCE)/source/bin/smbpasswd $(TARGET_DIR)/usr/sbin/smbpasswd
	$(STRIP) $(TARGET_DIR)/usr/sbin/smbpasswd
	cp $(DIR_SOURCE)/source/bin/nmbd $(TARGET_DIR)/usr/sbin/nmbd
	$(STRIP) $(TARGET_DIR)/usr/sbin/nmbd
	cp $(DIR_SOURCE)/source/bin/CP850.so $(TARGET_DIR)/lib/CP850.so
	cp $(DIR_SOURCE)/source/bin/CP437.so $(TARGET_DIR)/lib/CP437.so
	cp -rf $(DIR_SOURCE)/etc/samba.conf $(TARGET_DIR)/usr/etc/
	test -d $(TARGET_DIR)/usr/etc/samba || mkdir $(TARGET_DIR)/usr/etc/samba
	cp -rf $(DIR_SOURCE)/source/bin/libbigballofmud.so $(TARGET_DIR)/lib/libbigballofmud.so
	ln -snf libbigballofmud.so $(TARGET_DIR)/lib/libbigballofmud.so.0

romfs:
#	$(ROMFSINST) source/bin/CP850.so /lib/CP850.so
#	$(ROMFSINST) source/bin/CP437.so /lib/CP437.so
#	$(ROMFSINST) source/bin/nmbd /sbin/nmbd
#	$(ROMFSINST) source/bin/smbd /sbin/smbd
#	$(ROMFSINST) source/bin/smbpasswd /sbin/smbpasswd
#	$(ROMFSINST) /bin/samba.sh
#	$(ROMFSINST) /bin/samba_add_dir.sh

clean:
	$(MAKE) -C source clean
