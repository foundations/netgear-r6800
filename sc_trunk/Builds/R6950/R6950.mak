#
# Automatically generated make config: don't edit
# version: 
# Thu Oct 22 13:17:23 2015
#

#
# [ PROJECT Basic Configuration ]
#

#
# Make PID need HW_ID VER PRODUCT_ID
#
BOARD_ID=R6950
HW_ID=BZV
VER=1.1.0.42
SUB_VER=1.0.1
GUI_VER=1.1.0.42
HW_TYPE=R6950
PRODUCT_ID=A001
REGION=WW
ANNEX=V

#
# [ PROJECT SDK / CPU / DSL / FLASH / CUSTOMER Configuration ]
#
NEEDED_SDKPATH=sdk5010
NEEDED_SDK_ADDON=R6950
NEEDED_PATCH_PATH=patchs
NEEDED_CPU=CPU_MT7621
CHIP_VENDOR=MTK

#
# Do we have ADSL WAN? 1-Yes 0-No
#
ADSL=0
ADSL_VENDOR=
FLASH=128M
BOOT_NAND_FLASH=0
FLASH_NAND_BLOCK_SIZE=131072
MTK_NAND_FLASH_API=0
CUSTOMER_NAME=Netgear
CUSTOMER_ID=72
SVN_REPOSITORY_URL=http://172.21.67.238/svn/
SDK_PKG_SVN_URL=$(SVN_REPOSITORY_URL)/NTGR_TRUNK_CODE/trunk/warehouse/sdk
SDK_PKG=MediaTek_APSoC_SDK5010_4410cp1ntgr20170331_20170401.tgz
SDK_DIR=sdk5010
TOOLCHAIN_DIR=buildroot-gcc463_NPTLSHAREDLIBGCC
TOOLCHAIN_DECOMP_PATH=/opt
TOOLCHAIN_PKG_SVN_URL=$(SVN_REPOSITORY_URL)/NTGR_TRUNK_CODE/trunk/warehouse/toolchain
TOOLCHAIN_PKG=buildroot-gcc463_NPTLSHAREDLIBGCC_32bits.tar.bz2
TOOLCHAIN_PKT_SRC=buildroot-gcc463-src-offlinebuild_NPTLSHAREDLIBGCC.tar.bz2
APP_PKG_SVN_URL=$(SVN_REPOSITORY_URL)/NTGR_TRUNK_CODE/trunk/warehouse/app
LINUX_PLATFORM=mipsel-linux
#
# [ PROJECT TYPE Configuration ]
#
WIFI_ROUTER_APCLI=1
WIFI_REPEATER_APCLI=0
APCLI=1
MT_CODE=0

#
# [ PROJECT Multi Versions Configuration ]
#

#
# < Files that differents between projects >
#
ENV_FILE=R6950/env.mak
MOUNT_SH_FILE=mount.sh.r6220
INITTAB_FILE=inittab.normal
DEFAULT_FILE=default.r6950
PROJECT_DEFINES_H_FILE=sc_project_defines_r6950.h
IF_NAME_H_FILE=if_name_gmac2.h
SC_SDK_CONFIG_FILE=sdk5010_config
SC_KERNEL_CONFIG_FILE=sdk5010_kernel.config
SC_SINGLE_SKU_DIR=r6220_singlesku_02261739
RC_BOARD_INIT_FILE=board_init.c_r6950
RC_WLAN_USE=wifi_mtk
RC_IGMP_FILE=igmp.c_r6220
RC_TRAFFICMETER_FILE=tm.c_r6220
RC_WAN_INTERFACE_SETUP=eth_ctl_mtk.x
RC_LAN_INTERFACE_SETUP=device_lan_mtk.x
RC_DSL_SETUP=dsl_ctl_null.x

# wifi eeprom 
REPLACE_EEPROM_BIN=1
# one of the iPAiLNA/ePAeLNA
WIFI_2G_PALNA_TYPE=iPAiLNA
WIFI_5G_PALNA_TYPE=ePAeLNA
WIFI_2G_EEPROM_FILE=R6800_2G_EEPROM_20170105.bin
WIFI_5G_EEPROM_FILE=R6800_5G_EEPROM_20170105.bin

#
# < Apps multi versions support >
#
MINI_HTTPD_USE=mini_httpd-1.24
MINIUPNPD_USE=miniupnpd-20080427_R6220
SCFGMGR_USE=scfgmgr_R6220
DETECTWAN_USE=detectwan_spec1.9_R6220
IPTABLES_USE=iptables-1.4.10
LED_APP_USE=R6950
LED_KERNEL_USE=R6950
BFTPD_USE=bftpd-4.4
OPENVPN_USE=openvpn-2.3.16
IPROUTE_USE=iproute2-4.6.0
RADVD_USE=radvd-2.13
TOTD_USE=totd-1.5.3
DHCPV6_USE=dhcpv6_R6220
SPEEDTEST_USE=speedtest-2.0
WIZARD_ICON_USE=wizard_R6800

#
# < 3rd_parth multi versions support >
#
KCODES_USE=kcodes_R6950
BONJOUR_USE=bonjour_R6950

#
# [ PROJECT FEATURES ]
#
BASIC=1
WIFI=1
# NTGR smart connect band steering
WIFI_SMART_CONNECT=0
USB=1
USB_PRINTER=1
EXT4=1
IPMAC=0
MULTI_PVC=0
VPN=0
SNMP=0
DNSHJ=1
VOIP=0
CA=1
POT=1
DPF=0
LLTD=1
IGMP=1
PPTPC=1
L2TP=1
BCM_WPS=1
MSSID=1
WIZARD_LOG=1
SETUPWIZARD=1
SETUPWIZARD_USE=SetupWizard_3.0_spec_3.00
PORTTRIGGER=1
PORTFORWARD=1
HTTPS=1
SMTP_SSL=1
IPV6=1
HTTPD_IPV6_SUPPORT=1
IP6_6RD_CE=0
IPV6_LOGO=0
IPQOS=1
TURBO_VIDEO=1
TR069=0
TRAFFIC_METER=1
TRAFFIC_METER_USE=traffic_meter_R6220
TM_DEVPOLL=0
WIFI_ISOLATION=1

#
# Netgear Router Debugging Moe -- Spec V1.9
#
DEBUG_MODE=1

#
# Hide password in config file
#
HIDDEN_PASSWORD=1
GREEN_DOWNLOAD=0
US_ONLY=0
AUTOUPG=1
MODIFYMAC=0
OPENDNS=1
CONENAT=1
CNAPT_USE=cnapt_R6220
IP_ASSIGN_CHK=1
DUAL_WAN=0
DUAL_BAND=1
SCFGMGR=1
SMTPC=1
SYSLOGD=1
DDNS=1

#
# ----------Block Site-----------
#
ALL_TCP_CHECK=1

#
# ------Apple time machine-------
#
TIME_MACHINE=1

#
# iTunes server, depends on TIME_MACHINE
#
ITUNES_SERVER=1

#
# ---------genie GUI------------
#
GENIE_GUI=1

#
# ------------DLNA----------------
#
DLNA=1
USE_MINIDLNA=1
MEDIASERVER_USE=mediaserver_R6220

#
# ----------BT client--------------
#
TRANSMISSION=0
NEW_TRANSMISSION=0
RU_SPEC=1
OPENSSL_USE=openssl-1.0.2h

#
# ---------LED control settings----
#
LED_CONTROL=1

#
# -----------3G Feature------------
#
3G_FEATURE=0
DETECT_SIM_INFO_V1=0
ZIP_3G=0
FAILOVER=0
NZ_FEATURE=0

#
# ------------WIFI AP Mode-------------
#
WIFI_AP=1

#
# ------------DEVICE MODE---------------
#
DEVICE_MODE_CONTROL=0

#
# ---------------IPTV---------------------
#
ENABLE_IPTV=0
MIIICASA=0
WPA_PSK=0
NTGR_CLOUD=0

#
# -------Follow Soap API spec 2.00---------
#
SOAP_V2=1

#
# -------------Facebook WiFi---------------
#
FBWIFI=0
VLAN_SUPPORT=1

#
# ---------AUTO IP for Spec Recv 13---------
#
AUTOIP=1
OPENVPN=1

#
# ------------Access Control-----------------
#
ACCESSCNTL=1
VLAN=1
DHCP_OPTION=0

#
# ------------DSL logs of Wizard--------------
#
DSL_WIZARD_LOG=0

#
# -----------------SpeedTest-------------------
#
SPEEDTEST=1

#
# --------------Microsoft PNP-x Support--------
#
PNPX=1
PARAGON_NTFS=1
PARAGONNTFS_USE=paragon_ntfs_R6950
GUEST_MANAGEMENT=1
GUI_V14=1

#
# ----------need support GUI_V14 first----------
#
DETECT_AP=1
KWILT=0
TX_POWER_CONTROL_SHOW=0

#
# -----WPS auto disable after several fails-------
#
WIFI_WPS_AVOID_PIN_ATTACK=1

#
# ------XCLOUD: Remote Genie & Ready CLOUD--------
#
XCLOUD=1
XCLOUD_BINARY_USE=XCLOUD_R6950
USE_JEFFS2_FS=0
# now only support DFS at europe region: 
# RDRegion/Europe in rc/wifi.c, get_dfs_ch_enable in cgi
# use index 6 in wifi GUI.
WIFI_DFS=1

#
# WIFI_SPATIAL_STREAMS used to represent antennas
#
WIFI_SPATIAL_STREAMS=X3

#
# Enable RWFS your need to specify your mtdblock id and size for RWFS in project.h
#
RWFS=0

#Enable when U need make a ECC bin file
ECC_BIN=1

