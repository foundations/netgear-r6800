#include "totd.h"
#include <errno.h>

#define _SC_DEBUG_ENABLE_
#include "sc_debug.h"

char *dnshj_sprint_inet(struct sockaddr *sa, char *address_str) {
    char tmp[MAX_DNAME];

	address_str[0] = '\0';
#ifdef USE_INET4
    	if (sa->sa_family == AF_INET) {
		struct sockaddr_in *sin_p = (struct sockaddr_in *) sa;
		inet_ntop (sin_p->sin_family, (void *) &sin_p->sin_addr, tmp,
			   MAX_DNAME);
		snprintf (address_str, MAX_DNAME, "[%s]:%d", tmp,
			  ntohs(sin_p->sin_port));
    	}
#endif
#ifdef USE_INET6
    	if (sa->sa_family == AF_INET6) {
		struct sockaddr_in6 *sin6_p = (struct sockaddr_in6 *) sa;
		inet_ntop(sin6_p->sin6_family, (void *) &sin6_p->sin6_addr,
			  tmp, MAX_DNAME);
		snprintf (address_str, MAX_DNAME, "[%s]:%d", tmp,
			  ntohs(sin6_p->sin6_port));
    	}
#endif
    	return address_str;
}

struct sockaddr *dnshj_parse_and_alloc_addr (char *caddr, int port, int *sa_len_ret) {
	char address[MAX_DNAME] = "";
	struct sockaddr *sa_p;
	int sa_len, af = 0;
	char *colon;

#ifdef USE_INET4
	sa_len = sizeof (struct sockaddr_in);
	af = AF_INET;
	
#endif

#ifdef USE_INET6
	colon = strchr (caddr, ':');
	if (colon || T.use_mapped) {
		sa_len = sizeof (struct sockaddr_in6);
		af = AF_INET6;
	}
#endif
	
	
	
	if (!af)
		return NULL;

	sa_p = malloc (sa_len);
	if (!sa_p)
		return NULL;
	memset ((void *) sa_p, 0, sa_len);
#ifdef HAVE_SA_LEN_FIELD
	sa_p->sa_len = sa_len;
#endif
	sa_p->sa_family = af;

	if (!colon && T.use_mapped)
		strcpy(address, "::ffff:");

	if (strlcat (address, caddr, MAX_DNAME) >= sizeof(address))
		return NULL;

#ifdef USE_INET4
	if (af == AF_INET) {
		struct sockaddr_in *sin_p;

		sin_p = (struct sockaddr_in *) sa_p;
		sin_p->sin_port = htons (port);
		if (!inet_aton (address, &sin_p->sin_addr)) {
			free(sa_p);
			return NULL;
		}
	}
#endif

#ifdef USE_INET6
	if (af == AF_INET6) {
		struct addrinfo hints, *res;
		char portstr[NI_MAXSERV];
		int error;

		snprintf(portstr, NI_MAXSERV, "%d", port);

		memset(&hints, 0, sizeof(hints));
           	hints.ai_family = af;
		hints.ai_socktype = SOCK_STREAM;
		hints.ai_flags = AI_NUMERICHOST;
           	error = getaddrinfo(address, portstr, &hints, &res);
           	if (error) {
                	syslog(LOG_ERR, "%s", gai_strerror(error));
			if (res)
				freeaddrinfo(res);
			return NULL;
		}
		memcpy(sa_p, res->ai_addr, sa_len);

		if (res)
			freeaddrinfo(res);
	}
#endif

	if (sa_len_ret)
		*sa_len_ret = sa_len;

	return sa_p;
}

void dnshj_free (Dnshj *dnshj_ptr) {
    if (dnshj_ptr) {
	if (dnshj_ptr->sa)
	    free (dnshj_ptr->sa);
	
	free (dnshj_ptr);
    }
    return;
}

void dnshj_freev (void *dnshj_p) {
	dnshj_free ((Dnshj *) dnshj_p);
	return;
}

Dnshj *dnshj_alloc (void) {
	Dnshj *dnshj_p = NULL;	/* alloc'ed */
	char *fn = "dnshj_alloc()";

	dnshj_p = malloc (sizeof (Dnshj));
	if (!dnshj_p) {
		syslog (LOG_ERR, "%s: Cannot allocate memory", fn);
		goto error;
	}
	dnshj_p->sa = malloc (sizeof (struct sockaddr_storage));
	if (!dnshj_p->sa) {
		syslog (LOG_ERR, "%s: Cannot allocate memory", fn);
		goto error;
	}

	return dnshj_p;

error:
	dnshj_free (dnshj_p);
	return NULL;
}
#ifdef HTTPD_IPV6_SUPPORT
void dnshj_init (void) {
	G_List *gl_tmp;

	if (!T.dnshj_list)
		return;

	for (gl_tmp = T.dnshj_list->next; gl_tmp->list_data; gl_tmp = gl_tmp->next) {
        	struct sockaddr *sa_p;
        	int sa_p_len;
		Dnshj *dnshj_p = NULL;

		dnshj_p = (Dnshj *) (gl_tmp->list_data);

		sa_p = dnshj_parse_and_alloc_addr (dnshj_p->hostname, 53, &sa_p_len);
		if (sa_p) {
			syslog (LOG_INFO, "Dnshj %s configured, port %d", dnshj_p->hostname, 53);
			memcpy (dnshj_p->sa, sa_p, sa_p_len);
			dnshj_p->sa_len = sa_p_len;
		} else {
			syslog (LOG_ERR, "Can't configure Dnshj %s", dnshj_p->hostname);
			if (dnshj_p->sa)
	    			free (dnshj_p->sa);
			dnshj_p->sa = NULL;
			dnshj_p->sa_len = 0;
		}
	}
}
#endif

int dnshj_add (char *hostname, char *address) {
	Dnshj *dnshj_p;
	int i, len, skip_len = 0, wp = 0;
	char 	str[MAX_DNAME + 1] = {0};
	char *sp, *dp;
	
	sp = hostname;
	dp = str;
	
	syslog (LOG_DEBUG, "fwd_add(): start");

	if (!T.dnshj_list)
		return -1;

	dnshj_p = dnshj_alloc ();
	if (!dnshj_p)
		return -1;

	strlcpy(dnshj_p->hostname, hostname, MAX_DNAME);
	strlcpy(dp+1, hostname, MAX_DNAME);
	len = strlen(hostname);
	if (len > MAX_DNAME)
		len = MAX_DNAME;
	i = 0;
	skip_len = 0;
	wp = 0;
	while (i <= len )
	{
		if(*(sp+i) == '.' || (*(sp+i) == 0))
		{
			*(dp+wp) = skip_len;
			
			wp = wp + skip_len + 1;
			skip_len = 0;
			i++;
			if (*(sp+i) == 0)
			{
				break;
			}
		}
		else
		{
			i++;
			skip_len++;
		}
			
	}
	strlcpy(dnshj_p->dns_hostname, str, MAX_DNAME);
	
#ifndef HTTPD_IPV6_SUPPORT		
	if(!inet_aton(address, (struct in_addr *)dnshj_p->sa))
	{
		syslog(LOG_ERR, "%s", gai_strerror(errno));
		dnshj_free(dnshj_p);
		return -1;
	}
	
	dnshj_p->sa_len = sizeof(struct in_addr);
#else
	// saving the dnshj ipv6 address in hostname since no other room for a ipv6 address in Dnshj (?), (dns_hostname is the saving place for dnshj domain)
	// so that later dnshj_parse_and_alloc_addr can use getifinfo to fill the dnshj_p->sa 
	// which will be used in dnshj_select for return the dnshj_p->sa 
    strlcpy(dnshj_p->hostname, address, MAX_DNAME); 
#endif

	if (list_add_tail (T.dnshj_list, dnshj_p) < 0) {
		dnshj_free (dnshj_p);
		return -1;
	}
//	seep("<%s><%s>\n", dnshj_p->dns_hostname, inet_ntoa(*((struct in_addr *)dnshj_p->sa)));
	return 0;
}

/*
 * check whether this domain is hi-jack
 */
void *dnshj_select ( char *domain) {
	char *fn = "dnshj_select";
	G_List *gl;
	Dnshj *dnshj_tmp;
	int find = 0;
	void * pt = NULL;

	syslog (LOG_DEBUG, "%s: start()", fn);
	


    for (gl = T.dnshj_list->next; gl->list_data; gl = gl->next)
	{
		dnshj_tmp = (Dnshj *)gl->list_data;

		if (strcmp(domain, dnshj_tmp->dns_hostname))
		{
			continue;
		}
		else
		{
			find = 1;
#ifndef HTTPD_IPV6_SUPPORT
	    	syslog (LOG_DEBUG, "find dnshj %s", dnshj_tmp->hostname);
		    pt =  (void *)dnshj_tmp->sa;
#else
		    struct sockaddr_in6 *addr = NULL;
		    			
		    syslog (LOG_DEBUG, "find dnshj %s", dnshj_tmp->hostname);
		    addr = (struct sockaddr_in6 *)dnshj_tmp->sa;
		    pt = (void *)(&addr->sin6_addr);
#endif		    
		    break;
		}
	}

    if (!find) {
	/* we didn't find a valid dnshj domain at all */
    	syslog (LOG_DEBUG, "No dnshj finded!");
		pt =  (void *)NULL;
    }
//	seep("find =%d\n", find);
	syslog (LOG_DEBUG, "%s: end()", fn);
	return (void *) pt;
}

