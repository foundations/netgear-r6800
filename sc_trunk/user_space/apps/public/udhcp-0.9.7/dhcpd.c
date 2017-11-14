/* dhcpd.c
 *
 * Moreton Bay DHCP Server
 * Copyright (C) 1999 Matthew Ramsay <matthewr@moreton.com.au>
 *			Chris Trew <ctrew@moreton.com.au>
 *
 * Rewrite by Russ Dill <Russ.Dill@asu.edu> July 2001
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

#include <fcntl.h>
#include <string.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <signal.h>
#include <errno.h>
#include <sys/ioctl.h>
#include <time.h>
#include <sys/time.h>

#include "debug.h"
#include "dhcpd.h"
#include "arpping.h"
#include "socket.h"
#include "options.h"
#include "files.h"
#include "leases.h"
#include "packet.h"
#include "serverpacket.h"
#include "pidfile.h"
#include "sc_debug.h"

/* globals */
struct dhcpOfferedAddr *leases;
struct server_config_t server_config[MAX_INTERFACES];
int no_of_ifaces = 0;

/* Exit and cleanup */
static void exit_server(int retval, int ifid)
{
	pidfile_delete(server_config[ifid].pidfile);
	CLOSE_LOG();
	exit(retval);
}


/* SIGTERM handler */
static void udhcpd_killed(int sig)
{
	sig = 0;
	if (sig)
	{
		/* let gcc463 not complain. */
	}
	syslog(LOG_INFO, "DHCP server stopped");
	LOG(LOG_INFO, "Received SIGTERM");
	exit_server(0, 0);
}

#ifdef COMBINED_BINARY
int udhcpd(int argc, char *argv[])
#else
int main(int argc, char *argv[])
#endif
{
	fd_set rfds;
	struct timeval tv;
	int server_socket[MAX_INTERFACES];
	int bytes, retval;
	struct dhcpMessage packet;
	unsigned char *state;
	unsigned char *server_id, *requested , *clinet_id;;
	u_int32_t server_id_align, requested_align;
	unsigned long timeout_end[MAX_INTERFACES];
	struct option_set *option;
	struct dhcpOfferedAddr *lease , *lease1 = NULL;
	int pid_fd;
	int i;
	//add jim
	//char myClientId[7];
	//char packetClientId[254][7];
	//char leaseClientId[254][7];
	//int myID=0;
#ifdef TR069
    unsigned char *vendor_spc=NULL;
#endif

	if (lease1)
	{
		/* let gcc463 not complain. */
	}

	OPEN_LOG("udhcpd");
	syslog(LOG_INFO, "DHCP server started");
	LOG(LOG_INFO, "udhcp server (v%s) started", VERSION);

	for (i = 0; i < MAX_INTERFACES; i++)
		memset(&server_config[i], 0, sizeof(struct server_config_t));

	if (argc < 2)
		read_config(DHCPD_CONF_FILE);
	else read_config(argv[1]);

	if (no_of_ifaces == 0)
		exit(0);

	for (i = 0; i < no_of_ifaces; i++)
	{
		pid_fd = pidfile_acquire(server_config[i].pidfile);
		pidfile_write_release(pid_fd);

		if ((option = find_option(server_config[i].options, DHCP_LEASE_TIME))) {
			memcpy(&server_config[i].lease, option->data + 2, 4);
			server_config[i].lease = ntohl(server_config[i].lease);
		}
		else server_config[i].lease = LEASE_TIME;

		leases = malloc(sizeof(struct dhcpOfferedAddr) * server_config[i].max_leases);
		memset(leases, 0, sizeof(struct dhcpOfferedAddr) * server_config[i].max_leases);

		read_leases(server_config[i].lease_file, i);

		if (read_interface(server_config[i].interface, &server_config[i].ifindex,
			   &server_config[i].server, server_config[i].arp) < 0)
			server_config[i].active = FALSE;
		else
			server_config[i].active = TRUE;

#ifndef DEBUGGING
		pid_fd = pidfile_acquire(server_config[i].pidfile); /* hold lock during fork. */
		/* cfgmr req: do not fork */
		/*
		if (daemon(0, 0) == -1) {
			perror("fork");
			exit_server(1, i);
		}
		*/

		pidfile_write_release(pid_fd);
#endif

		signal(SIGUSR1, write_leases);
		signal(SIGTERM, udhcpd_killed);
	}

	for (i = 0; i < no_of_ifaces; i++) {
		timeout_end[i] = time(0) + server_config[i].auto_time;
		server_socket[i] = -1;
		LOG(LOG_INFO, "interface: %s, start : %x end : %x\n", server_config[i].interface, server_config[i].start, server_config[i].end);
	}

	while(1) { /* loop until universe collapses */
		for (i = 0; i < no_of_ifaces; i++)
		{
			if (server_config[i].active == FALSE)
					continue;

			if (server_socket[i] < 0)
				if ((server_socket[i] = listen_socket(INADDR_ANY, SERVER_PORT, server_config[i].interface)) < 0) {
					LOG(LOG_ERR, "FATAL: couldn't create server socket, %s", strerror(errno));
					exit_server(0, i);
				}

			FD_ZERO(&rfds);
			FD_SET(server_socket[i], &rfds);

			if (server_config[i].auto_time) {
				tv.tv_sec = timeout_end[i] - time(0);
				if (tv.tv_sec <= 0) {
					tv.tv_sec = server_config[i].auto_time;
					timeout_end[i] = time(0) + server_config[i].auto_time;
					write_leases(i);
				}
				tv.tv_usec = 0;
			}
			retval = select(server_socket[i] + 1, &rfds, NULL, NULL, server_config[i].auto_time ? &tv : NULL);

			if (retval == 0) {
				write_leases(i);
				timeout_end[i] = time(0) + server_config[i].auto_time;
				continue;
			} else if (retval < 0) {
				DEBUG(LOG_INFO, "error on select");
				continue;
			}

			if ((bytes = get_packet(&packet, server_socket[i])) < 0) { /* this waits for a packet - idle */
				if (bytes == -1 && errno != EINTR) {
					DEBUG(LOG_INFO, "error on read, %s, reopening socket", strerror(errno));
					close(server_socket[i]);
					server_socket[i] = -1;
				}
				continue;
			}

			if ((state = get_option(&packet, DHCP_MESSAGE_TYPE)) == NULL) {
				DEBUG(LOG_ERR, "couldn't get option from packet, ignoring");
				continue;
			}

			/* ADDME: look for a static lease */
			clinet_id = get_option(&packet, DHCP_CLIENT_ID);
			lease = find_lease_by_chaddr(packet.chaddr, i);
			if(clinet_id!=NULL)
			lease1 = find_lease_by_cid(clinet_id, i);
			else
			lease1=NULL;
			switch (state[0]) {
				case DHCPDISCOVER:
					DEBUG(LOG_INFO, "DHCP server received DISCOVER from %02X:%02X:%02X:%02X:%02X:%02X",
						   packet.chaddr[0], packet.chaddr[1], packet.chaddr[2], packet.chaddr[3], packet.chaddr[4], packet.chaddr[5]);
					SC_CFPRINTF("DHCP server received DISCOVER from %02X:%02X:%02X:%02X:%02X:%02X \n",
					        packet.chaddr[0], packet.chaddr[1], packet.chaddr[2], packet.chaddr[3], packet.chaddr[4], packet.chaddr[5]);
					/*
					if(lease1 && !lease)
					sendNAK(&packet, i);
			           else
			        */
			        {
    					if (sendOffer(&packet, i) < 0) {
    						LOG(LOG_ERR, "send OFFER failed");
    						printf("test1\n");
    					}
					}
					break;

				case DHCPREQUEST:
					DEBUG(LOG_INFO, "DHCP server received REQUEST from %02X:%02X:%02X:%02X:%02X:%02X",
						   packet.chaddr[0], packet.chaddr[1], packet.chaddr[2], packet.chaddr[3], packet.chaddr[4], packet.chaddr[5]);
					SC_CFPRINTF("DHCP server received REQUEST from %02X:%02X:%02X:%02X:%02X:%02X\n",
						   packet.chaddr[0], packet.chaddr[1], packet.chaddr[2], packet.chaddr[3], packet.chaddr[4], packet.chaddr[5]);
					requested = get_option(&packet, DHCP_REQUESTED_IP);
					server_id = get_option(&packet, DHCP_SERVER_ID);
					if (requested) memcpy(&requested_align, requested, 4);
					if (server_id) memcpy(&server_id_align, server_id, 4);
                    
					if (lease) 
					{ /*ADDME: or static lease */
					    SC_CFPRINTF("lease->yiaddr is %x, packet.ciaddr is %x\n", lease->yiaddr, packet.ciaddr);
						if (server_id) 
						{
							/* SELECTING State */
							DEBUG(LOG_INFO, "server_id = %08x", ntohl(server_id_align));
							if (server_id_align == server_config[i].server && requested &&
					    		requested_align == lease->yiaddr) {
								sendACK(&packet, lease->yiaddr, i);
							}
						} 
					    else 
					    {
							if (requested) {
								/* INIT-REBOOT State if the REQUEST IP is lan ip, need send NAK.*/
								if ((lease->yiaddr == requested_align) && (requested_align != server_config[i].server) )
									sendACK(&packet, lease->yiaddr, i);
								else
								{
								    SC_CFPRINTF("\n*************NACK 1***********\n");
								    sendNAK(&packet, i);
								}
							} else {
								/* RENEWING or REBINDING State */
								if (lease->yiaddr == packet.ciaddr)
								{
								    SC_CFPRINTF("295, find the record\n");
									sendACK(&packet, lease->yiaddr, i);
								}
								else {
								     SC_CFPRINTF("\n*************NACK 2***********\n");
									SC_CFPRINTF("lease->yiaddr is %x, packet.ciaddr is %x\n", lease->yiaddr, packet.ciaddr);
									/* don't know what to do!!!! */
									sendNAK(&packet, i);
								}
							}
						}
                        			/* what to do if we have no record of the client */
					 } 
					 else if (server_id) 
					 {
					                /* SELECTING State */
                            if (server_id_align == server_config[i].server)
							{
							     SC_CFPRINTF("\n*************NACK 3***********\n");
							    sendNAK(&packet,i);
							}

                     } else if (requested) 
                     {
					               /* INIT-REBOOT State */
					    if ((lease = find_lease_by_yiaddr(requested_align,i)))
					    {
		            		if (lease_expired(lease)) 
		            		{
		               			/* probably best if we drop this lease */
			               		memset(lease->chaddr, 0, 16);
			               		/* make some contention for this address */
			            	} 
			            	else 
			            	{
			            	    SC_CFPRINTF("\n*************NACK 4***********\n");
					            sendNAK(&packet,i);
			    		    }
			                        	
			             } else if (requested_align < server_config[i].start ||
					                   requested_align > server_config[i].end) 
					     {
					       SC_CFPRINTF("\n*************NACK 5***********\n");
					                  sendNAK(&packet,i);
					     } 
					     else 
					     {
					        SC_CFPRINTF("\n*************NACK 6***********\n");
								/* else remain silent */
						    sendNAK(&packet,i);
					     }

		              } 
		              else {
		                SC_CFPRINTF("\n*************NACK 7***********\n");
		                SC_CFPRINTF("\nLease find fail and no request ip and server id option\n");
                               				/* RENEWING or REBINDING State */
							sendNAK(&packet,i);

                       }
                        	break;


				case DHCPDECLINE:
					DEBUG(LOG_INFO,"received DECLINE");
					SC_CFPRINTF("received DECLINE\n");
					/* except static IP lease */
					if (lease) {
						memset(lease->chaddr, 0, 16);
						lease->expires = time(0) + server_config[i].decline_time;
					}
					break;

				case DHCPRELEASE:
					DEBUG(LOG_INFO,"received RELEASE");
					SC_CFPRINTF("received RELEASE\n");
					/* except static IP lease */
					if (lease) {
					    lease->expires = time(0);
#ifdef TR069
                        requested = get_option(&packet, DHCP_REQUESTED_IP);
					    if(requested){
					        memcpy(&requested_align, requested, 4);
					        delete_vendor_list(requested_align);
					    }
#endif
					}
					break;

				case DHCPINFORM:
					DEBUG(LOG_INFO,"received INFORM");
					SC_CFPRINTF("received INFORM\n");
					send_inform(&packet, i);
					break;

				default:
					LOG(LOG_WARNING, "unsupported DHCP message (%02x) -- ignoring", state[0]);
			}
		}
	}
	return 0;
}

