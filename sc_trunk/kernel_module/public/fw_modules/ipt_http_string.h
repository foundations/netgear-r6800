#ifndef _IPT_HTTP_STRING_H_
#define _IPT_HTTP_STRING_H_

#define MAX_PATTERN_LENGTH         60

/*
 * .CLASS .JAR
 */ 
#define IPT_HTTP_FLAG_JAVA         (1<<0)
/*
 * .OCX .CAB
 */ 
#define IPT_HTTP_FLAG_ACTIVEX      (1<<1)
#define IPT_HTTP_FLAG_COOKIE       (1<<2)
#define IPT_HTTP_FLAG_PROXY        (1<<3)
#define IPT_HTTP_FLAG_TRUST		   (1<<7) /*is from trust ip*/

#define IPT_PATTERN_URI_NUMBER     255
#define IPT_PATTERN_HOST_NUMBER    0
#define IPT_PATTERN_CONTENT_NUMBER 0

#define IPT_PATTERN_NUMBER (IPT_PATTERN_URI_NUMBER +    \
                            IPT_PATTERN_HOST_NUMBER +   \
                            IPT_PATTERN_CONTENT_NUMBER  \
                            )

enum {
    IPT_PATTERN_TYPE_UNKNOWN = 0,
    IPT_PATTERN_TYPE_CONTENT,
    IPT_PATTERN_TYPE_HOST,
    IPT_PATTERN_TYPE_URI
};

struct ipt_http_pattern {
    char type;
    char patlen;
    char pattern[MAX_PATTERN_LENGTH+1];
};

struct ipt_http_string_info {
    int flags;
    unsigned short invert;
    struct ipt_http_pattern iptpat[IPT_PATTERN_NUMBER];
};

#endif /* _IPT_HTTP_STRING_H_ */
