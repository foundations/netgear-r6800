

CURR_DIR := $(shell pwd)
BUILD_DIR:=$(subst /user_space, /user_space,$(CURR_DIR))
BUILD_DIR:=$(word 1, $(BUILD_DIR))

include $(BUILD_DIR)/Rules.mak


GPL             :=
SUBDIRS         :=

#------------------------------------
ifeq ($(BASIC),1)
GPL             += dnrd-2.19/src
GPL             += mini_httpd/$(MINI_HTTPD_USE)
GPL             += udhcp-0.9.7
GPL             += rp-pppoe
GPL             += bridge-utils
GPL             += iptables/$(IPTABLES_USE)
GPL             += cron
ifneq ($(CUSTOMER_NAME),Amped)
GPL             += iproute-2.6.20-070313
endif
GPL				+= noip-2.1.9-1
GPL             += reaim-0.8
GPL             += miniupnpd_multi/$(MINIUPNPD_USE)
#GPL             += zebra-0.95a
#GPL             += netkit-routedv2-0.1 # D7000 will compile it, but can't find D7000 use it so just not compile
#GPL             += bind-9.2.3 # D7000 will compile it, but can't find D7000 use it so just not compile
endif

ifeq ($(OPENVPN),1)
GPL			+= openvpn/$(OPENVPN_USE)
GPL			+= zip30
endif

ifeq ($(DEBUG_MODE), 1)
GPL             += utelnetd-0.1.9
endif

ifeq ($(USB),1)
GPL             += bftpd/$(BFTPD_USE)
GPL += mtool
GPL += lsof_4.82_src
ifeq ($(EXT4),1)
GPL += e2fsprogs
endif
# mtk will use samba in its sdk, which is better throughput on mtk paltform 
ifneq ($(CHIP_VENDOR),MTK)
GPL += samba-3.0.22
endif
ifeq ($(TIME_MACHINE),1)
GPL             +=apple
GPL += wol
else
ifeq ($(ITUNES_SERVER),1)
GPL             +=apple
endif
endif
GPL                     += hd-idle
GPL                     += hdparm-9.43
ifeq ($(GREEN_DOWNLOAD), 1)
GPL         += wget-1.10.2
ifeq ($(TRANSMISSION), 1)
GPL         += transmission
else
GPL         += ctorrent-dnh3.3.2
endif
endif
ifeq ($(DLNA),1)
GPL += mediaserver/$(MEDIASERVER_USE)
endif
endif

ifeq ($(FBWIFI),1)
#NOTE!!! fbwifi not GPL, for build only!
GPL += fbwifi
endif

ifeq ($(SMTPC),1)
GPL             += smtpc
endif

ifeq ($(SYSLOGD),1)
GPL             += syslogd
endif

ifeq ($(ADSL),1)
GPL             += linux-atm-2.4.0 
endif

# mtk use its own ppp
ifneq ($(CHIP_VENDOR), MTK)
GPL             += ppp-2.4.1.pppoe4.orig
endif

ifeq ($(IPQOS),1)
GPL             += bwping-1.1
endif

ifeq ($(DDNS),1)
GPL             += ez-ipupdate-3.0.11b8
endif

ifeq ($(IPV6),1)
GPL += ip6_radvd/$(RADVD_USE)
GPL += ip6_dhcpv6/$(DHCPV6_USE)
GPL += ip6_totd/$(TOTD_USE)
# use rc lan_reset now
#PRIVATE += ethsw_disable
# only for 6rd, the binary will be Ip so that it can exist together with 2.6.20 ip.
# since current used 2.6.20 iproute NOT have ip tunnel 6rd, and it has many sc_build, hard to merge to 2.6.38, 
# just compile a simple 2.6.33 iproute to support 6rd ip tunnel command.
GPL += iproute2-2.6.38_6rd
endif


#------------------------------------
SUBDIRS += nvram  
ifeq ($(CONENAT),1)
SUBDIRS += cnapt_multi/$(CNAPT_USE)/user/lib 
endif
SUBDIRS += $(GPL)

$(SUBDIRS)::
	@for i in $(SUBDIRS) ; do echo -e "\\e[35mapps: make \\e[36m$$i \\e[0m"; $(MAKE) ARCH=$(ARCH) CROSS_COMPILE=$(CROSS_DIR)$(CROSS_COMPILE)  -C $$i || exit 1; done


all:    $(SUBDIRS)

clean:
	@for i in $(SUBDIRS) ; do echo -e "\\e[35mapps: clean \\e[36m$$i \\e[0m"; $(MAKE) -C $$i clean  || exit 1; done

install:
	@for i in $(SUBDIRS) ; do echo -e "\\e[35mapps: install \\e[36m$$i \\e[0m"; $(MAKE) -C $$i install  || exit 1; done

