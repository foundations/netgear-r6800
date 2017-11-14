
/* A Bison parser, made by GNU Bison 2.4.1.  */

/* Skeleton implementation for Bison's Yacc-like parsers in C
   
      Copyright (C) 1984, 1989, 1990, 2000, 2001, 2002, 2003, 2004, 2005, 2006
   Free Software Foundation, Inc.
   
   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.
   
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.
   
   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.
   
   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

/* C LALR(1) parser skeleton written by Richard Stallman, by
   simplifying the original so-called "semantic" parser.  */

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Identify Bison output.  */
#define YYBISON 1

/* Bison version.  */
#define YYBISON_VERSION "2.4.1"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1

/* Using locations.  */
#define YYLSP_NEEDED 0



/* Copy the first part of user declarations.  */

/* Line 189 of yacc.c  */
#line 2 "l2tp_config_parse.y"

#include <sys/types.h>
#include <sys/param.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include <netdb.h>
#include <ctype.h>
#include <errno.h>
#include <stdlib.h>
#include <syslog.h>

#include "l2tp_config_types.h"

#include "l2tp_rpc.h"

static struct l2tp_api_system_msg_data system_config;
static struct l2tp_api_peer_profile_msg_data peer_profile;
static struct l2tp_api_tunnel_profile_msg_data tunnel_profile;
static struct l2tp_api_session_profile_msg_data session_profile;
static struct l2tp_api_ppp_profile_msg_data ppp_profile;
static struct l2tp_api_tunnel_msg_data tunnel;
static struct l2tp_api_session_msg_data session;

extern void l2tp_log(int level, char *fmt, ...);
extern void yyfatal(const char *s);



/* Line 189 of yacc.c  */
#line 107 "l2tp_config_parse.c"

/* Enabling traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif

/* Enabling verbose error messages.  */
#ifdef YYERROR_VERBOSE
# undef YYERROR_VERBOSE
# define YYERROR_VERBOSE 1
#else
# define YYERROR_VERBOSE 0
#endif

/* Enabling the token table.  */
#ifndef YYTOKEN_TABLE
# define YYTOKEN_TABLE 0
#endif


/* Tokens.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
   /* Put the tokens into the symbol table, so that GDB and other debuggers
      know about them.  */
   enum yytokentype {
     SYSTEM = 258,
     TUNNEL = 259,
     SESSION = 260,
     PROFILE = 261,
     PEER = 262,
     PPP = 263,
     CREATE = 264,
     MODIFY = 265,
     DIGITAL_ANALOG = 266,
     TUNNEL_AUTH_MODE = 267,
     PEER_IPADDR = 268,
     LAC_LNS = 269,
     TUNNEL_ID = 270,
     TUNNEL_NAME = 271,
     SESSION_ID = 272,
     SESSION_NAME = 273,
     UDP_PORT = 274,
     TRACE_FLAGS = 275,
     MAX_TUNNELS = 276,
     MAX_SESSIONS = 277,
     DRAIN_TUNNELS = 278,
     TUNNEL_ESTABLISH_TIMEOUT = 279,
     SESSION_ESTABLISH_TIMEOUT = 280,
     TUNNEL_PERSIST_PEND_TIMEOUT = 281,
     SESSION_PERSIST_PEND_TIMEOUT = 282,
     DENY_LOCAL_TUNNEL_CREATES = 283,
     DENY_REMOTE_TUNNEL_CREATES = 284,
     HIDE_AVPS = 285,
     OUR_ADDR = 286,
     PEER_ADDR = 287,
     OUR_UDP_PORT = 288,
     PEER_UDP_PORT = 289,
     CONFIG_ID = 290,
     AUTH_MODE = 291,
     FRAMING_CAP = 292,
     BEARER_CAP = 293,
     USE_TIEBREAKER = 294,
     ALLOW_PPP_PROXY = 295,
     USE_PPP_PROXY = 296,
     USE_UDP_CHECKSUMS = 297,
     HELLO_TIMEOUT = 298,
     MAX_RETRIES = 299,
     RX_WINDOW_SIZE = 300,
     TX_WINDOW_SIZE = 301,
     RETRY_TIMEOUT = 302,
     IDLE_TIMEOUT = 303,
     DO_PMTU_DISCOVERY = 304,
     PERSIST = 305,
     MTU = 306,
     SECRET = 307,
     HOST_NAME = 308,
     TUNNEL_PROFILE_NAME = 309,
     PEER_PROFILE_NAME = 310,
     SESSION_PROFILE_NAME = 311,
     PPP_PROFILE_NAME = 312,
     INTERFACE_NAME = 313,
     SESSION_TYPE = 314,
     USER_NAME = 315,
     USER_PASSWORD = 316,
     PRIV_GROUP_ID = 317,
     PROFILE_NAME = 318,
     SEQUENCING_REQUIRED = 319,
     USE_SEQUENCE_NUMBERS = 320,
     NO_PPP = 321,
     REORDER_TIMEOUT = 322,
     FRAMING_TYPE = 323,
     BEARER_TYPE = 324,
     MINIMUM_BPS = 325,
     MAXIMUM_BPS = 326,
     CONNECT_SPEED = 327,
     PEER_PORT = 328,
     WE_CAN_BE_LAC = 329,
     WE_CAN_BE_LNS = 330,
     DEFAULT_TUNNEL_PROFILE_NAME = 331,
     DEFAULT_SESSION_PROFILE_NAME = 332,
     DEFAULT_PPP_PROFILE_NAME = 333,
     NETMASK = 334,
     ASYNCMAP = 335,
     MRU = 336,
     AUTH_FLAGS = 337,
     SYNC_MODE = 338,
     CHAP_INTERVAL = 339,
     CHAP_MAX_CHALLENGE = 340,
     CHAP_RESTART = 341,
     PAP_MAX_AUTH_REQUESTS = 342,
     PAP_RESTART_INTERVAL = 343,
     PAP_TIMEOUT = 344,
     IPCP_MAX_CONFIG_REQUESTS = 345,
     IPCP_MAX_CONFIG_NAKS = 346,
     IPCP_MAX_TERMINATE_REQUESTS = 347,
     IPCP_RETRANSMIT_INTERVAL = 348,
     LCP_ECHO_FAILURE_COUNT = 349,
     LCP_ECHO_INTERVAL = 350,
     LCP_MAX_CONFIG_REQUESTS = 351,
     LCP_MAX_CONFIG_NAKS = 352,
     LCP_MAX_TERMINATE_REQUESTS = 353,
     LCP_RETRANSMIT_INTERVAL = 354,
     MAX_CONNECT_TIME = 355,
     MAX_FAILURE_COUNT = 356,
     LOCAL_IPADDR = 357,
     REMOTE_IPADDR = 358,
     DNS_IPADDR_PRI = 359,
     DNS_IPADDR_SEC = 360,
     WINS_IPADDR_PRI = 361,
     WINS_IPADDR_SEC = 362,
     IP_POOL_NAME = 363,
     USE_RADIUS = 364,
     RADIUS_HINT = 365,
     USE_AS_DEFAULT_ROUTE = 366,
     MULTILINK = 367,
     PROXY_ARP = 368,
     EOT = 369,
     SLASH = 370,
     BLCL = 371,
     ELCL = 372,
     EQUALS = 373,
     DECSTRING = 374,
     STRING = 375,
     HEXSTRING = 376,
     QUOTEDSTRING = 377,
     BOOL = 378,
     IPADDRESS = 379,
     INITIAL_RCVD_LCP_CONFREQ = 380,
     CALLING_NUMBER = 381,
     PROXY_AUTH_NAME = 382,
     SUB_ADDRESS = 383,
     PROXY_AUTH_TYPE = 384,
     PROXY_AUTH_RESPONSE = 385,
     LAST_RCVD_LCP_CONFREQ = 386,
     CALLED_NUMBER = 387,
     PROXY_AUTH_CHALLENGE = 388,
     LAST_SENT_LCP_CONFREQ = 389,
     AUTH_PAP = 390,
     AUTH_CHAP = 391,
     AUTH_MSCHAPV1 = 392,
     AUTH_MSCHAPV2 = 393,
     AUTH_EAP = 394,
     AUTH_NOAUTH = 395,
     AUTH_PEER = 396,
     SRC_IPADDR = 397,
     DEST_IPADDR = 398,
     LOCAL_NAME = 399,
     REMOTE_NAME = 400
   };
#endif



#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef union YYSTYPE
{

/* Line 214 of yacc.c  */
#line 35 "l2tp_config_parse.y"

	int num;
	unsigned long ulnum;
	l2tp_byte_vector_t val;



/* Line 214 of yacc.c  */
#line 296 "l2tp_config_parse.c"
} YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
#endif


/* Copy the second part of user declarations.  */


/* Line 264 of yacc.c  */
#line 308 "l2tp_config_parse.c"

#ifdef short
# undef short
#endif

#ifdef YYTYPE_UINT8
typedef YYTYPE_UINT8 yytype_uint8;
#else
typedef unsigned char yytype_uint8;
#endif

#ifdef YYTYPE_INT8
typedef YYTYPE_INT8 yytype_int8;
#elif (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
typedef signed char yytype_int8;
#else
typedef short int yytype_int8;
#endif

#ifdef YYTYPE_UINT16
typedef YYTYPE_UINT16 yytype_uint16;
#else
typedef unsigned short int yytype_uint16;
#endif

#ifdef YYTYPE_INT16
typedef YYTYPE_INT16 yytype_int16;
#else
typedef short int yytype_int16;
#endif

#ifndef YYSIZE_T
# ifdef __SIZE_TYPE__
#  define YYSIZE_T __SIZE_TYPE__
# elif defined size_t
#  define YYSIZE_T size_t
# elif ! defined YYSIZE_T && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# else
#  define YYSIZE_T unsigned int
# endif
#endif

#define YYSIZE_MAXIMUM ((YYSIZE_T) -1)

#ifndef YY_
# if YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> /* INFRINGES ON USER NAME SPACE */
#   define YY_(msgid) dgettext ("bison-runtime", msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(msgid) msgid
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YYUSE(e) ((void) (e))
#else
# define YYUSE(e) /* empty */
#endif

/* Identity function, used to suppress warnings about constant conditions.  */
#ifndef lint
# define YYID(n) (n)
#else
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static int
YYID (int yyi)
#else
static int
YYID (yyi)
    int yyi;
#endif
{
  return yyi;
}
#endif

#if ! defined yyoverflow || YYERROR_VERBOSE

/* The parser invokes alloca or malloc; define the necessary symbols.  */

# ifdef YYSTACK_USE_ALLOCA
#  if YYSTACK_USE_ALLOCA
#   ifdef __GNUC__
#    define YYSTACK_ALLOC __builtin_alloca
#   elif defined __BUILTIN_VA_ARG_INCR
#    include <alloca.h> /* INFRINGES ON USER NAME SPACE */
#   elif defined _AIX
#    define YYSTACK_ALLOC __alloca
#   elif defined _MSC_VER
#    include <malloc.h> /* INFRINGES ON USER NAME SPACE */
#    define alloca _alloca
#   else
#    define YYSTACK_ALLOC alloca
#    if ! defined _ALLOCA_H && ! defined _STDLIB_H && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
#     include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#     ifndef _STDLIB_H
#      define _STDLIB_H 1
#     endif
#    endif
#   endif
#  endif
# endif

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's `empty if-body' warning.  */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (YYID (0))
#  ifndef YYSTACK_ALLOC_MAXIMUM
    /* The OS might guarantee only one guard page at the bottom of the stack,
       and a page size can be as small as 4096 bytes.  So we cannot safely
       invoke alloca (N) if N exceeds 4096.  Use a slightly smaller number
       to allow for a few compiler-allocated temporary stack slots.  */
#   define YYSTACK_ALLOC_MAXIMUM 4032 /* reasonable circa 2006 */
#  endif
# else
#  define YYSTACK_ALLOC YYMALLOC
#  define YYSTACK_FREE YYFREE
#  ifndef YYSTACK_ALLOC_MAXIMUM
#   define YYSTACK_ALLOC_MAXIMUM YYSIZE_MAXIMUM
#  endif
#  if (defined __cplusplus && ! defined _STDLIB_H \
       && ! ((defined YYMALLOC || defined malloc) \
	     && (defined YYFREE || defined free)))
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   ifndef _STDLIB_H
#    define _STDLIB_H 1
#   endif
#  endif
#  ifndef YYMALLOC
#   define YYMALLOC malloc
#   if ! defined malloc && ! defined _STDLIB_H && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
void *malloc (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE free
#   if ! defined free && ! defined _STDLIB_H && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
void free (void *); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
# endif
#endif /* ! defined yyoverflow || YYERROR_VERBOSE */


#if (! defined yyoverflow \
     && (! defined __cplusplus \
	 || (defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  yytype_int16 yyss_alloc;
  YYSTYPE yyvs_alloc;
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (sizeof (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (sizeof (yytype_int16) + sizeof (YYSTYPE)) \
      + YYSTACK_GAP_MAXIMUM)

/* Copy COUNT objects from FROM to TO.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(To, From, Count) \
      __builtin_memcpy (To, From, (Count) * sizeof (*(From)))
#  else
#   define YYCOPY(To, From, Count)		\
      do					\
	{					\
	  YYSIZE_T yyi;				\
	  for (yyi = 0; yyi < (Count); yyi++)	\
	    (To)[yyi] = (From)[yyi];		\
	}					\
      while (YYID (0))
#  endif
# endif

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack_alloc, Stack)				\
    do									\
      {									\
	YYSIZE_T yynewbytes;						\
	YYCOPY (&yyptr->Stack_alloc, Stack, yysize);			\
	Stack = &yyptr->Stack_alloc;					\
	yynewbytes = yystacksize * sizeof (*Stack) + YYSTACK_GAP_MAXIMUM; \
	yyptr += yynewbytes / sizeof (*yyptr);				\
      }									\
    while (YYID (0))

#endif

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  3
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   759

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  146
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  36
/* YYNRULES -- Number of rules.  */
#define YYNRULES  257
/* YYNRULES -- Number of states.  */
#define YYNSTATES  626

/* YYTRANSLATE(YYLEX) -- Bison symbol number corresponding to YYLEX.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   400

#define YYTRANSLATE(YYX)						\
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[YYLEX] -- Bison symbol number corresponding to YYLEX.  */
static const yytype_uint8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     1,     2,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      35,    36,    37,    38,    39,    40,    41,    42,    43,    44,
      45,    46,    47,    48,    49,    50,    51,    52,    53,    54,
      55,    56,    57,    58,    59,    60,    61,    62,    63,    64,
      65,    66,    67,    68,    69,    70,    71,    72,    73,    74,
      75,    76,    77,    78,    79,    80,    81,    82,    83,    84,
      85,    86,    87,    88,    89,    90,    91,    92,    93,    94,
      95,    96,    97,    98,    99,   100,   101,   102,   103,   104,
     105,   106,   107,   108,   109,   110,   111,   112,   113,   114,
     115,   116,   117,   118,   119,   120,   121,   122,   123,   124,
     125,   126,   127,   128,   129,   130,   131,   132,   133,   134,
     135,   136,   137,   138,   139,   140,   141,   142,   143,   144,
     145
};

#if YYDEBUG
/* YYPRHS[YYN] -- Index of the first RHS symbol of rule number YYN in
   YYRHS.  */
static const yytype_uint16 yyprhs[] =
{
       0,     0,     3,     4,     6,     9,    11,    13,    15,    17,
      19,    21,    23,    28,    29,    32,    36,    40,    44,    48,
      52,    56,    60,    64,    68,    72,    74,    76,    82,    88,
      89,    92,    96,   100,   104,   108,   112,   116,   120,   124,
     128,   132,   136,   140,   142,   144,   150,   156,   157,   160,
     164,   168,   172,   176,   180,   184,   188,   192,   196,   200,
     204,   208,   212,   216,   220,   224,   228,   232,   236,   240,
     244,   248,   252,   256,   260,   264,   268,   272,   276,   280,
     284,   288,   292,   294,   296,   302,   308,   309,   312,   316,
     320,   324,   328,   332,   336,   340,   344,   348,   352,   356,
     360,   364,   368,   372,   376,   380,   384,   386,   388,   394,
     400,   401,   404,   408,   412,   416,   420,   424,   428,   432,
     436,   440,   444,   448,   452,   456,   460,   464,   468,   472,
     476,   480,   484,   488,   492,   496,   500,   504,   508,   512,
     516,   520,   524,   528,   532,   536,   540,   544,   548,   552,
     556,   560,   564,   568,   572,   576,   580,   584,   588,   592,
     596,   600,   604,   608,   610,   612,   617,   622,   623,   626,
     630,   634,   638,   642,   646,   650,   654,   658,   662,   666,
     670,   674,   678,   682,   686,   690,   694,   698,   702,   706,
     710,   714,   718,   722,   726,   730,   734,   738,   742,   746,
     750,   754,   758,   762,   766,   770,   774,   778,   782,   786,
     788,   790,   795,   800,   801,   804,   808,   812,   816,   820,
     824,   828,   832,   836,   840,   844,   848,   852,   856,   860,
     864,   868,   872,   876,   880,   884,   888,   892,   896,   900,
     904,   908,   912,   916,   920,   924,   928,   932,   936,   940,
     944,   948,   952,   956,   960,   964,   968,   972
};

/* YYRHS -- A `-1'-separated list of the rules' RHS.  */
static const yytype_int16 yyrhs[] =
{
     147,     0,    -1,    -1,   114,    -1,   147,   148,    -1,   149,
      -1,   152,    -1,   157,    -1,   162,    -1,   167,    -1,   172,
      -1,   177,    -1,     3,    10,   150,   114,    -1,    -1,   150,
     151,    -1,    20,   118,   119,    -1,    21,   118,   119,    -1,
      22,   118,   119,    -1,    23,   118,   123,    -1,    24,   118,
     119,    -1,    25,   118,   119,    -1,    26,   118,   119,    -1,
      27,   118,   119,    -1,    28,   118,   123,    -1,    29,   118,
     123,    -1,   153,    -1,   154,    -1,     7,     6,     9,   155,
     114,    -1,     7,     6,    10,   155,   114,    -1,    -1,   155,
     156,    -1,    63,   118,   120,    -1,    63,   118,   122,    -1,
      13,   118,   124,    -1,    73,   118,   119,    -1,    79,   118,
     124,    -1,    14,   118,   120,    -1,    54,   118,   120,    -1,
      54,   118,   122,    -1,    56,   118,   120,    -1,    56,   118,
     122,    -1,    57,   118,   120,    -1,    57,   118,   122,    -1,
     158,    -1,   159,    -1,     4,     6,     9,   160,   114,    -1,
       4,     6,    10,   160,   114,    -1,    -1,   160,   161,    -1,
      63,   118,   120,    -1,    63,   118,   122,    -1,    30,   118,
     123,    -1,    36,   118,   120,    -1,    37,   118,   120,    -1,
      38,   118,   120,    -1,    39,   118,   123,    -1,    43,   118,
     119,    -1,    44,   118,   119,    -1,    45,   118,   119,    -1,
      46,   118,   119,    -1,    47,   118,   119,    -1,    48,   118,
     119,    -1,    52,   118,   120,    -1,    52,   118,   122,    -1,
      40,   118,   123,    -1,    20,   118,   119,    -1,    42,   118,
     123,    -1,    53,   118,   120,    -1,    53,   118,   122,    -1,
      22,   118,   119,    -1,   142,   118,   124,    -1,   143,   118,
     124,    -1,    33,   118,   119,    -1,    34,   118,   119,    -1,
      55,   118,   120,    -1,    55,   118,   122,    -1,    56,   118,
     120,    -1,    56,   118,   122,    -1,    57,   118,   120,    -1,
      57,   118,   122,    -1,    49,   118,   123,    -1,    51,   118,
     119,    -1,   163,    -1,   164,    -1,     5,     6,     9,   165,
     114,    -1,     5,     6,    10,   165,   114,    -1,    -1,   165,
     166,    -1,    63,   118,   120,    -1,    63,   118,   122,    -1,
      20,   118,   119,    -1,    64,   118,   123,    -1,    57,   118,
     120,    -1,    57,   118,   122,    -1,    59,   118,   120,    -1,
      62,   118,   120,    -1,    62,   118,   122,    -1,    68,   118,
     120,    -1,    69,   118,   120,    -1,    70,   118,   119,    -1,
      71,   118,   119,    -1,    72,   118,   120,    -1,    41,   118,
     123,    -1,    65,   118,   123,    -1,    66,   118,   123,    -1,
      67,   118,   119,    -1,   168,    -1,   169,    -1,     8,     6,
       9,   170,   114,    -1,     8,     6,    10,   170,   114,    -1,
      -1,   170,   171,    -1,    63,   118,   120,    -1,    63,   118,
     122,    -1,    20,   118,   119,    -1,    80,   118,   119,    -1,
      81,   118,   119,    -1,    51,   118,   119,    -1,   109,   118,
     123,    -1,   110,   118,   120,    -1,   110,   118,   122,    -1,
     135,   118,   123,    -1,   136,   118,   123,    -1,   137,   118,
     123,    -1,   138,   118,   123,    -1,   139,   118,   123,    -1,
     140,   118,   123,    -1,   141,   118,   123,    -1,    83,   118,
     120,    -1,    84,   118,   119,    -1,    85,   118,   119,    -1,
      86,   118,   119,    -1,    87,   118,   119,    -1,    88,   118,
     119,    -1,    89,   118,   119,    -1,    48,   118,   119,    -1,
      90,   118,   119,    -1,    91,   118,   119,    -1,    92,   118,
     119,    -1,    93,   118,   119,    -1,    94,   118,   119,    -1,
      95,   118,   119,    -1,    96,   118,   119,    -1,    97,   118,
     119,    -1,    98,   118,   119,    -1,    99,   118,   119,    -1,
     100,   118,   119,    -1,   101,   118,   119,    -1,   104,   118,
     124,    -1,   105,   118,   124,    -1,   106,   118,   124,    -1,
     107,   118,   124,    -1,   102,   118,   124,    -1,   103,   118,
     124,    -1,   108,   118,   120,    -1,   108,   118,   122,    -1,
     111,   118,   123,    -1,   112,   118,   123,    -1,   113,   118,
     123,    -1,   144,   118,   120,    -1,   144,   118,   122,    -1,
     145,   118,   120,    -1,   145,   118,   122,    -1,   173,    -1,
     174,    -1,     4,     9,   175,   114,    -1,     4,    10,   175,
     114,    -1,    -1,   175,   176,    -1,    30,   118,   123,    -1,
     142,   118,   124,    -1,   143,   118,   124,    -1,    35,   118,
     119,    -1,    33,   118,   119,    -1,    36,   118,   120,    -1,
      37,   118,   120,    -1,    38,   118,   120,    -1,    39,   118,
     123,    -1,    50,   118,   123,    -1,    43,   118,   119,    -1,
      44,   118,   119,    -1,    45,   118,   119,    -1,    46,   118,
     119,    -1,    47,   118,   119,    -1,    48,   118,   119,    -1,
      34,   118,   119,    -1,    52,   118,   120,    -1,    52,   118,
     122,    -1,    40,   118,   123,    -1,    20,   118,   119,    -1,
      42,   118,   123,    -1,    53,   118,   120,    -1,    53,   118,
     122,    -1,    15,   118,   119,    -1,    63,   118,   120,    -1,
      63,   118,   122,    -1,    22,   118,   119,    -1,    16,   118,
     120,    -1,    16,   118,   122,    -1,    55,   118,   120,    -1,
      55,   118,   122,    -1,    56,   118,   120,    -1,    56,   118,
     122,    -1,    57,   118,   120,    -1,    57,   118,   122,    -1,
      49,   118,   123,    -1,    51,   118,   119,    -1,    58,   118,
     120,    -1,    58,   118,   122,    -1,   178,    -1,   179,    -1,
       5,     9,   180,   114,    -1,     5,    10,   180,   114,    -1,
      -1,   180,   181,    -1,    20,   118,   119,    -1,    64,   118,
     123,    -1,    57,   118,   120,    -1,    57,   118,   122,    -1,
      59,   118,   120,    -1,    62,   118,   120,    -1,    62,   118,
     122,    -1,    68,   118,   120,    -1,    69,   118,   120,    -1,
      70,   118,   119,    -1,    71,   118,   119,    -1,    72,   118,
     120,    -1,    41,   118,   123,    -1,   129,   118,   120,    -1,
     127,   118,   120,    -1,   133,   118,   121,    -1,   130,   118,
     121,    -1,   126,   118,   120,    -1,   126,   118,   119,    -1,
     132,   118,   120,    -1,   132,   118,   119,    -1,   128,   118,
     120,    -1,   128,   118,   119,    -1,   125,   118,   121,    -1,
     134,   118,   121,    -1,   131,   118,   121,    -1,    60,   118,
     120,    -1,    60,   118,   122,    -1,    61,   118,   120,    -1,
      61,   118,   122,    -1,    65,   118,   123,    -1,    66,   118,
     123,    -1,    67,   118,   119,    -1,    16,   118,   120,    -1,
      16,   118,   122,    -1,    15,   118,   119,    -1,    18,   118,
     120,    -1,    18,   118,   122,    -1,    17,   118,   119,    -1,
      58,   118,   120,    -1,    58,   118,   122,    -1,    63,   118,
     120,    -1,    63,   118,   122,    -1
};

/* YYRLINE[YYN] -- source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,   178,   178,   180,   181,   187,   188,   189,   190,   191,
     192,   193,   197,   207,   209,   213,   218,   223,   228,   233,
     238,   243,   248,   253,   258,   266,   267,   271,   286,   300,
     302,   306,   310,   314,   321,   326,   333,   350,   356,   362,
     368,   374,   380,   389,   390,   394,   409,   423,   425,   429,
     433,   437,   442,   455,   471,   487,   492,   497,   502,   507,
     512,   517,   522,   528,   534,   539,   544,   549,   555,   561,
     566,   573,   580,   585,   590,   596,   602,   608,   614,   620,
     626,   631,   639,   640,   644,   659,   673,   675,   679,   683,
     687,   692,   697,   703,   709,   725,   731,   737,   754,   771,
     776,   781,   798,   803,   808,   813,   821,   822,   826,   841,
     855,   857,   861,   865,   869,   874,   879,   884,   889,   894,
     900,   906,   915,   924,   933,   942,   951,   960,   969,   982,
     987,   992,   997,  1002,  1007,  1012,  1017,  1022,  1027,  1032,
    1037,  1042,  1047,  1052,  1057,  1062,  1067,  1072,  1077,  1084,
    1091,  1098,  1105,  1112,  1119,  1125,  1131,  1136,  1141,  1146,
    1152,  1158,  1164,  1173,  1174,  1178,  1191,  1203,  1205,  1209,
    1214,  1221,  1228,  1233,  1238,  1251,  1267,  1283,  1288,  1293,
    1298,  1303,  1308,  1313,  1318,  1323,  1328,  1334,  1340,  1345,
    1350,  1355,  1361,  1367,  1372,  1378,  1384,  1389,  1395,  1401,
    1407,  1413,  1419,  1425,  1431,  1437,  1442,  1447,  1453,  1462,
    1463,  1467,  1480,  1492,  1494,  1498,  1503,  1508,  1514,  1520,
    1535,  1541,  1547,  1563,  1579,  1584,  1589,  1605,  1610,  1627,
    1633,  1639,  1645,  1651,  1659,  1665,  1673,  1679,  1687,  1693,
    1699,  1705,  1711,  1717,  1723,  1729,  1734,  1739,  1744,  1750,
    1756,  1760,  1766,  1772,  1776,  1782,  1788,  1794
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || YYTOKEN_TABLE
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "SYSTEM", "TUNNEL", "SESSION", "PROFILE",
  "PEER", "PPP", "CREATE", "MODIFY", "DIGITAL_ANALOG", "TUNNEL_AUTH_MODE",
  "PEER_IPADDR", "LAC_LNS", "TUNNEL_ID", "TUNNEL_NAME", "SESSION_ID",
  "SESSION_NAME", "UDP_PORT", "TRACE_FLAGS", "MAX_TUNNELS", "MAX_SESSIONS",
  "DRAIN_TUNNELS", "TUNNEL_ESTABLISH_TIMEOUT", "SESSION_ESTABLISH_TIMEOUT",
  "TUNNEL_PERSIST_PEND_TIMEOUT", "SESSION_PERSIST_PEND_TIMEOUT",
  "DENY_LOCAL_TUNNEL_CREATES", "DENY_REMOTE_TUNNEL_CREATES", "HIDE_AVPS",
  "OUR_ADDR", "PEER_ADDR", "OUR_UDP_PORT", "PEER_UDP_PORT", "CONFIG_ID",
  "AUTH_MODE", "FRAMING_CAP", "BEARER_CAP", "USE_TIEBREAKER",
  "ALLOW_PPP_PROXY", "USE_PPP_PROXY", "USE_UDP_CHECKSUMS", "HELLO_TIMEOUT",
  "MAX_RETRIES", "RX_WINDOW_SIZE", "TX_WINDOW_SIZE", "RETRY_TIMEOUT",
  "IDLE_TIMEOUT", "DO_PMTU_DISCOVERY", "PERSIST", "MTU", "SECRET",
  "HOST_NAME", "TUNNEL_PROFILE_NAME", "PEER_PROFILE_NAME",
  "SESSION_PROFILE_NAME", "PPP_PROFILE_NAME", "INTERFACE_NAME",
  "SESSION_TYPE", "USER_NAME", "USER_PASSWORD", "PRIV_GROUP_ID",
  "PROFILE_NAME", "SEQUENCING_REQUIRED", "USE_SEQUENCE_NUMBERS", "NO_PPP",
  "REORDER_TIMEOUT", "FRAMING_TYPE", "BEARER_TYPE", "MINIMUM_BPS",
  "MAXIMUM_BPS", "CONNECT_SPEED", "PEER_PORT", "WE_CAN_BE_LAC",
  "WE_CAN_BE_LNS", "DEFAULT_TUNNEL_PROFILE_NAME",
  "DEFAULT_SESSION_PROFILE_NAME", "DEFAULT_PPP_PROFILE_NAME", "NETMASK",
  "ASYNCMAP", "MRU", "AUTH_FLAGS", "SYNC_MODE", "CHAP_INTERVAL",
  "CHAP_MAX_CHALLENGE", "CHAP_RESTART", "PAP_MAX_AUTH_REQUESTS",
  "PAP_RESTART_INTERVAL", "PAP_TIMEOUT", "IPCP_MAX_CONFIG_REQUESTS",
  "IPCP_MAX_CONFIG_NAKS", "IPCP_MAX_TERMINATE_REQUESTS",
  "IPCP_RETRANSMIT_INTERVAL", "LCP_ECHO_FAILURE_COUNT",
  "LCP_ECHO_INTERVAL", "LCP_MAX_CONFIG_REQUESTS", "LCP_MAX_CONFIG_NAKS",
  "LCP_MAX_TERMINATE_REQUESTS", "LCP_RETRANSMIT_INTERVAL",
  "MAX_CONNECT_TIME", "MAX_FAILURE_COUNT", "LOCAL_IPADDR", "REMOTE_IPADDR",
  "DNS_IPADDR_PRI", "DNS_IPADDR_SEC", "WINS_IPADDR_PRI", "WINS_IPADDR_SEC",
  "IP_POOL_NAME", "USE_RADIUS", "RADIUS_HINT", "USE_AS_DEFAULT_ROUTE",
  "MULTILINK", "PROXY_ARP", "EOT", "SLASH", "BLCL", "ELCL", "EQUALS",
  "DECSTRING", "STRING", "HEXSTRING", "QUOTEDSTRING", "BOOL", "IPADDRESS",
  "INITIAL_RCVD_LCP_CONFREQ", "CALLING_NUMBER", "PROXY_AUTH_NAME",
  "SUB_ADDRESS", "PROXY_AUTH_TYPE", "PROXY_AUTH_RESPONSE",
  "LAST_RCVD_LCP_CONFREQ", "CALLED_NUMBER", "PROXY_AUTH_CHALLENGE",
  "LAST_SENT_LCP_CONFREQ", "AUTH_PAP", "AUTH_CHAP", "AUTH_MSCHAPV1",
  "AUTH_MSCHAPV2", "AUTH_EAP", "AUTH_NOAUTH", "AUTH_PEER", "SRC_IPADDR",
  "DEST_IPADDR", "LOCAL_NAME", "REMOTE_NAME", "$accept", "commands",
  "command", "system_command", "system_statements", "system_statement",
  "peer_profile_command", "peer_profile_create_command",
  "peer_profile_modify_command", "peer_profile_statements",
  "peer_profile_statement", "tunnel_profile_command",
  "tunnel_profile_create_command", "tunnel_profile_modify_command",
  "tunnel_profile_statements", "tunnel_profile_statement",
  "session_profile_command", "session_profile_create_command",
  "session_profile_modify_command", "session_profile_statements",
  "session_profile_statement", "ppp_profile_command",
  "ppp_profile_create_command", "ppp_profile_modify_command",
  "ppp_profile_statements", "ppp_profile_statement", "tunnel_command",
  "tunnel_create_command", "tunnel_modify_command", "tunnel_statements",
  "tunnel_statement", "session_command", "session_create_command",
  "session_modify_command", "session_statements", "session_statement", 0
};
#endif

# ifdef YYPRINT
/* YYTOKNUM[YYLEX-NUM] -- Internal token number corresponding to
   token YYLEX-NUM.  */
static const yytype_uint16 yytoknum[] =
{
       0,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,   277,   278,   279,   280,   281,   282,   283,   284,
     285,   286,   287,   288,   289,   290,   291,   292,   293,   294,
     295,   296,   297,   298,   299,   300,   301,   302,   303,   304,
     305,   306,   307,   308,   309,   310,   311,   312,   313,   314,
     315,   316,   317,   318,   319,   320,   321,   322,   323,   324,
     325,   326,   327,   328,   329,   330,   331,   332,   333,   334,
     335,   336,   337,   338,   339,   340,   341,   342,   343,   344,
     345,   346,   347,   348,   349,   350,   351,   352,   353,   354,
     355,   356,   357,   358,   359,   360,   361,   362,   363,   364,
     365,   366,   367,   368,   369,   370,   371,   372,   373,   374,
     375,   376,   377,   378,   379,   380,   381,   382,   383,   384,
     385,   386,   387,   388,   389,   390,   391,   392,   393,   394,
     395,   396,   397,   398,   399,   400
};
# endif

/* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint8 yyr1[] =
{
       0,   146,   147,   147,   147,   148,   148,   148,   148,   148,
     148,   148,   149,   150,   150,   151,   151,   151,   151,   151,
     151,   151,   151,   151,   151,   152,   152,   153,   154,   155,
     155,   156,   156,   156,   156,   156,   156,   156,   156,   156,
     156,   156,   156,   157,   157,   158,   159,   160,   160,   161,
     161,   161,   161,   161,   161,   161,   161,   161,   161,   161,
     161,   161,   161,   161,   161,   161,   161,   161,   161,   161,
     161,   161,   161,   161,   161,   161,   161,   161,   161,   161,
     161,   161,   162,   162,   163,   164,   165,   165,   166,   166,
     166,   166,   166,   166,   166,   166,   166,   166,   166,   166,
     166,   166,   166,   166,   166,   166,   167,   167,   168,   169,
     170,   170,   171,   171,   171,   171,   171,   171,   171,   171,
     171,   171,   171,   171,   171,   171,   171,   171,   171,   171,
     171,   171,   171,   171,   171,   171,   171,   171,   171,   171,
     171,   171,   171,   171,   171,   171,   171,   171,   171,   171,
     171,   171,   171,   171,   171,   171,   171,   171,   171,   171,
     171,   171,   171,   172,   172,   173,   174,   175,   175,   176,
     176,   176,   176,   176,   176,   176,   176,   176,   176,   176,
     176,   176,   176,   176,   176,   176,   176,   176,   176,   176,
     176,   176,   176,   176,   176,   176,   176,   176,   176,   176,
     176,   176,   176,   176,   176,   176,   176,   176,   176,   177,
     177,   178,   179,   180,   180,   181,   181,   181,   181,   181,
     181,   181,   181,   181,   181,   181,   181,   181,   181,   181,
     181,   181,   181,   181,   181,   181,   181,   181,   181,   181,
     181,   181,   181,   181,   181,   181,   181,   181,   181,   181,
     181,   181,   181,   181,   181,   181,   181,   181
};

/* YYR2[YYN] -- Number of symbols composing right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     0,     1,     2,     1,     1,     1,     1,     1,
       1,     1,     4,     0,     2,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     1,     1,     5,     5,     0,
       2,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     1,     1,     5,     5,     0,     2,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     1,     1,     5,     5,     0,     2,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     1,     1,     5,     5,
       0,     2,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     1,     1,     4,     4,     0,     2,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     1,
       1,     4,     4,     0,     2,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3
};

/* YYDEFACT[STATE-NAME] -- Default rule to reduce with in state
   STATE-NUM when YYTABLE doesn't specify something else to do.  Zero
   means the default is an error.  */
static const yytype_uint16 yydefact[] =
{
       2,     3,     0,     1,     0,     0,     0,     0,     0,     4,
       5,     6,    25,    26,     7,    43,    44,     8,    82,    83,
       9,   106,   107,    10,   163,   164,    11,   209,   210,    13,
       0,   167,   167,     0,   213,   213,     0,     0,     0,    47,
      47,     0,     0,    86,    86,     0,     0,    29,    29,   110,
     110,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    12,    14,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   165,     0,     0,   168,   166,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   211,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   214,   212,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    45,     0,     0,    48,
      46,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    84,    87,
      85,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    27,    30,    28,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   108,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   111,   109,    15,    16,    17,    18,    19,    20,    21,
      22,    23,    24,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     193,   197,   198,   189,   196,   169,   173,   185,   172,   174,
     175,   176,   177,   188,   190,   179,   180,   181,   182,   183,
     184,   205,   178,   206,   186,   187,   191,   192,   199,   200,
     201,   202,   203,   204,   207,   208,   194,   195,   170,   171,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   250,   248,   249,   253,   251,
     252,   215,   227,   217,   218,   254,   255,   219,   241,   242,
     243,   244,   220,   221,   256,   257,   216,   245,   246,   247,
     222,   223,   224,   225,   226,   238,   233,   232,   229,   237,
     236,   228,   231,   240,   235,   234,   230,   239,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    65,    69,    51,    72,    73,    52,    53,    54,
      55,    64,    66,    56,    57,    58,    59,    60,    61,    80,
      81,    62,    63,    67,    68,    74,    75,    76,    77,    78,
      79,    49,    50,    70,    71,    90,   102,    92,    93,    94,
      95,    96,    88,    89,    91,   103,   104,   105,    97,    98,
      99,   100,   101,    33,    36,    37,    38,    39,    40,    41,
      42,    31,    32,    34,    35,   114,   135,   117,   112,   113,
     115,   116,   128,   129,   130,   131,   132,   133,   134,   136,
     137,   138,   139,   140,   141,   142,   143,   144,   145,   146,
     147,   152,   153,   148,   149,   150,   151,   154,   155,   118,
     119,   120,   156,   157,   158,   121,   122,   123,   124,   125,
     126,   127,   159,   160,   161,   162
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,     2,     9,    10,    38,    62,    11,    12,    13,   137,
     272,    14,    15,    16,    63,   179,    17,    18,    19,   100,
     229,    20,    21,    22,   139,   321,    23,    24,    25,    41,
      98,    26,    27,    28,    45,   135
};

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
#define YYPACT_NINF -119
static const yytype_int16 yypact[] =
{
    -111,  -119,     6,  -119,    -2,    99,   105,    33,    49,  -119,
    -119,  -119,  -119,  -119,  -119,  -119,  -119,  -119,  -119,  -119,
    -119,  -119,  -119,  -119,  -119,  -119,  -119,  -119,  -119,  -119,
       7,  -119,  -119,    37,  -119,  -119,    51,   160,   109,  -119,
    -119,   -15,    29,  -119,  -119,   414,   447,  -119,  -119,  -119,
    -119,  -106,   -92,   -68,   -48,   -35,   -24,   -21,    -8,    21,
      31,  -119,  -119,   314,   352,    36,    57,   101,   103,   117,
     131,   152,   185,   186,   187,   189,   190,   191,   192,   193,
     194,   195,   196,   197,   198,   199,   200,   201,   202,   203,
     204,   212,   213,   217,   219,  -119,   220,   221,  -119,  -119,
     382,   529,   222,   223,   224,   225,   227,   228,   231,   237,
     246,   250,   255,   257,   258,   260,   261,   262,   263,   265,
     266,   269,   275,   288,  -119,   292,   293,   294,   295,   296,
     298,   299,   300,   301,   302,  -119,  -119,    39,    44,    93,
     188,    -3,    32,   108,   -20,   303,   305,   306,   307,     2,
     183,   309,   315,   317,   318,   319,   320,   322,   324,   325,
     340,   341,   342,   343,   350,   351,   369,   371,   372,   373,
     374,   375,   379,   380,   381,   383,  -119,   384,   385,  -119,
    -119,   401,  -118,   402,   403,   347,   404,   405,   406,   407,
     409,   410,   377,   408,   411,   413,   416,   417,   418,   419,
     431,   428,   429,   434,   -66,   -32,   -31,   -16,    -1,     0,
       4,   297,   430,   415,   437,   438,   439,   440,   441,   442,
     444,   445,   446,   448,   449,   450,   451,   453,  -119,  -119,
    -119,   463,    20,   464,    25,   465,   462,    26,    30,   467,
      35,    40,    41,    45,   466,   479,   480,   471,   484,   485,
     487,   488,   489,    38,   106,   490,   145,   491,    43,    92,
     147,   492,   493,   494,   497,   498,   499,   500,   501,   502,
     503,  -119,  -119,  -119,   504,   505,   506,   507,   508,   509,
     510,   511,   512,   513,   514,   515,   516,   517,   518,   519,
     520,   521,   522,   523,   524,   526,   527,   528,   530,   531,
     532,   533,   534,   535,   536,   537,   538,   539,   540,   541,
     542,  -119,   543,   544,   545,   546,   547,   548,   549,   550,
     551,  -119,  -119,  -119,  -119,  -119,  -119,  -119,  -119,  -119,
    -119,  -119,  -119,   552,   553,   554,   555,   556,   558,   559,
     560,   561,   562,   563,   557,   564,   568,   569,   570,   571,
     572,   573,    46,    89,    90,    94,    95,    98,   567,   574,
    -119,  -119,  -119,  -119,  -119,  -119,  -119,  -119,  -119,  -119,
    -119,  -119,  -119,  -119,  -119,  -119,  -119,  -119,  -119,  -119,
    -119,  -119,  -119,  -119,  -119,  -119,  -119,  -119,  -119,  -119,
    -119,  -119,  -119,  -119,  -119,  -119,  -119,  -119,  -119,  -119,
     575,   576,   102,   577,   120,   121,   578,   579,   580,   581,
     584,   585,   587,   588,   589,  -119,  -119,  -119,  -119,  -119,
    -119,  -119,  -119,  -119,  -119,  -119,  -119,  -119,  -119,  -119,
    -119,  -119,  -119,  -119,  -119,  -119,  -119,  -119,  -119,  -119,
    -119,  -119,  -119,  -119,  -119,  -119,  -119,  -119,  -119,  -119,
    -119,  -119,  -119,  -119,  -119,  -119,  -119,  -119,   586,   591,
     124,   125,   128,   132,   593,   590,   594,   596,   597,   133,
     598,   599,   600,   602,   603,   604,   605,   606,   607,   608,
     609,   610,   611,   612,   613,   614,   615,   616,   617,   618,
     619,   595,   620,   621,   622,   623,   624,   136,   626,   137,
     627,   628,   629,   630,   631,   632,   633,   634,   635,   636,
     140,   141,  -119,  -119,  -119,  -119,  -119,  -119,  -119,  -119,
    -119,  -119,  -119,  -119,  -119,  -119,  -119,  -119,  -119,  -119,
    -119,  -119,  -119,  -119,  -119,  -119,  -119,  -119,  -119,  -119,
    -119,  -119,  -119,  -119,  -119,  -119,  -119,  -119,  -119,  -119,
    -119,  -119,  -119,  -119,  -119,  -119,  -119,  -119,  -119,  -119,
    -119,  -119,  -119,  -119,  -119,  -119,  -119,  -119,  -119,  -119,
    -119,  -119,  -119,  -119,  -119,  -119,  -119,  -119,  -119,  -119,
    -119,  -119,  -119,  -119,  -119,  -119,  -119,  -119,  -119,  -119,
    -119,  -119,  -119,  -119,  -119,  -119,  -119,  -119,  -119,  -119,
    -119,  -119,  -119,  -119,  -119,  -119,  -119,  -119,  -119,  -119,
    -119,  -119,  -119,  -119,  -119,  -119,  -119,  -119,  -119,  -119,
    -119,  -119,  -119,  -119,  -119,  -119
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -119,  -119,  -119,  -119,  -119,  -119,  -119,  -119,  -119,   478,
    -119,  -119,  -119,  -119,   525,  -119,  -119,  -119,  -119,   637,
    -119,  -119,  -119,  -119,   643,  -119,  -119,  -119,  -119,   638,
    -119,  -119,  -119,  -119,   647,  -119
};

/* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule which
   number is the opposite.  If zero, do what YYDEFACT says.
   If YYTABLE_NINF, syntax error.  */
#define YYTABLE_NINF -1
static const yytype_uint16 yytable[] =
{
      65,    66,   361,     1,   362,    67,     3,    68,    29,     4,
       5,     6,   141,     7,     8,    69,    39,    40,    70,    71,
      72,    73,    74,    75,    76,    77,   142,    78,    79,    80,
      81,    82,    83,    84,    85,    86,    87,    88,    89,    36,
      90,    91,    92,    93,    65,    66,    43,    44,    94,    67,
     143,    68,   263,   264,   384,    37,   385,   263,   264,    69,
      47,    48,    70,    71,    72,    73,    74,    75,    76,    77,
     144,    78,    79,    80,    81,    82,    83,    84,    85,    86,
      87,    88,    89,   145,    90,    91,    92,    93,   386,   388,
     387,   389,    94,   265,   146,   266,   267,   147,   265,    95,
     266,   267,   268,   326,   390,    30,   391,   268,    31,    32,
     148,    33,   269,   274,    34,    35,   323,   269,   270,   392,
     394,   393,   395,   270,   396,   331,   397,    96,    97,    51,
      52,    53,    54,    55,    56,    57,    58,    59,    60,   149,
     416,   275,   417,    99,   276,   419,   423,   420,   424,   150,
     425,   324,   426,   271,   181,   428,   277,   429,   273,   445,
     430,   432,   431,   433,   452,   434,   531,   435,   532,    49,
      50,    96,    97,   278,   279,   182,   280,   281,   282,   283,
     284,   285,   286,   287,   288,   289,   290,   291,   292,   293,
     294,   295,   296,   297,   298,   299,   300,   301,   302,   303,
     304,   305,   306,   307,   308,   309,   310,   311,   274,   533,
     535,   534,   536,   453,   537,   539,   538,   540,   541,   183,
     542,   184,   547,    61,   548,   446,   447,   325,   312,   313,
     314,   315,   316,   317,   318,   185,   275,   319,   320,   276,
     550,   552,   551,   553,   565,   567,   566,   568,   569,   186,
     570,   277,   571,   578,   572,   579,   607,   610,   608,   611,
     622,   624,   623,   625,   449,   450,   454,   455,   278,   279,
     187,   280,   281,   282,   283,   284,   285,   286,   287,   288,
     289,   290,   291,   292,   293,   294,   295,   296,   297,   298,
     299,   300,   301,   302,   303,   304,   305,   306,   307,   308,
     309,   310,   322,   188,   189,   190,   332,   191,   192,   193,
     194,   195,   196,   197,   198,   199,   200,   201,   202,   203,
     204,   205,   206,   312,   313,   314,   315,   316,   317,   318,
     207,   208,   319,   320,   151,   209,   152,   210,   211,   212,
     231,   232,   233,   234,   153,   235,   236,   154,   155,   237,
     156,   157,   158,   159,   160,   238,   161,   162,   163,   164,
     165,   166,   167,   168,   239,   169,   170,   171,   240,   172,
     173,   174,   151,   241,   152,   242,   243,   175,   244,   245,
     246,   247,   153,   248,   249,   154,   155,   250,   156,   157,
     158,   159,   160,   251,   161,   162,   163,   164,   165,   166,
     167,   168,   213,   169,   170,   171,   252,   172,   173,   174,
     253,   254,   255,   256,   257,   175,   258,   259,   260,   261,
     262,   398,   327,   214,   328,   329,   330,   333,   176,   102,
     103,   104,   105,   334,   106,   335,   336,   337,   338,   215,
     339,   216,   340,   341,   217,   218,   219,   220,   221,   222,
     223,   224,   225,   226,   227,   107,   177,   178,   342,   343,
     344,   345,   102,   103,   104,   105,   180,   106,   346,   347,
     365,   108,   109,   110,   111,   112,   113,   114,   115,   116,
     117,   118,   119,   120,   121,   122,   123,   348,   107,   349,
     350,   351,   352,   353,   177,   178,   228,   354,   355,   356,
     372,   357,   358,   359,   108,   109,   110,   111,   112,   113,
     114,   115,   116,   117,   118,   119,   120,   121,   122,   123,
     360,   363,   364,   366,   367,   368,   138,   369,   124,   370,
     371,   373,   375,   400,   374,   376,   377,   378,   379,   125,
     126,   127,   128,   129,   130,   131,   132,   133,   134,   213,
     380,   381,   382,   383,   399,   401,   402,   403,   404,   405,
     406,   136,   407,   408,   409,    64,   410,   411,   412,   413,
     214,   414,   125,   126,   127,   128,   129,   130,   131,   132,
     133,   134,   415,   418,   421,   422,   215,   427,   216,   436,
     439,   217,   218,   219,   220,   221,   222,   223,   224,   225,
     226,   227,   437,   438,   440,   441,   442,   443,     0,   444,
     448,   451,   458,   456,   457,   459,   460,   461,   462,   463,
     464,   465,   466,   467,   468,   469,   470,   471,   472,   473,
     474,   475,   476,   477,   478,   479,   480,   481,   482,   483,
     484,   485,   486,   230,   487,   488,   489,     0,   490,   491,
     492,   493,   494,   495,   496,   497,   498,   499,   500,   501,
     502,   503,   504,   505,   506,   507,   508,   509,   510,   511,
      42,   512,   513,     0,   515,   516,   523,   514,   517,   518,
     519,   101,    46,   524,   520,   521,   522,   525,   526,   527,
     528,   543,   530,   140,   545,   529,     0,   549,   544,   546,
     557,   554,   555,   556,   558,   559,   560,   561,     0,   562,
     563,   564,   573,   575,   574,   576,   577,   580,   581,   601,
     582,   583,   584,   585,   586,   587,   588,   589,   590,   591,
     592,   593,   594,   595,   596,   597,   598,   599,   600,     0,
       0,     0,     0,     0,   602,   603,   604,   605,   606,   609,
     612,   613,   614,   615,   616,   617,   618,   619,   620,   621
};

static const yytype_int16 yycheck[] =
{
      15,    16,   120,   114,   122,    20,     0,    22,    10,     3,
       4,     5,   118,     7,     8,    30,     9,    10,    33,    34,
      35,    36,    37,    38,    39,    40,   118,    42,    43,    44,
      45,    46,    47,    48,    49,    50,    51,    52,    53,     6,
      55,    56,    57,    58,    15,    16,     9,    10,    63,    20,
     118,    22,    13,    14,   120,     6,   122,    13,    14,    30,
       9,    10,    33,    34,    35,    36,    37,    38,    39,    40,
     118,    42,    43,    44,    45,    46,    47,    48,    49,    50,
      51,    52,    53,   118,    55,    56,    57,    58,   120,   120,
     122,   122,    63,    54,   118,    56,    57,   118,    54,   114,
      56,    57,    63,   123,   120,     6,   122,    63,     9,    10,
     118,     6,    73,    20,     9,    10,   119,    73,    79,   120,
     120,   122,   122,    79,   120,   123,   122,   142,   143,    20,
      21,    22,    23,    24,    25,    26,    27,    28,    29,   118,
     120,    48,   122,   114,    51,   120,   120,   122,   122,   118,
     120,   119,   122,   114,   118,   120,    63,   122,   114,   121,
     120,   120,   122,   122,   121,   120,   120,   122,   122,     9,
      10,   142,   143,    80,    81,   118,    83,    84,    85,    86,
      87,    88,    89,    90,    91,    92,    93,    94,    95,    96,
      97,    98,    99,   100,   101,   102,   103,   104,   105,   106,
     107,   108,   109,   110,   111,   112,   113,   114,    20,   120,
     120,   122,   122,   121,   120,   120,   122,   122,   120,   118,
     122,   118,   120,   114,   122,   119,   120,   119,   135,   136,
     137,   138,   139,   140,   141,   118,    48,   144,   145,    51,
     120,   120,   122,   122,   120,   120,   122,   122,   120,   118,
     122,    63,   120,   120,   122,   122,   120,   120,   122,   122,
     120,   120,   122,   122,   119,   120,   119,   120,    80,    81,
     118,    83,    84,    85,    86,    87,    88,    89,    90,    91,
      92,    93,    94,    95,    96,    97,    98,    99,   100,   101,
     102,   103,   104,   105,   106,   107,   108,   109,   110,   111,
     112,   113,   114,   118,   118,   118,   123,   118,   118,   118,
     118,   118,   118,   118,   118,   118,   118,   118,   118,   118,
     118,   118,   118,   135,   136,   137,   138,   139,   140,   141,
     118,   118,   144,   145,    20,   118,    22,   118,   118,   118,
     118,   118,   118,   118,    30,   118,   118,    33,    34,   118,
      36,    37,    38,    39,    40,   118,    42,    43,    44,    45,
      46,    47,    48,    49,   118,    51,    52,    53,   118,    55,
      56,    57,    20,   118,    22,   118,   118,    63,   118,   118,
     118,   118,    30,   118,   118,    33,    34,   118,    36,    37,
      38,    39,    40,   118,    42,    43,    44,    45,    46,    47,
      48,    49,    20,    51,    52,    53,   118,    55,    56,    57,
     118,   118,   118,   118,   118,    63,   118,   118,   118,   118,
     118,   124,   119,    41,   119,   119,   119,   118,   114,    15,
      16,    17,    18,   118,    20,   118,   118,   118,   118,    57,
     118,    59,   118,   118,    62,    63,    64,    65,    66,    67,
      68,    69,    70,    71,    72,    41,   142,   143,   118,   118,
     118,   118,    15,    16,    17,    18,   114,    20,   118,   118,
     123,    57,    58,    59,    60,    61,    62,    63,    64,    65,
      66,    67,    68,    69,    70,    71,    72,   118,    41,   118,
     118,   118,   118,   118,   142,   143,   114,   118,   118,   118,
     123,   118,   118,   118,    57,    58,    59,    60,    61,    62,
      63,    64,    65,    66,    67,    68,    69,    70,    71,    72,
     119,   119,   119,   119,   119,   119,    48,   120,   114,   120,
     120,   123,   119,   118,   123,   119,   119,   119,   119,   125,
     126,   127,   128,   129,   130,   131,   132,   133,   134,    20,
     119,   123,   123,   119,   124,   118,   118,   118,   118,   118,
     118,   114,   118,   118,   118,    40,   118,   118,   118,   118,
      41,   118,   125,   126,   127,   128,   129,   130,   131,   132,
     133,   134,   119,   119,   119,   123,    57,   120,    59,   123,
     119,    62,    63,    64,    65,    66,    67,    68,    69,    70,
      71,    72,   123,   123,   120,   120,   119,   119,    -1,   120,
     120,   120,   118,   121,   121,   118,   118,   118,   118,   118,
     118,   118,   118,   118,   118,   118,   118,   118,   118,   118,
     118,   118,   118,   118,   118,   118,   118,   118,   118,   118,
     118,   118,   118,   114,   118,   118,   118,    -1,   118,   118,
     118,   118,   118,   118,   118,   118,   118,   118,   118,   118,
     118,   118,   118,   118,   118,   118,   118,   118,   118,   118,
      32,   119,   119,    -1,   119,   119,   119,   123,   120,   120,
     120,    44,    35,   119,   123,   123,   123,   119,   119,   119,
     119,   124,   119,    50,   119,   123,    -1,   120,   124,   123,
     119,   123,   123,   123,   120,   120,   119,   119,    -1,   120,
     124,   120,   119,   119,   124,   119,   119,   119,   119,   124,
     120,   119,   119,   119,   119,   119,   119,   119,   119,   119,
     119,   119,   119,   119,   119,   119,   119,   119,   119,    -1,
      -1,    -1,    -1,    -1,   124,   124,   124,   124,   124,   123,
     123,   123,   123,   123,   123,   123,   123,   123,   123,   123
};

/* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
   symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,   114,   147,     0,     3,     4,     5,     7,     8,   148,
     149,   152,   153,   154,   157,   158,   159,   162,   163,   164,
     167,   168,   169,   172,   173,   174,   177,   178,   179,    10,
       6,     9,    10,     6,     9,    10,     6,     6,   150,     9,
      10,   175,   175,     9,    10,   180,   180,     9,    10,     9,
      10,    20,    21,    22,    23,    24,    25,    26,    27,    28,
      29,   114,   151,   160,   160,    15,    16,    20,    22,    30,
      33,    34,    35,    36,    37,    38,    39,    40,    42,    43,
      44,    45,    46,    47,    48,    49,    50,    51,    52,    53,
      55,    56,    57,    58,    63,   114,   142,   143,   176,   114,
     165,   165,    15,    16,    17,    18,    20,    41,    57,    58,
      59,    60,    61,    62,    63,    64,    65,    66,    67,    68,
      69,    70,    71,    72,   114,   125,   126,   127,   128,   129,
     130,   131,   132,   133,   134,   181,   114,   155,   155,   170,
     170,   118,   118,   118,   118,   118,   118,   118,   118,   118,
     118,    20,    22,    30,    33,    34,    36,    37,    38,    39,
      40,    42,    43,    44,    45,    46,    47,    48,    49,    51,
      52,    53,    55,    56,    57,    63,   114,   142,   143,   161,
     114,   118,   118,   118,   118,   118,   118,   118,   118,   118,
     118,   118,   118,   118,   118,   118,   118,   118,   118,   118,
     118,   118,   118,   118,   118,   118,   118,   118,   118,   118,
     118,   118,   118,    20,    41,    57,    59,    62,    63,    64,
      65,    66,    67,    68,    69,    70,    71,    72,   114,   166,
     114,   118,   118,   118,   118,   118,   118,   118,   118,   118,
     118,   118,   118,   118,   118,   118,   118,   118,   118,   118,
     118,   118,   118,   118,   118,   118,   118,   118,   118,   118,
     118,   118,   118,    13,    14,    54,    56,    57,    63,    73,
      79,   114,   156,   114,    20,    48,    51,    63,    80,    81,
      83,    84,    85,    86,    87,    88,    89,    90,    91,    92,
      93,    94,    95,    96,    97,    98,    99,   100,   101,   102,
     103,   104,   105,   106,   107,   108,   109,   110,   111,   112,
     113,   114,   135,   136,   137,   138,   139,   140,   141,   144,
     145,   171,   114,   119,   119,   119,   123,   119,   119,   119,
     119,   123,   123,   118,   118,   118,   118,   118,   118,   118,
     118,   118,   118,   118,   118,   118,   118,   118,   118,   118,
     118,   118,   118,   118,   118,   118,   118,   118,   118,   118,
     119,   120,   122,   119,   119,   123,   119,   119,   119,   120,
     120,   120,   123,   123,   123,   119,   119,   119,   119,   119,
     119,   123,   123,   119,   120,   122,   120,   122,   120,   122,
     120,   122,   120,   122,   120,   122,   120,   122,   124,   124,
     118,   118,   118,   118,   118,   118,   118,   118,   118,   118,
     118,   118,   118,   118,   118,   119,   120,   122,   119,   120,
     122,   119,   123,   120,   122,   120,   122,   120,   120,   122,
     120,   122,   120,   122,   120,   122,   123,   123,   123,   119,
     120,   120,   119,   119,   120,   121,   119,   120,   120,   119,
     120,   120,   121,   121,   119,   120,   121,   121,   118,   118,
     118,   118,   118,   118,   118,   118,   118,   118,   118,   118,
     118,   118,   118,   118,   118,   118,   118,   118,   118,   118,
     118,   118,   118,   118,   118,   118,   118,   118,   118,   118,
     118,   118,   118,   118,   118,   118,   118,   118,   118,   118,
     118,   118,   118,   118,   118,   118,   118,   118,   118,   118,
     118,   118,   119,   119,   123,   119,   119,   120,   120,   120,
     123,   123,   123,   119,   119,   119,   119,   119,   119,   123,
     119,   120,   122,   120,   122,   120,   122,   120,   122,   120,
     122,   120,   122,   124,   124,   119,   123,   120,   122,   120,
     120,   122,   120,   122,   123,   123,   123,   119,   120,   120,
     119,   119,   120,   124,   120,   120,   122,   120,   122,   120,
     122,   120,   122,   119,   124,   119,   119,   119,   120,   122,
     119,   119,   120,   119,   119,   119,   119,   119,   119,   119,
     119,   119,   119,   119,   119,   119,   119,   119,   119,   119,
     119,   124,   124,   124,   124,   124,   124,   120,   122,   123,
     120,   122,   123,   123,   123,   123,   123,   123,   123,   123,
     123,   123,   120,   122,   120,   122
};

#define yyerrok		(yyerrstatus = 0)
#define yyclearin	(yychar = YYEMPTY)
#define YYEMPTY		(-2)
#define YYEOF		0

#define YYACCEPT	goto yyacceptlab
#define YYABORT		goto yyabortlab
#define YYERROR		goto yyerrorlab


/* Like YYERROR except do call yyerror.  This remains here temporarily
   to ease the transition to the new meaning of YYERROR, for GCC.
   Once GCC version 2 has supplanted version 1, this can go.  */

#define YYFAIL		goto yyerrlab

#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)					\
do								\
  if (yychar == YYEMPTY && yylen == 1)				\
    {								\
      yychar = (Token);						\
      yylval = (Value);						\
      yytoken = YYTRANSLATE (yychar);				\
      YYPOPSTACK (1);						\
      goto yybackup;						\
    }								\
  else								\
    {								\
      yyerror (YY_("syntax error: cannot back up")); \
      YYERROR;							\
    }								\
while (YYID (0))


#define YYTERROR	1
#define YYERRCODE	256


/* YYLLOC_DEFAULT -- Set CURRENT to span from RHS[1] to RHS[N].
   If N is 0, then set CURRENT to the empty location which ends
   the previous symbol: RHS[0] (always defined).  */

#define YYRHSLOC(Rhs, K) ((Rhs)[K])
#ifndef YYLLOC_DEFAULT
# define YYLLOC_DEFAULT(Current, Rhs, N)				\
    do									\
      if (YYID (N))                                                    \
	{								\
	  (Current).first_line   = YYRHSLOC (Rhs, 1).first_line;	\
	  (Current).first_column = YYRHSLOC (Rhs, 1).first_column;	\
	  (Current).last_line    = YYRHSLOC (Rhs, N).last_line;		\
	  (Current).last_column  = YYRHSLOC (Rhs, N).last_column;	\
	}								\
      else								\
	{								\
	  (Current).first_line   = (Current).last_line   =		\
	    YYRHSLOC (Rhs, 0).last_line;				\
	  (Current).first_column = (Current).last_column =		\
	    YYRHSLOC (Rhs, 0).last_column;				\
	}								\
    while (YYID (0))
#endif


/* YY_LOCATION_PRINT -- Print the location on the stream.
   This macro was not mandated originally: define only if we know
   we won't break user code: when these are the locations we know.  */

#ifndef YY_LOCATION_PRINT
# if YYLTYPE_IS_TRIVIAL
#  define YY_LOCATION_PRINT(File, Loc)			\
     fprintf (File, "%d.%d-%d.%d",			\
	      (Loc).first_line, (Loc).first_column,	\
	      (Loc).last_line,  (Loc).last_column)
# else
#  define YY_LOCATION_PRINT(File, Loc) ((void) 0)
# endif
#endif


/* YYLEX -- calling `yylex' with the right arguments.  */

#ifdef YYLEX_PARAM
# define YYLEX yylex (YYLEX_PARAM)
#else
# define YYLEX yylex ()
#endif

/* Enable debugging if requested.  */
#if YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)			\
do {						\
  if (yydebug)					\
    YYFPRINTF Args;				\
} while (YYID (0))

# define YY_SYMBOL_PRINT(Title, Type, Value, Location)			  \
do {									  \
  if (yydebug)								  \
    {									  \
      YYFPRINTF (stderr, "%s ", Title);					  \
      yy_symbol_print (stderr,						  \
		  Type, Value); \
      YYFPRINTF (stderr, "\n");						  \
    }									  \
} while (YYID (0))


/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

/*ARGSUSED*/
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_symbol_value_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep)
#else
static void
yy_symbol_value_print (yyoutput, yytype, yyvaluep)
    FILE *yyoutput;
    int yytype;
    YYSTYPE const * const yyvaluep;
#endif
{
  if (!yyvaluep)
    return;
# ifdef YYPRINT
  if (yytype < YYNTOKENS)
    YYPRINT (yyoutput, yytoknum[yytype], *yyvaluep);
# else
  YYUSE (yyoutput);
# endif
  switch (yytype)
    {
      default:
	break;
    }
}


/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_symbol_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep)
#else
static void
yy_symbol_print (yyoutput, yytype, yyvaluep)
    FILE *yyoutput;
    int yytype;
    YYSTYPE const * const yyvaluep;
#endif
{
  if (yytype < YYNTOKENS)
    YYFPRINTF (yyoutput, "token %s (", yytname[yytype]);
  else
    YYFPRINTF (yyoutput, "nterm %s (", yytname[yytype]);

  yy_symbol_value_print (yyoutput, yytype, yyvaluep);
  YYFPRINTF (yyoutput, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_stack_print (yytype_int16 *yybottom, yytype_int16 *yytop)
#else
static void
yy_stack_print (yybottom, yytop)
    yytype_int16 *yybottom;
    yytype_int16 *yytop;
#endif
{
  YYFPRINTF (stderr, "Stack now");
  for (; yybottom <= yytop; yybottom++)
    {
      int yybot = *yybottom;
      YYFPRINTF (stderr, " %d", yybot);
    }
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)				\
do {								\
  if (yydebug)							\
    yy_stack_print ((Bottom), (Top));				\
} while (YYID (0))


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_reduce_print (YYSTYPE *yyvsp, int yyrule)
#else
static void
yy_reduce_print (yyvsp, yyrule)
    YYSTYPE *yyvsp;
    int yyrule;
#endif
{
  int yynrhs = yyr2[yyrule];
  int yyi;
  unsigned long int yylno = yyrline[yyrule];
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %lu):\n",
	     yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      YYFPRINTF (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr, yyrhs[yyprhs[yyrule] + yyi],
		       &(yyvsp[(yyi + 1) - (yynrhs)])
		       		       );
      YYFPRINTF (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)		\
do {					\
  if (yydebug)				\
    yy_reduce_print (yyvsp, Rule); \
} while (YYID (0))

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;
#else /* !YYDEBUG */
# define YYDPRINTF(Args)
# define YY_SYMBOL_PRINT(Title, Type, Value, Location)
# define YY_STACK_PRINT(Bottom, Top)
# define YY_REDUCE_PRINT(Rule)
#endif /* !YYDEBUG */


/* YYINITDEPTH -- initial size of the parser's stacks.  */
#ifndef	YYINITDEPTH
# define YYINITDEPTH 200
#endif

/* YYMAXDEPTH -- maximum size the stacks can grow to (effective only
   if the built-in stack extension method is used).

   Do not make this value too large; the results are undefined if
   YYSTACK_ALLOC_MAXIMUM < YYSTACK_BYTES (YYMAXDEPTH)
   evaluated with infinite-precision integer arithmetic.  */

#ifndef YYMAXDEPTH
# define YYMAXDEPTH 10000
#endif



#if YYERROR_VERBOSE

# ifndef yystrlen
#  if defined __GLIBC__ && defined _STRING_H
#   define yystrlen strlen
#  else
/* Return the length of YYSTR.  */
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static YYSIZE_T
yystrlen (const char *yystr)
#else
static YYSIZE_T
yystrlen (yystr)
    const char *yystr;
#endif
{
  YYSIZE_T yylen;
  for (yylen = 0; yystr[yylen]; yylen++)
    continue;
  return yylen;
}
#  endif
# endif

# ifndef yystpcpy
#  if defined __GLIBC__ && defined _STRING_H && defined _GNU_SOURCE
#   define yystpcpy stpcpy
#  else
/* Copy YYSRC to YYDEST, returning the address of the terminating '\0' in
   YYDEST.  */
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static char *
yystpcpy (char *yydest, const char *yysrc)
#else
static char *
yystpcpy (yydest, yysrc)
    char *yydest;
    const char *yysrc;
#endif
{
  char *yyd = yydest;
  const char *yys = yysrc;

  while ((*yyd++ = *yys++) != '\0')
    continue;

  return yyd - 1;
}
#  endif
# endif

# ifndef yytnamerr
/* Copy to YYRES the contents of YYSTR after stripping away unnecessary
   quotes and backslashes, so that it's suitable for yyerror.  The
   heuristic is that double-quoting is unnecessary unless the string
   contains an apostrophe, a comma, or backslash (other than
   backslash-backslash).  YYSTR is taken from yytname.  If YYRES is
   null, do not copy; instead, return the length of what the result
   would have been.  */
static YYSIZE_T
yytnamerr (char *yyres, const char *yystr)
{
  if (*yystr == '"')
    {
      YYSIZE_T yyn = 0;
      char const *yyp = yystr;

      for (;;)
	switch (*++yyp)
	  {
	  case '\'':
	  case ',':
	    goto do_not_strip_quotes;

	  case '\\':
	    if (*++yyp != '\\')
	      goto do_not_strip_quotes;
	    /* Fall through.  */
	  default:
	    if (yyres)
	      yyres[yyn] = *yyp;
	    yyn++;
	    break;

	  case '"':
	    if (yyres)
	      yyres[yyn] = '\0';
	    return yyn;
	  }
    do_not_strip_quotes: ;
    }

  if (! yyres)
    return yystrlen (yystr);

  return yystpcpy (yyres, yystr) - yyres;
}
# endif

/* Copy into YYRESULT an error message about the unexpected token
   YYCHAR while in state YYSTATE.  Return the number of bytes copied,
   including the terminating null byte.  If YYRESULT is null, do not
   copy anything; just return the number of bytes that would be
   copied.  As a special case, return 0 if an ordinary "syntax error"
   message will do.  Return YYSIZE_MAXIMUM if overflow occurs during
   size calculation.  */
static YYSIZE_T
yysyntax_error (char *yyresult, int yystate, int yychar)
{
  int yyn = yypact[yystate];

  if (! (YYPACT_NINF < yyn && yyn <= YYLAST))
    return 0;
  else
    {
      int yytype = YYTRANSLATE (yychar);
      YYSIZE_T yysize0 = yytnamerr (0, yytname[yytype]);
      YYSIZE_T yysize = yysize0;
      YYSIZE_T yysize1;
      int yysize_overflow = 0;
      enum { YYERROR_VERBOSE_ARGS_MAXIMUM = 5 };
      char const *yyarg[YYERROR_VERBOSE_ARGS_MAXIMUM];
      int yyx;

# if 0
      /* This is so xgettext sees the translatable formats that are
	 constructed on the fly.  */
      YY_("syntax error, unexpected %s");
      YY_("syntax error, unexpected %s, expecting %s");
      YY_("syntax error, unexpected %s, expecting %s or %s");
      YY_("syntax error, unexpected %s, expecting %s or %s or %s");
      YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s");
# endif
      char *yyfmt;
      char const *yyf;
      static char const yyunexpected[] = "syntax error, unexpected %s";
      static char const yyexpecting[] = ", expecting %s";
      static char const yyor[] = " or %s";
      char yyformat[sizeof yyunexpected
		    + sizeof yyexpecting - 1
		    + ((YYERROR_VERBOSE_ARGS_MAXIMUM - 2)
		       * (sizeof yyor - 1))];
      char const *yyprefix = yyexpecting;

      /* Start YYX at -YYN if negative to avoid negative indexes in
	 YYCHECK.  */
      int yyxbegin = yyn < 0 ? -yyn : 0;

      /* Stay within bounds of both yycheck and yytname.  */
      int yychecklim = YYLAST - yyn + 1;
      int yyxend = yychecklim < YYNTOKENS ? yychecklim : YYNTOKENS;
      int yycount = 1;

      yyarg[0] = yytname[yytype];
      yyfmt = yystpcpy (yyformat, yyunexpected);

      for (yyx = yyxbegin; yyx < yyxend; ++yyx)
	if (yycheck[yyx + yyn] == yyx && yyx != YYTERROR)
	  {
	    if (yycount == YYERROR_VERBOSE_ARGS_MAXIMUM)
	      {
		yycount = 1;
		yysize = yysize0;
		yyformat[sizeof yyunexpected - 1] = '\0';
		break;
	      }
	    yyarg[yycount++] = yytname[yyx];
	    yysize1 = yysize + yytnamerr (0, yytname[yyx]);
	    yysize_overflow |= (yysize1 < yysize);
	    yysize = yysize1;
	    yyfmt = yystpcpy (yyfmt, yyprefix);
	    yyprefix = yyor;
	  }

      yyf = YY_(yyformat);
      yysize1 = yysize + yystrlen (yyf);
      yysize_overflow |= (yysize1 < yysize);
      yysize = yysize1;

      if (yysize_overflow)
	return YYSIZE_MAXIMUM;

      if (yyresult)
	{
	  /* Avoid sprintf, as that infringes on the user's name space.
	     Don't have undefined behavior even if the translation
	     produced a string with the wrong number of "%s"s.  */
	  char *yyp = yyresult;
	  int yyi = 0;
	  while ((*yyp = *yyf) != '\0')
	    {
	      if (*yyp == '%' && yyf[1] == 's' && yyi < yycount)
		{
		  yyp += yytnamerr (yyp, yyarg[yyi++]);
		  yyf += 2;
		}
	      else
		{
		  yyp++;
		  yyf++;
		}
	    }
	}
      return yysize;
    }
}
#endif /* YYERROR_VERBOSE */


/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

/*ARGSUSED*/
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yydestruct (const char *yymsg, int yytype, YYSTYPE *yyvaluep)
#else
static void
yydestruct (yymsg, yytype, yyvaluep)
    const char *yymsg;
    int yytype;
    YYSTYPE *yyvaluep;
#endif
{
  YYUSE (yyvaluep);

  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yytype, yyvaluep, yylocationp);

  switch (yytype)
    {

      default:
	break;
    }
}

/* Prevent warnings from -Wmissing-prototypes.  */
#ifdef YYPARSE_PARAM
#if defined __STDC__ || defined __cplusplus
int yyparse (void *YYPARSE_PARAM);
#else
int yyparse ();
#endif
#else /* ! YYPARSE_PARAM */
#if defined __STDC__ || defined __cplusplus
int yyparse (void);
#else
int yyparse ();
#endif
#endif /* ! YYPARSE_PARAM */


/* The lookahead symbol.  */
int yychar;

/* The semantic value of the lookahead symbol.  */
YYSTYPE yylval;

/* Number of syntax errors so far.  */
int yynerrs;



/*-------------------------.
| yyparse or yypush_parse.  |
`-------------------------*/

#ifdef YYPARSE_PARAM
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
int
yyparse (void *YYPARSE_PARAM)
#else
int
yyparse (YYPARSE_PARAM)
    void *YYPARSE_PARAM;
#endif
#else /* ! YYPARSE_PARAM */
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
int
yyparse (void)
#else
int
yyparse ()

#endif
#endif
{


    int yystate;
    /* Number of tokens to shift before error messages enabled.  */
    int yyerrstatus;

    /* The stacks and their tools:
       `yyss': related to states.
       `yyvs': related to semantic values.

       Refer to the stacks thru separate pointers, to allow yyoverflow
       to reallocate them elsewhere.  */

    /* The state stack.  */
    yytype_int16 yyssa[YYINITDEPTH];
    yytype_int16 *yyss;
    yytype_int16 *yyssp;

    /* The semantic value stack.  */
    YYSTYPE yyvsa[YYINITDEPTH];
    YYSTYPE *yyvs;
    YYSTYPE *yyvsp;

    YYSIZE_T yystacksize;

  int yyn;
  int yyresult;
  /* Lookahead token as an internal (translated) token number.  */
  int yytoken;
  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;

#if YYERROR_VERBOSE
  /* Buffer for error messages, and its allocated size.  */
  char yymsgbuf[128];
  char *yymsg = yymsgbuf;
  YYSIZE_T yymsg_alloc = sizeof yymsgbuf;
#endif

#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N))

  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  yytoken = 0;
  yyss = yyssa;
  yyvs = yyvsa;
  yystacksize = YYINITDEPTH;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yystate = 0;
  yyerrstatus = 0;
  yynerrs = 0;
  yychar = YYEMPTY; /* Cause a token to be read.  */

  /* Initialize stack pointers.
     Waste one element of value and location stack
     so that they stay on the same level as the state stack.
     The wasted elements are never initialized.  */
  yyssp = yyss;
  yyvsp = yyvs;

  goto yysetstate;

/*------------------------------------------------------------.
| yynewstate -- Push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
 yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed.  So pushing a state here evens the stacks.  */
  yyssp++;

 yysetstate:
  *yyssp = yystate;

  if (yyss + yystacksize - 1 <= yyssp)
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYSIZE_T yysize = yyssp - yyss + 1;

#ifdef yyoverflow
      {
	/* Give user a chance to reallocate the stack.  Use copies of
	   these so that the &'s don't force the real ones into
	   memory.  */
	YYSTYPE *yyvs1 = yyvs;
	yytype_int16 *yyss1 = yyss;

	/* Each stack pointer address is followed by the size of the
	   data in use in that stack, in bytes.  This used to be a
	   conditional around just the two extra args, but that might
	   be undefined if yyoverflow is a macro.  */
	yyoverflow (YY_("memory exhausted"),
		    &yyss1, yysize * sizeof (*yyssp),
		    &yyvs1, yysize * sizeof (*yyvsp),
		    &yystacksize);

	yyss = yyss1;
	yyvs = yyvs1;
      }
#else /* no yyoverflow */
# ifndef YYSTACK_RELOCATE
      goto yyexhaustedlab;
# else
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
	goto yyexhaustedlab;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
	yystacksize = YYMAXDEPTH;

      {
	yytype_int16 *yyss1 = yyss;
	union yyalloc *yyptr =
	  (union yyalloc *) YYSTACK_ALLOC (YYSTACK_BYTES (yystacksize));
	if (! yyptr)
	  goto yyexhaustedlab;
	YYSTACK_RELOCATE (yyss_alloc, yyss);
	YYSTACK_RELOCATE (yyvs_alloc, yyvs);
#  undef YYSTACK_RELOCATE
	if (yyss1 != yyssa)
	  YYSTACK_FREE (yyss1);
      }
# endif
#endif /* no yyoverflow */

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;

      YYDPRINTF ((stderr, "Stack size increased to %lu\n",
		  (unsigned long int) yystacksize));

      if (yyss + yystacksize - 1 <= yyssp)
	YYABORT;
    }

  YYDPRINTF ((stderr, "Entering state %d\n", yystate));

  if (yystate == YYFINAL)
    YYACCEPT;

  goto yybackup;

/*-----------.
| yybackup.  |
`-----------*/
yybackup:

  /* Do appropriate processing given the current state.  Read a
     lookahead token if we need one and don't already have one.  */

  /* First try to decide what to do without reference to lookahead token.  */
  yyn = yypact[yystate];
  if (yyn == YYPACT_NINF)
    goto yydefault;

  /* Not known => get a lookahead token if don't already have one.  */

  /* YYCHAR is either YYEMPTY or YYEOF or a valid lookahead symbol.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token: "));
      yychar = YYLEX;
    }

  if (yychar <= YYEOF)
    {
      yychar = yytoken = YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  else
    {
      yytoken = YYTRANSLATE (yychar);
      YY_SYMBOL_PRINT ("Next token is", yytoken, &yylval, &yylloc);
    }

  /* If the proper action on seeing token YYTOKEN is to reduce or to
     detect an error, take that action.  */
  yyn += yytoken;
  if (yyn < 0 || YYLAST < yyn || yycheck[yyn] != yytoken)
    goto yydefault;
  yyn = yytable[yyn];
  if (yyn <= 0)
    {
      if (yyn == 0 || yyn == YYTABLE_NINF)
	goto yyerrlab;
      yyn = -yyn;
      goto yyreduce;
    }

  /* Count tokens shifted since error; after three, turn off error
     status.  */
  if (yyerrstatus)
    yyerrstatus--;

  /* Shift the lookahead token.  */
  YY_SYMBOL_PRINT ("Shifting", yytoken, &yylval, &yylloc);

  /* Discard the shifted token.  */
  yychar = YYEMPTY;

  yystate = yyn;
  *++yyvsp = yylval;

  goto yynewstate;


/*-----------------------------------------------------------.
| yydefault -- do the default action for the current state.  |
`-----------------------------------------------------------*/
yydefault:
  yyn = yydefact[yystate];
  if (yyn == 0)
    goto yyerrlab;
  goto yyreduce;


/*-----------------------------.
| yyreduce -- Do a reduction.  |
`-----------------------------*/
yyreduce:
  /* yyn is the number of a rule to reduce with.  */
  yylen = yyr2[yyn];

  /* If YYLEN is nonzero, implement the default value of the action:
     `$$ = $1'.

     Otherwise, the following line sets YYVAL to garbage.
     This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
  yyval = yyvsp[1-yylen];


  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
        case 4:

/* Line 1455 of yacc.c  */
#line 182 "l2tp_config_parse.y"
    {
		;}
    break;

  case 12:

/* Line 1455 of yacc.c  */
#line 198 "l2tp_config_parse.y"
    {
			bool_t status;
			int result;
			status = l2tp_system_modify_1_svc(system_config, &result, NULL);
			if ((status != TRUE) || (result < 0))
				l2tp_log(LOG_ERR, "system: command failed: rc=%d", l2tp_strerror(-result));
		;}
    break;

  case 15:

/* Line 1455 of yacc.c  */
#line 214 "l2tp_config_parse.y"
    {
			system_config.config.flags |= L2TP_API_CONFIG_FLAG_TRACE_FLAGS;
			system_config.config.trace_flags = (yyvsp[(3) - (3)].ulnum);
		;}
    break;

  case 16:

/* Line 1455 of yacc.c  */
#line 219 "l2tp_config_parse.y"
    {
			system_config.config.flags |= L2TP_API_CONFIG_FLAG_MAX_TUNNELS;
			system_config.config.max_tunnels = (yyvsp[(3) - (3)].ulnum);
		;}
    break;

  case 17:

/* Line 1455 of yacc.c  */
#line 224 "l2tp_config_parse.y"
    {
			system_config.config.flags |= L2TP_API_CONFIG_FLAG_MAX_SESSIONS;
			system_config.config.max_sessions = (yyvsp[(3) - (3)].ulnum);
		;}
    break;

  case 18:

/* Line 1455 of yacc.c  */
#line 229 "l2tp_config_parse.y"
    {
			system_config.config.flags |= L2TP_API_CONFIG_FLAG_DRAIN_TUNNELS;
			system_config.config.drain_tunnels = (yyvsp[(3) - (3)].num);
		;}
    break;

  case 19:

/* Line 1455 of yacc.c  */
#line 234 "l2tp_config_parse.y"
    {
			system_config.config.flags |= L2TP_API_CONFIG_FLAG_TUNNEL_ESTABLISH_TIMEOUT;
			system_config.config.tunnel_establish_timeout = (yyvsp[(3) - (3)].ulnum);
		;}
    break;

  case 20:

/* Line 1455 of yacc.c  */
#line 239 "l2tp_config_parse.y"
    {
			system_config.config.flags |= L2TP_API_CONFIG_FLAG_SESSION_ESTABLISH_TIMEOUT;
			system_config.config.session_establish_timeout = (yyvsp[(3) - (3)].ulnum);
		;}
    break;

  case 21:

/* Line 1455 of yacc.c  */
#line 244 "l2tp_config_parse.y"
    {
			system_config.config.flags |= L2TP_API_CONFIG_FLAG_TUNNEL_PERSIST_PEND_TIMEOUT;
			system_config.config.tunnel_persist_pend_timeout = (yyvsp[(3) - (3)].ulnum);
		;}
    break;

  case 22:

/* Line 1455 of yacc.c  */
#line 249 "l2tp_config_parse.y"
    {
			system_config.config.flags |= L2TP_API_CONFIG_FLAG_SESSION_PERSIST_PEND_TIMEOUT;
			system_config.config.session_persist_pend_timeout = (yyvsp[(3) - (3)].ulnum);
		;}
    break;

  case 23:

/* Line 1455 of yacc.c  */
#line 254 "l2tp_config_parse.y"
    {
			system_config.config.flags |= L2TP_API_CONFIG_FLAG_DENY_LOCAL_TUNNEL_CREATES;
			system_config.config.deny_local_tunnel_creates = (yyvsp[(3) - (3)].num);
		;}
    break;

  case 24:

/* Line 1455 of yacc.c  */
#line 259 "l2tp_config_parse.y"
    {
			system_config.config.flags |= L2TP_API_CONFIG_FLAG_DENY_REMOTE_TUNNEL_CREATES;
			system_config.config.deny_remote_tunnel_creates = (yyvsp[(3) - (3)].num);
		;}
    break;

  case 27:

/* Line 1455 of yacc.c  */
#line 272 "l2tp_config_parse.y"
    {
			bool_t status;
			int result;
			if (peer_profile.profile_name == NULL)
				yyfatal("missing profile name");
			status = l2tp_peer_profile_create_1_svc(peer_profile, &result, NULL);
			if ((status != TRUE) || ((result < 0) && (result != -L2TP_ERR_PROFILE_ALREADY_EXISTS)))
				l2tp_log(LOG_ERR, "peer profile create: command failed: rc=%d", l2tp_strerror(-result));

			memset(&peer_profile, 0, sizeof(peer_profile));
		;}
    break;

  case 28:

/* Line 1455 of yacc.c  */
#line 287 "l2tp_config_parse.y"
    {
			bool_t status;
			int result;
			if (peer_profile.profile_name == NULL)
				yyfatal("missing profile name");
			status = l2tp_peer_profile_modify_1_svc(peer_profile, &result, NULL);
			if ((status != TRUE) || (result < 0))
				l2tp_log(LOG_ERR, "peer profile modify: command failed: rc=%d", l2tp_strerror(-result));

			memset(&peer_profile, 0, sizeof(peer_profile));
		;}
    break;

  case 31:

/* Line 1455 of yacc.c  */
#line 307 "l2tp_config_parse.y"
    {
			peer_profile.profile_name = (yyvsp[(3) - (3)].val).buf;
		;}
    break;

  case 32:

/* Line 1455 of yacc.c  */
#line 311 "l2tp_config_parse.y"
    {
			peer_profile.profile_name = (yyvsp[(3) - (3)].val).buf;
		;}
    break;

  case 33:

/* Line 1455 of yacc.c  */
#line 315 "l2tp_config_parse.y"
    {
			struct in_addr addr;
			addr.s_addr = htonl((yyvsp[(3) - (3)].ulnum));
			peer_profile.flags |= L2TP_API_PEER_PROFILE_FLAG_PEER_IPADDR;
			peer_profile.peer_addr.s_addr = addr.s_addr;
		;}
    break;

  case 34:

/* Line 1455 of yacc.c  */
#line 322 "l2tp_config_parse.y"
    {
			peer_profile.flags |= L2TP_API_PEER_PROFILE_FLAG_PEER_PORT;
			peer_profile.peer_port = (yyvsp[(3) - (3)].ulnum);
		;}
    break;

  case 35:

/* Line 1455 of yacc.c  */
#line 327 "l2tp_config_parse.y"
    {
			struct in_addr addr;
			addr.s_addr = htonl((yyvsp[(3) - (3)].ulnum));
			peer_profile.flags |= L2TP_API_PEER_PROFILE_FLAG_NETMASK;
			peer_profile.netmask.s_addr = addr.s_addr;
		;}
    break;

  case 36:

/* Line 1455 of yacc.c  */
#line 334 "l2tp_config_parse.y"
    {
			peer_profile.flags |= L2TP_API_PEER_PROFILE_FLAG_LACLNS;
			if (strcasecmp((yyvsp[(3) - (3)].val).buf, "lac") == 0) {
				peer_profile.we_can_be_lac = 1;
				peer_profile.we_can_be_lns = 0;
			} else if (strcasecmp((yyvsp[(3) - (3)].val).buf, "lns") == 0) {
				peer_profile.we_can_be_lac = 0;
				peer_profile.we_can_be_lns = 1;
			} else if ((strcasecmp((yyvsp[(3) - (3)].val).buf, "laclns") == 0) ||
				   (strcasecmp((yyvsp[(3) - (3)].val).buf, "lnslac") == 0)) {
				peer_profile.we_can_be_lac = 1;
				peer_profile.we_can_be_lns = 1;
			} else {
				yyfatal("invalid lac_lns value");
			}
		;}
    break;

  case 37:

/* Line 1455 of yacc.c  */
#line 351 "l2tp_config_parse.y"
    {
			peer_profile.flags |= L2TP_API_PEER_PROFILE_FLAG_TUNNEL_PROFILE_NAME;
			peer_profile.default_tunnel_profile_name.optstring_u.value = (yyvsp[(3) - (3)].val).buf;
			peer_profile.default_tunnel_profile_name.valid = 1;
		;}
    break;

  case 38:

/* Line 1455 of yacc.c  */
#line 357 "l2tp_config_parse.y"
    {
			peer_profile.flags |= L2TP_API_PEER_PROFILE_FLAG_TUNNEL_PROFILE_NAME;
			peer_profile.default_tunnel_profile_name.optstring_u.value = (yyvsp[(3) - (3)].val).buf;
			peer_profile.default_tunnel_profile_name.valid = 1;
		;}
    break;

  case 39:

/* Line 1455 of yacc.c  */
#line 363 "l2tp_config_parse.y"
    {
			peer_profile.flags |= L2TP_API_PEER_PROFILE_FLAG_SESSION_PROFILE_NAME;
			peer_profile.default_session_profile_name.optstring_u.value = (yyvsp[(3) - (3)].val).buf;
			peer_profile.default_session_profile_name.valid = 1;
		;}
    break;

  case 40:

/* Line 1455 of yacc.c  */
#line 369 "l2tp_config_parse.y"
    {
			peer_profile.flags |= L2TP_API_PEER_PROFILE_FLAG_SESSION_PROFILE_NAME;
			peer_profile.default_session_profile_name.optstring_u.value = (yyvsp[(3) - (3)].val).buf;
			peer_profile.default_session_profile_name.valid = 1;
		;}
    break;

  case 41:

/* Line 1455 of yacc.c  */
#line 375 "l2tp_config_parse.y"
    {
			peer_profile.flags |= L2TP_API_PEER_PROFILE_FLAG_PPP_PROFILE_NAME;
			peer_profile.default_ppp_profile_name.optstring_u.value = (yyvsp[(3) - (3)].val).buf;
			peer_profile.default_ppp_profile_name.valid = 1;
		;}
    break;

  case 42:

/* Line 1455 of yacc.c  */
#line 381 "l2tp_config_parse.y"
    {
			peer_profile.flags |= L2TP_API_PEER_PROFILE_FLAG_PPP_PROFILE_NAME;
			peer_profile.default_ppp_profile_name.optstring_u.value = (yyvsp[(3) - (3)].val).buf;
			peer_profile.default_ppp_profile_name.valid = 1;
		;}
    break;

  case 45:

/* Line 1455 of yacc.c  */
#line 395 "l2tp_config_parse.y"
    {
			bool_t status;
			int result;
			if (tunnel_profile.profile_name == NULL)
				yyfatal("missing profile name");
			status = l2tp_tunnel_profile_create_1_svc(tunnel_profile, &result, NULL);
			if ((status != TRUE) || ((result < 0) && (result != -L2TP_ERR_PROFILE_ALREADY_EXISTS)))
				l2tp_log(LOG_ERR, "tunnel profile create: command failed: rc=%d", l2tp_strerror(-result));

			memset(&tunnel_profile, 0, sizeof(tunnel_profile));
		;}
    break;

  case 46:

/* Line 1455 of yacc.c  */
#line 410 "l2tp_config_parse.y"
    {
			bool_t status;
			int result;
			if (tunnel_profile.profile_name == NULL)
				yyfatal("missing profile name");
			status = l2tp_tunnel_profile_modify_1_svc(tunnel_profile, &result, NULL);
			if ((status != TRUE) || (result < 0))
				l2tp_log(LOG_ERR, "tunnel profile modify: command failed: rc=%d", l2tp_strerror(-result));

			memset(&tunnel_profile, 0, sizeof(tunnel_profile));
		;}
    break;

  case 49:

/* Line 1455 of yacc.c  */
#line 430 "l2tp_config_parse.y"
    {
			tunnel_profile.profile_name = (yyvsp[(3) - (3)].val).buf;
		;}
    break;

  case 50:

/* Line 1455 of yacc.c  */
#line 434 "l2tp_config_parse.y"
    {
			tunnel_profile.profile_name = (yyvsp[(3) - (3)].val).buf;
		;}
    break;

  case 51:

/* Line 1455 of yacc.c  */
#line 438 "l2tp_config_parse.y"
    {
			tunnel_profile.flags |= L2TP_API_TUNNEL_PROFILE_FLAG_HIDE_AVPS;
			tunnel_profile.hide_avps = (yyvsp[(3) - (3)].num);
		;}
    break;

  case 52:

/* Line 1455 of yacc.c  */
#line 443 "l2tp_config_parse.y"
    {
			tunnel_profile.flags |= L2TP_API_TUNNEL_PROFILE_FLAG_AUTH_MODE;
			if (strcasecmp((yyvsp[(3) - (3)].val).buf, "none") == 0) {
				tunnel_profile.auth_mode = L2TP_API_TUNNEL_AUTH_MODE_NONE;
			} else if (strcasecmp((yyvsp[(3) - (3)].val).buf, "simple") == 0) {
				tunnel_profile.auth_mode = L2TP_API_TUNNEL_AUTH_MODE_SIMPLE;
			} else if (strcasecmp((yyvsp[(3) - (3)].val).buf, "challenge") == 0) {
				tunnel_profile.auth_mode = L2TP_API_TUNNEL_AUTH_MODE_CHALLENGE;
			} else {
				yyfatal("invalid auth_mode");
			}
		;}
    break;

  case 53:

/* Line 1455 of yacc.c  */
#line 456 "l2tp_config_parse.y"
    {
			tunnel_profile.flags |= L2TP_API_TUNNEL_PROFILE_FLAG_FRAMING_CAP;
			if (strcasecmp((yyvsp[(3) - (3)].val).buf, "sync") == 0) {
				tunnel_profile.framing_cap_sync = 1;
				tunnel_profile.framing_cap_async = 0;
			} else if (strcasecmp((yyvsp[(3) - (3)].val).buf, "async") == 0) {
				tunnel_profile.framing_cap_sync = 0;
				tunnel_profile.framing_cap_async = 1;
			} else if (strcasecmp((yyvsp[(3) - (3)].val).buf, "any") == 0) {
				tunnel_profile.framing_cap_sync = 1;
				tunnel_profile.framing_cap_async = 1;
			} else {
				yyfatal("invalid framing_cap");
			}
		;}
    break;

  case 54:

/* Line 1455 of yacc.c  */
#line 472 "l2tp_config_parse.y"
    {
			tunnel_profile.flags |= L2TP_API_TUNNEL_PROFILE_FLAG_BEARER_CAP;
			if (strcasecmp((yyvsp[(3) - (3)].val).buf, "analog") == 0) {
				tunnel_profile.bearer_cap_analog = 1;
				tunnel_profile.bearer_cap_digital = 0;
			} else if (strcasecmp((yyvsp[(3) - (3)].val).buf, "digital") == 0) {
				tunnel_profile.bearer_cap_analog = 0;
				tunnel_profile.bearer_cap_digital = 1;
			} else if (strcasecmp((yyvsp[(3) - (3)].val).buf, "any") == 0) {
				tunnel_profile.bearer_cap_analog = 1;
				tunnel_profile.bearer_cap_digital = 1;
			} else {
				yyfatal("invalid bearer_cap");
			}
		;}
    break;

  case 55:

/* Line 1455 of yacc.c  */
#line 488 "l2tp_config_parse.y"
    {
			tunnel_profile.flags |= L2TP_API_TUNNEL_PROFILE_FLAG_USE_TIEBREAKER;
			tunnel_profile.use_tiebreaker = (yyvsp[(3) - (3)].num);
		;}
    break;

  case 56:

/* Line 1455 of yacc.c  */
#line 493 "l2tp_config_parse.y"
    {
			tunnel_profile.flags |= L2TP_API_TUNNEL_PROFILE_FLAG_HELLO_TIMEOUT;
			tunnel_profile.hello_timeout = (yyvsp[(3) - (3)].ulnum);
		;}
    break;

  case 57:

/* Line 1455 of yacc.c  */
#line 498 "l2tp_config_parse.y"
    {
			tunnel_profile.flags |= L2TP_API_TUNNEL_PROFILE_FLAG_MAX_RETRIES;
			tunnel_profile.max_retries = (yyvsp[(3) - (3)].ulnum);
		;}
    break;

  case 58:

/* Line 1455 of yacc.c  */
#line 503 "l2tp_config_parse.y"
    {
			tunnel_profile.flags |= L2TP_API_TUNNEL_PROFILE_FLAG_RX_WINDOW_SIZE;
			tunnel_profile.rx_window_size = (yyvsp[(3) - (3)].ulnum);
		;}
    break;

  case 59:

/* Line 1455 of yacc.c  */
#line 508 "l2tp_config_parse.y"
    {
			tunnel_profile.flags |= L2TP_API_TUNNEL_PROFILE_FLAG_TX_WINDOW_SIZE;
			tunnel_profile.tx_window_size = (yyvsp[(3) - (3)].ulnum);
		;}
    break;

  case 60:

/* Line 1455 of yacc.c  */
#line 513 "l2tp_config_parse.y"
    {
			tunnel_profile.flags |= L2TP_API_TUNNEL_PROFILE_FLAG_RETRY_TIMEOUT;
			tunnel_profile.retry_timeout = (yyvsp[(3) - (3)].ulnum);
		;}
    break;

  case 61:

/* Line 1455 of yacc.c  */
#line 518 "l2tp_config_parse.y"
    {
			tunnel_profile.flags |= L2TP_API_TUNNEL_PROFILE_FLAG_IDLE_TIMEOUT;
			tunnel_profile.idle_timeout = (yyvsp[(3) - (3)].ulnum);
		;}
    break;

  case 62:

/* Line 1455 of yacc.c  */
#line 523 "l2tp_config_parse.y"
    {
			tunnel_profile.flags |= L2TP_API_TUNNEL_PROFILE_FLAG_SECRET;
			tunnel_profile.secret.optstring_u.value = (yyvsp[(3) - (3)].val).buf;
			tunnel_profile.secret.valid = 1;
		;}
    break;

  case 63:

/* Line 1455 of yacc.c  */
#line 529 "l2tp_config_parse.y"
    {
			tunnel_profile.flags |= L2TP_API_TUNNEL_PROFILE_FLAG_SECRET;
			tunnel_profile.secret.optstring_u.value = (yyvsp[(3) - (3)].val).buf;
			tunnel_profile.secret.valid = 1;
		;}
    break;

  case 64:

/* Line 1455 of yacc.c  */
#line 535 "l2tp_config_parse.y"
    {
			tunnel_profile.flags |= L2TP_API_TUNNEL_PROFILE_FLAG_ALLOW_PPP_PROXY;
			tunnel_profile.allow_ppp_proxy = (yyvsp[(3) - (3)].num);
		;}
    break;

  case 65:

/* Line 1455 of yacc.c  */
#line 540 "l2tp_config_parse.y"
    {
			tunnel_profile.flags |= L2TP_API_TUNNEL_PROFILE_FLAG_TRACE_FLAGS;
			tunnel_profile.trace_flags = (yyvsp[(3) - (3)].ulnum);
		;}
    break;

  case 66:

/* Line 1455 of yacc.c  */
#line 545 "l2tp_config_parse.y"
    {
			tunnel_profile.flags |= L2TP_API_TUNNEL_PROFILE_FLAG_USE_UDP_CHECKSUMS;
			tunnel_profile.use_udp_checksums = (yyvsp[(3) - (3)].num);
		;}
    break;

  case 67:

/* Line 1455 of yacc.c  */
#line 550 "l2tp_config_parse.y"
    {
			tunnel_profile.flags |= L2TP_API_TUNNEL_PROFILE_FLAG_HOST_NAME;
			tunnel_profile.host_name.optstring_u.value = (yyvsp[(3) - (3)].val).buf;
			tunnel_profile.host_name.valid = 1;
		;}
    break;

  case 68:

/* Line 1455 of yacc.c  */
#line 556 "l2tp_config_parse.y"
    {
			tunnel_profile.flags |= L2TP_API_TUNNEL_PROFILE_FLAG_HOST_NAME;
			tunnel_profile.host_name.optstring_u.value = (yyvsp[(3) - (3)].val).buf;
			tunnel_profile.host_name.valid = 1;
		;}
    break;

  case 69:

/* Line 1455 of yacc.c  */
#line 562 "l2tp_config_parse.y"
    {
			tunnel_profile.flags |= L2TP_API_TUNNEL_PROFILE_FLAG_MAX_SESSIONS;
			tunnel_profile.max_sessions = (yyvsp[(3) - (3)].ulnum);
		;}
    break;

  case 70:

/* Line 1455 of yacc.c  */
#line 567 "l2tp_config_parse.y"
    {
			struct in_addr addr;
			tunnel_profile.flags |= L2TP_API_TUNNEL_PROFILE_FLAG_OUR_ADDR;
			addr.s_addr = htonl((yyvsp[(3) - (3)].ulnum));
			tunnel_profile.our_addr.s_addr = addr.s_addr;
		;}
    break;

  case 71:

/* Line 1455 of yacc.c  */
#line 574 "l2tp_config_parse.y"
    {
			struct in_addr addr;
			tunnel_profile.flags |= L2TP_API_TUNNEL_PROFILE_FLAG_PEER_ADDR;
			addr.s_addr = htonl((yyvsp[(3) - (3)].ulnum));
			tunnel_profile.peer_addr.s_addr = addr.s_addr;
		;}
    break;

  case 72:

/* Line 1455 of yacc.c  */
#line 581 "l2tp_config_parse.y"
    {
			tunnel_profile.flags |= L2TP_API_TUNNEL_PROFILE_FLAG_OUR_UDP_PORT;
			tunnel_profile.our_udp_port = (yyvsp[(3) - (3)].ulnum);
		;}
    break;

  case 73:

/* Line 1455 of yacc.c  */
#line 586 "l2tp_config_parse.y"
    {
			tunnel_profile.flags |= L2TP_API_TUNNEL_PROFILE_FLAG_PEER_UDP_PORT;
			tunnel_profile.peer_udp_port = (yyvsp[(3) - (3)].ulnum);
		;}
    break;

  case 74:

/* Line 1455 of yacc.c  */
#line 591 "l2tp_config_parse.y"
    {
			tunnel_profile.flags |= L2TP_API_TUNNEL_PROFILE_FLAG_PEER_PROFILE_NAME;
			tunnel_profile.peer_profile_name.optstring_u.value = (yyvsp[(3) - (3)].val).buf;
			tunnel_profile.peer_profile_name.valid = 1;
		;}
    break;

  case 75:

/* Line 1455 of yacc.c  */
#line 597 "l2tp_config_parse.y"
    {
			tunnel_profile.flags |= L2TP_API_TUNNEL_PROFILE_FLAG_PEER_PROFILE_NAME;
			tunnel_profile.peer_profile_name.optstring_u.value = (yyvsp[(3) - (3)].val).buf;
			tunnel_profile.peer_profile_name.valid = 1;
		;}
    break;

  case 76:

/* Line 1455 of yacc.c  */
#line 603 "l2tp_config_parse.y"
    {
			tunnel_profile.flags |= L2TP_API_TUNNEL_PROFILE_FLAG_SESSION_PROFILE_NAME;
			tunnel_profile.session_profile_name.optstring_u.value = (yyvsp[(3) - (3)].val).buf;
			tunnel_profile.session_profile_name.valid = 1;
		;}
    break;

  case 77:

/* Line 1455 of yacc.c  */
#line 609 "l2tp_config_parse.y"
    {
			tunnel_profile.flags |= L2TP_API_TUNNEL_PROFILE_FLAG_SESSION_PROFILE_NAME;
			tunnel_profile.session_profile_name.optstring_u.value = (yyvsp[(3) - (3)].val).buf;
			tunnel_profile.session_profile_name.valid = 1;
		;}
    break;

  case 78:

/* Line 1455 of yacc.c  */
#line 615 "l2tp_config_parse.y"
    {
			tunnel_profile.flags |= L2TP_API_TUNNEL_PROFILE_FLAG_PPP_PROFILE_NAME;
			tunnel_profile.ppp_profile_name.optstring_u.value = (yyvsp[(3) - (3)].val).buf;
			tunnel_profile.ppp_profile_name.valid = 1;
		;}
    break;

  case 79:

/* Line 1455 of yacc.c  */
#line 621 "l2tp_config_parse.y"
    {
			tunnel_profile.flags |= L2TP_API_TUNNEL_PROFILE_FLAG_PPP_PROFILE_NAME;
			tunnel_profile.ppp_profile_name.optstring_u.value = (yyvsp[(3) - (3)].val).buf;
			tunnel_profile.ppp_profile_name.valid = 1;
		;}
    break;

  case 80:

/* Line 1455 of yacc.c  */
#line 627 "l2tp_config_parse.y"
    {
			tunnel_profile.flags |= L2TP_API_TUNNEL_PROFILE_FLAG_DO_PMTU_DISCOVERY;
			tunnel_profile.do_pmtu_discovery = (yyvsp[(3) - (3)].num);
		;}
    break;

  case 81:

/* Line 1455 of yacc.c  */
#line 632 "l2tp_config_parse.y"
    {
			tunnel_profile.flags |= L2TP_API_TUNNEL_PROFILE_FLAG_MTU;
			tunnel_profile.mtu = (yyvsp[(3) - (3)].ulnum);
		;}
    break;

  case 84:

/* Line 1455 of yacc.c  */
#line 645 "l2tp_config_parse.y"
    {
			bool_t status;
			int result;
			if (session_profile.profile_name == NULL)
				yyfatal("missing profile name");
			status = l2tp_session_profile_create_1_svc(session_profile, &result, NULL);
			if ((status != TRUE) || ((result < 0) && (result != -L2TP_ERR_PROFILE_ALREADY_EXISTS)))
				l2tp_log(LOG_ERR, "session profile create: command failed: rc=%d", l2tp_strerror(-result));

			memset(&session_profile, 0, sizeof(session_profile));
		;}
    break;

  case 85:

/* Line 1455 of yacc.c  */
#line 660 "l2tp_config_parse.y"
    {
			bool_t status;
			int result;
			if (session_profile.profile_name == NULL)
				yyfatal("missing profile name");
			status = l2tp_session_profile_modify_1_svc(session_profile, &result, NULL);
			if ((status != TRUE) || (result < 0))
				l2tp_log(LOG_ERR, "session profile modify: command failed: rc=%d", l2tp_strerror(-result));

			memset(&session_profile, 0, sizeof(session_profile));
		;}
    break;

  case 88:

/* Line 1455 of yacc.c  */
#line 680 "l2tp_config_parse.y"
    {
			session_profile.profile_name = (yyvsp[(3) - (3)].val).buf;
		;}
    break;

  case 89:

/* Line 1455 of yacc.c  */
#line 684 "l2tp_config_parse.y"
    {
			session_profile.profile_name = (yyvsp[(3) - (3)].val).buf;
		;}
    break;

  case 90:

/* Line 1455 of yacc.c  */
#line 688 "l2tp_config_parse.y"
    {
			session_profile.flags |= L2TP_API_SESSION_PROFILE_FLAG_TRACE_FLAGS;
			session_profile.trace_flags = (yyvsp[(3) - (3)].ulnum);
		;}
    break;

  case 91:

/* Line 1455 of yacc.c  */
#line 693 "l2tp_config_parse.y"
    {
			session_profile.flags |= L2TP_API_SESSION_PROFILE_FLAG_SEQUENCING_REQUIRED;
			session_profile.sequencing_required = (yyvsp[(3) - (3)].num);
		;}
    break;

  case 92:

/* Line 1455 of yacc.c  */
#line 698 "l2tp_config_parse.y"
    {
			session_profile.flags |= L2TP_API_SESSION_PROFILE_FLAG_PPP_PROFILE_NAME;
			OPTSTRING(session_profile.ppp_profile_name) = (yyvsp[(3) - (3)].val).buf;
			session_profile.ppp_profile_name.valid = 1;
		;}
    break;

  case 93:

/* Line 1455 of yacc.c  */
#line 704 "l2tp_config_parse.y"
    {
			session_profile.flags |= L2TP_API_SESSION_PROFILE_FLAG_PPP_PROFILE_NAME;
			OPTSTRING(session_profile.ppp_profile_name) = (yyvsp[(3) - (3)].val).buf;
			session_profile.ppp_profile_name.valid = 1;
		;}
    break;

  case 94:

/* Line 1455 of yacc.c  */
#line 710 "l2tp_config_parse.y"
    {
			session_profile.flags |= L2TP_API_SESSION_PROFILE_FLAG_SESSION_TYPE;
			session_profile.flags |= L2TP_API_SESSION_FLAG_SESSION_TYPE;
			if (strcasecmp((yyvsp[(3) - (3)].val).buf, "laic") == 0) {
				session_profile.session_type = L2TP_API_SESSION_TYPE_LAIC;
			} else if (strcasecmp((yyvsp[(3) - (3)].val).buf, "laoc") == 0) {
				session_profile.session_type = L2TP_API_SESSION_TYPE_LAOC;
			} else if (strcasecmp((yyvsp[(3) - (3)].val).buf, "lnic") == 0) {
				session_profile.session_type = L2TP_API_SESSION_TYPE_LNIC;
			} else if (strcasecmp((yyvsp[(3) - (3)].val).buf, "lnoc") == 0) {
				session_profile.session_type = L2TP_API_SESSION_TYPE_LNOC;
			} else {
				yyfatal("Bad session type: expecting laic|laoc|lnic|lnoc");
			}
		;}
    break;

  case 95:

/* Line 1455 of yacc.c  */
#line 726 "l2tp_config_parse.y"
    {
			session_profile.flags |= L2TP_API_SESSION_PROFILE_FLAG_PRIV_GROUP_ID;
			OPTSTRING(session_profile.priv_group_id) = (yyvsp[(3) - (3)].val).buf;
			session_profile.priv_group_id.valid = 1;
		;}
    break;

  case 96:

/* Line 1455 of yacc.c  */
#line 732 "l2tp_config_parse.y"
    {
			session_profile.flags |= L2TP_API_SESSION_PROFILE_FLAG_PRIV_GROUP_ID;
			OPTSTRING(session_profile.priv_group_id) = (yyvsp[(3) - (3)].val).buf;
			session_profile.priv_group_id.valid = 1;
		;}
    break;

  case 97:

/* Line 1455 of yacc.c  */
#line 738 "l2tp_config_parse.y"
    {
			session_profile.flags |= L2TP_API_SESSION_PROFILE_FLAG_FRAMING_TYPE;
			session_profile.flags |= L2TP_API_SESSION_FLAG_FRAMING_TYPE;
			if (strcasecmp((yyvsp[(3) - (3)].val).buf, "sync") == 0) {
				session_profile.framing_type_sync = 1;
				session_profile.framing_type_async = 0;
			} else if (strcasecmp((yyvsp[(3) - (3)].val).buf, "async") == 0) {
				session_profile.framing_type_sync = 0;
				session_profile.framing_type_async = 1;
			} else if (strcasecmp((yyvsp[(3) - (3)].val).buf, "any") == 0) {
				session_profile.framing_type_sync = 1;
				session_profile.framing_type_async = 1;
			} else {
				yyfatal("invalid framing_type");
			}
		;}
    break;

  case 98:

/* Line 1455 of yacc.c  */
#line 755 "l2tp_config_parse.y"
    {
			session_profile.flags |= L2TP_API_SESSION_PROFILE_FLAG_BEARER_TYPE;
			session_profile.flags |= L2TP_API_SESSION_FLAG_BEARER_TYPE;
			if (strcasecmp((yyvsp[(3) - (3)].val).buf, "analog") == 0) {
				session_profile.bearer_type_analog = 1;
				session_profile.bearer_type_digital = 0;
			} else if (strcasecmp((yyvsp[(3) - (3)].val).buf, "digital") == 0) {
				session_profile.bearer_type_analog = 0;
				session_profile.bearer_type_digital = 1;
			} else if (strcasecmp((yyvsp[(3) - (3)].val).buf, "any") == 0) {
				session_profile.bearer_type_analog = 1;
				session_profile.bearer_type_digital = 1;
			} else {
				yyfatal("invalid bearer_type");
			}
		;}
    break;

  case 99:

/* Line 1455 of yacc.c  */
#line 772 "l2tp_config_parse.y"
    {
			session_profile.flags |= L2TP_API_SESSION_PROFILE_FLAG_MINIMUM_BPS;
			session_profile.minimum_bps = (yyvsp[(3) - (3)].ulnum);
		;}
    break;

  case 100:

/* Line 1455 of yacc.c  */
#line 777 "l2tp_config_parse.y"
    {
			session_profile.flags |= L2TP_API_SESSION_PROFILE_FLAG_MAXIMUM_BPS;
			session_profile.maximum_bps = (yyvsp[(3) - (3)].ulnum);
		;}
    break;

  case 101:

/* Line 1455 of yacc.c  */
#line 782 "l2tp_config_parse.y"
    {
			session_profile.flags |= L2TP_API_SESSION_PROFILE_FLAG_CONNECT_SPEED;
			int ints[2];
			int num_matches;
			session_profile.flags |= L2TP_API_SESSION_FLAG_CONNECT_SPEED;
			num_matches = sscanf((yyvsp[(3) - (3)].val).buf, "%d:%d", &ints[0], &ints[1]);
			if (num_matches >= 1) {
				session_profile.rx_connect_speed = ints[0];
				session_profile.tx_connect_speed = ints[0];
				if (num_matches == 2) {
					session_profile.tx_connect_speed = ints[1];
				}
			} else {
				yyfatal("Expecting connect_speed[:tx_connect_speed]");
			}
		;}
    break;

  case 102:

/* Line 1455 of yacc.c  */
#line 799 "l2tp_config_parse.y"
    {
			session_profile.flags |= L2TP_API_SESSION_PROFILE_FLAG_USE_PPP_PROXY;
			session_profile.use_ppp_proxy = (yyvsp[(3) - (3)].num);
		;}
    break;

  case 103:

/* Line 1455 of yacc.c  */
#line 804 "l2tp_config_parse.y"
    {
			session_profile.flags |= L2TP_API_SESSION_PROFILE_FLAG_USE_SEQUENCE_NUMBERS;
			session_profile.use_sequence_numbers = (yyvsp[(3) - (3)].num);
		;}
    break;

  case 104:

/* Line 1455 of yacc.c  */
#line 809 "l2tp_config_parse.y"
    {
			session_profile.flags |= L2TP_API_SESSION_PROFILE_FLAG_NO_PPP;
			session_profile.no_ppp = (yyvsp[(3) - (3)].num);
		;}
    break;

  case 105:

/* Line 1455 of yacc.c  */
#line 814 "l2tp_config_parse.y"
    {
			session_profile.flags |= L2TP_API_SESSION_PROFILE_FLAG_REORDER_TIMEOUT;
			session_profile.reorder_timeout = (yyvsp[(3) - (3)].ulnum);
		;}
    break;

  case 108:

/* Line 1455 of yacc.c  */
#line 827 "l2tp_config_parse.y"
    {
			bool_t status;
			int result;
			if (ppp_profile.profile_name == NULL)
				yyfatal("missing profile name");
			status = l2tp_ppp_profile_create_1_svc(ppp_profile, &result, NULL);
			if ((status != TRUE) || ((result < 0) && (result != -L2TP_ERR_PROFILE_ALREADY_EXISTS)))
				l2tp_log(LOG_ERR, "ppp profile create: command failed: rc=%d", l2tp_strerror(-result));

			memset(&ppp_profile, 0, sizeof(ppp_profile));
		;}
    break;

  case 109:

/* Line 1455 of yacc.c  */
#line 842 "l2tp_config_parse.y"
    {
			bool_t status;
			int result;
			if (ppp_profile.profile_name == NULL)
				yyfatal("missing profile name");
			status = l2tp_ppp_profile_modify_1_svc(ppp_profile, &result, NULL);
			if ((status != TRUE) || (result < 0))
				l2tp_log(LOG_ERR, "ppp profile modify: command failed: rc=%d", l2tp_strerror(-result));

			memset(&ppp_profile, 0, sizeof(ppp_profile));
		;}
    break;

  case 112:

/* Line 1455 of yacc.c  */
#line 862 "l2tp_config_parse.y"
    {
			ppp_profile.profile_name = (yyvsp[(3) - (3)].val).buf;
		;}
    break;

  case 113:

/* Line 1455 of yacc.c  */
#line 866 "l2tp_config_parse.y"
    {
			ppp_profile.profile_name = (yyvsp[(3) - (3)].val).buf;
		;}
    break;

  case 114:

/* Line 1455 of yacc.c  */
#line 870 "l2tp_config_parse.y"
    {
			ppp_profile.flags |= L2TP_API_PPP_PROFILE_FLAG_TRACE_FLAGS;
			ppp_profile.trace_flags = (yyvsp[(3) - (3)].ulnum);
		;}
    break;

  case 115:

/* Line 1455 of yacc.c  */
#line 875 "l2tp_config_parse.y"
    {
			ppp_profile.flags |= L2TP_API_PPP_PROFILE_FLAG_ASYNCMAP;
			ppp_profile.asyncmap = (yyvsp[(3) - (3)].ulnum);
		;}
    break;

  case 116:

/* Line 1455 of yacc.c  */
#line 880 "l2tp_config_parse.y"
    {
			ppp_profile.flags |= L2TP_API_PPP_PROFILE_FLAG_MRU;
			ppp_profile.mru = (yyvsp[(3) - (3)].ulnum);
		;}
    break;

  case 117:

/* Line 1455 of yacc.c  */
#line 885 "l2tp_config_parse.y"
    {
			ppp_profile.flags |= L2TP_API_PPP_PROFILE_FLAG_MTU;
			ppp_profile.mtu = (yyvsp[(3) - (3)].ulnum);
		;}
    break;

  case 118:

/* Line 1455 of yacc.c  */
#line 890 "l2tp_config_parse.y"
    {
			ppp_profile.flags |= L2TP_API_PPP_PROFILE_FLAG_USE_RADIUS;
			ppp_profile.use_radius = (yyvsp[(3) - (3)].num);
		;}
    break;

  case 119:

/* Line 1455 of yacc.c  */
#line 895 "l2tp_config_parse.y"
    {
			ppp_profile.flags |= L2TP_API_PPP_PROFILE_FLAG_RADIUS_HINT;
			OPTSTRING(ppp_profile.radius_hint) = (yyvsp[(3) - (3)].val).buf;
			ppp_profile.radius_hint.valid = 1;
		;}
    break;

  case 120:

/* Line 1455 of yacc.c  */
#line 901 "l2tp_config_parse.y"
    {
			ppp_profile.flags |= L2TP_API_PPP_PROFILE_FLAG_RADIUS_HINT;
			OPTSTRING(ppp_profile.radius_hint) = (yyvsp[(3) - (3)].val).buf;
			ppp_profile.radius_hint.valid = 1;
		;}
    break;

  case 121:

/* Line 1455 of yacc.c  */
#line 907 "l2tp_config_parse.y"
    {
			ppp_profile.flags2 |= L2TP_API_PPP_PROFILE_FLAG_AUTH_REFUSE_PAP;
			if ((yyvsp[(3) - (3)].num)) {
				ppp_profile.auth_refuse_pap = 0;
			} else {
				ppp_profile.auth_refuse_pap = -1;
			}
		;}
    break;

  case 122:

/* Line 1455 of yacc.c  */
#line 916 "l2tp_config_parse.y"
    {
			ppp_profile.flags2 |= L2TP_API_PPP_PROFILE_FLAG_AUTH_REFUSE_CHAP;
			if ((yyvsp[(3) - (3)].num)) {
				ppp_profile.auth_refuse_chap = 0;
			} else {
				ppp_profile.auth_refuse_chap = -1;
			}
		;}
    break;

  case 123:

/* Line 1455 of yacc.c  */
#line 925 "l2tp_config_parse.y"
    {
			ppp_profile.flags2 |= L2TP_API_PPP_PROFILE_FLAG_AUTH_REFUSE_MSCHAP;
			if ((yyvsp[(3) - (3)].num)) {
				ppp_profile.auth_refuse_mschap = 0;
			} else {
				ppp_profile.auth_refuse_mschap = -1;
			}
		;}
    break;

  case 124:

/* Line 1455 of yacc.c  */
#line 934 "l2tp_config_parse.y"
    {
			ppp_profile.flags2 |= L2TP_API_PPP_PROFILE_FLAG_AUTH_REFUSE_MSCHAPV2;
			if ((yyvsp[(3) - (3)].num)) {
				ppp_profile.auth_refuse_mschapv2 = 0;
			} else {
				ppp_profile.auth_refuse_mschapv2 = -1;
			}
		;}
    break;

  case 125:

/* Line 1455 of yacc.c  */
#line 943 "l2tp_config_parse.y"
    {
			ppp_profile.flags2 |= L2TP_API_PPP_PROFILE_FLAG_AUTH_REFUSE_EAP;
			if ((yyvsp[(3) - (3)].num)) {
				ppp_profile.auth_refuse_eap = 0;
			} else {
				ppp_profile.auth_refuse_eap = -1;
			}
		;}
    break;

  case 126:

/* Line 1455 of yacc.c  */
#line 952 "l2tp_config_parse.y"
    {
			ppp_profile.flags2 |= L2TP_API_PPP_PROFILE_FLAG_AUTH_NONE;
			if ((yyvsp[(3) - (3)].num)) {
				ppp_profile.auth_none = -1;
			} else {
				ppp_profile.auth_none = 0;
			}
		;}
    break;

  case 127:

/* Line 1455 of yacc.c  */
#line 961 "l2tp_config_parse.y"
    {
			ppp_profile.flags2 |= L2TP_API_PPP_PROFILE_FLAG_AUTH_PEER;
			if ((yyvsp[(3) - (3)].num)) {
				ppp_profile.auth_peer = -1;
			} else {
				ppp_profile.auth_peer = 0;
			}
		;}
    break;

  case 128:

/* Line 1455 of yacc.c  */
#line 970 "l2tp_config_parse.y"
    {
			ppp_profile.flags |= L2TP_API_PPP_PROFILE_FLAG_SYNC_MODE;
			if (strcasecmp((yyvsp[(3) - (3)].val).buf, "sync") == 0) {
				ppp_profile.sync_mode = L2TP_API_PPP_SYNCMODE_SYNC;
			} else if (strcasecmp((yyvsp[(3) - (3)].val).buf, "async") == 0) {
				ppp_profile.sync_mode = L2TP_API_PPP_SYNCMODE_ASYNC;
			} else if (strcasecmp((yyvsp[(3) - (3)].val).buf, "any") == 0) {
				ppp_profile.sync_mode = L2TP_API_PPP_SYNCMODE_SYNC_ASYNC;
			} else {
				yyfatal("Bad sync mode: expecting sync|async|any");
			}
		;}
    break;

  case 129:

/* Line 1455 of yacc.c  */
#line 983 "l2tp_config_parse.y"
    {
			ppp_profile.flags |= L2TP_API_PPP_PROFILE_FLAG_CHAP_INTERVAL;
			ppp_profile.chap_interval = (yyvsp[(3) - (3)].ulnum);
		;}
    break;

  case 130:

/* Line 1455 of yacc.c  */
#line 988 "l2tp_config_parse.y"
    {
			ppp_profile.flags |= L2TP_API_PPP_PROFILE_FLAG_CHAP_MAX_CHALLENGE;
			ppp_profile.chap_max_challenge = (yyvsp[(3) - (3)].ulnum);
		;}
    break;

  case 131:

/* Line 1455 of yacc.c  */
#line 993 "l2tp_config_parse.y"
    {
			ppp_profile.flags |= L2TP_API_PPP_PROFILE_FLAG_CHAP_RESTART;
			ppp_profile.chap_restart = (yyvsp[(3) - (3)].ulnum);
		;}
    break;

  case 132:

/* Line 1455 of yacc.c  */
#line 998 "l2tp_config_parse.y"
    {
			ppp_profile.flags |= L2TP_API_PPP_PROFILE_FLAG_PAP_MAX_AUTH_REQUESTS;
			ppp_profile.pap_max_auth_requests = (yyvsp[(3) - (3)].ulnum);
		;}
    break;

  case 133:

/* Line 1455 of yacc.c  */
#line 1003 "l2tp_config_parse.y"
    {
			ppp_profile.flags |= L2TP_API_PPP_PROFILE_FLAG_PAP_RESTART_INTERVAL;
			ppp_profile.pap_restart_interval = (yyvsp[(3) - (3)].ulnum);
		;}
    break;

  case 134:

/* Line 1455 of yacc.c  */
#line 1008 "l2tp_config_parse.y"
    {
			ppp_profile.flags |= L2TP_API_PPP_PROFILE_FLAG_PAP_TIMEOUT;
			ppp_profile.pap_timeout = (yyvsp[(3) - (3)].ulnum);
		;}
    break;

  case 135:

/* Line 1455 of yacc.c  */
#line 1013 "l2tp_config_parse.y"
    {
			ppp_profile.flags |= L2TP_API_PPP_PROFILE_FLAG_IDLE_TIMEOUT;
			ppp_profile.idle_timeout = (yyvsp[(3) - (3)].ulnum);
		;}
    break;

  case 136:

/* Line 1455 of yacc.c  */
#line 1018 "l2tp_config_parse.y"
    {
			ppp_profile.flags |= L2TP_API_PPP_PROFILE_FLAG_IPCP_MAX_CONFIG_REQUESTS;
			ppp_profile.ipcp_max_config_requests = (yyvsp[(3) - (3)].ulnum);
		;}
    break;

  case 137:

/* Line 1455 of yacc.c  */
#line 1023 "l2tp_config_parse.y"
    {
			ppp_profile.flags |= L2TP_API_PPP_PROFILE_FLAG_IPCP_MAX_CONFIG_NAKS;
			ppp_profile.ipcp_max_config_naks = (yyvsp[(3) - (3)].ulnum);
		;}
    break;

  case 138:

/* Line 1455 of yacc.c  */
#line 1028 "l2tp_config_parse.y"
    {
			ppp_profile.flags |= L2TP_API_PPP_PROFILE_FLAG_IPCP_MAX_TERMINATE_REQUESTS;
			ppp_profile.lcp_max_terminate_requests = (yyvsp[(3) - (3)].ulnum);
		;}
    break;

  case 139:

/* Line 1455 of yacc.c  */
#line 1033 "l2tp_config_parse.y"
    {
			ppp_profile.flags |= L2TP_API_PPP_PROFILE_FLAG_IPCP_RETRANSMIT_INTERVAL;
			ppp_profile.ipcp_retransmit_interval = (yyvsp[(3) - (3)].ulnum);
		;}
    break;

  case 140:

/* Line 1455 of yacc.c  */
#line 1038 "l2tp_config_parse.y"
    {
			ppp_profile.flags |= L2TP_API_PPP_PROFILE_FLAG_LCP_ECHO_FAILURE_COUNT;
			ppp_profile.lcp_echo_failure_count = (yyvsp[(3) - (3)].ulnum);
		;}
    break;

  case 141:

/* Line 1455 of yacc.c  */
#line 1043 "l2tp_config_parse.y"
    {
			ppp_profile.flags |= L2TP_API_PPP_PROFILE_FLAG_LCP_ECHO_INTERVAL;
			ppp_profile.lcp_echo_interval = (yyvsp[(3) - (3)].ulnum);
		;}
    break;

  case 142:

/* Line 1455 of yacc.c  */
#line 1048 "l2tp_config_parse.y"
    {
			ppp_profile.flags |= L2TP_API_PPP_PROFILE_FLAG_LCP_MAX_CONFIG_REQUESTS;
			ppp_profile.lcp_max_config_requests = (yyvsp[(3) - (3)].ulnum);
		;}
    break;

  case 143:

/* Line 1455 of yacc.c  */
#line 1053 "l2tp_config_parse.y"
    {
			ppp_profile.flags |= L2TP_API_PPP_PROFILE_FLAG_LCP_MAX_CONFIG_NAKS;
			ppp_profile.lcp_max_config_naks = (yyvsp[(3) - (3)].ulnum);
		;}
    break;

  case 144:

/* Line 1455 of yacc.c  */
#line 1058 "l2tp_config_parse.y"
    {
			ppp_profile.flags |= L2TP_API_PPP_PROFILE_FLAG_LCP_MAX_TERMINATE_REQUESTS;
			ppp_profile.lcp_max_terminate_requests = (yyvsp[(3) - (3)].ulnum);
		;}
    break;

  case 145:

/* Line 1455 of yacc.c  */
#line 1063 "l2tp_config_parse.y"
    {
			ppp_profile.flags |= L2TP_API_PPP_PROFILE_FLAG_LCP_RETRANSMIT_INTERVAL;
			ppp_profile.lcp_retransmit_interval = (yyvsp[(3) - (3)].ulnum);
		;}
    break;

  case 146:

/* Line 1455 of yacc.c  */
#line 1068 "l2tp_config_parse.y"
    {
			ppp_profile.flags |= L2TP_API_PPP_PROFILE_FLAG_MAX_CONNECT_TIME;
			ppp_profile.max_connect_time = (yyvsp[(3) - (3)].ulnum);
		;}
    break;

  case 147:

/* Line 1455 of yacc.c  */
#line 1073 "l2tp_config_parse.y"
    {
			ppp_profile.flags |= L2TP_API_PPP_PROFILE_FLAG_MAX_FAILURE_COUNT;
			ppp_profile.max_failure_count = (yyvsp[(3) - (3)].ulnum);
		;}
    break;

  case 148:

/* Line 1455 of yacc.c  */
#line 1078 "l2tp_config_parse.y"
    {
			struct in_addr addr;
			ppp_profile.flags2 |= L2TP_API_PPP_PROFILE_FLAG_DNS_ADDR_1;
			addr.s_addr = htonl((yyvsp[(3) - (3)].ulnum));
			ppp_profile.dns_addr_1.s_addr = addr.s_addr;
		;}
    break;

  case 149:

/* Line 1455 of yacc.c  */
#line 1085 "l2tp_config_parse.y"
    {
			struct in_addr addr;
			ppp_profile.flags2 |= L2TP_API_PPP_PROFILE_FLAG_DNS_ADDR_2;
			addr.s_addr = htonl((yyvsp[(3) - (3)].ulnum));
			ppp_profile.dns_addr_2.s_addr = addr.s_addr;
		;}
    break;

  case 150:

/* Line 1455 of yacc.c  */
#line 1092 "l2tp_config_parse.y"
    {
			struct in_addr addr;
			ppp_profile.flags2 |= L2TP_API_PPP_PROFILE_FLAG_WINS_ADDR_1;
			addr.s_addr = htonl((yyvsp[(3) - (3)].ulnum));
			ppp_profile.wins_addr_1.s_addr = addr.s_addr;
		;}
    break;

  case 151:

/* Line 1455 of yacc.c  */
#line 1099 "l2tp_config_parse.y"
    {
			struct in_addr addr;
			ppp_profile.flags2 |= L2TP_API_PPP_PROFILE_FLAG_WINS_ADDR_2;
			addr.s_addr = htonl((yyvsp[(3) - (3)].ulnum));
			ppp_profile.wins_addr_2.s_addr = addr.s_addr;
		;}
    break;

  case 152:

/* Line 1455 of yacc.c  */
#line 1106 "l2tp_config_parse.y"
    {
			struct in_addr addr;
			ppp_profile.flags2 |= L2TP_API_PPP_PROFILE_FLAG_LOCAL_IP_ADDR;
			addr.s_addr = htonl((yyvsp[(3) - (3)].ulnum));
			ppp_profile.local_ip_addr.s_addr = addr.s_addr;
		;}
    break;

  case 153:

/* Line 1455 of yacc.c  */
#line 1113 "l2tp_config_parse.y"
    {
			struct in_addr addr;
			ppp_profile.flags2 |= L2TP_API_PPP_PROFILE_FLAG_PEER_IP_ADDR;
			addr.s_addr = htonl((yyvsp[(3) - (3)].ulnum));
			ppp_profile.peer_ip_addr.s_addr = addr.s_addr;
		;}
    break;

  case 154:

/* Line 1455 of yacc.c  */
#line 1120 "l2tp_config_parse.y"
    {
			ppp_profile.flags2 |= L2TP_API_PPP_PROFILE_FLAG_IP_POOL_NAME;
			OPTSTRING(ppp_profile.ip_pool_name) = (yyvsp[(3) - (3)].val).buf;
			ppp_profile.ip_pool_name.valid = 1;
		;}
    break;

  case 155:

/* Line 1455 of yacc.c  */
#line 1126 "l2tp_config_parse.y"
    {
			ppp_profile.flags2 |= L2TP_API_PPP_PROFILE_FLAG_IP_POOL_NAME;
			OPTSTRING(ppp_profile.ip_pool_name) = (yyvsp[(3) - (3)].val).buf;
			ppp_profile.ip_pool_name.valid = 1;
		;}
    break;

  case 156:

/* Line 1455 of yacc.c  */
#line 1132 "l2tp_config_parse.y"
    {
			ppp_profile.flags2 |= L2TP_API_PPP_PROFILE_FLAG_USE_AS_DEFAULT_ROUTE;
			ppp_profile.use_as_default_route = (yyvsp[(3) - (3)].num);
		;}
    break;

  case 157:

/* Line 1455 of yacc.c  */
#line 1137 "l2tp_config_parse.y"
    {
			ppp_profile.flags2 |= L2TP_API_PPP_PROFILE_FLAG_MULTILINK;
			ppp_profile.multilink = (yyvsp[(3) - (3)].num);
		;}
    break;

  case 158:

/* Line 1455 of yacc.c  */
#line 1142 "l2tp_config_parse.y"
    {
			ppp_profile.flags2 |= L2TP_API_PPP_PROFILE_FLAG_PROXY_ARP;
			ppp_profile.proxy_arp = (yyvsp[(3) - (3)].num);
		;}
    break;

  case 159:

/* Line 1455 of yacc.c  */
#line 1147 "l2tp_config_parse.y"
    {
			ppp_profile.flags2 |= L2TP_API_PPP_PROFILE_FLAG_LOCAL_NAME;
			OPTSTRING(ppp_profile.local_name) = (yyvsp[(3) - (3)].val).buf;
			ppp_profile.local_name.valid = 1;
		;}
    break;

  case 160:

/* Line 1455 of yacc.c  */
#line 1153 "l2tp_config_parse.y"
    {
			ppp_profile.flags2 |= L2TP_API_PPP_PROFILE_FLAG_LOCAL_NAME;
			OPTSTRING(ppp_profile.local_name) = (yyvsp[(3) - (3)].val).buf;
			ppp_profile.local_name.valid = 1;
		;}
    break;

  case 161:

/* Line 1455 of yacc.c  */
#line 1159 "l2tp_config_parse.y"
    {
			ppp_profile.flags2 |= L2TP_API_PPP_PROFILE_FLAG_REMOTE_NAME;
			OPTSTRING(ppp_profile.remote_name) = (yyvsp[(3) - (3)].val).buf;
			ppp_profile.remote_name.valid = 1;
		;}
    break;

  case 162:

/* Line 1455 of yacc.c  */
#line 1165 "l2tp_config_parse.y"
    {
			ppp_profile.flags2 |= L2TP_API_PPP_PROFILE_FLAG_REMOTE_NAME;
			OPTSTRING(ppp_profile.remote_name) = (yyvsp[(3) - (3)].val).buf;
			ppp_profile.remote_name.valid = 1;
		;}
    break;

  case 165:

/* Line 1455 of yacc.c  */
#line 1179 "l2tp_config_parse.y"
    {
			bool_t status;
			int result;
			status = l2tp_tunnel_create_1_svc(tunnel, &result, NULL);
			if ((status != TRUE) || ((result < 0) && (result != -L2TP_ERR_TUNNEL_ALREADY_EXISTS)))
				l2tp_log(LOG_ERR, "tunnel create: command failed: rc=%d", l2tp_strerror(-result));

			memset(&tunnel, 0, sizeof(tunnel));
		;}
    break;

  case 166:

/* Line 1455 of yacc.c  */
#line 1192 "l2tp_config_parse.y"
    {
			bool_t status;
			int result;
			status = l2tp_tunnel_modify_1_svc(tunnel, &result, NULL);
			if ((status != TRUE) || (result < 0))
				l2tp_log(LOG_ERR, "tunnel modify: command failed: rc=%d", l2tp_strerror(-result));

			memset(&tunnel, 0, sizeof(tunnel));
		;}
    break;

  case 169:

/* Line 1455 of yacc.c  */
#line 1210 "l2tp_config_parse.y"
    {
			tunnel.flags |= L2TP_API_TUNNEL_FLAG_HIDE_AVPS;
			tunnel.hide_avps = (yyvsp[(3) - (3)].num);
		;}
    break;

  case 170:

/* Line 1455 of yacc.c  */
#line 1215 "l2tp_config_parse.y"
    {
			struct in_addr addr;
			tunnel.flags |= L2TP_API_TUNNEL_FLAG_OUR_ADDR;
			addr.s_addr = htonl((yyvsp[(3) - (3)].ulnum));
			tunnel.our_addr.s_addr = addr.s_addr;
		;}
    break;

  case 171:

/* Line 1455 of yacc.c  */
#line 1222 "l2tp_config_parse.y"
    {
			struct in_addr addr;
			tunnel.flags |= L2TP_API_TUNNEL_FLAG_PEER_ADDR;
			addr.s_addr = htonl((yyvsp[(3) - (3)].ulnum));
			tunnel.peer_addr.s_addr = addr.s_addr;
		;}
    break;

  case 172:

/* Line 1455 of yacc.c  */
#line 1229 "l2tp_config_parse.y"
    {
			tunnel.flags |= L2TP_API_TUNNEL_FLAG_CONFIG_ID;
			tunnel.config_id = (yyvsp[(3) - (3)].ulnum);
		;}
    break;

  case 173:

/* Line 1455 of yacc.c  */
#line 1234 "l2tp_config_parse.y"
    {
			tunnel.flags |= L2TP_API_TUNNEL_FLAG_OUR_UDP_PORT;
			tunnel.our_udp_port = (yyvsp[(3) - (3)].ulnum);
		;}
    break;

  case 174:

/* Line 1455 of yacc.c  */
#line 1239 "l2tp_config_parse.y"
    {
			tunnel.flags |= L2TP_API_TUNNEL_FLAG_AUTH_MODE;
			if (strcasecmp((yyvsp[(3) - (3)].val).buf, "none") == 0) {
				tunnel.auth_mode = L2TP_API_TUNNEL_AUTH_MODE_NONE;
			} else if (strcasecmp((yyvsp[(3) - (3)].val).buf, "simple") == 0) {
				tunnel.auth_mode = L2TP_API_TUNNEL_AUTH_MODE_SIMPLE;
			} else if (strcasecmp((yyvsp[(3) - (3)].val).buf, "challenge") == 0) {
				tunnel.auth_mode = L2TP_API_TUNNEL_AUTH_MODE_CHALLENGE;
			} else {
				yyfatal("invalid auth_mode");
			}
		;}
    break;

  case 175:

/* Line 1455 of yacc.c  */
#line 1252 "l2tp_config_parse.y"
    {
			tunnel.flags |= L2TP_API_TUNNEL_FLAG_FRAMING_CAP;
			if (strcasecmp((yyvsp[(3) - (3)].val).buf, "sync") == 0) {
				tunnel.framing_cap_sync = 1;
				tunnel.framing_cap_async = 0;
			} else if (strcasecmp((yyvsp[(3) - (3)].val).buf, "async") == 0) {
				tunnel.framing_cap_sync = 0;
				tunnel.framing_cap_async = 1;
			} else if (strcasecmp((yyvsp[(3) - (3)].val).buf, "any") == 0) {
				tunnel.framing_cap_sync = 1;
				tunnel.framing_cap_async = 1;
			} else {
				yyfatal("invalid framing_cap");
			}
		;}
    break;

  case 176:

/* Line 1455 of yacc.c  */
#line 1268 "l2tp_config_parse.y"
    {
			tunnel.flags |= L2TP_API_TUNNEL_FLAG_BEARER_CAP;
			if (strcasecmp((yyvsp[(3) - (3)].val).buf, "analog") == 0) {
				tunnel.bearer_cap_analog = 1;
				tunnel.bearer_cap_digital = 0;
			} else if (strcasecmp((yyvsp[(3) - (3)].val).buf, "digital") == 0) {
				tunnel.bearer_cap_analog = 0;
				tunnel.bearer_cap_digital = 1;
			} else if (strcasecmp((yyvsp[(3) - (3)].val).buf, "any") == 0) {
				tunnel.bearer_cap_analog = 1;
				tunnel.bearer_cap_digital = 1;
			} else {
				yyfatal("invalid bearer_cap");
			}
		;}
    break;

  case 177:

/* Line 1455 of yacc.c  */
#line 1284 "l2tp_config_parse.y"
    {
			tunnel.flags |= L2TP_API_TUNNEL_FLAG_USE_TIEBREAKER;
			tunnel.use_tiebreaker = (yyvsp[(3) - (3)].num);
		;}
    break;

  case 178:

/* Line 1455 of yacc.c  */
#line 1289 "l2tp_config_parse.y"
    {
			tunnel.flags |= L2TP_API_TUNNEL_FLAG_PERSIST;
			tunnel.persist = (yyvsp[(3) - (3)].num);
		;}
    break;

  case 179:

/* Line 1455 of yacc.c  */
#line 1294 "l2tp_config_parse.y"
    {
			tunnel.flags |= L2TP_API_TUNNEL_FLAG_HELLO_TIMEOUT;
			tunnel.hello_timeout = (yyvsp[(3) - (3)].ulnum);
		;}
    break;

  case 180:

/* Line 1455 of yacc.c  */
#line 1299 "l2tp_config_parse.y"
    {
			tunnel.flags |= L2TP_API_TUNNEL_FLAG_MAX_RETRIES;
			tunnel.max_retries = (yyvsp[(3) - (3)].ulnum);
		;}
    break;

  case 181:

/* Line 1455 of yacc.c  */
#line 1304 "l2tp_config_parse.y"
    {
			tunnel.flags |= L2TP_API_TUNNEL_FLAG_RX_WINDOW_SIZE;
			tunnel.rx_window_size = (yyvsp[(3) - (3)].ulnum);
		;}
    break;

  case 182:

/* Line 1455 of yacc.c  */
#line 1309 "l2tp_config_parse.y"
    {
			tunnel.flags |= L2TP_API_TUNNEL_FLAG_TX_WINDOW_SIZE;
			tunnel.tx_window_size = (yyvsp[(3) - (3)].ulnum);
		;}
    break;

  case 183:

/* Line 1455 of yacc.c  */
#line 1314 "l2tp_config_parse.y"
    {
			tunnel.flags |= L2TP_API_TUNNEL_FLAG_RETRY_TIMEOUT;
			tunnel.retry_timeout = (yyvsp[(3) - (3)].ulnum);
		;}
    break;

  case 184:

/* Line 1455 of yacc.c  */
#line 1319 "l2tp_config_parse.y"
    {
			tunnel.flags |= L2TP_API_TUNNEL_FLAG_IDLE_TIMEOUT;
			tunnel.idle_timeout = (yyvsp[(3) - (3)].ulnum);
		;}
    break;

  case 185:

/* Line 1455 of yacc.c  */
#line 1324 "l2tp_config_parse.y"
    {
			tunnel.flags |= L2TP_API_TUNNEL_FLAG_PEER_UDP_PORT;
			tunnel.peer_udp_port = (yyvsp[(3) - (3)].ulnum);
		;}
    break;

  case 186:

/* Line 1455 of yacc.c  */
#line 1329 "l2tp_config_parse.y"
    {
			tunnel.flags |= L2TP_API_TUNNEL_FLAG_SECRET;
			OPTSTRING(tunnel.secret) = (yyvsp[(3) - (3)].val).buf;
			tunnel.secret.valid = 1;
		;}
    break;

  case 187:

/* Line 1455 of yacc.c  */
#line 1335 "l2tp_config_parse.y"
    {
			tunnel.flags |= L2TP_API_TUNNEL_FLAG_SECRET;
			OPTSTRING(tunnel.secret) = (yyvsp[(3) - (3)].val).buf;
			tunnel.secret.valid = 1;
		;}
    break;

  case 188:

/* Line 1455 of yacc.c  */
#line 1341 "l2tp_config_parse.y"
    {
			tunnel.flags |= L2TP_API_TUNNEL_FLAG_ALLOW_PPP_PROXY;
			tunnel.allow_ppp_proxy = (yyvsp[(3) - (3)].num);
		;}
    break;

  case 189:

/* Line 1455 of yacc.c  */
#line 1346 "l2tp_config_parse.y"
    {
			tunnel.flags |= L2TP_API_TUNNEL_FLAG_TRACE_FLAGS;
			tunnel.trace_flags = (yyvsp[(3) - (3)].ulnum);
		;}
    break;

  case 190:

/* Line 1455 of yacc.c  */
#line 1351 "l2tp_config_parse.y"
    {
			tunnel.flags |= L2TP_API_TUNNEL_FLAG_USE_UDP_CHECKSUMS;
			tunnel.use_udp_checksums = (yyvsp[(3) - (3)].num);
		;}
    break;

  case 191:

/* Line 1455 of yacc.c  */
#line 1356 "l2tp_config_parse.y"
    {
			tunnel.flags |= L2TP_API_TUNNEL_FLAG_HOST_NAME;
			OPTSTRING(tunnel.host_name) = (yyvsp[(3) - (3)].val).buf;
			tunnel.host_name.valid = 1;
		;}
    break;

  case 192:

/* Line 1455 of yacc.c  */
#line 1362 "l2tp_config_parse.y"
    {
			tunnel.flags |= L2TP_API_TUNNEL_FLAG_HOST_NAME;
			OPTSTRING(tunnel.host_name) = (yyvsp[(3) - (3)].val).buf;
			tunnel.host_name.valid = 1;
		;}
    break;

  case 193:

/* Line 1455 of yacc.c  */
#line 1368 "l2tp_config_parse.y"
    {
			tunnel.flags |= L2TP_API_TUNNEL_FLAG_TUNNEL_ID;
			tunnel.tunnel_id = (yyvsp[(3) - (3)].ulnum);
		;}
    break;

  case 194:

/* Line 1455 of yacc.c  */
#line 1373 "l2tp_config_parse.y"
    {
			tunnel.flags |= L2TP_API_TUNNEL_FLAG_PROFILE_NAME;
			OPTSTRING(tunnel.tunnel_profile_name) = (yyvsp[(3) - (3)].val).buf;
			tunnel.tunnel_profile_name.valid = 1;
		;}
    break;

  case 195:

/* Line 1455 of yacc.c  */
#line 1379 "l2tp_config_parse.y"
    {
			tunnel.flags |= L2TP_API_TUNNEL_FLAG_PROFILE_NAME;
			OPTSTRING(tunnel.tunnel_profile_name) = (yyvsp[(3) - (3)].val).buf;
			tunnel.tunnel_profile_name.valid = 1;
		;}
    break;

  case 196:

/* Line 1455 of yacc.c  */
#line 1385 "l2tp_config_parse.y"
    {
			tunnel.flags |= L2TP_API_TUNNEL_FLAG_MAX_SESSIONS;
			tunnel.max_sessions = (yyvsp[(3) - (3)].ulnum);
		;}
    break;

  case 197:

/* Line 1455 of yacc.c  */
#line 1390 "l2tp_config_parse.y"
    {
			tunnel.flags |= L2TP_API_TUNNEL_FLAG_TUNNEL_NAME;
			OPTSTRING(tunnel.tunnel_name) = (yyvsp[(3) - (3)].val).buf;
			tunnel.tunnel_name.valid = 1;
		;}
    break;

  case 198:

/* Line 1455 of yacc.c  */
#line 1396 "l2tp_config_parse.y"
    {
			tunnel.flags |= L2TP_API_TUNNEL_FLAG_TUNNEL_NAME;
			OPTSTRING(tunnel.tunnel_name) = (yyvsp[(3) - (3)].val).buf;
			tunnel.tunnel_name.valid = 1;
		;}
    break;

  case 199:

/* Line 1455 of yacc.c  */
#line 1402 "l2tp_config_parse.y"
    {
			tunnel.flags |= L2TP_API_TUNNEL_FLAG_PEER_PROFILE_NAME;
			OPTSTRING(tunnel.peer_profile_name) = (yyvsp[(3) - (3)].val).buf;
			tunnel.peer_profile_name.valid = 1;
		;}
    break;

  case 200:

/* Line 1455 of yacc.c  */
#line 1408 "l2tp_config_parse.y"
    {
			tunnel.flags |= L2TP_API_TUNNEL_FLAG_PEER_PROFILE_NAME;
			OPTSTRING(tunnel.peer_profile_name) = (yyvsp[(3) - (3)].val).buf;
			tunnel.peer_profile_name.valid = 1;
		;}
    break;

  case 201:

/* Line 1455 of yacc.c  */
#line 1414 "l2tp_config_parse.y"
    {
			tunnel.flags |= L2TP_API_TUNNEL_FLAG_SESSION_PROFILE_NAME;
			OPTSTRING(tunnel.session_profile_name) = (yyvsp[(3) - (3)].val).buf;
			tunnel.session_profile_name.valid = 1;
		;}
    break;

  case 202:

/* Line 1455 of yacc.c  */
#line 1420 "l2tp_config_parse.y"
    {
			tunnel.flags |= L2TP_API_TUNNEL_FLAG_SESSION_PROFILE_NAME;
			OPTSTRING(tunnel.session_profile_name) = (yyvsp[(3) - (3)].val).buf;
			tunnel.session_profile_name.valid = 1;
		;}
    break;

  case 203:

/* Line 1455 of yacc.c  */
#line 1426 "l2tp_config_parse.y"
    {
			tunnel.flags |= L2TP_API_TUNNEL_FLAG_PPP_PROFILE_NAME;
			OPTSTRING(tunnel.ppp_profile_name) = (yyvsp[(3) - (3)].val).buf;
			tunnel.ppp_profile_name.valid = 1;
		;}
    break;

  case 204:

/* Line 1455 of yacc.c  */
#line 1432 "l2tp_config_parse.y"
    {
			tunnel.flags |= L2TP_API_TUNNEL_FLAG_PPP_PROFILE_NAME;
			OPTSTRING(tunnel.ppp_profile_name) = (yyvsp[(3) - (3)].val).buf;
			tunnel.ppp_profile_name.valid = 1;
		;}
    break;

  case 205:

/* Line 1455 of yacc.c  */
#line 1438 "l2tp_config_parse.y"
    {
			tunnel.flags |= L2TP_API_TUNNEL_FLAG_DO_PMTU_DISCOVERY;
			tunnel.do_pmtu_discovery = (yyvsp[(3) - (3)].num);
		;}
    break;

  case 206:

/* Line 1455 of yacc.c  */
#line 1443 "l2tp_config_parse.y"
    {
			tunnel.flags |= L2TP_API_TUNNEL_FLAG_MTU;
			tunnel.mtu = (yyvsp[(3) - (3)].ulnum);
		;}
    break;

  case 207:

/* Line 1455 of yacc.c  */
#line 1448 "l2tp_config_parse.y"
    {
			tunnel.flags2 |= L2TP_API_TUNNEL_FLAG_INTERFACE_NAME;
			OPTSTRING(tunnel.interface_name) = (yyvsp[(3) - (3)].val).buf;
			tunnel.interface_name.valid = 1;
		;}
    break;

  case 208:

/* Line 1455 of yacc.c  */
#line 1454 "l2tp_config_parse.y"
    {
			tunnel.flags2 |= L2TP_API_TUNNEL_FLAG_INTERFACE_NAME;
			OPTSTRING(tunnel.interface_name) = (yyvsp[(3) - (3)].val).buf;
			tunnel.interface_name.valid = 1;
		;}
    break;

  case 211:

/* Line 1455 of yacc.c  */
#line 1468 "l2tp_config_parse.y"
    {
			bool_t status;
			int result;
			status = l2tp_session_create_1_svc(session, &result, NULL);
			if ((status != TRUE) || ((result < 0) && (result != -L2TP_ERR_SESSION_ALREADY_EXISTS)))
				l2tp_log(LOG_ERR, "session create: command failed: rc=%d", l2tp_strerror(-result));

			memset(&session, 0, sizeof(session));
		;}
    break;

  case 212:

/* Line 1455 of yacc.c  */
#line 1481 "l2tp_config_parse.y"
    {
			bool_t status;
			int result;
			status = l2tp_session_modify_1_svc(session, &result, NULL);
			if ((status != TRUE) || (result < 0))
				l2tp_log(LOG_ERR, "session modify: command failed: rc=%d", l2tp_strerror(-result));

			memset(&session, 0, sizeof(session));
		;}
    break;

  case 215:

/* Line 1455 of yacc.c  */
#line 1499 "l2tp_config_parse.y"
    {
			session.flags |= L2TP_API_SESSION_FLAG_TRACE_FLAGS;
			session.trace_flags = (yyvsp[(3) - (3)].ulnum);
		;}
    break;

  case 216:

/* Line 1455 of yacc.c  */
#line 1504 "l2tp_config_parse.y"
    {
			session.flags |= L2TP_API_SESSION_FLAG_SEQUENCING_REQUIRED;
			session.sequencing_required = (yyvsp[(3) - (3)].num);
		;}
    break;

  case 217:

/* Line 1455 of yacc.c  */
#line 1509 "l2tp_config_parse.y"
    {
			session.flags |= L2TP_API_SESSION_FLAG_PPP_PROFILE_NAME;
			OPTSTRING(session.ppp_profile_name) = (yyvsp[(3) - (3)].val).buf;
			session.ppp_profile_name.valid = 1;
		;}
    break;

  case 218:

/* Line 1455 of yacc.c  */
#line 1515 "l2tp_config_parse.y"
    {
			session.flags |= L2TP_API_SESSION_FLAG_PPP_PROFILE_NAME;
			OPTSTRING(session.ppp_profile_name) = (yyvsp[(3) - (3)].val).buf;
			session.ppp_profile_name.valid = 1;
		;}
    break;

  case 219:

/* Line 1455 of yacc.c  */
#line 1521 "l2tp_config_parse.y"
    {
			session.flags |= L2TP_API_SESSION_FLAG_SESSION_TYPE;
			if (strcasecmp((yyvsp[(3) - (3)].val).buf, "laic") == 0) {
				session.session_type = L2TP_API_SESSION_TYPE_LAIC;
			} else if (strcasecmp((yyvsp[(3) - (3)].val).buf, "laoc") == 0) {
				session.session_type = L2TP_API_SESSION_TYPE_LAOC;
			} else if (strcasecmp((yyvsp[(3) - (3)].val).buf, "lnic") == 0) {
				session.session_type = L2TP_API_SESSION_TYPE_LNIC;
			} else if (strcasecmp((yyvsp[(3) - (3)].val).buf, "lnoc") == 0) {
				session.session_type = L2TP_API_SESSION_TYPE_LNOC;
			} else {
				yyfatal("Bad session type: expecting laic|laoc|lnic|lnoc");
			}
		;}
    break;

  case 220:

/* Line 1455 of yacc.c  */
#line 1536 "l2tp_config_parse.y"
    {
			session.flags |= L2TP_API_SESSION_FLAG_PRIV_GROUP_ID;
			OPTSTRING(session.priv_group_id) = (yyvsp[(3) - (3)].val).buf;
			session.priv_group_id.valid = 1;
		;}
    break;

  case 221:

/* Line 1455 of yacc.c  */
#line 1542 "l2tp_config_parse.y"
    {
			session.flags |= L2TP_API_SESSION_FLAG_PRIV_GROUP_ID;
			OPTSTRING(session.priv_group_id) = (yyvsp[(3) - (3)].val).buf;
			session.priv_group_id.valid = 1;
		;}
    break;

  case 222:

/* Line 1455 of yacc.c  */
#line 1548 "l2tp_config_parse.y"
    {
			session.flags |= L2TP_API_SESSION_FLAG_FRAMING_TYPE;
			if (strcasecmp((yyvsp[(3) - (3)].val).buf, "sync") == 0) {
				session.framing_type_sync = 1;
				session.framing_type_async = 0;
			} else if (strcasecmp((yyvsp[(3) - (3)].val).buf, "async") == 0) {
				session.framing_type_sync = 0;
				session.framing_type_async = 1;
			} else if (strcasecmp((yyvsp[(3) - (3)].val).buf, "any") == 0) {
				session.framing_type_sync = 1;
				session.framing_type_async = 1;
			} else {
				yyfatal("invalid framing_type");
			}
		;}
    break;

  case 223:

/* Line 1455 of yacc.c  */
#line 1564 "l2tp_config_parse.y"
    {
			session.flags |= L2TP_API_SESSION_FLAG_BEARER_TYPE;
			if (strcasecmp((yyvsp[(3) - (3)].val).buf, "analog") == 0) {
				session.bearer_type_analog = 1;
				session.bearer_type_digital = 0;
			} else if (strcasecmp((yyvsp[(3) - (3)].val).buf, "digital") == 0) {
				session.bearer_type_analog = 0;
				session.bearer_type_digital = 1;
			} else if (strcasecmp((yyvsp[(3) - (3)].val).buf, "any") == 0) {
				session.bearer_type_analog = 1;
				session.bearer_type_digital = 1;
			} else {
				yyfatal("invalid bearer_type");
			}
		;}
    break;

  case 224:

/* Line 1455 of yacc.c  */
#line 1580 "l2tp_config_parse.y"
    {
			session.flags |= L2TP_API_SESSION_FLAG_MINIMUM_BPS;
			session.minimum_bps = (yyvsp[(3) - (3)].ulnum);
		;}
    break;

  case 225:

/* Line 1455 of yacc.c  */
#line 1585 "l2tp_config_parse.y"
    {
			session.flags |= L2TP_API_SESSION_FLAG_MAXIMUM_BPS;
			session.maximum_bps = (yyvsp[(3) - (3)].ulnum);
		;}
    break;

  case 226:

/* Line 1455 of yacc.c  */
#line 1590 "l2tp_config_parse.y"
    {
			int ints[2];
			int num_matches;
			session.flags |= L2TP_API_SESSION_FLAG_CONNECT_SPEED;
			num_matches = sscanf((yyvsp[(3) - (3)].val).buf, "%d:%d", &ints[0], &ints[1]);
			if (num_matches >= 1) {
				session.rx_connect_speed = ints[0];
				session.tx_connect_speed = ints[0];
				if (num_matches == 2) {
					session.tx_connect_speed = ints[1];
				}
			} else {
				yyfatal("Expecting connect_speed[:tx_connect_speed]");
			}
		;}
    break;

  case 227:

/* Line 1455 of yacc.c  */
#line 1606 "l2tp_config_parse.y"
    {
			session.flags |= L2TP_API_SESSION_FLAG_USE_PPP_PROXY;
			session.use_ppp_proxy = (yyvsp[(3) - (3)].num);
		;}
    break;

  case 228:

/* Line 1455 of yacc.c  */
#line 1611 "l2tp_config_parse.y"
    {
			session.flags |= L2TP_API_SESSION_FLAG_PROXY_AUTH_TYPE;
			if (strcasecmp((yyvsp[(3) - (3)].val).buf, "text") == 0) {
				session.proxy_auth_type = L2TP_API_SESSION_PROXY_AUTH_TYPE_PLAIN_TEXT;
			} else if (strcasecmp((yyvsp[(3) - (3)].val).buf, "chap") == 0) {
				session.proxy_auth_type = L2TP_API_SESSION_PROXY_AUTH_TYPE_PPP_CHAP;
			} else if (strcasecmp((yyvsp[(3) - (3)].val).buf, "pap") == 0) {
				session.proxy_auth_type = L2TP_API_SESSION_PROXY_AUTH_TYPE_PPP_PAP;
			} else if (strcasecmp((yyvsp[(3) - (3)].val).buf, "none") == 0) {
				session.proxy_auth_type = L2TP_API_SESSION_PROXY_AUTH_TYPE_NO_AUTH;
			} else if (strcasecmp((yyvsp[(3) - (3)].val).buf, "mschap") == 0) {
				session.proxy_auth_type = L2TP_API_SESSION_PROXY_AUTH_TYPE_PPP_MSCHAPV1;
			} else {
				yyfatal("Bad auth type: expecting none|text|chap|pap|mschap");
			}
		;}
    break;

  case 229:

/* Line 1455 of yacc.c  */
#line 1628 "l2tp_config_parse.y"
    {
			session.flags |= L2TP_API_SESSION_FLAG_PROXY_AUTH_NAME;
			OPTSTRING(session.proxy_auth_name) = (yyvsp[(3) - (3)].val).buf;
			session.proxy_auth_name.valid = 1;
		;}
    break;

  case 230:

/* Line 1455 of yacc.c  */
#line 1634 "l2tp_config_parse.y"
    {
			session.flags |= L2TP_API_SESSION_FLAG_PROXY_AUTH_CHALLENGE;
			session.proxy_auth_challenge.proxy_auth_challenge_len = (yyvsp[(3) - (3)].val).len;
			session.proxy_auth_challenge.proxy_auth_challenge_val = (yyvsp[(3) - (3)].val).buf;
		;}
    break;

  case 231:

/* Line 1455 of yacc.c  */
#line 1640 "l2tp_config_parse.y"
    {
			session.flags |= L2TP_API_SESSION_FLAG_PROXY_AUTH_RESPONSE;
			session.proxy_auth_response.proxy_auth_response_len = (yyvsp[(3) - (3)].val).len;
			session.proxy_auth_response.proxy_auth_response_val = (yyvsp[(3) - (3)].val).buf;
		;}
    break;

  case 232:

/* Line 1455 of yacc.c  */
#line 1646 "l2tp_config_parse.y"
    {
			session.flags |= L2TP_API_SESSION_FLAG_CALLING_NUMBER;
			OPTSTRING(session.calling_number) = (yyvsp[(3) - (3)].val).buf;
			session.calling_number.valid = 1;
		;}
    break;

  case 233:

/* Line 1455 of yacc.c  */
#line 1652 "l2tp_config_parse.y"
    {
			char str[16];
			session.flags |= L2TP_API_SESSION_FLAG_CALLING_NUMBER;
			sprintf(str, "%lu", (yyvsp[(3) - (3)].ulnum));
			OPTSTRING(session.calling_number) = strdup(str);
			session.calling_number.valid = 1;
		;}
    break;

  case 234:

/* Line 1455 of yacc.c  */
#line 1660 "l2tp_config_parse.y"
    {
			session.flags |= L2TP_API_SESSION_FLAG_CALLED_NUMBER;
			OPTSTRING(session.called_number) = (yyvsp[(3) - (3)].val).buf;
			session.called_number.valid = 1;
		;}
    break;

  case 235:

/* Line 1455 of yacc.c  */
#line 1666 "l2tp_config_parse.y"
    {
			char str[16];
			session.flags |= L2TP_API_SESSION_FLAG_CALLED_NUMBER;
			sprintf(str, "%lu", (yyvsp[(3) - (3)].ulnum));
			OPTSTRING(session.called_number) = strdup(str);
			session.called_number.valid = 1;
		;}
    break;

  case 236:

/* Line 1455 of yacc.c  */
#line 1674 "l2tp_config_parse.y"
    {
			session.flags |= L2TP_API_SESSION_FLAG_SUB_ADDRESS;
			OPTSTRING(session.sub_address) = (yyvsp[(3) - (3)].val).buf;
			session.sub_address.valid = 1;
		;}
    break;

  case 237:

/* Line 1455 of yacc.c  */
#line 1680 "l2tp_config_parse.y"
    {
			char str[16];
			session.flags |= L2TP_API_SESSION_FLAG_SUB_ADDRESS;
			sprintf(str, "%lu", (yyvsp[(3) - (3)].ulnum));
			OPTSTRING(session.sub_address) = strdup(str);
			session.sub_address.valid = 1;
		;}
    break;

  case 238:

/* Line 1455 of yacc.c  */
#line 1688 "l2tp_config_parse.y"
    {
			session.flags |= L2TP_API_SESSION_FLAG_INITIAL_RCVD_LCP_CONFREQ;
			session.initial_rcvd_lcp_confreq.initial_rcvd_lcp_confreq_len = (yyvsp[(3) - (3)].val).len;
			session.initial_rcvd_lcp_confreq.initial_rcvd_lcp_confreq_val = (yyvsp[(3) - (3)].val).buf;
		;}
    break;

  case 239:

/* Line 1455 of yacc.c  */
#line 1694 "l2tp_config_parse.y"
    {
			session.flags |= L2TP_API_SESSION_FLAG_LAST_SENT_LCP_CONFREQ;
			session.last_sent_lcp_confreq.last_sent_lcp_confreq_len = (yyvsp[(3) - (3)].val).len;
			session.last_sent_lcp_confreq.last_sent_lcp_confreq_val = (yyvsp[(3) - (3)].val).buf;
		;}
    break;

  case 240:

/* Line 1455 of yacc.c  */
#line 1700 "l2tp_config_parse.y"
    {
			session.flags |= L2TP_API_SESSION_FLAG_LAST_RCVD_LCP_CONFREQ;
			session.last_rcvd_lcp_confreq.last_rcvd_lcp_confreq_len = (yyvsp[(3) - (3)].val).len;
			session.last_rcvd_lcp_confreq.last_rcvd_lcp_confreq_val = (yyvsp[(3) - (3)].val).buf;
		;}
    break;

  case 241:

/* Line 1455 of yacc.c  */
#line 1706 "l2tp_config_parse.y"
    {
			session.flags |= L2TP_API_SESSION_FLAG_USER_NAME;
			OPTSTRING(session.user_name) = (yyvsp[(3) - (3)].val).buf;
			session.user_name.valid = 1;
		;}
    break;

  case 242:

/* Line 1455 of yacc.c  */
#line 1712 "l2tp_config_parse.y"
    {
			session.flags |= L2TP_API_SESSION_FLAG_USER_NAME;
			OPTSTRING(session.user_name) = (yyvsp[(3) - (3)].val).buf;
			session.user_name.valid = 1;
		;}
    break;

  case 243:

/* Line 1455 of yacc.c  */
#line 1718 "l2tp_config_parse.y"
    {
			session.flags |= L2TP_API_SESSION_FLAG_USER_PASSWORD;
			OPTSTRING(session.user_password) = (yyvsp[(3) - (3)].val).buf;
			session.user_password.valid = 1;
		;}
    break;

  case 244:

/* Line 1455 of yacc.c  */
#line 1724 "l2tp_config_parse.y"
    {
			session.flags |= L2TP_API_SESSION_FLAG_USER_PASSWORD;
			OPTSTRING(session.user_password) = (yyvsp[(3) - (3)].val).buf;
			session.user_password.valid = 1;
		;}
    break;

  case 245:

/* Line 1455 of yacc.c  */
#line 1730 "l2tp_config_parse.y"
    {
			session.flags |= L2TP_API_SESSION_FLAG_USE_SEQUENCE_NUMBERS;
			session.use_sequence_numbers = (yyvsp[(3) - (3)].num);
		;}
    break;

  case 246:

/* Line 1455 of yacc.c  */
#line 1735 "l2tp_config_parse.y"
    {
			session.flags |= L2TP_API_SESSION_FLAG_NO_PPP;
			session.no_ppp = (yyvsp[(3) - (3)].num);
		;}
    break;

  case 247:

/* Line 1455 of yacc.c  */
#line 1740 "l2tp_config_parse.y"
    {
			session.flags |= L2TP_API_SESSION_FLAG_REORDER_TIMEOUT;
			session.reorder_timeout = (yyvsp[(3) - (3)].ulnum);
		;}
    break;

  case 248:

/* Line 1455 of yacc.c  */
#line 1745 "l2tp_config_parse.y"
    {
			session.flags |= L2TP_API_SESSION_FLAG_TUNNEL_NAME;
			OPTSTRING(session.tunnel_name) = (yyvsp[(3) - (3)].val).buf;
			session.tunnel_name.valid = 1;
		;}
    break;

  case 249:

/* Line 1455 of yacc.c  */
#line 1751 "l2tp_config_parse.y"
    {
			session.flags |= L2TP_API_SESSION_FLAG_TUNNEL_NAME;
			OPTSTRING(session.tunnel_name) = (yyvsp[(3) - (3)].val).buf;
			session.tunnel_name.valid = 1;
		;}
    break;

  case 250:

/* Line 1455 of yacc.c  */
#line 1757 "l2tp_config_parse.y"
    {
			session.tunnel_id = (yyvsp[(3) - (3)].ulnum);
		;}
    break;

  case 251:

/* Line 1455 of yacc.c  */
#line 1761 "l2tp_config_parse.y"
    {
			session.flags |= L2TP_API_SESSION_FLAG_SESSION_NAME;
			OPTSTRING(session.session_name) = (yyvsp[(3) - (3)].val).buf;
			session.session_name.valid = 1;
		;}
    break;

  case 252:

/* Line 1455 of yacc.c  */
#line 1767 "l2tp_config_parse.y"
    {
			session.flags |= L2TP_API_SESSION_FLAG_SESSION_NAME;
			OPTSTRING(session.session_name) = (yyvsp[(3) - (3)].val).buf;
			session.session_name.valid = 1;
		;}
    break;

  case 253:

/* Line 1455 of yacc.c  */
#line 1773 "l2tp_config_parse.y"
    {
			session.session_id = (yyvsp[(3) - (3)].ulnum);
		;}
    break;

  case 254:

/* Line 1455 of yacc.c  */
#line 1777 "l2tp_config_parse.y"
    {
			session.flags |= L2TP_API_SESSION_FLAG_INTERFACE_NAME;
			OPTSTRING(session.interface_name) = (yyvsp[(3) - (3)].val).buf;
			session.interface_name.valid = 1;
		;}
    break;

  case 255:

/* Line 1455 of yacc.c  */
#line 1783 "l2tp_config_parse.y"
    {
			session.flags |= L2TP_API_SESSION_FLAG_INTERFACE_NAME;
			OPTSTRING(session.interface_name) = (yyvsp[(3) - (3)].val).buf;
			session.interface_name.valid = 1;
		;}
    break;

  case 256:

/* Line 1455 of yacc.c  */
#line 1789 "l2tp_config_parse.y"
    {
			session.flags |= L2TP_API_SESSION_FLAG_PROFILE_NAME;
			OPTSTRING(session.profile_name) = (yyvsp[(3) - (3)].val).buf;
			session.profile_name.valid = 1;
		;}
    break;

  case 257:

/* Line 1455 of yacc.c  */
#line 1795 "l2tp_config_parse.y"
    {
			session.flags |= L2TP_API_SESSION_FLAG_PROFILE_NAME;
			OPTSTRING(session.profile_name) = (yyvsp[(3) - (3)].val).buf;
			session.profile_name.valid = 1;
		;}
    break;



/* Line 1455 of yacc.c  */
#line 4692 "l2tp_config_parse.c"
      default: break;
    }
  YY_SYMBOL_PRINT ("-> $$ =", yyr1[yyn], &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);

  *++yyvsp = yyval;

  /* Now `shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */

  yyn = yyr1[yyn];

  yystate = yypgoto[yyn - YYNTOKENS] + *yyssp;
  if (0 <= yystate && yystate <= YYLAST && yycheck[yystate] == *yyssp)
    yystate = yytable[yystate];
  else
    yystate = yydefgoto[yyn - YYNTOKENS];

  goto yynewstate;


/*------------------------------------.
| yyerrlab -- here on detecting error |
`------------------------------------*/
yyerrlab:
  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
#if ! YYERROR_VERBOSE
      yyerror (YY_("syntax error"));
#else
      {
	YYSIZE_T yysize = yysyntax_error (0, yystate, yychar);
	if (yymsg_alloc < yysize && yymsg_alloc < YYSTACK_ALLOC_MAXIMUM)
	  {
	    YYSIZE_T yyalloc = 2 * yysize;
	    if (! (yysize <= yyalloc && yyalloc <= YYSTACK_ALLOC_MAXIMUM))
	      yyalloc = YYSTACK_ALLOC_MAXIMUM;
	    if (yymsg != yymsgbuf)
	      YYSTACK_FREE (yymsg);
	    yymsg = (char *) YYSTACK_ALLOC (yyalloc);
	    if (yymsg)
	      yymsg_alloc = yyalloc;
	    else
	      {
		yymsg = yymsgbuf;
		yymsg_alloc = sizeof yymsgbuf;
	      }
	  }

	if (0 < yysize && yysize <= yymsg_alloc)
	  {
	    (void) yysyntax_error (yymsg, yystate, yychar);
	    yyerror (yymsg);
	  }
	else
	  {
	    yyerror (YY_("syntax error"));
	    if (yysize != 0)
	      goto yyexhaustedlab;
	  }
      }
#endif
    }



  if (yyerrstatus == 3)
    {
      /* If just tried and failed to reuse lookahead token after an
	 error, discard it.  */

      if (yychar <= YYEOF)
	{
	  /* Return failure if at end of input.  */
	  if (yychar == YYEOF)
	    YYABORT;
	}
      else
	{
	  yydestruct ("Error: discarding",
		      yytoken, &yylval);
	  yychar = YYEMPTY;
	}
    }

  /* Else will try to reuse lookahead token after shifting the error
     token.  */
  goto yyerrlab1;


/*---------------------------------------------------.
| yyerrorlab -- error raised explicitly by YYERROR.  |
`---------------------------------------------------*/
yyerrorlab:

  /* Pacify compilers like GCC when the user code never invokes
     YYERROR and the label yyerrorlab therefore never appears in user
     code.  */
  if (/*CONSTCOND*/ 0)
     goto yyerrorlab;

  /* Do not reclaim the symbols of the rule which action triggered
     this YYERROR.  */
  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);
  yystate = *yyssp;
  goto yyerrlab1;


/*-------------------------------------------------------------.
| yyerrlab1 -- common code for both syntax error and YYERROR.  |
`-------------------------------------------------------------*/
yyerrlab1:
  yyerrstatus = 3;	/* Each real token shifted decrements this.  */

  for (;;)
    {
      yyn = yypact[yystate];
      if (yyn != YYPACT_NINF)
	{
	  yyn += YYTERROR;
	  if (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYTERROR)
	    {
	      yyn = yytable[yyn];
	      if (0 < yyn)
		break;
	    }
	}

      /* Pop the current state because it cannot handle the error token.  */
      if (yyssp == yyss)
	YYABORT;


      yydestruct ("Error: popping",
		  yystos[yystate], yyvsp);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  *++yyvsp = yylval;


  /* Shift the error token.  */
  YY_SYMBOL_PRINT ("Shifting", yystos[yyn], yyvsp, yylsp);

  yystate = yyn;
  goto yynewstate;


/*-------------------------------------.
| yyacceptlab -- YYACCEPT comes here.  |
`-------------------------------------*/
yyacceptlab:
  yyresult = 0;
  goto yyreturn;

/*-----------------------------------.
| yyabortlab -- YYABORT comes here.  |
`-----------------------------------*/
yyabortlab:
  yyresult = 1;
  goto yyreturn;

#if !defined(yyoverflow) || YYERROR_VERBOSE
/*-------------------------------------------------.
| yyexhaustedlab -- memory exhaustion comes here.  |
`-------------------------------------------------*/
yyexhaustedlab:
  yyerror (YY_("memory exhausted"));
  yyresult = 2;
  /* Fall through.  */
#endif

yyreturn:
  if (yychar != YYEMPTY)
     yydestruct ("Cleanup: discarding lookahead",
		 yytoken, &yylval);
  /* Do not reclaim the symbols of the rule which action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
		  yystos[*yyssp], yyvsp);
      YYPOPSTACK (1);
    }
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif
#if YYERROR_VERBOSE
  if (yymsg != yymsgbuf)
    YYSTACK_FREE (yymsg);
#endif
  /* Make sure YYID is used.  */
  return YYID (yyresult);
}



/* Line 1675 of yacc.c  */
#line 1802 "l2tp_config_parse.y"



