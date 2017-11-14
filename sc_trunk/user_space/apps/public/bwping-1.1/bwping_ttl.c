#include <sys/types.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <netinet/in_systm.h>
#include <netinet/ip.h>
#include <netinet/ip_icmp.h>
#include <netdb.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sysexits.h>
#include <errno.h>
#include <string.h>
#include <strings.h>
#include <sys/ioctl.h>
#include <net/if.h>
#include <netinet/udp.h>

#define	SND_NUM  20
unsigned char	packet[IP_MAXPACKET] __attribute__((aligned(4))); /* ?????? */
struct ip *outip = NULL;
struct udphdr *outudp = NULL ;
struct outdata *outdata = NULL;
/* Data section of the probe packet */
typedef struct outdata {
	unsigned char seq;             /* sequence number of this packet */
	unsigned char ttl;             /* ttl packet left with */
	char date[1];
} outdata_t;

/*
 * Overlay for ip header used by other protocols (tcp, udp).
 */
struct ipovly {
	unsigned char  ih_x1[9];               /* (unused) */
	unsigned char  ih_pr;                  /* protocol */
	short   ih_len;                 /* protocol length */
	struct  in_addr ih_src;         /* source internet address */
	struct  in_addr ih_dst;         /* destination internet address */
};
/*
 * UDP kernel structures and variables.
 */
struct udpiphdr {
	struct  ipovly ui_i;            /* overlaid ip structure */
	struct  udphdr ui_u;            /* udp header */
};
#define ui_next         ui_i.ih_next
#define ui_prev         ui_i.ih_prev
#define ui_x1           ui_i.ih_x1
#define ui_pr           ui_i.ih_pr
#define ui_len          ui_i.ih_len
#define ui_src          ui_i.ih_src
#define ui_dst          ui_i.ih_dst
#define ui_sport        ui_u.uh_sport
#define ui_dport        ui_u.uh_dport
#define ui_ulen         ui_u.uh_ulen
#define ui_sum          ui_u.uh_sum

/* Host name and address list */
struct hostinfo {
	char *name;
	int n;
	uint32_t *addrs;
};
#if 0

#define SC_CFPRINTF printf

#else

#define _SC_DEBUG_ENABLE_
#include "sc_debug.h"

#endif

struct tv32 {
    unsigned int tv32_sec;
    unsigned int tv32_usec;
};

/* minus end and start, and return the result in microseconds(1/1000000s). */
static long long int tvsub(struct timeval *t1, struct timeval *t2)
{
    if (t1->tv_usec > t2->tv_usec) {
		return (t1->tv_sec - t2->tv_sec)     * 1000000 + (t1->tv_usec           - t2->tv_usec);
    } else {
		return (t1->tv_sec - t2->tv_sec - 1) * 1000000 + (t1->tv_usec + 1000000 - t2->tv_usec);
    }
}

static unsigned short int cksum (unsigned short *addr, int len)
{
    int				nleft, sum;
    unsigned short int		*w, answer;
    union {
	unsigned short int	us;
	unsigned char		uc[2];
    } last;

    nleft = len;
    sum   = 0;
    w     = addr;

    while (nleft>1) {
	sum   += *w++;
	nleft -= 2;
    }

    if (nleft==1) {
	last.uc[0] = *(u_char *)w;
	last.uc[1] = 0;
	sum       += last.us;
    }

    sum    = (sum >> 16) + (sum & 0xFFFF);
    sum   += (sum >> 16);
    answer = ~sum;

    return(answer);
}

static void send_probe(int sndsock, struct sockaddr_in *to, unsigned long int mtu, int ident, unsigned int ttl, unsigned int *seq)
{
	int cc;
	struct udpiphdr *ui, *oui;
	struct ip tip;

	outip->ip_ttl = ttl;
	outip->ip_id = htons(ident + *seq);

	outip->ip_sum =
	    cksum((uint16_t *)outip, sizeof(*outip));
	if (outip->ip_sum == 0)
		outip->ip_sum = 0xffff;

	/* Payload */
	outdata->seq = ttl+0x10;
	outdata->ttl = ttl+0x10;

    outudp->dest = htons(33434 + ttl);

	/* Checksum (we must save and restore ip header) */
	tip = *outip;
	ui = (struct udpiphdr *)outip;
	oui = (struct udpiphdr *)&tip;
	/* Easier to zero and put back things that are ok */
	memset((char *)ui, 0, sizeof(ui->ui_i));
	ui->ui_src = oui->ui_src;
    ui->ui_dst = oui->ui_dst;
    ui->ui_pr = oui->ui_pr;
	ui->ui_len = outudp->len;
	outudp->check = 0;
	outudp->check = cksum((unsigned short *)ui, mtu);
	if (outudp->check == 0)
		outudp->check = 0xffff;
	*outip = tip;

	cc = sendto(sndsock, (char *)outip,
	    mtu, 0, (struct sockaddr *)to, sizeof(*to));
	if (cc != mtu)  {
		perror("sendto UDP fail: cc!=mtu");
	}
	else
	    (*seq)++;
}
#if 0
static int send_ping (int sock, struct sockaddr_in *to, unsigned long int mtu, int ident, int first_in_burst, unsigned int *transmitted_number)
{
    int			size, res;
    unsigned char	packet[IP_MAXPACKET] __attribute__((aligned(4))); /* ?????? */
    struct icmp		*icmp;

	/* prepare to build the icmp pkt. */
    icmp = (struct icmp *)packet;

	/* value to packet. BE carefull, if value is more than 1 byte(int16, int32). */
    icmp->icmp_type  = ICMP_ECHO;
    icmp->icmp_code  = 0;
    icmp->icmp_cksum = 0;
    icmp->icmp_seq   = htons(*transmitted_number);
    icmp->icmp_id    = ident;

    size = mtu - sizeof(struct ip);

    icmp->icmp_cksum = cksum((u_short *)icmp, size);

    res = sendto(sock, (char *)packet, size, 0, (struct sockaddr *)to, sizeof(*to));

    if (res==-1 || res!=size) {
		if (res==-1) {
		    perror("bwping: sendto() failed");
		} else {
		    fprintf(stderr, "bwping: partial write: packet size: %d, sent: %d\n", size, res);
		}
		return 1;
    } else
    	(*transmitted_number)++;

    return 0;
}
#endif
static int recv_ping (int sock, int ident, unsigned int transmitted_number, unsigned int *received_number)
{
    int				res, hlen;
    unsigned char		packet[IP_MAXPACKET] __attribute__((aligned(4)));
    struct sockaddr_in		from;
    struct iovec		iov;
    struct msghdr		msg;
    struct ip			*ip;
    struct icmp			*icmp;

    iov.iov_base = packet;
    iov.iov_len  = IP_MAXPACKET;

    msg.msg_name    = (caddr_t)&from;
    msg.msg_iov     = &iov;
    msg.msg_iovlen  = 1;
    msg.msg_namelen = sizeof(from);

    res = recvmsg(sock, &msg, MSG_DONTWAIT);

    if (res>0) {
		ip = (struct ip *)packet;

		hlen = ip->ip_hl << 2;

		if (res>=hlen + ICMP_MINLEN) {
		    icmp = (struct icmp *)(packet + hlen);

		    if (icmp->icmp_type == ICMP_TIME_EXCEEDED || icmp->icmp_type == ICMP_ECHOREPLY) {
#if 0
				struct ip *p_ip;
				struct icmp *p_icmp;

				p_ip = (struct ip *)(&(icmp->icmp_ip));
				p_icmp = (void*)p_ip+((p_ip->ip_hl)*4);



				SC_CFPRINTF("start:%x, ip:%x, ip_len:%x, icmp:%x. type:%x, id:%x, seq:%x\n",
					      (unsigned int)packet, (unsigned int)p_ip, (p_ip->ip_hl)*4, (unsigned int)p_icmp,
							p_icmp->icmp_type, ntohs(p_icmp->icmp_id), ntohs(p_icmp->icmp_seq));

#endif
				(*received_number)++;
				return 1;
		    }
		}
    }

    return 0;
}

char *getWanIPAddress(char *ifname)
{
    struct ifreq ifr;
    struct sockaddr_in *saddr;
    struct in_addr ip;
    int fd;

    fd=socket(AF_INET, SOCK_RAW, IPPROTO_RAW);
    if(fd==-1)
    {
        perror("socket creating failed");
        return NULL;
    }

    strcpy(ifr.ifr_name, ifname);
    if (ioctl(fd, SIOCGIFADDR, &ifr) == 0)
    {
        saddr = (struct sockaddr_in *)&(ifr.ifr_addr);
        ip = saddr->sin_addr;

        close(fd);
        return inet_ntoa(ip);
    }
    else
    {
        close(fd);
        return "0.0.0.0";
    }

    return "0.0.0.0";
}

int main (int argc, char **argv)
{
    int sndsock, rcvsock, exitval=EX_OK, ident, n;
    unsigned int transmitted_number=0, received_number=0, ttl=0, mtu=0, link_speed=0;
    char ch;
    char *ep, *target=NULL, *mode=NULL, *ifname=NULL;
    fd_set fds;
    struct sockaddr_in from, to;
    unsigned char *outp;
    struct timeval begin, end, seltimeout, test, time1, time2;
    unsigned long long delay=0, total_pktlen_sent, speed;
    int ether_pkt_len;
    int h = 0;

	while ((ch = getopt(argc, argv, "i:u:s:l:m:"))!=-1)
	{
	    switch (ch) {
        case 'i':
            ifname = optarg;
            break;
		case 'u':
		    link_speed = strtoul(optarg, &ep, 0);

		    if (*ep || ep==optarg)
		    {
				exitval = EX_DATAERR;
		    }

		    break;

		case 'm':
		    mode = optarg;
		    break;

		case 'l':
		    ttl = strtoul(optarg, &ep, 0);

		    if (*ep || ep==optarg)
		    {
				exitval = EX_DATAERR;
		    }

		    break;

		case 's':
		    mtu = strtoul(optarg, &ep, 0);

		    if (*ep || ep==optarg)
		    {
				exitval = EX_DATAERR;
		    }
		    break;

		default:
		    exitval = EX_USAGE;
	    }
	}
	target = argv[optind];

	if ( ttl==0 || mtu==0 || link_speed==0 || mode==NULL || target==NULL)
	{
	    exitval = EX_DATAERR;
	} else if (argc - optind!=1)
	{
	    exitval = EX_USAGE;
	}
	if (exitval!=EX_OK)
	{
		fprintf(stderr, "Usage: %s -m wan_mode -l ttl -s mtu -u linkspeed target\n",
				argv[0]);
		return exitval;
	}
	if ((mtu<sizeof(struct ip) + ICMP_MINLEN) || (mtu>IP_MAXPACKET))
	{
		fprintf(stderr, "%s: invalid packet size, should be between %d and %d\n",
				argv[0], (int)(sizeof(struct ip) + ICMP_MINLEN + sizeof(struct tv32)), IP_MAXPACKET);
		return EX_DATAERR;
	}

	/* 1. create UDP(Send)/ICMP(Recv) socket. */
    rcvsock = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP);
    if (rcvsock == -1)
    {
		perror("socket fail");
		return EX_SOFTWARE;
    }
    sndsock = socket(AF_INET, SOCK_RAW, IPPROTO_RAW);
    if (sndsock == -1)
    {
		perror("socket fail");
		return EX_SOFTWARE;
    }
    /*
	if (setsockopt(sndsock, SOL_SOCKET, SO_SNDBUF, &mtu, sizeof(mtu)) < 0) {
		perror("SO_SNDBUF");
	}
    */

	/* 2.init sockaddr_in */

	bzero(&to, sizeof(to));
	to.sin_family = AF_INET;

	/* 2.1 try convert ip string to ip */
	if (inet_aton(target, &to.sin_addr)==0) {
		/* 2.2 if fail, target is a domain name. try to store resolved ip to hostent. */
		struct hostent *hp = gethostbyname(target);
		if (!hp)
		{
			/* 2.2.1 fail */
			fprintf(stderr, "bwping: cannot resolve %s: %s\n", target, hstrerror(h_errno));
			exitval = EX_DATAERR;
            goto _exit;
		} else if ((unsigned)hp->h_length>sizeof(to.sin_addr))
		{
			/* 2.2.2 wrong hostent */
			fprintf(stderr, "bwping: gethostbyname() returned an illegal address\n");
			exitval = EX_DATAERR;
            goto _exit;
		} else
		{
			/* 2.2.3 store right ip to hostent. */
			memcpy(&to.sin_addr, hp->h_addr_list[0], sizeof(to.sin_addr));
		}
	}
	/* Init sndsock udp header and ip header */
	memset(packet, 0, sizeof(packet));
    outip = (struct ip *)packet;
	outip->ip_v = IPVERSION;
	outip->ip_len = htons(mtu);
	outip->ip_off = htons(0); /* Do not frag*/
	outp = (unsigned char *)(outip + 1);
    outip->ip_dst = to.sin_addr;

	outip->ip_hl = (outp - (unsigned char *)outip) >> 2;
	ident = (getpid() & 0xffff) | 0x8000;

	outip->ip_p = IPPROTO_UDP;
	outudp = (struct udphdr *)outp;
	outudp->source = htons(ident);
	outudp->len = htons((unsigned short)(mtu - (sizeof(*outip))));
	outdata = (outdata_t *)(outudp + 1);

    bzero(&from, sizeof(from));
    from.sin_family = AF_INET;
    inet_pton(AF_INET, getWanIPAddress(ifname), &from.sin_addr.s_addr);
    outip->ip_src = from.sin_addr;

    if (bind(sndsock, (struct sockaddr *)&from, sizeof(struct sockaddr)) == -1) {
        perror("bind sndsock fail");
        exitval = EX_SOFTWARE;
        goto _exit;
    }

    if (setsockopt(sndsock, IPPROTO_IP, IP_TTL, (char *)&ttl, sizeof(ttl))==-1) {
        fprintf(stderr, "bwping: setsockopt(IP_TTL, %u) failed: %s\n", ttl, strerror(errno));
        exitval = EX_SOFTWARE;
        goto _exit;
    }
/*
   not follow spec
   increase the ttl, for receiving packets better.
*/
    for (h = 0; h < 3; h++,ttl++)
    {
        
        SC_CFPRINTF("Target: %s (%s), mtu: %u bytes, ttl = %d.\n",
            target, inet_ntoa(to.sin_addr), mtu, ttl);

		transmitted_number = 0;
        received_number = 0;
		memset(&begin, 0, sizeof(begin));
		memset(&end, 0, sizeof(end));
		memset(&test, 0, sizeof(test));
		memset(&time1, 0, sizeof(time1));
		memset(&time2, 0, sizeof(time2));

		gettimeofday(&time1, NULL);
		
		while (1)
		{
			send_probe(sndsock, &to, mtu, ident, ttl, &transmitted_number);
			SC_CFPRINTF("send %d\n", transmitted_number);

			do
			{
				FD_ZERO(&fds);
				FD_SET(rcvsock, &fds);

				seltimeout.tv_sec  = 0;
				seltimeout.tv_usec = 10;

				/* 4. receive...  */
				n = select(rcvsock + 1, &fds, NULL, NULL, &seltimeout);

				if (n>0)
				{
					/* recv_ping will received_number++ if recv ok. */
					if (recv_ping(rcvsock, ident, transmitted_number, &received_number))
					{
						gettimeofday(&test, NULL);
						SC_CFPRINTF("recv %d. %ld.%ld\n", received_number, test.tv_sec, test.tv_usec);
						/* record the 1st */
						if (received_number==1)
						{
							gettimeofday(&begin, NULL);
							SC_CFPRINTF("begin:%ld.%ld\n", begin.tv_sec, begin.tv_usec);
						}

						/* ok, receive all SND_NUM pkts, process then exit. */
						if (received_number==SND_NUM)
						{
							goto _out;
						}
					}
                }
                gettimeofday(&time2, NULL);
                if((time2.tv_sec - time1.tv_sec) >= 3)
                    goto _out;
			} while (transmitted_number==SND_NUM);
		}

_out:
	if(received_number==SND_NUM)
	{
		gettimeofday(&end, NULL);
		delay = tvsub(&end, &begin);
		SC_CFPRINTF("end:%ld.%ld\n", end.tv_sec, end.tv_usec);
	}
	else if((received_number > 10) && (received_number != SND_NUM))
	{
		SC_CFPRINTF("Not receive all packets\n");
		delay = tvsub(&test, &begin);
	}
	else /* receive num < 10, not check bandwidth */
		goto _exit;

	if (strcmp(mode, "dhcpc")==0 || strcmp(mode, "ip")==0)
		ether_pkt_len = mtu+14; /* 14=srcmac(6)+dstmac(6)+type(2) */
	else
		ether_pkt_len = mtu+22; /* 22=srcmac(6)+dstmac(6)+type(2)+pppoe/pptpheader(8) */
	
    /* Spec 2.0 speed : 0.9*(E*8)*(receive_number-1)/T bps*/
    		total_pktlen_sent = (9*ether_pkt_len*8*(received_number-1))/10;
	speed = ((unsigned long long)(total_pktlen_sent*1000000)) / delay;

	if ((speed/1000)>link_speed)
	{
		SC_CFPRINTF("warning: link %u. speed:%llu. total:%llu, delay:%llu\n",
			link_speed, speed, total_pktlen_sent, delay);
		goto _exit;
	}
	/* print to stdout so myPipe will get the result. */
    		printf("speed:%llu. total:%llu, delay:%llu\n",
            speed, total_pktlen_sent, delay);

            break;
	}
_exit:
    close(sndsock);
    close(rcvsock);
    SC_CFPRINTF("exitval: %d \n", exitval);
    return exitval;
}
