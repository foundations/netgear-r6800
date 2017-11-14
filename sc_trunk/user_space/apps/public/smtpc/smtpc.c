#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <errno.h>
#include <unistd.h>
#include <time.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <syslog.h>
#include <stdio.h>
#include <netdb.h>
#include <string.h>
#include <ctype.h>
#include <pwd.h>
#include "misc.h"
#ifdef SMTP_SSL
#include "nvram.h"
#endif
#define _SC_DEBUG_ENABLE_
#include "sc_debug.h"

#ifdef SMTP_SSL
#include <openssl/ssl.h>
#include <openssl/err.h>
#endif

#define log my_log  /* log() is built-in function */
typedef struct {
	int	mime_style;
	int	mailport;
	char	*from_addr;
	char	*mailhost;
	char	*recipient;
	char	*subject;
	char	*username;
	char	*password;
}smtp_conf;

extern char *optarg;

static FILE *sfp;
static FILE *rfp;

#ifdef SMTP_SSL
static int do_ssl;
static SSL* ssl = NULL;
static SSL_CTX* ssl_ctx = NULL;
//SSL_METHOD* ssl_method = NULL;
#endif
static int do_starttls=0;
static int clear_syslog=0;  /* Should we clear syslog message after email sent? 1 -- Yes, 0 -- No. */
static int	verbose=0;
static int	need_auth=0;
static smtp_conf conf;

#define dprintf  if (verbose) printf
#define dvprintf if (verbose) vprintf

/* hack for Ultrix */
#ifndef LOG_DAEMON
#define LOG_DAEMON 0
#endif

/* date header */
void smtp_date(char *timestamp,char *time_zone,int daylight)
{
	char *wday[7]={"Sun","Mon","Tue","Wed","Thu","Fri","Sat"};
	char *month[12]={"Jan","Feb","Mar","Apr","May","Jun","Jul","Aug","Sep","Oct","Nov","Dec"};
	time_t now;
	struct tm *st;

	//setenv("TZ",time_zone,1);
	unsetenv("TZ");

	time(&now);
	st=localtime(&now);

	sprintf(timestamp,"%s,%d %s %d %02d:%02d:%02d %c%02d%s %s"
			,wday[st->tm_wday]
			,st->tm_mday
			,month[st->tm_mon]
			,st->tm_year+1900
			,st->tm_hour
			,st->tm_min
			,st->tm_sec
			,strchr(time_zone,'-')?'+':'-'
			,atoi(&time_zone[4])
			,(strchr(time_zone,':')?"30":"00")
			,(daylight)?"(GMT Daylight Time)":"");

}

void SSL_negotiate(int s)
{
	SC_CFPRINTF("SSL_negotiate\n");
	sfp = fopen("/tmp/smtp_s_tmp", "w+");
	if(!sfp){
		exit(1);
	}
	rfp = fopen("/tmp/smtp_r_tmp", "w+");
	if(!rfp){
		fclose(sfp);
		exit(1);
	}
	SSL_library_init();
	//ssl_method = TLSv1_client_method();
	ssl_ctx = SSL_CTX_new(TLSv1_client_method());
	SSL_CTX_set_verify(ssl_ctx, SSL_VERIFY_NONE, 0);
	ssl = SSL_new( ssl_ctx );
	SSL_set_fd( ssl, s );
	SC_CFPRINTF("SSL_connect ....\n");			
	if ( SSL_connect( ssl ) != 1 )
	{
		SSL_shutdown(ssl);
		SSL_free(ssl);
		SSL_CTX_free(ssl_ctx);
		close(s);
		fclose(sfp);
		fclose(rfp);
		exit(1);
	}
}

/*
**  logging support
*/
void log(char *str, ...)
{
    va_list ap;
    char buf[1024];

    va_start(ap, str);
    vsprintf(buf, str, ap);
    dprintf("%s\n",buf);
    syslog(LOG_INFO,buf);
    va_end(ap);
    return;
}
/*
**  examine message from server
*/
#ifdef SMTP_SSL
void chat(char *fmt, ...);
/*
**  examine message from server
*/
void get_response_ssl(void)
{
    char buf[BUFSIZ]="";
    char tmp[BUFSIZ]="";

	if(SSL_read(ssl, tmp, sizeof(tmp))){
		if(fwrite(tmp, sizeof(char), sizeof(tmp), rfp)){
			rewind(rfp);
			while (fgets(buf, sizeof(buf), rfp)) {
				buf[strlen(buf)-2] = 0;
				dprintf("server->%s\n",buf);
				SC_XCFPRINTF("server->%s\n",buf);
				if (!isdigit(buf[0]) || buf[0] > '3') {
					/*Email format "535 #5.7.0 Authentication failed"*/
					char *pt = buf;
					char *pt1 = buf;
					if(isdigit(*pt))
					{
						while((pt=strstr(pt+1," ")) !=NULL)
						{
							pt1 =pt+1;
							if(*pt1!='#'&&*pt1 !=' '&&*pt1!='\0')
								break;
						}
						if(pt ==NULL)
							pt1 = buf;
					}	
					//log("unexpected reply: %s", buf);
					NETGEAR_SYSLOG("17 00[email failed] %s", buf);
					chat("QUIT\r\n");
					SSL_shutdown(ssl);
					SSL_free(ssl);
					SSL_CTX_free(ssl_ctx);
				fclose(sfp);
					fclose(rfp);
					exit(1);
				}
				/* check auth */
				if (!(strncmp(&buf[4],"AUTH", 4)))
					need_auth=1;

				if (buf[3] != '-')
					break;
			}
		}
	ftruncate(fileno(rfp), 0);
	rewind(rfp);
}
    return;
}
#endif

void get_response(void)
{
    char buf[BUFSIZ]="";

    while (fgets(buf, sizeof(buf), rfp)) {
        buf[strlen(buf)-2] = 0;
        dprintf("server->%s\n",buf);
        SC_XCFPRINTF("server->%s\n",buf);
        if (!isdigit(buf[0]) || buf[0] > '3') {
	/*DGND3700v2 TD 131, Email format "535 #5.7.0 Authentication failed", 
 	* Porting from D7000: Nick*/
		char *pt = buf;
		char *pt1 = buf;
			
		if(isdigit(*pt))/*start from next space*/
		{
			while((pt=strstr(pt+1," ")) != NULL)
			{
				pt1 = pt + 1;
				if(*pt1!='#' && *pt1!=' '  && *pt1!='\0')
					break;
			}
			if(pt == NULL)
				pt1 = buf;
			}		
			if(strstr(pt1,"5.7.0 Must issue a STARTTLS command first"))
			{
				do_starttls = 1;
				return;
			}
			if(strstr(pt1,"5.7.1 Authentication required"))
			{
				need_auth = 1;
				return;
		}			
            //log("unexpected reply: %s", buf);
            	NETGEAR_SYSLOG("17 00[email failed] %s", pt1);
	        fprintf(sfp,"QUIT\r\n");
            	exit(1);
        }
	/* check auth */
	if (!(strncmp(&buf[4],"AUTH", 4)))
        	need_auth=1;

        if (buf[3] != '-')
            break;
    }

    return;
}

/* esmtp */

static char table64[]=
  "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
void base64Encode(char *intext, char *output)
{
  unsigned char ibuf[3];
  unsigned char obuf[4];
  int i;
  int inputparts;

  while(*intext) {
    for (i = inputparts = 0; i < 3; i++) {
      if(*intext) {
        inputparts++;
        ibuf[i] = *intext;
        intext++;
      }
      else
        ibuf[i] = 0;
    }

    obuf [0] = (ibuf [0] & 0xFC) >> 2;
    obuf [1] = ((ibuf [0] & 0x03) << 4) | ((ibuf [1] & 0xF0) >> 4);
    obuf [2] = ((ibuf [1] & 0x0F) << 2) | ((ibuf [2] & 0xC0) >> 6);
    obuf [3] = ibuf [2] & 0x3F;

    switch(inputparts) {
      case 1: /* only one byte read */
        sprintf(output, "%c%c==",
            table64[obuf[0]],
            table64[obuf[1]]);
        break;
      case 2: /* two bytes read */
        sprintf(output, "%c%c%c=",
            table64[obuf[0]],
            table64[obuf[1]],
            table64[obuf[2]]);
        break;
      default:
        sprintf(output, "%c%c%c%c",
            table64[obuf[0]],
            table64[obuf[1]],
            table64[obuf[2]],
            table64[obuf[3]] );
        break;
    }
    output += 4;
  }
  *output=0;
}

/*
**  say something to server and check the response
*/

void chat(char *fmt, ...)
{
	va_list ap;
#ifdef SMTP_SSL
	char buf[BUFSIZ]="";
#endif
    va_start(ap, fmt);
    vfprintf(sfp, fmt, ap);
    va_end(ap);

    va_start(ap, fmt);
    dprintf("server <-- ");
    dvprintf(fmt, ap);
    va_end(ap);

#ifdef _SC_DEBUG_ENABLE_
{
    FILE *fp;

    fp = fopen("/dev/console", "a");
    if(fp) {
        va_start(ap, fmt);
        fprintf(fp, "server <-- ");
        vfprintf(fp, fmt, ap);
        va_end(ap);
        fclose(fp);
    }
}
#endif

#ifdef SMTP_SSL
	if(do_ssl) {
		rewind(sfp);
		/* sfp include all email body content, the max size is over  20K
		 * current bug size is 4K ,so we need using while loop.
		 */
		while(fread(buf, sizeof(char), sizeof(buf), sfp))
			SSL_write(ssl, buf, strlen(buf));
		ftruncate(fileno(sfp), 0);
		rewind(sfp);

		get_response_ssl();
	}
	else {
#endif
		fflush(sfp);
		get_response();
#ifdef SMTP_SSL
	}
#endif
}

/*
**  transform to MIME-style quoted printable
**
**  Extracted from the METAMAIL version 2.7 source code (codes.c)
**  and modified to emit \r\n at line boundaries.
*/

static char basis_hex[] = "0123456789ABCDEF";

void toqp(FILE *infile, FILE *outfile)
{
    int c;
    int ct = 0;
    int prevc = 255;

    while ((c = getc(infile)) != EOF) {
        if (   (c < 32 && (c != '\n' && c != '\t'))
            || (c == '=')
            || (c >= 127)
            || (ct == 0 && c == '.')               ) {
        putc('=', outfile);
        putc(basis_hex[c >> 4], outfile);
        putc(basis_hex[c & 0xF], outfile);
        ct += 3;
        prevc = 'A'; /* close enough */
    }
    else if (c == '\n') {
        if (prevc == ' ' || prevc == '\t') {
	    putc('=', outfile);  /* soft & hard lines */
	    putc(c, outfile);
        }
        putc(c, outfile);
        ct = 0;
        prevc = c;
    }
    else {
        if (c == 'F' && prevc == '\n') {
        /*
         * HORRIBLE but clever hack suggested by MTR for
         * sendmail-avoidance
         */
        c = getc(infile);
        if (c == 'r') {
            c = getc(infile);
            if (c == 'o') {
            c = getc(infile);
            if (c == 'm') {
                c = getc(infile);
                if (c == ' ') {
                /* This is the case we are looking for */
                fputs("=46rom", outfile);
                ct += 6;
                } else {
                fputs("From", outfile);
                ct += 4;
                }
            } else {
                fputs("Fro", outfile);
                ct += 3;
            }
            }
            else {
            fputs("Fr", outfile);
            ct += 2;
            }
        }
        else {
            putc('F', outfile);
            ++ct;
        }
        ungetc(c, infile);
        prevc = 'x'; /* close enough -- printable */
        }
        else {
        putc(c, outfile);
        ++ct;
        prevc = c;
        }
    }
    if (ct > 72) {
        putc('=', outfile);
        putc('\r', outfile);
        putc('\n', outfile);
        ct = 0;
        prevc = '\n';
    }
    }
    if (ct) {
    putc('=', outfile);
    putc('\r', outfile);
    putc('\n', outfile);
    }
    return;
}

void alarm_handler(int sig) {
    SC_CFPRINTF("in %s()\n", __FUNCTION__);
    NETGEAR_SYSLOG("17 00[email failed] Cannot send mail to %s", conf.recipient);
#ifdef SMTP_SSL
	if(do_ssl){
		SSL_shutdown(ssl);
		SSL_free(ssl);
		SSL_CTX_free(ssl_ctx);
		fclose(sfp);
		fclose(rfp);
	}
#endif
    exit(1);
}

void usage(void)
{
	puts("==========================================================");
	puts("Usage: ./smtpc [m:s:f:r:h:p:U:P:cv] < files		");
	puts("	-m mime type						");
	puts("	-s subject						");
	puts("	-f from addr (if NULL use recipient)			");
	puts("	-r recipient						");
	puts("	-h mail server							");
	puts("	-p mail port (default=25)				");
	puts("	-U user name (ESMTP)					");
	puts("	-P password  (ESMTP)					");
	puts("	-c Clear syslog					");
	puts("	-v verbose   (DEBUG)					");
	puts("========================================================\n");
}

   int main(int argc, char **argv)
{
	char buf[BUFSIZ];
	struct sockaddr_in sin;
	struct hostent *hp;
	int s;
	int r;
	char *cp;
	char enc_user[128]="";
	char enc_pw[128]="";
	char c;
	char *time_zone=NULL;
	int  daylight=0;
	char timestamp[128]="";

#ifdef SMTP_SSL
	do_ssl =  atoi(nvram_get("mail_ssl"));
	SC_CFPRINTF("do ssl <%d>\n",do_ssl);
#endif
    SC_CFPRINTF("Start to do smtpc...\n");
#ifdef _SC_DEBUG_ENABLE_
{
    int i;

    for(i=0; i<argc; i++) {
        SC_XCFPRINTF("%s ", argv[i]);
    }
    SC_XCFPRINTF("\n");
}
#endif

	/*  Go away when something gets stuck.   */
    signal(SIGALRM, alarm_handler);
	alarm(60);

	memset((char *)&conf, 0, sizeof(smtp_conf));

	/* init */
	if(conf.mailport==0){
		conf.mailport=atoi(nvram_get("mail_ssl_port"));
	}
	SC_CFPRINTF("mail port = <%d>\n",conf.mailport);
    	/*  Parse options */
	for (;;) {
#ifdef NTGR_CLOUD
		c = getopt( argc, argv, "mns:f:r:h:p:U:P:t:d:cv");
#else
		c = getopt( argc, argv, "ms:f:r:h:p:U:P:t:d:cv");
#endif
		if (c == EOF) break;
		switch (c) {
			case 'm':
				conf.mime_style=1;
				break;
#ifdef NTGR_CLOUD
			case 'n':
				conf.mime_style=2;
				break;
#endif
			case 'h':
				conf.mailhost=optarg;
				break;
			case 's':
				conf.subject=optarg;
				break;
			case 'f':
				conf.from_addr=optarg;
				break;
			case 'r':
				conf.recipient=optarg;
				break;
			case 'p':
				conf.mailport=atoi(optarg);
				break;
			case 'U':
				conf.username=optarg;
				base64Encode(conf.username, enc_user);
				break;
			case 'P':
				conf.password=optarg;
				base64Encode(conf.password, enc_pw);
				break;
			case 'v':
				verbose=1;
				break;
			case 'c':
				clear_syslog=1;
				break;
			case 't':
				time_zone=optarg;
				break;
			case 'd':
				daylight=atoi(optarg);
				break;
			default:
				usage();
				exit(1);
				break;
		}
	}

	/* check recipient and mailhost */
	if(conf.recipient==NULL || conf.mailhost==NULL){
		usage();
		exit(1);
	}

	openlog("smtp",0,LOG_SYSLOG);

	/*  Determine SMTP server  */
	if (conf.mailhost == NULL) {
		if ((cp = getenv("SMTPSERVER")) != NULL)
		        conf.mailhost = cp;
        	else
        		conf.mailhost = strdup("localhost");
	}

	/*  Connect to smtp daemon on mailhost.  */
	if ((hp = gethostbyname(conf.mailhost)) == NULL) {
        //log("%s: unknown host\n", conf.mailhost);
 	NETGEAR_SYSLOG("17 00[email failed] %s: unknown host", conf.mailhost);
		exit(1);
	}
	if (hp->h_addrtype != AF_INET) {
        //log("unknown address family: %d", hp->h_addrtype);
        NETGEAR_SYSLOG("17 00[email failed] unknown address family: %d", hp->h_addrtype);
		exit(1);
	}
	memset((char *)&sin, 0, sizeof(sin));
	memcpy((char *)&sin.sin_addr, hp->h_addr, hp->h_length);
	sin.sin_family = hp->h_addrtype;
	sin.sin_port = htons(conf.mailport);
	if ((s = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
		exit(1);
	}
    SC_CFPRINTF("Start to connect...\n");
	if (connect(s, (struct sockaddr *)&sin, sizeof(sin)) < 0) {
	    NETGEAR_SYSLOG("17 00[email failed] Cannot connect to %s", conf.mailhost);
        exit(1);
	}
	SC_CFPRINTF("Start to connect done...!!!!!!!!!!!!!\n");
#ifdef SMTP_SSL
	if(do_ssl) {
		SSL_negotiate(s);
    }
	else {
#endif
		if ((r = dup(s)) < 0) {
        	exit(1);
		}
		if ((sfp = fdopen(s, "w")) == 0) {
        	exit(1);
		}

		if ((rfp = fdopen(r, "r")) == 0) {
        	exit(1);
		}
#ifdef SMTP_SSL
	}
#endif
    SC_CFPRINTF("Talking to server...\n");

    /*  Give out SMTP headers. */
#ifdef SMTP_SSL
    if(do_ssl)
		get_response_ssl(); /* banner */
	else
#endif
		get_response();

	if(conf.username)
		chat("EHLO %s\r\n", "unknown");
	else
		chat("HELO %s\r\n", "unknown");

	dprintf("need_auth==%d\n",need_auth);

	if(need_auth){ /* authtication */
		SC_CFPRINTF("enc_user <%s>, enc_pw <%s>\n",enc_user, enc_pw);
		chat ("AUTH LOGIN\r\n");
		chat ("%s\r\n", enc_user);
		chat ("%s\r\n", enc_pw);
		need_auth = 0;
	}
	SC_CFPRINTF("need_auth<%d>\n",need_auth);
	if(conf.from_addr)
		chat("MAIL FROM: <%s>\r\n", conf.from_addr);
	else
		chat("MAIL FROM: <%s>\r\n", conf.recipient);

	if(need_auth){ /* authtication *///for yahoo mail port 587 starttls
		SC_CFPRINTF("enc_user <%s>, enc_pw <%s>\n",enc_user, enc_pw);
		chat ("AUTH LOGIN\r\n");
		if(!do_starttls){
			chat ("%s\r\n", enc_user);
			chat ("%s\r\n", enc_pw);
		}
	}
	if(do_starttls)
	{
		chat("STARTTLS\r\n");
		SSL_negotiate(s);

		do_ssl=1;

		if(conf.username)
			chat("EHLO %s\r\n", "unknown");
		else
			chat("HELO %s\r\n", "unknown");

		dprintf("need_auth==%d\n",need_auth);
		need_auth = 1;
		if(need_auth){ /* authtication */
			SC_CFPRINTF("enc_user <%s>, enc_pw <%s>\n",enc_user, enc_pw);
			chat ("AUTH LOGIN\r\n");
			chat ("%s\r\n", enc_user);
			chat ("%s\r\n", enc_pw);
		}
	
		if(conf.from_addr)
			chat("MAIL FROM: <%s>\r\n", conf.from_addr);
		else
			chat("MAIL FROM: <%s>\r\n", conf.recipient);
	}
	chat("RCPT TO: <%s>\r\n",conf.recipient); //Ron
	SC_CFPRINTF("RCPT TO: <%s>\r\n",conf.recipient); //Ron

	chat("DATA\r\n");

	if(time_zone){
		smtp_date(timestamp,time_zone,daylight);

       		fprintf(sfp , "Date: %s\r\n",timestamp);
	}

	/* Give out Message header. */
	if(conf.from_addr)
		fprintf(sfp, "From: %s\r\n", conf.from_addr);
	else
		fprintf(sfp, "From: %s\r\n", conf.recipient);
	
	/*Follow Netgear Spec2.0:
	 * The e-mail in the "Send To This E-mail Addres"s field
         * is receiver's e-mail address, and this is used for "To"
         * and "From" of the e-mail. The "Sender" is used as
         * "root@model name", where the model name denotes to the
         * model name of the product you are developing and please
         * use upper case for the model name.
         */
	fprintf(sfp, "Sender: root@%s\r\n", BOARD_ID); 

	if (conf.subject)
        	fprintf(sfp, "Subject: %s\r\n", conf.subject);

	fprintf(sfp, "To: %s\r\n",conf.recipient); //Ron

	if (conf.mime_style ==1 ) {
		fprintf(sfp, "MIME-Version: 1.0\r\n");
		fprintf(sfp, "Content-Type: text/plain; charset=ISO-8859-1\r\n");
	        fprintf(sfp, "Content-Transfer-Encoding: quoted-printable\r\n");
	}
#ifdef NTGR_CLOUD
	else if(conf.mime_style == 2) {
		fprintf(sfp, "MIME-Version: 1.0\r\n");
		fprintf(sfp, "Content-Type: text/html; charset=ISO-8859-1\r\n");
		fprintf(sfp, "Content-Transfer-Encoding: quoted-printable\r\n");
	}
#endif

	fprintf(sfp, "\r\n");

	/* Give out Message body. */
	while (fgets(buf, sizeof(buf), stdin)) {
		buf[strlen(buf)-1] = 0;
		if (strcmp(buf, ".") == 0)
			fprintf(sfp, "..\r\n");
		else
			fprintf(sfp, "%s\r\n", buf);
	}

	if (conf.mime_style) {
        	toqp(stdin, sfp);
	} else {
        	while (fgets(buf, sizeof(buf), stdin)) {
		        buf[strlen(buf)-1] = 0;
        		if (strcmp(buf, ".") == 0) { /* quote alone dots */
                		fprintf(sfp, "..\r\n");
		        } else { /* pass thru mode */
        		        fprintf(sfp, "%s\r\n", buf);
	        	}
        	}
	}

	/* Give out SMTP end. */
	if(do_starttls)
	{
		chat("\r\n.\r\n");
	}
	else
	{
	chat(".\r\n");
	}
	chat("QUIT\r\n");
	//log("Send E-mail Success!");
	clear_syslog = 1; //Send mail success, clear the log.
    if(clear_syslog) {
        /* Clear syslog */
        SC_CFPRINTF("==================clearlog=============\n");
        system("killall -SIGUSR1 syslogd");
    }

    SC_CFPRINTF("[email sent to: %s]\n", conf.recipient);
	NETGEAR_SYSLOG("16 00[email sent to: %s]", conf.recipient);
	closelog();
#ifdef SMTP_SSL
	if(do_ssl){
		SSL_shutdown(ssl);
		SSL_free(ssl);
		SSL_CTX_free(ssl_ctx);
		close(s);
		fclose(sfp);
		fclose(rfp);
	}
#endif
	/*  Die gracefully ...  */
	exit(0);
}

/*EOF*/
