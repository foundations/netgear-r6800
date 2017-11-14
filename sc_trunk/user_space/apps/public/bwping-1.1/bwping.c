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

#define	CALIBRATE_RETRIES	50
#define DEF_MIN_RTT		0xFFFFFFFF

struct tv32 {
    unsigned int tv32_sec;
    unsigned int tv32_usec;
};

unsigned long long int min_rtt, max_rtt, average_rtt;

static long long int tvsub (struct timeval *t1, struct timeval *t2)
{
    if (t1->tv_usec>t2->tv_usec) {
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

static unsigned long long int calibrate_timer (void)
{
    int				i, n;
    unsigned long long int	sum;
    struct timeval		begin, end, seltimeout;

    sum = 0;

    for (i=0; i<CALIBRATE_RETRIES; i++) {
	n = -1;

	seltimeout.tv_sec  = 0;
	seltimeout.tv_usec = 10;

	while (n<0) {
	    gettimeofday(&begin, NULL);

	    n = select(0, NULL, NULL, NULL, &seltimeout);
	}

	gettimeofday(&end, NULL);

	sum += tvsub(&end, &begin);
    }

    return sum / CALIBRATE_RETRIES;
}

static void send_ping (int sock, struct sockaddr_in *to, unsigned long int pktsize, int ident, int first_in_burst, unsigned int *transmitted_number)
{
    int			size, res;
    unsigned char	packet[IP_MAXPACKET] __attribute__((aligned(4)));
    struct icmp		*icmp;
    struct timeval	now;
    struct tv32		tv32;

    icmp = (struct icmp *)packet;

    icmp->icmp_type  = ICMP_ECHO;
    icmp->icmp_code  = 0;
    icmp->icmp_cksum = 0;
    icmp->icmp_seq   = htons(*transmitted_number);
    icmp->icmp_id    = ident;

    gettimeofday(&now, NULL);

    if (first_in_burst) {
	tv32.tv32_sec  = htonl(now.tv_sec);
	tv32.tv32_usec = htonl(now.tv_usec);
    } else {
	bzero(&tv32, sizeof(tv32));
    }

    bcopy((void *)&tv32, (void *)&packet[ICMP_MINLEN], sizeof(tv32));

    size = pktsize - sizeof(struct ip);

    icmp->icmp_cksum = cksum((u_short *)icmp, size);

    res = sendto(sock, (char *)packet, size, 0, (struct sockaddr *)to, sizeof(*to));

    if (res==-1 || res!=size) {
	if (res==-1) {
	    perror("bwping: sendto() failed");
	} else {
	    fprintf(stderr, "bwping: partial write: packet size: %d, sent: %d\n", size, res);
	}
    }

    (*transmitted_number)++;
}

static int recv_ping (int sock, int ident, unsigned int transmitted_number, unsigned int *received_number, unsigned long int *received_volume)
{
    int				res, hlen;
    unsigned long long int	rtt;
    unsigned char		packet[IP_MAXPACKET] __attribute__((aligned(4)));
    struct sockaddr_in		from;
    struct iovec		iov;
    struct msghdr		msg;
    struct ip			*ip;
    struct icmp			*icmp;
    struct timeval		now, pkttime;
    struct tv32			tv32;

    iov.iov_base = packet;
    iov.iov_len  = IP_MAXPACKET;

    msg.msg_name    = (caddr_t)&from;
    msg.msg_iov     = &iov;
    msg.msg_iovlen  = 1;
    msg.msg_namelen = sizeof(from);

    gettimeofday(&now, NULL);

    res = recvmsg(sock, &msg, MSG_DONTWAIT);

    if (res>0) {
	ip = (struct ip *)packet;

	hlen = ip->ip_hl << 2;

	if (res>=hlen + ICMP_MINLEN) {
	    icmp = (struct icmp *)(packet + hlen);

	    if (icmp->icmp_type==ICMP_ECHOREPLY) {
		if (icmp->icmp_id==ident) {
		    (*received_number)++;
		    (*received_volume) += res;

		    if (res - hlen - ICMP_MINLEN>=sizeof(tv32)) {
			memcpy(&tv32, icmp->icmp_data, sizeof(tv32));

			pkttime.tv_sec  = ntohl(tv32.tv32_sec);
			pkttime.tv_usec = ntohl(tv32.tv32_usec);

			if (pkttime.tv_sec!=0 || pkttime.tv_usec!=0) {
			    rtt = tvsub(&now, &pkttime) / 1000;

			    if (min_rtt>rtt) {
				min_rtt = rtt;
			    }
			    if (max_rtt<rtt) {
				max_rtt = rtt;
			    }
			    average_rtt = *received_number?((average_rtt * (*received_number - 1)) + rtt) / *received_number:average_rtt;
			}
		    }

		    return 1;
		}
	    }
	}
    }

    return 0;
}

int main (int argc, char **argv)
{
    int				sock, exitval, ident, finish, pktburst, i, n;
    unsigned int		bufsize, tos, transmitted_number, received_number,beginms,endms;
    unsigned long int		kbps, pktsize, volume, timeout, rperiod, received_volume;
    unsigned long long int	min_interval, interval, current_interval, integral_error;
    char			ch;
    char			*ep, *target;
    fd_set			fds;
    struct sockaddr_in		to;
    struct hostent		*hp;
    struct timeval		begin, end, report, start, now, seltimeout;

    sock = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP);

    if (sock==-1) {
	perror("bwping: socket(AF_INET, SOCK_RAW, IPPROTO_ICMP) failed");

	exit(EX_SOFTWARE);
    } else {
	setuid(getuid());

	kbps    = 0;
	pktsize = 0;
	volume  = 0;
	timeout = 1;
	bufsize = 0;
	rperiod = 0;
	tos     = 0;

	exitval = EX_OK;

	while ((ch = getopt(argc, argv, "b:s:v:t:u:r:T:"))!=-1) {
	    switch (ch) {
		case 'b':
		    kbps = strtoul(optarg, &ep, 0);

		    if (*ep || ep==optarg) {
			exitval = EX_DATAERR;
		    }

		    break;
		case 's':
		    pktsize = strtoul(optarg, &ep, 0);

		    if (*ep || ep==optarg) {
			exitval = EX_DATAERR;
		    }

		    break;
		case 'v':
		    volume = strtoul(optarg, &ep, 0);

		    if (*ep || ep==optarg) {
			exitval = EX_DATAERR;
		    }

		    break;
		case 't':
		    timeout = strtoul(optarg, &ep, 0);

		    if (*ep || ep==optarg) {
			exitval = EX_DATAERR;
		    }

		    break;
		case 'u':
		    bufsize = strtoul(optarg, &ep, 0);

		    if (*ep || ep==optarg) {
			exitval = EX_DATAERR;
		    }

		    break;
		case 'r':
		    rperiod = strtoul(optarg, &ep, 0);

		    if (*ep || ep==optarg) {
			exitval = EX_DATAERR;
		    }

		    break;
		case 'T':
		    tos = strtoul(optarg, &ep, 0);

		    if (*ep || ep==optarg) {
			exitval = EX_DATAERR;
		    }

		    break;
		default:
		    exitval = EX_USAGE;
	    }
	}// end of getopt

	if (kbps==0 || pktsize==0 || volume==0 || timeout==0) {
	    exitval = EX_DATAERR;
	} else if (argc - optind!=1) {
	    exitval = EX_USAGE;
	}

	if (exitval==EX_OK) {
		if (pktsize<sizeof(struct ip) + ICMP_MINLEN + sizeof(struct tv32) || pktsize>IP_MAXPACKET) {
			fprintf(stderr, "bwping: invalid packet size, should be between %d and %d\n", (int)(sizeof(struct ip) + ICMP_MINLEN + sizeof(struct tv32)), IP_MAXPACKET);
			exitval = EX_DATAERR;
		} else {
			target = argv[optind];

			bzero(&to, sizeof(to));

			to.sin_family = AF_INET;
	#ifdef HAVE_SOCKADDR_IN_SIN_LEN
			to.sin_len    = sizeof(to);
	#endif

			if (inet_aton(target, &to.sin_addr)==0) {
				hp = gethostbyname(target);

				if (!hp) {
					fprintf(stderr, "bwping: cannot resolve %s: %s\n", target, hstrerror(h_errno));
					exitval = EX_DATAERR;
				} else if ((unsigned)hp->h_length>sizeof(to.sin_addr)) {
					fprintf(stderr, "bwping: gethostbyname() returned an illegal address\n");
					exitval = EX_DATAERR;
				} else {
					memcpy(&to.sin_addr, hp->h_addr_list[0], sizeof(to.sin_addr));
				}
			}

			if (exitval==EX_OK) {
				ident = getpid() & 0xFFFF;

				printf("Target: %s (%s), transfer speed: %lu kbps, packet size: %lu bytes, traffic volume: %lu bytes\n",
						target, inet_ntoa(to.sin_addr), kbps, pktsize, volume);

				min_rtt     = DEF_MIN_RTT;
				max_rtt     = 0;
				average_rtt = 0;

				transmitted_number = 0;
				received_number    = 0;
				received_volume    = 0;
				finish             = 0;

				interval = pktsize * 8000 / kbps;

				min_interval = calibrate_timer() * 2;
				if (interval>=min_interval) {
					pktburst = 1;
				} else if (interval==0) {
					pktburst = min_interval;
					interval = min_interval;

					fprintf(stderr, "bwping: requested transfer speed is too high, effective transfer speed will be %lu kbps\n", pktsize * 8000);
				} else {
					pktburst = min_interval / interval;
					interval = min_interval - (min_interval % interval);
				}

				if (bufsize==0) {
					bufsize = pktsize * pktburst + 128;
				}

				if (setsockopt(sock, SOL_SOCKET, SO_RCVBUF, (char *)&bufsize, sizeof(bufsize))==-1) {
					fprintf(stderr, "bwping: setsockopt(SO_RCVBUF, %u) failed: %s\n", bufsize, strerror(errno));
				}
				if (setsockopt(sock, SOL_SOCKET, SO_SNDBUF, (char *)&bufsize, sizeof(bufsize))==-1) {
					fprintf(stderr, "bwping: setsockopt(SO_SNDBUF, %u) failed: %s\n", bufsize, strerror(errno));
				}

	#ifdef IP_TOS
				if (setsockopt(sock, IPPROTO_IP, IP_TOS, (char *)&tos, sizeof(tos))==-1) {
				fprintf(stderr, "bwping: setsockopt(IP_TOS, %u) failed: %s\n", tos, strerror(errno));
				}
	#endif

				gettimeofday(&begin,  NULL);
				gettimeofday(&end,    NULL);
				gettimeofday(&report, NULL);

				current_interval = interval;
				integral_error   = 0;

				while (!finish) {
					gettimeofday(&start, NULL);

					for (i = 0; i<pktburst; i++) {
						if (pktsize * transmitted_number<volume) {
						send_ping(sock, &to, pktsize, ident, !i, &transmitted_number);
						}
					}
					if (pktsize * transmitted_number>=volume) {
						current_interval = timeout * 1000000;
					}

					while (1) {
						FD_ZERO(&fds);
						FD_SET(sock, &fds);

						seltimeout.tv_sec  = 0;
						seltimeout.tv_usec = 10;

						n = select(sock + 1, &fds, NULL, NULL, &seltimeout);

						gettimeofday(&now, NULL);

						if (n>0) {
						if (recv_ping(sock, ident, transmitted_number, &received_number, &received_volume)) {
							gettimeofday(&end, NULL);
						}

						if (pktsize * transmitted_number>=volume && received_number>=transmitted_number) {
							finish = 1;
							break;
						}
						}
						if (tvsub(&now, &start)>=current_interval) {
							if (pktsize * transmitted_number>=volume) {
								finish = 1;
							} else {
								integral_error += tvsub(&now, &start) - current_interval;

								if (integral_error>=interval / 2) {
								current_interval = interval / 2;
								integral_error   = integral_error - interval / 2;
								} else {
								current_interval = interval;
								}
							}
							break;
						}
					}

					if (rperiod!=0 && now.tv_sec - report.tv_sec>=rperiod) {
						printf("Periodic: pkts sent/rcvd: %u/%u, volume rcvd: %lu bytes, time: %d sec, speed: %lu kbps, rtt min/max/average: %llu/%llu/%llu ms\n",
						transmitted_number, received_number, received_volume, (int)(end.tv_sec - begin.tv_sec),
						end.tv_sec - begin.tv_sec?((received_volume / (end.tv_sec - begin.tv_sec)) * 8) / 1000:(received_volume * 8) / 1000,
						min_rtt==DEF_MIN_RTT?0:min_rtt, max_rtt, average_rtt);

						gettimeofday(&report, NULL);
					}
				}

				beginms = begin.tv_sec*1000+begin.tv_usec/1000;
				endms = end.tv_sec*1000+end.tv_usec/1000;
				printf("Total: pkts sent/rcvd: %u/%u, volume rcvd: %lu bytes, time: %d milli second , speed: %lu kbps, rtt min/max/average: %llu/%llu/%llu ms\n",
						transmitted_number, received_number, received_volume, (int)(endms - beginms),
						endms - beginms?((received_volume / (endms - beginms)) * 8) :(received_volume * 8) / 1000,
						min_rtt==DEF_MIN_RTT?0:min_rtt, max_rtt, average_rtt);
			}
		}
	} else {
		fprintf(stderr, "%s\n", "Usage: bwping [-t timeout] [-u bufsize] [-r reporting_period] [-T tos] -b kbps -s pktsize -v volume target");
	}

	close(sock);

	exit(exitval);
    }
}
