#include <stdio.h>
#include <netdb.h>
#include <string.h>
#include <stdlib.h>
#include <getopt.h>
#include <ctype.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <xtables.h>

#include "ipt_http_string.h"

#define HTTP_STRING_DEBUG

/* Function which prints out usage message. */
static void
help(void)
{
        printf(
        "HTTP STRING match v%s options:\n"
        "--string [!] string             Match a string in a packet\n",
        XTABLES_VERSION);

        fputc('\n', stdout);
}

static struct option opts[] = {
        { "file", 1, 0, 'f' },
		{ "trust", 1, 0, 't' },
        {0}
};

#define HTTP_STRING_FILE 0x01
#define HTTP_STRING_TRUST 0x02

/* Initialize the match. */
static void
init(struct xt_entry_match *m/*, unsigned int *nfcache*/)
{
//      *nfcache |= NFC_UNKNOWN;
}

/* Final check; must have specified --string. */
static void
final_check(unsigned int flags)
{
        if (!flags)
                xtables_error(PARAMETER_PROBLEM,
                   "STRING match: You must specify `--string'");
}

/** 
 * The qsort compare funcion used to sort the pattern type by DECREASE
 * order
 * 
 * @param iptpat1 
 * @param iptpat2 
 * 
 * @return 
 */
static int
qsort_cmppat(const void *pat1, const void *pat2)
{
    const struct ipt_http_pattern *iptpat1 = pat1;
    const struct ipt_http_pattern *iptpat2 = pat2;

    if(iptpat1->type == iptpat2->type)
        return 0;

    return (iptpat1->type < iptpat2->type)?1:-1;
}

static int get_http_match_condition(char *file,
                             struct ipt_http_string_info *http_match_info)
{
        FILE *fp;
    int rc;
    
    memset(http_match_info, 0, sizeof(struct ipt_http_string_info));
    
        if((fp = fopen(file, "r")) < 0)
        {
                printf("open %s fail\n", file);
                return -1;
        }
    
        if((rc = fread(http_match_info, 1, sizeof(struct ipt_http_string_info),
                   fp)) != sizeof(struct ipt_http_string_info))
        {
               printf("read %s fail, rc = %d, %d, %d\n",
               file, rc, sizeof(struct ipt_http_string_info), sizeof(struct ipt_http_pattern));
        fclose(fp);
                return -1;
        }
    
        fclose(fp);

    /* Reorder the patterns in iptinfo to speed the later search */
    qsort(http_match_info->iptpat,
          IPT_PATTERN_NUMBER,
          sizeof(struct ipt_http_pattern),
          qsort_cmppat);

        return 1;
}

#ifdef HTTP_STRING_DEBUG
static void dump_http_string(struct ipt_http_string_info *http_match_info)
{
        int i;

        printf("http string list:\n");

    for(i = 0; i < IPT_PATTERN_NUMBER; i++)
    {
        if(http_match_info->iptpat[i].patlen != 0)
        {
            printf("[%d] %d, %d, %s\n",
                   i,
                   http_match_info->iptpat[i].type,
                   http_match_info->iptpat[i].patlen,
                   http_match_info->iptpat[i].pattern);
        }
    }    
}
#endif

/* Function which parses command options; returns true if it
   ate an option */
static int
parse(int c, char **argv, int invert, unsigned int *flags,
      const void *entry,
      struct xt_entry_match **match)
{
        struct ipt_http_string_info *http_match_info =
        (struct ipt_http_string_info *)(*match)->data;
    
        switch (c) {
        case 'f':
            xtables_check_inverse(optarg, &invert, &optind, 0,argv);            
            if(get_http_match_condition(argv[optind-1], http_match_info) < 0)
                return -1;           
            if (invert)
                http_match_info->invert = 1;
			*flags |= HTTP_STRING_FILE;
            break;
		/*Add this to indicate trust IP*/
        case 't':
            xtables_check_inverse(optarg, &invert, &optind, 0,argv);
            *flags |= HTTP_STRING_TRUST;
            if(*(argv[optind-1]) == '1')/*trust IP*/
            {
                http_match_info->flags |= IPT_HTTP_FLAG_TRUST;
            }    
            break;
        default:
            return 0;
        }
    
#ifdef HTTP_STRING_DEBUG
        dump_http_string(http_match_info);
#endif
        return 1;
}


/* Prints out the matchinfo. */
static void
print(const void *ip,
      const struct xt_entry_match *match,
      int numeric)
{
        printf("HTTP STRING match ");
}

/* Saves the union ipt_matchinfo in parsable form to stdout. */
static void
save(const void *ip, const struct xt_entry_match *match)
{
        printf("Can not Support save ");
}

static
struct xtables_match http_string = {
    .name          = "http_string",
    .version       = XTABLES_VERSION,
    .family        = NFPROTO_IPV4,
    .size          = XT_ALIGN(sizeof(struct ipt_http_string_info)),
    .userspacesize = XT_ALIGN(sizeof(struct ipt_http_string_info)),
    .help          = &help,
    .init          = &init,
    .parse         = &parse,
    .final_check   = &final_check,
    .print         = &print,
    .save          = &save,
    .extra_opts    = opts
};

void _init(void)
{
        xtables_register_match(&http_string);
}
