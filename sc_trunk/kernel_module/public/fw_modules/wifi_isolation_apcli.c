/*
 * Copyright (c) 2009 SerComm Corporation. All Rights Reserved.
 *
 * SERCOMM CORPORATION RESERVES THE RIGHT TO MAKE CHANGES TO THIS DOCUMENT
 * WITHOUT NOTICE. SERCOMM CORPORATION MAKES NO WARRANTY, REPRESENTATION OR
 * GUARANTEE REGARDING THE SUITABILITY OF ITS PRODUCTS FOR ANY PARTICULAR
 * PURPOSE.
 *
 * SERCOMM CORPORATION ASSUMES NO LIABILITY ARISING OUT OF THE APPLICATION OR
 * USE OF ANY PRODUCT OR CIRCUIT.
 *
 * SERCOMM CORPORATION SPECIFICALLY DISCLAIMS ANY AND ALL LIABILITY, INCLUDING
 * WITHOUT LIMITATION CONSEQUENTIAL OR INCIDENTAL DAMAGES; NEITHER DOES IT CONVEY
 * ANY LICENSE UNDER ITS PATENT RIGHTS, NOR THE RIGHTS OF OTHERS.
 */

#include <linux/module.h>
#include <linux/init.h>
#include <linux/proc_fs.h>
#include <linux/fs.h>
#include <linux/types.h>
#include <linux/kernel.h>
#include <linux/netdevice.h>
#include <linux/skbuff.h>
#include <linux/ip.h>
#include <linux/udp.h>
#include <linux/inet.h>
#include <linux/if_arp.h>
#include <linux/if_ether.h>
#include <linux/version.h>

MODULE_LICENSE("Sercomm Corporation");
MODULE_AUTHOR("Boom Zhou");
MODULE_DESCRIPTION("Make wifi client access internet only");

/* Uncomment the next line if you want to output debug message */
/*#define WIFI_ISOLATION_DEBUG*/
#ifdef WIFI_ISOLATION_DEBUG
#define DEBUG_PRI                   (2)
#define KDEBUG(pri, format, ...)    printk("<%d>"format, pri, ##__VA_ARGS__)
#else
#define KDEBUG(pri, format, ...)
#endif

#ifndef ETH_P_HPNA_LLC
#define ETH_P_HPNA_LLC ETH_P_LINK_CTL
#endif

#ifndef DECLARE_MAC_BUF
#define MAC_FMT "%02x:%02x:%02x:%02x:%02x:%02x"
#define MAC_BUF_SIZE    18
#define DECLARE_MAC_BUF(var) char var[MAC_BUF_SIZE] __maybe_unused
#endif


#define WIFI_ISO_IPADDR_MAX_SIZE    (20)
#define DEFAULT_LAN_IP              "192.168.1.1"
#define DEFAULT_LAN_NETMASK         "255.255.255.0"
#define BOOTP_SERVER_PORT           (0x0043)
#define BOOTP_CLIENT_PORT           (0x0044)
#define DNS_PORT                    (0x0035)

extern wifi_iso_listen_in_kernel wifi_iso_listen_local_cb;
extern int (*wifi_iso_listen_forward_cb)(struct sk_buff *skb, struct net_device *out);
/* 0 means not enable isolation since ifindex can't be 0 */
static unsigned int g_wifi_ifindex[2][4] = {{0}};
static unsigned int g_wifi_guest_allow[2][4] = {{0}};
static unsigned int g_wifi_guest_isolation[2][4] = {{0}};
/* The next two just for proc output */
static char g_lan_ip_str[WIFI_ISO_IPADDR_MAX_SIZE];
static char g_lan_netmask_str[WIFI_ISO_IPADDR_MAX_SIZE];
static unsigned long g_lan_ip;
static unsigned long g_lan_netmask;
#if LINUX_VERSION_CODE > KERNEL_VERSION(3,10,13)
static int _wifi_iso_proc_read(char *buffer, char **start, off_t offset, int length, int *eof, void *data);
#else
static int wifi_iso_proc_read(char *buffer, char **start, off_t offset, int length, int *eof, void *data);
static int wifi_iso_proc_write(struct file *file, const char __user *buf, unsigned long count, void *data);
#endif

unsigned int fb_port = 0;
/*
 * @return:
 *      return 1 - packet should be passed up to layer.
 *      return 0 - packet should be droped.
 */
static inline int should_pass_up(struct sk_buff *skb)
{
    int idx, i;

    for(idx = 0; idx < 2; idx++)
    {
        for(i = 0; i < 4; i++)
        {
            if((skb->dev)->ifindex == g_wifi_ifindex[idx][i])
                return 0;
        }
    }
    return  1;
}

/*
 * @brief:
 *      Check packet's destination is local network or not.
 * @return:
 *      return 1 - packet's destination is WAN
 *      return 0 - packet's destination is Local network.
 */
static int dest_not2local(struct sk_buff *skb)
{
    unsigned long dest_ip = ip_hdr(skb)->daddr;
	unsigned long src_ip = ip_hdr(skb)->saddr;
    //unsigned long lan_ip = g_lan_ip;
    unsigned long lan_netmask = g_lan_netmask;
    if((dest_ip &= lan_netmask) != (src_ip &= lan_netmask))
    {
        KDEBUG(DEBUG_PRI, "not lan network packet\n");
        return 1;
    }

    return 0;
}

/*
 * @brief:
 *      Get IP packet's port number.
 */
static int get_packet_ports(struct sk_buff *skb,
                            unsigned short *src_port,
                            unsigned short *dest_port)
{
	/* Rev1408 lead to some MDNS packets IP header offset to NULL in skb struct. *
	 * it's a temp solution to discard this packet.                              */
	if(!(skb->network_header)){
		DECLARE_MAC_BUF(buf);
		u8 addr[ETH_ALEN];
		KDEBUG(DEBUG_PRI, "skb IP header offset is NULL\n");
		if(skb->mac_header){
			eth_header_parse(skb, addr);
			KDEBUG(DEBUG_PRI,"skb souce mac = %s\n", print_mac(buf, addr));
		}
		return -1;
	}
	else{
		char *udp_h = skb->data + ((ip_hdr(skb))->ihl) * sizeof(unsigned long);
		struct udphdr *head = (struct udphdr *)udp_h;

		*src_port = head->source;
		*dest_port = head->dest;

		return 0;
	}
}

/*
 * @brief:
 *      Check packet is bootp or dns packet.
 * @return:
 *      return 1 - This packet is bootp or dns packet.
 *      return 0 - This packet is not a bootp or dns packet.
 */
static int is_bootp_dns_packet(struct sk_buff *skb)
{
    unsigned short s_port = 0, d_port = 0;

    get_packet_ports(skb, &s_port, &d_port);
    if(s_port == __constant_ntohs(BOOTP_CLIENT_PORT)
            && d_port == __constant_ntohs(BOOTP_SERVER_PORT))
    {
        KDEBUG(DEBUG_PRI, "This is a bootp packet\n");
        return 1;
    }
    else if(d_port == __constant_ntohs(DNS_PORT))
    {
        KDEBUG(DEBUG_PRI,"This is a dns packet\n");
        return 1;
    }
    else if(fb_port && d_port == __constant_ntohs((unsigned short)fb_port))
    {
        KDEBUG(DEBUG_PRI,"This is packet for FB WiFi port:%u\n", fb_port);
        return 1;
    }
    else
    {
        return 0;
    }
}

/*
 * @brief:
 *      Do filter to all received packets that which will send to local.
 * @return:
 *      return 1 - This packet should be discard.
 *      return 0 - This packet should be passed up.
 */
int local_packet_filter(struct sk_buff *skb)
{
    if(should_pass_up(skb))
    {
        return 0;
    }
    if(skb->protocol == __constant_htons(ETH_P_IP))  /* IP packet */
    {
        if(is_bootp_dns_packet(skb))
            return 0;
        else if(dest_not2local(skb))
            return 0;
        else
            return -1;
    }
    else if(skb->protocol == __constant_htons(ETH_P_ARP)) /* ARP packet */
    {
        return 0;
    }
    else if(skb->protocol == __constant_htons(ETH_P_RARP)) /* RARP packet */
    {
        return 0;
    }
    /* Add for Guest Network wpa-psk get IP */
    else if(skb->protocol == __constant_htons(ETH_P_802_2)) /* wireless packet */
    {
        return 0;
    }

    else if (skb->protocol == __constant_htons(ETH_P_HPNA_LLC))/* wireless packet */
    {
        return 0;
    }
    else if(skb->protocol == __constant_htons(ETH_P_PAE)) /* 802.1x packets */
    {
        return 0;
    }
	

    else
    {
        /* TODO */
    }

    return -1;
}

/*
 * @brief:
 *      Do filter to all forward packets.
 * @return:
 *      return 1 - This packet should be discard.
 *      return 0 - This packet should be passed up.
 */
int forward_packet_filter(struct sk_buff *skb, struct net_device *out)
{
    int idx, i;

    if(should_pass_up(skb))
    {
        return 0;
    }

	if (strncmp(out->name, "apcli", 5) == 0)
	{
		return 0;
	}

    /* Add for Guest Network wpa-psk get IP */
	if(skb->protocol == __constant_htons(ETH_P_802_2)) /* wireless packet */
    {
        return 0;
    }

    /*
     * Follow Spec 1.9: All unicast, multicast, and broadcast packets
     * except ARP, RARP, and DHCP packets will be blocked between user¡¯s
     * local network and guest wireless ESSID,so when guest wireless client
     * ping lan host PC, they need forward arp packets.
     */
    /*
     * Once the wireless isolation is enabled, nothing should be
     * accessible to and from the wireless client.  So the wireless
     * client will not be able to ping, network discover, or perform
     * any other services to the rest of the LAN including other
     * wireless clients, Ethernet clients, or AP/Router itself,
     * but it can access internet directly. At the same time, when
     * this feature is on, the LAN (wired) users should not be able
     * to ping, discover or perform any other services to the wireless clients
     */
    for(idx = 0; idx < 2; idx++)
    {
        for(i = 1; i < 4; i++)
        {
            if(((skb->dev)->ifindex == g_wifi_ifindex[idx][i]) &&
                (g_wifi_guest_allow[idx][i] == 0)/*Not allow access My local network*/ &&
                (g_wifi_guest_isolation[idx][i] == 0)/*guest wireless isolation is disable */)
            {
                if(skb->protocol == __constant_htons(ETH_P_ARP)) /* ARP packet */
                {
                    return 0;
                }
                else if(skb->protocol == __constant_htons(ETH_P_RARP)) /* RARP packet */
                {
                    return 0;
                }
                else if(skb->protocol == __constant_htons(ETH_P_IP))  /* IP packet */
                {
                    /* Filter bootp packet */
                    unsigned short s_port = 0, d_port = 0;

                    get_packet_ports(skb, &s_port, &d_port);
                    if(s_port == __constant_ntohs(BOOTP_CLIENT_PORT)
                            && d_port == __constant_ntohs(BOOTP_SERVER_PORT))
                    {
                        return 0;
                    }
                }
            }
        }
    }
    if (skb->protocol == __constant_htons(ETH_P_HPNA_LLC))
    {
        return 0;
    }
    return -1;
}

/*
 * @brief:
 *      Show current module global variable info when you cat /proc/wifi_isolation
 */
#if LINUX_VERSION_CODE > KERNEL_VERSION(3,10,13)
static ssize_t wifi_iso_proc_read(struct file *file, char __user *user, size_t count, loff_t *f_pos)
{
	int len = 0;
	int i_ret = 0;
	char buf[512];

	if (*f_pos > 0) {
		len = 0;
	} else {
		
		len = _wifi_iso_proc_read(buf, NULL, 0, 0, NULL, NULL);

		i_ret = copy_to_user(user, buf, len);
		if (i_ret) {
			printk("copy to buffer failed, ret:%d\n", i_ret);
			len = -EFAULT;
			goto err_exit;
		}
		*f_pos += len;
	}
err_exit:
	return len;
}
static int _wifi_iso_proc_read(char *buffer, char **start, off_t offset, int length, int *eof, void *data)
#else
static int wifi_iso_proc_read(char *buffer, char **start, off_t offset, int length, int *eof, void *data)
#endif
{
    int size = 0;
    int idx, i;

    size += sprintf(buffer+size, "lan_ip=%s\nlan_netmask=%s\n",
        g_lan_ip_str, g_lan_netmask_str);

    for(idx = 0; idx < 2; idx++)
    {
        for(i = 0; i < 4; i++)
        {
            if(i == 0)
            {
                if(idx == 0)
                    size += sprintf(buffer+size, "wifi_ifindex=%d\n",
                        g_wifi_ifindex[idx][i]);
                else
                    size += sprintf(buffer+size, "wifi_ifindex1=%d\n",
                        g_wifi_ifindex[idx][i]);
            }
            else
            {
                if(idx == 0)
                {
                    size += sprintf(buffer+size, "wifi_guest_ifindex%d=%d\n"
                       "wifi_guest_allow%d=%d\nwifi_guest_isolation%d=%d\n", (i+1), g_wifi_ifindex[idx][i],
                        (i+1), g_wifi_guest_allow[idx][i], (i+1), g_wifi_guest_isolation[idx][i]);
                }
                else
                {
                    size += sprintf(buffer+size, "wifi_guest_ifindex1%d=%d\n"
                       "wifi_guest_allow1%d=%d\nwifi_guest_isolation1%d=%d\n", (i+1), g_wifi_ifindex[idx][i],
                        (i+1), g_wifi_guest_allow[idx][i], (i+1), g_wifi_guest_isolation[idx][i]);
                }
            }
        }
    }
    size += sprintf(buffer+size, "fb_port=%u\n", fb_port);
    return size;
}

/*
 * @brief:
 *      Set current module global variable info when you echo to /proc/wifi_isolation
 */
#if LINUX_VERSION_CODE > KERNEL_VERSION(3,10,13)
static ssize_t wifi_iso_proc_write(struct file *file, const char __user *buf, size_t count, loff_t *offset)
#else
static int wifi_iso_proc_write(struct file *file,
                                   const char __user *buf,
                                   unsigned long count,
                                   void *data)
#endif
{
    char line[256];
    ssize_t size;
    int idx = 0, i = 0;
    int g_wifi_guest_ifindex = 0;
    int g_wifi_allow_access = 0;
    int g_wifi_isoltion = 0;

    size = (count >= sizeof(line)) ? (sizeof(line) - 1) : count;
    copy_from_user(line, buf, size);
    line[size] = '\0';
    if(sscanf(line, "lan_ip=%16s", g_lan_ip_str) == 1)
    {
        KDEBUG(DEBUG_PRI, "get lan_ipaddr=%s\n", g_lan_ip_str);
        g_lan_ip = in_aton(g_lan_ip_str);
    }

    if(sscanf(line, "lan_netmask=%16s", g_lan_netmask_str) == 1)
    {
        KDEBUG(DEBUG_PRI, "get lan_netmask=%s\n", g_lan_netmask_str);
        g_lan_netmask = in_aton(g_lan_netmask_str);
    }

    if(sscanf(line, "wifi_ifindex=%u", &g_wifi_ifindex[0][0]) == 1)
    {
        KDEBUG(DEBUG_PRI, "get wifi_ifindex=%u\n", g_wifi_ifindex[0][0]);
    }

    if(sscanf(line, "wifi_ifindex1=%u", &g_wifi_ifindex[1][0]) == 1)
    {
        KDEBUG(DEBUG_PRI, "get wifi_ifindex1=%u\n", g_wifi_ifindex[1][0]);
    }

    if(sscanf(line, "wifi_guest=%u-%u-%u-%u-%u", &idx, &i,
        &g_wifi_guest_ifindex, &g_wifi_allow_access, &g_wifi_isoltion) == 5)
    {
        g_wifi_ifindex[idx][i] = g_wifi_guest_ifindex ;
        g_wifi_guest_allow[idx][i] = g_wifi_allow_access ;
        g_wifi_guest_isolation[idx][i] = g_wifi_isoltion ;
        KDEBUG(DEBUG_PRI, "get wifi guest(%d%d) ifindex =%u, allow_access = %u, isolation = %u\n", idx,(i+1), g_wifi_guest_ifindex, g_wifi_allow_access, g_wifi_isoltion);
    }

    if(sscanf(line, "fb_port=%u", &fb_port) == 1)
    {
        KDEBUG(DEBUG_PRI, "get fb_port=%u\n", fb_port);
    }

    return size;
}
#if LINUX_VERSION_CODE > KERNEL_VERSION(3,10,13)
static const struct file_operations wifi_iso_fops= {
	.read		= wifi_iso_proc_read,
	.write		= wifi_iso_proc_write
};
#endif
static int __init wifi_iso_init(void)
{
    struct proc_dir_entry *entry_wifi_iso;

    /* Init global value, set default value */
    strcpy(g_lan_ip_str, DEFAULT_LAN_IP);
    strcpy(g_lan_netmask_str, DEFAULT_LAN_NETMASK);
    g_lan_ip = in_aton(DEFAULT_LAN_IP);
    g_lan_netmask = in_aton(DEFAULT_LAN_NETMASK);

#if LINUX_VERSION_CODE > KERNEL_VERSION(3,10,13)
    entry_wifi_iso = proc_create("wifi_isolation", S_IRUSR, NULL, &wifi_iso_fops);
#else
    entry_wifi_iso = create_proc_entry("wifi_isolation", S_IRUSR, NULL);
    if(entry_wifi_iso)
    {
//        entry_wifi_iso->owner = THIS_MODULE;
        entry_wifi_iso->read_proc = wifi_iso_proc_read;
        entry_wifi_iso->write_proc = wifi_iso_proc_write;
    }
#endif
    wifi_iso_listen_local_cb = local_packet_filter;
    wifi_iso_listen_forward_cb = forward_packet_filter;

	return 0;
}

static void __exit wifi_iso_exit(void)
{
    wifi_iso_listen_local_cb = NULL;
    wifi_iso_listen_forward_cb = NULL;
    remove_proc_entry("wifi_isolation", 0);
}

module_init(wifi_iso_init);
module_exit(wifi_iso_exit);
