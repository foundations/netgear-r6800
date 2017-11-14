/* dhcpd.c
 *
 * udhcp DHCP client
 *
 * Russ Dill <Russ.Dill@asu.edu> July 2001
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

#include <stdio.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/file.h>
#include <unistd.h>
#include <getopt.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <signal.h>
#include <time.h>
#include <string.h>
#include <sys/ioctl.h>
#include <net/if.h>
#include <errno.h>
#include <sys/sysinfo.h>
#include "dhcpd.h"
#include "dhcpc.h"
#include "options.h"
#include "clientpacket.h"
#include "packet.h"
#include "script.h"
#include "socket.h"
#include "debug.h"
#include "pidfile.h"
#include "filenames.h"
#include "led_action.h"
#include "arpping.h"
static int state;
static unsigned long requested_ip; /* = 0 */
static unsigned long server_addr;
static unsigned long timeout;
static int packet_num; /* = 0 */
static int fd;
#define LISTEN_NONE 0
#define LISTEN_KERNEL 1
#define LISTEN_RAW 2
static int listen_mode;

#define OFFER_SELECT_TIME  1   /*select offer by domain*/
#define OFFER_WAIT_TIME  2   /*wait for offer after first 2 DISCOVER send, make sure longer than OFFER_SELECT_TIME*/
#define OFFER_WAIT_LONGER_TIME  10   /*wait for offer after 3rd DISCOVER send*/

extern int route_count;/* How many routes we have got in option 33 and 121 */

#define DEFAULT_SCRIPT	"/usr/share/udhcpc/default.script"

struct client_config_t client_config = {
	/* Default options. */
	abort_if_no_lease: 0,
	foreground: 0,
	quit_after_lease: 0,
	interface: "eth0",
	pidfile: NULL,
	script: DEFAULT_SCRIPT,
	clientid: NULL,
	hostname: NULL,
#ifdef CHECK_DOMAIN
	domain: NULL,
#endif
	ifindex: 0,
	arp: "\0\0\0\0\0\0",		/* appease gcc-3.0 */
	w: NULL,
	info: NULL,
	rt: 254,
};

static void print_usage(void)
{
	printf(
"Usage: udhcpcd [OPTIONS]\n\n"
"  -c, --clientid=CLIENTID         Client identifier\n"
"  -H, --hostname=HOSTNAME         Client hostname\n"
#ifdef CHECK_DOMAIN
"  -D, --domain=DOMAIN             Domain name used to choose offer\n"
#endif
"  -f, --foreground                Do not fork after getting lease\n"
"  -i, --interface=INTERFACE       Interface to use (default: eth0)\n"
"  -n, --now                       Exit with failure if lease cannot be\n"
"                                  immediately negotiated.\n"
"  -p, --pidfile=file              Store process ID of daemon in file\n"
"  -q, --quit                      Quit after obtaining lease\n"
"  -r, --request=IP                IP address to request (default: none)\n"
"  -s, --script=file               Run file at dhcp events (default:\n"
"                                  " DEFAULT_SCRIPT ")\n"
"  -v, --version                   Display version\n"
"  -w, --w                    w argument\n"
"  -o, --info                  info argument \n"
"  -g, --g                      RouteTable  \n"
	);
}


/* just a little helper */
static void change_mode(int new_mode)
{
	DEBUG(LOG_INFO, "entering %s listen mode",
		new_mode ? (new_mode == 1 ? "kernel" : "raw") : "none");
	close(fd);
	fd = -1;
	listen_mode = new_mode;
}


/* SIGUSR1 handler (renew) */
static void renew_requested(int sig)
{
	sig = 0;
	if (sig)
	{
		/* let gcc463 not complain. */
	}
	LOG(LOG_INFO, "Received SIGUSR1 or sig 50, state=%d ", state);
	if (state == BOUND || state == RENEWING || state == REBINDING) {
        change_mode(LISTEN_KERNEL);
		packet_num = 0;
		state = RENEW_REQUESTED;
	}

	if (state == RELEASED) {
		change_mode(LISTEN_RAW);
	    state = INIT_SELECTING;
	}
	/* Kill any timeouts because the user wants this to hurry along */
	timeout = 0;
	/* should clear send packet num */
	packet_num = 0;
}


/* SIGUSR2 handler (release) */
static void release_requested(int sig)
{
//	sig = 0;
	LOG(LOG_INFO, "Received SIGUSR2, state=%d,sig=%d ", state, sig);

	if(sig == 51)
	    run_script(NULL, "cabledown");
	else
	{
    	/* send release packet */
    	if (state == BOUND || state == RENEWING || state == REBINDING) {
    		send_release(server_addr, requested_ip); /* unicast */
    		if(sig == SIGUSR2)/*release request*/
    		run_script(NULL, "deconfig");
    	}

    	change_mode(LISTEN_NONE);
    	state = RELEASED;
    	timeout = 0x7fffffff;
    }
}


/* Exit and cleanup */
static void exit_client(int retval)
{
	pidfile_delete(client_config.pidfile);
	CLOSE_LOG();
	exit(retval);
}


/* SIGTERM handler */
static void terminate(int sig)
{
	sig = 0;

	if (sig)
	{
		/* let gcc463 not complain. */
	}

    /* follow netgear Spec1.9
        Send a DHCP RELEASE message to the server under the case software shutdown or reboot.
    */
    release_requested(0);
	LOG(LOG_INFO, "Received SIGTERM");
	exit_client(0);
}

#if 0
static void background(void)
{
	int pid_fd;
	if (client_config.quit_after_lease) {
		exit_client(0);
	} else if (!client_config.foreground) {
		pid_fd = pidfile_acquire(client_config.pidfile); /* hold lock during fork. */
		if (daemon(0, 0) == -1) {
			perror("fork");
			exit_client(1);
		}
		client_config.foreground = 1; /* Do not fork again. */
		pidfile_write_release(pid_fd);
	}
}
#endif


#ifdef CHECK_DOMAIN
int get_domain_from_offer(char* dest, int dest_len, struct dhcpMessage* packet)
{
    unsigned char* temp = NULL;
    if(dest == NULL)
        return -1;
    memset(dest,0,dest_len);
    if((temp = get_option(packet, DHCP_DOMAIN_NAME)) != NULL)
    {
        int domain_len = temp[OPT_LEN - 2]>dest_len?dest_len:temp[OPT_LEN - 2];
        memcpy(dest, temp, domain_len);
        return 0;
    }
    return -1;
}

int from_last_selected_server(struct dhcpMessage* packet)
{
    unsigned char* temp = NULL;
    if((temp = get_option(packet, DHCP_SERVER_ID)) != NULL)
    {
	    FILE *fp;
        unsigned long server_in_packet;
        unsigned long server_in_file;
    	char old_server_ip[18]="";
    	 char server_file[]= DHCPC_SERVER"XX";

        memcpy(&server_in_packet, temp, 4);
        sprintf(server_file,DHCPC_SERVER"%d",atoi(client_config.w));
        if((fp=fopen(server_file,"r+")))
        {
            char *p = NULL;
        	fgets(old_server_ip,16,fp);
        	fclose(fp);

        	if((p = strchr(old_server_ip,'\n')) != NULL)
        	    *p = '\0';
        	server_in_file = inet_network(old_server_ip);
        	DEBUG(LOG_INFO,"new server: %lu VS old server: %lu(%s)",server_in_packet,server_in_file,old_server_ip);
        	if(server_in_packet == server_in_file){
        		return 1;
        	}
        }
    }
    return 0;
}
#endif

/* Get IP of @ifname. Return 0 if error. */
static unsigned long get_iface_ip(char *ifname) {
	int fd = -1;
	struct ifreq ifr;
	struct sockaddr_in *saddr;

	if ((fd = socket(PF_INET, SOCK_DGRAM, 0)) == -1) {
		perror("user: socket creating failed");
		return 0;
	}
    strcpy(ifr.ifr_name, ifname);
    ifr.ifr_addr.sa_family = AF_INET;
    /* get ip address */
    if (ioctl(fd, SIOCGIFADDR, &ifr)==0){
        close(fd);
        saddr = (struct sockaddr_in *)&ifr.ifr_addr;
        return saddr->sin_addr.s_addr;
    }
    close(fd);
    return 0;
}

#ifdef COMBINED_BINARY
int udhcpc(int argc, char *argv[])
#else
int main(int argc, char *argv[])
#endif
{
	unsigned char *temp = NULL, *message = NULL;
	unsigned long t1 = 0, t2 = 0, xid = 0;
	unsigned long start = 0, lease;
	fd_set rfds;
	int retval = 0;
	struct timeval tv;
	int c, len;
	struct dhcpMessage packet;
	struct dhcpMessage last_packet;
	struct in_addr temp_addr;
	int pid_fd;
	time_t now;
#ifdef CHECK_DOMAIN
	struct dhcpMessage* recv_offer=NULL;
	struct timespec timeout2;/*time of choosing offer in usec*/
	int use_record = 0;
	struct timespec now2;/*current time in nsec*/
	long usec_left = 0;
#endif

	static struct option options[] = {
		{"clientid",	required_argument,	0, 'c'},
		{"foreground",	no_argument,		0, 'f'},
		{"hostname",	required_argument,	0, 'H'},
#ifdef CHECK_DOMAIN
		{"domain",	required_argument,	0, 'D'},
#endif
		{"help",	no_argument,		0, 'h'},
		{"interface",	required_argument,	0, 'i'},
		{"now", 	no_argument,		0, 'n'},
		{"pidfile",	required_argument,	0, 'p'},
		{"quit",	no_argument,		0, 'q'},
		{"request",	required_argument,	0, 'r'},
		{"script",	required_argument,	0, 's'},
		{"version",	no_argument,		0, 'v'},
		{"w",	required_argument,	0, 'w'},
		{"info",	required_argument,	0, 'o'},
		{"g",	required_argument,	0, 'g'},
		{0, 0, 0, 0}
	};

	memset(&last_packet, 0, sizeof(last_packet));
	/* get options */
	while (1) {
		int option_index = 0;
#ifdef CHECK_DOMAIN
		c = getopt_long(argc, argv, "c:fH:D:g:hi:np:qr:s:vw:o:", options, &option_index);
#else
		c = getopt_long(argc, argv, "c:fH:hi:g:np:qr:s:vw:o:", options, &option_index);
#endif

		if (c == -1) break;

		switch (c) {
		case 'c':
			len = strlen(optarg) > 255 ? 255 : strlen(optarg);
			if (client_config.clientid) free(client_config.clientid);
			client_config.clientid = malloc(len + 2);
			client_config.clientid[OPT_CODE] = DHCP_CLIENT_ID;
			client_config.clientid[OPT_LEN] = len;
			client_config.clientid[OPT_DATA] = '\0';
			strncpy((char *)client_config.clientid + 3, optarg, len - 1);
			break;
		case 'f':
			client_config.foreground = 1;
			break;
		case 'H':
			len = strlen(optarg) > 255 ? 255 : strlen(optarg);
			if (client_config.hostname) free(client_config.hostname);
			client_config.hostname = malloc(len + 2);
			client_config.hostname[OPT_CODE] = DHCP_HOST_NAME;
			client_config.hostname[OPT_LEN] = len;
			strncpy((char *)client_config.hostname + 2, optarg, len);
			break;
#ifdef CHECK_DOMAIN
		case 'D':
			len = strlen(optarg) > 60 ? 60 : strlen(optarg);
			if (client_config.domain) free(client_config.domain);
			client_config.domain = malloc(len+1);
			strncpy(client_config.domain, optarg, len);
			break;
#endif
		case 'h':
			print_usage();
			return 0;
		case 'i':
			client_config.interface =  optarg;
			break;
		case 'n':
			client_config.abort_if_no_lease = 1;
			break;
		case 'p':
			client_config.pidfile = optarg;
			break;
		case 'q':
			client_config.quit_after_lease = 1;
			break;
		case 'r':
			requested_ip = inet_addr(optarg);
			break;
		case 's':
			client_config.script = optarg;
			break;
		case 'w':
			client_config.w = optarg;
			break;
		case 'o':
			client_config.info = optarg;
			break;
		case 'g':
			client_config.rt = atoi(optarg);
			break;
		case 'v':
			printf("udhcpcd, version %s\n\n", VERSION);
			exit_client(0);
			break;
		}
	}

	OPEN_LOG("udhcpc");
	LOG(LOG_INFO, "udhcp client (v%s) started", VERSION);

	pid_fd = pidfile_acquire(client_config.pidfile);
	pidfile_write_release(pid_fd);

	if (read_interface(client_config.interface, &client_config.ifindex,
			   NULL, client_config.arp) < 0)
		exit_client(1);

	if (!client_config.clientid) {
		client_config.clientid = malloc(6 + 3);
		client_config.clientid[OPT_CODE] = DHCP_CLIENT_ID;
		client_config.clientid[OPT_LEN] = 7;
		client_config.clientid[OPT_DATA] = 1;
		memcpy(client_config.clientid + 3, client_config.arp, 6);
	}

	/* setup signal handlers */
	signal(SIGUSR1, renew_requested);
	signal(51, release_requested);
	signal(SIGUSR2, release_requested);
	signal(SIGTERM, terminate);

	state = INIT_SELECTING;
    /* Why we need to call deconfig at this stage? Just clear interface IP is enough. -- Argon */
#if 0
	run_script(NULL, "deconfig");
#else
    /* Clear interface IP */
    {
        int sockfd;
        struct ifreq ifr;
        struct sockaddr_in *sin;

        if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) >= 0) {
            memset(&ifr, 0, sizeof(ifr));
            strncpy(ifr.ifr_name, client_config.interface, sizeof(ifr.ifr_name)-1);
            sin = (struct sockaddr_in *)&ifr.ifr_addr;
            sin->sin_family = AF_INET;
            memset(&sin->sin_addr, 0, sizeof(sin->sin_addr));
            ioctl(sockfd, SIOCSIFADDR, &ifr);
        }
    }
#endif

	change_mode(LISTEN_RAW);

	for (;;) {
#if 0 /*system time independent*/
        now = time(0);
#else
	    clock_gettime(CLOCK_MONOTONIC, &now2);
        now = now2.tv_sec;
#endif
#ifdef CHECK_DOMAIN
        usec_left = diff(now2,timeout2);
#endif
		tv.tv_sec = timeout - now;
		tv.tv_usec = 0;
		FD_ZERO(&rfds);

		if (listen_mode != LISTEN_NONE && fd < 0) {
			if (listen_mode == LISTEN_KERNEL)
				fd = listen_socket(INADDR_ANY, CLIENT_PORT, client_config.interface);
			else
				fd = raw_socket(client_config.ifindex);
			if (fd < 0) {
				LOG(LOG_ERR, "FATAL: couldn't listen on socket, %s", strerror(errno));
				exit_client(0);
			}
		}
		if (fd >= 0) FD_SET(fd, &rfds);

#ifdef CHECK_DOMAIN
			if(state == INIT_SELECTING
			    && usec_left <= 0
			    && recv_offer != NULL)
			{
		        /*use_record set to be 1 at this moment:
		        SELECTING, OFFER received before, choosing timeout*/
		        DEBUG(LOG_INFO,"no matched OFFER before choosing timeout, use record");
			    retval = 1;
			    use_record = 1;
			}
			else
			{
#endif
		if (tv.tv_sec > 0) {
#ifdef CHECK_DOMAIN

			{
    			if(state == INIT_SELECTING
    			    && usec_left > 0)
    			{
    			    /*choosing offer, block short time*/
    			    tv.tv_sec = 0;
		            tv.tv_usec = usec_left;
		            DEBUG(LOG_INFO,"%ld usec left, Choosing OFFER...",usec_left);
    			}
    			else
    			    DEBUG(LOG_INFO,"keep waiting: %ld sec ...",(long)tv.tv_sec);
#endif
			DEBUG(LOG_INFO, "Waiting on select...\n");
			retval = select(fd + 1, &rfds, NULL, NULL, &tv);
#ifdef CHECK_DOMAIN
    			if(retval == 0)/*no packet recved, loop again*/
    			{
    			        continue;
    			}
			}
#endif

		} else retval = 0; /* If we already timed out, fall through */

#ifdef CHECK_DOMAIN
            }
#endif
#if 0/*system time independent*/
		now = time(0);
#else
        clock_gettime(CLOCK_MONOTONIC, &now2);
        now = now2.tv_sec;
#endif
		if (retval == 0) {
			/* timeout dropped to zero */
			switch (state) {
			case INIT_SELECTING:
				if (packet_num < 3) {
					if (packet_num == 0)
					{
						xid = random_xid();
#ifdef CHECK_DOMAIN
						if(recv_offer != NULL)
						{
						    free(recv_offer);
						    recv_offer = NULL;
						}
						use_record = 0;
#endif
				    }
					/* send discover packet */
					send_discover(xid, requested_ip); /* broadcast */
					timeout = now + ((packet_num == 2) ? OFFER_WAIT_LONGER_TIME : OFFER_WAIT_TIME);
#ifdef CHECK_DOMAIN
#if 1
//					clock_gettime(CLOCK_MONOTONIC, &timeout2);
					timeout2.tv_sec = now2.tv_sec + OFFER_SELECT_TIME;
					timeout2.tv_nsec = now2.tv_nsec;
#else
					timeout2 = now + OFFER_SELECT_TIME;/*select offer in next 1 second*/
#endif
#endif
					packet_num++;
				} else {
					if (client_config.abort_if_no_lease) {
						LOG(LOG_INFO, "No lease, failing.");
						exit_client(1);
				  	}
					/* wait to try again */
					packet_num = 0;
                    /* -----------------------------------------------------
                     * Change DHCP client renew interval to 300s to fit for
                     * Netgear WNR2000v3 SPEC - Boom Zhou
                     */
					/*timeout = now + 35; // changed the time from 60 to 35 seconds*/
                    //timeout = now + 290;
                    /* Home router spec Rev12 request the DHCP procedure is 5 mins,
                  * but it brings to many issues, such as UK/TalkTalk, it results 
                  * user taking 7-8 mins to obtain IP since the procedure is too long.
                  * and Rev13 has changed the procedure from 5 mins to 10 secs. */
                    timeout = now + 10;
                    /* ---------------------------------------------------- */
//					system("/usr/sbin/led_app on 30");
					/* call auto ip process RFC 3927 */
					run_script(NULL, "FAIL");

				}
				break;
			case RENEW_REQUESTED:
			case REQUESTING:
				if (packet_num < 3) {
					/* send request packet */
					if (state == RENEW_REQUESTED)
						send_renew(xid, server_addr, requested_ip); /* unicast */
					else send_selecting(xid, server_addr, requested_ip); /* broadcast */

					if (state == RENEW_REQUESTED)
						timeout = now + 1;
					else 
						timeout = now + ((packet_num == 2) ? 10 : 2);
					packet_num++;
				} else {
					/* timed out, go back to init state */
					state = INIT_SELECTING;
					timeout = now;
					packet_num = 0;
					change_mode(LISTEN_RAW);
					/* Ron timeout */
//					system("/usr/sbin/led_app on 30");
				}
				break;
			case BOUND:
				/* Lease is starting to run out, time to enter renewing state */
                change_mode(LISTEN_KERNEL);
                /* leased time value 0xFFFFFFFF assigned from server means infinity. */
                if (lease == 0xFFFFFFFF) {
                    timeout = now + 0x7FFFFFFF;
                    break;
                }
				state = RENEWING;
				DEBUG(LOG_INFO, "Entering renew state");
				/* fall right through */
			case RENEWING:
				/* Either set a new T1, or enter REBINDING state */
				if ((t2 - t1) <= (lease / 14400 + 1)) {
					/* timed out, enter rebinding state */
					state = REBINDING;
					timeout = now + (t2 - t1);
					DEBUG(LOG_INFO, "Entering rebinding state");
				} else {
					send_renew(xid, server_addr, requested_ip); /* unicast */

					t1 = (t2 - t1) / 2 + t1;
					timeout = t1 + start;
				}
				break;
			case REBINDING:
				/* Either set a new T2, or enter INIT state */
				if ((lease - t2) <= (lease / 14400 + 1)) {
					/* timed out, enter init state */
					state = INIT_SELECTING;
					LOG(LOG_INFO, "Lease lost, entering init state");
					run_script(NULL, "deconfig");
					timeout = now;
					packet_num = 0;
					change_mode(LISTEN_RAW);
				} else {
					/* send a request packet */
					send_renew(xid, 0, requested_ip); /* broadcast */

					t2 = (lease - t2) / 2 + t2;
					timeout = t2 + start;
				}
				break;
			case RELEASED:
				/* yah, I know, *you* say it would never happen */
				timeout = 0x7fffffff;
				break;
			}
		} else if (retval > 0 &&
		    (
		    (listen_mode != LISTEN_NONE && FD_ISSET(fd, &rfds))/*received a packet*/

#ifdef CHECK_DOMAIN
		    || use_record == 1/*no new offer received, but using first one*/
#endif
		    )
		    ){
			/* a packet is ready, read it */
#ifdef CHECK_DOMAIN
            if(use_record == 1)
            {
                /*copy selected offer to packet, like we just received it*/
                DEBUG(LOG_INFO, "copy record OFFER back to use...");
                memcpy(&packet,recv_offer,sizeof(packet));
            }
            else
            {
#endif
    			if (listen_mode == LISTEN_KERNEL)
    				len = get_packet(&packet, fd);
    			else len = get_raw_packet(&packet, fd);

    			if (len == -1 && errno != EINTR) {
    				DEBUG(LOG_INFO, "error on read, %s, reopening socket", strerror(errno));
    				change_mode(listen_mode); /* just close and reopen */
    			}
    			if (len < 0) continue;

    			if (packet.xid != xid) {
    				DEBUG(LOG_INFO, "Ignoring XID %lx (our xid is %lx)",
    					(unsigned long) packet.xid, xid);
    				continue;
    			}

    			if ((message = get_option(&packet, DHCP_MESSAGE_TYPE)) == NULL) {
				DEBUG(LOG_ERR, "couldnt get option from packet -- ignoring");
    				continue;
    			}
#ifdef CHECK_DOMAIN
    	    }
#endif
            switch (state) {
			case INIT_SELECTING:
				/* Must be a DHCPOFFER to one of our xid's */
				if (*message == DHCPOFFER) {
#ifdef CHECK_DOMAIN
				    if(use_record != 1)
				    {
    				    char offer_domain[70+1];
    				    if( client_config.domain != NULL
    				        && *(char*)client_config.domain != '\0'
    				        && get_domain_from_offer(offer_domain,70,&packet) == 0
    				        && strcmp(offer_domain,client_config.domain)== 0)
    				    {
    				        DEBUG(LOG_INFO, "domain(%s) check OK, select this offer",client_config.domain);
    				    }
    				    else if(recv_offer != NULL &&
    				            from_last_selected_server(&packet))
    				    {
    				        DEBUG(LOG_INFO,"Same server with pre selected, use this offer?");
    				        /*overwrite recv_offer, go on waiting for damain matched offer*/
			                memcpy(recv_offer,&packet,sizeof(packet));
			                break;
    				    }
    				    else
    			        {/*got an offer, but domain not matched, recode it and wait*/

    			            if(recv_offer == NULL)/*no record yet*/
    			            {
    			                DEBUG(LOG_INFO, "domain(%s) check failed, record this as first offer(%s)",client_config.domain,offer_domain);
    			                recv_offer = malloc(sizeof(packet));
    			                if(recv_offer == NULL)
    			                {
    			                    DEBUG(LOG_INFO,"malloc failed, break...");
    			                    break;
    			                }
    			                memcpy(recv_offer,&packet,sizeof(packet));
    			                break;
    			            }
    			            else/*not first offer nor domain-matcher, ignore this*/
    			            {
    			                DEBUG(LOG_INFO, "domain(%s) check faild, ignore this offer(%s) ",client_config.domain,offer_domain);
    			                break;
    			            }
    			        }
			        }
			        else
			        {
			            DEBUG(LOG_INFO, "need no domain(%s) check at this moment(packet_num %d, use_record %d)",client_config.domain,packet_num,use_record);
			        }
			        /*state machine goes on, free copy of the offer*/
			        if(recv_offer)
			        {
			            free(recv_offer);
			            recv_offer = NULL;
			        }
			        use_record = 0;/*we had selected OFFER now, reset the flag*/

#endif

					if ((temp = get_option(&packet, DHCP_SERVER_ID))) {
						memcpy(&server_addr, temp, 4);
						xid = packet.xid;
						requested_ip = packet.yiaddr;

						/* enter requesting state */
						state = REQUESTING;
						timeout = now;
						packet_num = 0;
					} else {
						DEBUG(LOG_ERR, "No server ID in message");
					}
				}
				break;
			case RENEW_REQUESTED:
			case REQUESTING:
			case RENEWING:
			case REBINDING:
				if (*message == DHCPACK) {
					/* Ron */
					FILE *fp;
					int packet_changed;
					/* Ron */
					if (!(temp = get_option(&packet, DHCP_LEASE_TIME))) {
						LOG(LOG_ERR, "No lease time with ACK, using 1 hour lease");
						lease = 60 * 60;
					} else {
						memcpy(&lease, temp, 4);
						lease = ntohl(lease);
					}
                    /*  Follow netgear Spec1.9
                        Must use ARP to detect conflict with the assigned IP address
                            after DHCPACK is received.
                        if there is a conflict detected, send a DHCPDECLINE back to server,
                            restart dhcp process after 10 seconds
                     */
                    if(arpping(packet.yiaddr, NULL, packet.yiaddr, packet.chaddr, client_config.interface) == 0){
                        /* Send DHCPDECLINET to server */
                        send_decline(xid, server_addr, packet.yiaddr);
                        state = INIT_SELECTING;
                        timeout = now;
                        requested_ip = 0;
                        packet_num = 0;
                        change_mode(LISTEN_RAW);
                        sleep(10);
                        break;
                    }

					/* enter bound state */
					t1 = lease / 2;

					/* little fixed point for n * .875 */
					t2 = (lease * 0x7) >> 3;
					temp_addr.s_addr = packet.yiaddr;
					LOG(LOG_INFO, "Lease of %s obtained, lease time %ld",
						inet_ntoa(temp_addr), lease);
					/*
					 * Write info to DHCPC_LEASE
					 */
					{
					    struct sysinfo info;
                        char lease_file[]= DHCPC_LEASE"XX";
                        char server_file[]= DHCPC_SERVER"XX";
                        char cmdbuf[256]="";
                        sysinfo(&info);
					    temp_addr.s_addr = server_addr;

                        sprintf(lease_file,DHCPC_LEASE"%d",atoi(client_config.w));
                        sprintf(server_file,DHCPC_SERVER"%d",atoi(client_config.w));
					    fp=fopen(lease_file, "w");
					    if(fp!=NULL){
                            fprintf(fp,"%s\n%lu\n%ld\n" ,inet_ntoa(temp_addr), lease, info.uptime);
						    fclose(fp);
#ifdef CHECK_DOMAIN
                            sprintf(cmdbuf,"/bin/cp %s %s -f",lease_file,server_file);
						    system(cmdbuf);
						    if(atoi(client_config.w) == 1)/*todo: 1 as default wan*/
						    {
                                sprintf(cmdbuf,"/bin/cp %s %s -f",lease_file,DHCPC_LEASE);
    						    system(cmdbuf);
    						}
#endif
					    }
					}

					temp_addr.s_addr = packet.yiaddr;

					/* Ron */
					start = now;
					timeout = t1 + start;
					requested_ip = packet.yiaddr;
                    /* Run script only if we get different IP address. -- Argon */
                    /* Also Need run script even if we get same IP address
                     * but this is a renew for cable down and then cable up
                     * In this case, the /tmp/wan_uptime is deleted when cable down
                     * cindy , 2011-2-15
                     */
                    LOG(LOG_DEBUG, "DHCP get IP: %08lx, Current interface %s IP: %08lx.", requested_ip, client_config.interface, get_iface_ip(client_config.interface));
					packet_changed = dhcp_packet_cmp(&last_packet, &packet);
					if ( (requested_ip != get_iface_ip(client_config.interface) )
							|| packet_changed
							|| (access("/tmp/wan_uptime",F_OK)!=0) )	{
			 		    run_script(&packet,
						   ((state == RENEWING || state == REBINDING) ? "renew" : "bound"));
						memcpy(&last_packet, &packet, sizeof(packet));
				    }
					else
					{
						LED_ACTION_WAN_UP();
					}
					state = BOUND;
					change_mode(LISTEN_NONE);
                    /* Support option 121 249 33 */
                    /* change for having same behavior with v2 */
                    if (packet_changed)
                    {
                        char filename[128];
                        sprintf(filename, "%s%s", DHCPC_ROUTE_RELATED_OPTIONS_DEL, client_config.w);
                        /* clean old route record. */
                        if (access(filename, F_OK) == 0)
                        {
                            char cmdbuf[256];
                            sprintf(cmdbuf, "/bin/sh %s", filename);
                            system(cmdbuf);
                            sprintf(cmdbuf, "/bin/rm -f %s", filename);
                            system(cmdbuf);
                            /* restore static route. */
                            system("/usr/sbin/rc route restart");
                        }
                        route_count = 0;  /* We should initialized route_count before checking options. */
                        sc_option_route_process(&packet, DHCP_CLASSLESS_ROUTE);
                        sc_option_route_process(&packet, DHCP_MS_CLASSLESS_ROUTE);
                        sc_option33(&packet);
                    }
					//background();

				} else if (*message == DHCPNAK) {
					/* return to init state */
					LOG(LOG_INFO, "Received DHCP NAK");
					run_script(&packet, "nak");
					if (state != REQUESTING)
						run_script(NULL, "deconfig");
					state = INIT_SELECTING;
					timeout = now;
					requested_ip = 0;
					packet_num = 0;
					change_mode(LISTEN_RAW);

					sleep(3); /* avoid excessive network traffic */
				}
				break;
			/* case BOUND, RELEASED: - ignore all packets */
			}
		} else if (retval == -1 && errno == EINTR) {
			/* a signal was caught */

		} else {
			/* An error occured */
			DEBUG(LOG_ERR, "Error on select");
		}

	}
	return 0;
}

