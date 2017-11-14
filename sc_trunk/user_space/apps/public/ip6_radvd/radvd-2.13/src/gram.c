/* A Bison parser, made by GNU Bison 1.875c.  */

/* Skeleton parser for Yacc-like parsing with Bison,
   Copyright (C) 1984, 1989, 1990, 2000, 2001, 2002, 2003 Free Software Foundation, Inc.

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2, or (at your option)
   any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 59 Temple Place - Suite 330,
   Boston, MA 02111-1307, USA.  */

/* As a special exception, when this file is copied by Bison into a
   Bison output file, you may use that output file without restriction.
   This special exception was added by the Free Software Foundation
   in version 1.24 of Bison.  */

/* Written by Richard Stallman by simplifying the original so called
   ``semantic'' parser.  */

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Identify Bison output.  */
#define YYBISON 1

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Using locations.  */
#define YYLSP_NEEDED 0



/* Tokens.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
   /* Put the tokens into the symbol table, so that GDB and other debuggers
      know about them.  */
   enum yytokentype {
     T_INTERFACE = 258,
     T_PREFIX = 259,
     T_ROUTE = 260,
     T_RDNSS = 261,
     T_DNSSL = 262,
     T_CLIENTS = 263,
     T_LOWPANCO = 264,
     T_ABRO = 265,
     STRING = 266,
     NUMBER = 267,
     SIGNEDNUMBER = 268,
     DECIMAL = 269,
     SWITCH = 270,
     IPV6ADDR = 271,
     INFINITY = 272,
     T_IgnoreIfMissing = 273,
     T_AdvSendAdvert = 274,
     T_MaxRtrAdvInterval = 275,
     T_MinRtrAdvInterval = 276,
     T_MinDelayBetweenRAs = 277,
     T_AdvManagedFlag = 278,
     T_AdvOtherConfigFlag = 279,
     T_AdvLinkMTU = 280,
     T_AdvReachableTime = 281,
     T_AdvRetransTimer = 282,
     T_AdvCurHopLimit = 283,
     T_AdvDefaultLifetime = 284,
     T_AdvDefaultPreference = 285,
     T_AdvSourceLLAddress = 286,
     T_AdvOnLink = 287,
     T_AdvAutonomous = 288,
     T_AdvValidLifetime = 289,
     T_AdvPreferredLifetime = 290,
     T_DeprecatePrefix = 291,
     T_DecrementLifetimes = 292,
     T_AdvRouterAddr = 293,
     T_AdvHomeAgentFlag = 294,
     T_AdvIntervalOpt = 295,
     T_AdvHomeAgentInfo = 296,
     T_Base6Interface = 297,
     T_Base6to4Interface = 298,
     T_UnicastOnly = 299,
     T_HomeAgentPreference = 300,
     T_HomeAgentLifetime = 301,
     T_AdvRoutePreference = 302,
     T_AdvRouteLifetime = 303,
     T_RemoveRoute = 304,
     T_AdvRDNSSPreference = 305,
     T_AdvRDNSSOpenFlag = 306,
     T_AdvRDNSSLifetime = 307,
     T_FlushRDNSS = 308,
     T_AdvDNSSLLifetime = 309,
     T_FlushDNSSL = 310,
     T_AdvMobRtrSupportFlag = 311,
     T_AdvContextLength = 312,
     T_AdvContextCompressionFlag = 313,
     T_AdvContextID = 314,
     T_AdvLifeTime = 315,
     T_AdvContextPrefix = 316,
     T_AdvVersionLow = 317,
     T_AdvVersionHigh = 318,
     T_AdvValidLifeTime = 319,
     T_Adv6LBRaddress = 320,
     T_BAD_TOKEN = 321
   };
#endif
#define T_INTERFACE 258
#define T_PREFIX 259
#define T_ROUTE 260
#define T_RDNSS 261
#define T_DNSSL 262
#define T_CLIENTS 263
#define T_LOWPANCO 264
#define T_ABRO 265
#define STRING 266
#define NUMBER 267
#define SIGNEDNUMBER 268
#define DECIMAL 269
#define SWITCH 270
#define IPV6ADDR 271
#define INFINITY 272
#define T_IgnoreIfMissing 273
#define T_AdvSendAdvert 274
#define T_MaxRtrAdvInterval 275
#define T_MinRtrAdvInterval 276
#define T_MinDelayBetweenRAs 277
#define T_AdvManagedFlag 278
#define T_AdvOtherConfigFlag 279
#define T_AdvLinkMTU 280
#define T_AdvReachableTime 281
#define T_AdvRetransTimer 282
#define T_AdvCurHopLimit 283
#define T_AdvDefaultLifetime 284
#define T_AdvDefaultPreference 285
#define T_AdvSourceLLAddress 286
#define T_AdvOnLink 287
#define T_AdvAutonomous 288
#define T_AdvValidLifetime 289
#define T_AdvPreferredLifetime 290
#define T_DeprecatePrefix 291
#define T_DecrementLifetimes 292
#define T_AdvRouterAddr 293
#define T_AdvHomeAgentFlag 294
#define T_AdvIntervalOpt 295
#define T_AdvHomeAgentInfo 296
#define T_Base6Interface 297
#define T_Base6to4Interface 298
#define T_UnicastOnly 299
#define T_HomeAgentPreference 300
#define T_HomeAgentLifetime 301
#define T_AdvRoutePreference 302
#define T_AdvRouteLifetime 303
#define T_RemoveRoute 304
#define T_AdvRDNSSPreference 305
#define T_AdvRDNSSOpenFlag 306
#define T_AdvRDNSSLifetime 307
#define T_FlushRDNSS 308
#define T_AdvDNSSLLifetime 309
#define T_FlushDNSSL 310
#define T_AdvMobRtrSupportFlag 311
#define T_AdvContextLength 312
#define T_AdvContextCompressionFlag 313
#define T_AdvContextID 314
#define T_AdvLifeTime 315
#define T_AdvContextPrefix 316
#define T_AdvVersionLow 317
#define T_AdvVersionHigh 318
#define T_AdvValidLifeTime 319
#define T_Adv6LBRaddress 320
#define T_BAD_TOKEN 321




/* Copy the first part of user declarations.  */
#line 15 "gram.y"

#include "config.h"
#include "includes.h"
#include "radvd.h"
#include "defaults.h"

#include "nvram.h"

#define YYERROR_VERBOSE 1
static int countbits(int b);
static int count_mask(struct sockaddr_in6 *m);
static struct in6_addr get_prefix6(struct in6_addr const *addr, struct in6_addr const *mask);

#if 0 /* no longer necessary? */
#ifndef HAVE_IN6_ADDR_S6_ADDR
# ifdef __FreeBSD__
#  define s6_addr32 __u6_addr.__u6_addr32
#  define s6_addr16 __u6_addr.__u6_addr16
# endif
#endif
#endif

#define ADD_TO_LL(type, list, value) \
	do { \
		if (iface->list == NULL) \
			iface->list = value; \
		else { \
			type *current = iface->list; \
			while (current->next != NULL) \
				current = current->next; \
			current->next = value; \
		} \
	} while (0)



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

#if ! defined (YYSTYPE) && ! defined (YYSTYPE_IS_DECLARED)
#line 137 "gram.y"
typedef union YYSTYPE {
	unsigned int		num;
	int			snum;
	double			dec;
	struct in6_addr		*addr;
	char			*str;
	struct AdvPrefix	*pinfo;
	struct AdvRoute		*rinfo;
	struct AdvRDNSS		*rdnssinfo;
	struct AdvDNSSL		*dnsslinfo;
	struct Clients		*ainfo;
	struct AdvLowpanCo	*lowpancoinfo;
	struct AdvAbro		*abroinfo;
} YYSTYPE;
/* Line 186 of yacc.c.  */
#line 257 "gram.c"
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
# define YYSTYPE_IS_TRIVIAL 1
#endif



/* Copy the second part of user declarations.  */
#line 152 "gram.y"

extern int num_lines;
static char const * filename;
static struct Interface *iface;
static struct Interface *IfaceList;
static struct AdvPrefix *prefix;
static struct AdvRoute *route;
static struct AdvRDNSS *rdnss;
static struct AdvDNSSL *dnssl;
static struct AdvLowpanCo *lowpanco;
static struct AdvAbro  *abro;
static void cleanup(void);
#define ABORT	do { cleanup(); YYABORT; } while (0);
static void yyerror(char const * msg);


/* Line 214 of yacc.c.  */
#line 284 "gram.c"

#if ! defined (yyoverflow) || YYERROR_VERBOSE

# ifndef YYFREE
#  define YYFREE free
# endif
# ifndef YYMALLOC
#  define YYMALLOC malloc
# endif

/* The parser invokes alloca or malloc; define the necessary symbols.  */

# ifdef YYSTACK_USE_ALLOCA
#  if YYSTACK_USE_ALLOCA
#   define YYSTACK_ALLOC alloca
#  endif
# else
#  if defined (alloca) || defined (_ALLOCA_H)
#   define YYSTACK_ALLOC alloca
#  else
#   ifdef __GNUC__
#    define YYSTACK_ALLOC __builtin_alloca
#   endif
#  endif
# endif

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's `empty if-body' warning. */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (0)
# else
#  if defined (__STDC__) || defined (__cplusplus)
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   define YYSIZE_T size_t
#  endif
#  define YYSTACK_ALLOC YYMALLOC
#  define YYSTACK_FREE YYFREE
# endif
#endif /* ! defined (yyoverflow) || YYERROR_VERBOSE */


#if (! defined (yyoverflow) \
     && (! defined (__cplusplus) \
	 || (defined (YYSTYPE_IS_TRIVIAL) && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  short yyss;
  YYSTYPE yyvs;
  };

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (sizeof (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (sizeof (short) + sizeof (YYSTYPE))				\
      + YYSTACK_GAP_MAXIMUM)

/* Copy COUNT objects from FROM to TO.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined (__GNUC__) && 1 < __GNUC__
#   define YYCOPY(To, From, Count) \
      __builtin_memcpy (To, From, (Count) * sizeof (*(From)))
#  else
#   define YYCOPY(To, From, Count)		\
      do					\
	{					\
	  register YYSIZE_T yyi;		\
	  for (yyi = 0; yyi < (Count); yyi++)	\
	    (To)[yyi] = (From)[yyi];		\
	}					\
      while (0)
#  endif
# endif

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack)					\
    do									\
      {									\
	YYSIZE_T yynewbytes;						\
	YYCOPY (&yyptr->Stack, Stack, yysize);				\
	Stack = &yyptr->Stack;						\
	yynewbytes = yystacksize * sizeof (*Stack) + YYSTACK_GAP_MAXIMUM; \
	yyptr += yynewbytes / sizeof (*yyptr);				\
      }									\
    while (0)

#endif

#if defined (__STDC__) || defined (__cplusplus)
   typedef signed char yysigned_char;
#else
   typedef short yysigned_char;
#endif

/* YYFINAL -- State number of the termination state. */
#define YYFINAL  7
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   234

/* YYNTOKENS -- Number of terminals. */
#define YYNTOKENS  71
/* YYNNTS -- Number of nonterminals. */
#define YYNNTS  47
/* YYNRULES -- Number of rules. */
#define YYNRULES  116
/* YYNRULES -- Number of states. */
#define YYNSTATES  251

/* YYTRANSLATE(YYLEX) -- Bison symbol number corresponding to YYLEX.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   321

#define YYTRANSLATE(YYX) 						\
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[YYLEX] -- Bison symbol number corresponding to YYLEX.  */
static const unsigned char yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,    70,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,    69,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,    67,     2,    68,     2,     2,     2,     2,
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
      65,    66
};

#if YYDEBUG
/* YYPRHS[YYN] -- Index of the first RHS symbol of rule number YYN in
   YYRHS.  */
static const unsigned short yyprhs[] =
{
       0,     0,     3,     6,     8,    14,    17,    19,    22,    23,
      25,    27,    29,    31,    33,    35,    37,    39,    43,    47,
      51,    55,    59,    63,    67,    71,    75,    79,    83,    87,
      91,    95,    99,   103,   107,   111,   115,   119,   123,   127,
     131,   135,   141,   144,   148,   152,   157,   158,   161,   165,
     168,   170,   174,   178,   182,   186,   190,   194,   198,   202,
     206,   212,   217,   218,   220,   223,   225,   229,   233,   237,
     243,   246,   248,   250,   253,   254,   256,   259,   261,   265,
     269,   273,   277,   283,   286,   288,   290,   293,   294,   296,
     299,   301,   305,   309,   315,   317,   318,   320,   323,   325,
     329,   333,   337,   341,   347,   349,   351,   354,   359,   360,
     362,   365,   367,   371,   375,   379,   381
};

/* YYRHS -- A `-1'-separated list of the rules' RHS. */
static const yysigned_char yyrhs[] =
{
      72,     0,    -1,    72,    73,    -1,    73,    -1,    74,    67,
      76,    68,    69,    -1,     3,    75,    -1,    11,    -1,    76,
      77,    -1,    -1,    78,    -1,    81,    -1,    79,    -1,    86,
      -1,    91,    -1,    98,    -1,   105,    -1,   110,    -1,    21,
      12,    69,    -1,    20,    12,    69,    -1,    22,    12,    69,
      -1,    21,    14,    69,    -1,    20,    14,    69,    -1,    22,
      14,    69,    -1,    18,    15,    69,    -1,    19,    15,    69,
      -1,    23,    15,    69,    -1,    24,    15,    69,    -1,    25,
      12,    69,    -1,    26,    12,    69,    -1,    27,    12,    69,
      -1,    29,    12,    69,    -1,    30,    13,    69,    -1,    28,
      12,    69,    -1,    31,    15,    69,    -1,    40,    15,    69,
      -1,    41,    15,    69,    -1,    39,    15,    69,    -1,    45,
      12,    69,    -1,    46,    12,    69,    -1,    44,    15,    69,
      -1,    56,    15,    69,    -1,     8,    67,    80,    68,    69,
      -1,    16,    69,    -1,    80,    16,    69,    -1,    82,    83,
      69,    -1,     4,    16,    70,    12,    -1,    -1,    67,    68,
      -1,    67,    84,    68,    -1,    84,    85,    -1,    85,    -1,
      32,    15,    69,    -1,    33,    15,    69,    -1,    38,    15,
      69,    -1,    34,   117,    69,    -1,    35,   117,    69,    -1,
      36,    15,    69,    -1,    37,    15,    69,    -1,    42,    75,
      69,    -1,    43,    75,    69,    -1,    87,    67,    88,    68,
      69,    -1,     5,    16,    70,    12,    -1,    -1,    89,    -1,
      89,    90,    -1,    90,    -1,    47,    13,    69,    -1,    48,
     117,    69,    -1,    49,    15,    69,    -1,    94,    67,    95,
      68,    69,    -1,    92,    93,    -1,    93,    -1,    16,    -1,
       6,    92,    -1,    -1,    96,    -1,    96,    97,    -1,    97,
      -1,    50,    12,    69,    -1,    51,    15,    69,    -1,    52,
     117,    69,    -1,    53,    15,    69,    -1,   101,    67,   102,
      68,    69,    -1,    99,   100,    -1,   100,    -1,    11,    -1,
       7,    99,    -1,    -1,   103,    -1,   103,   104,    -1,   104,
      -1,    54,   117,    69,    -1,    55,    15,    69,    -1,   106,
      67,   107,    68,    69,    -1,     9,    -1,    -1,   108,    -1,
     108,   109,    -1,   109,    -1,    57,    12,    69,    -1,    58,
      15,    69,    -1,    59,    12,    69,    -1,    60,    12,    69,
      -1,   111,    67,   114,    68,    69,    -1,   112,    -1,   113,
      -1,    10,    16,    -1,    10,    16,    70,    12,    -1,    -1,
     115,    -1,   115,   116,    -1,   116,    -1,    62,    12,    69,
      -1,    63,    12,    69,    -1,    64,    12,    69,    -1,    12,
      -1,    17,    -1
};

/* YYRLINE[YYN] -- source line where rule number YYN was defined.  */
static const unsigned short yyrline[] =
{
       0,   171,   171,   172,   175,   185,   214,   221,   222,   225,
     226,   227,   228,   229,   230,   231,   232,   235,   239,   243,
     247,   251,   255,   259,   263,   267,   271,   275,   279,   283,
     287,   291,   295,   299,   303,   307,   311,   315,   319,   323,
     327,   333,   339,   350,   365,   463,   560,   561,   562,   565,
     566,   569,   583,   597,   606,   623,   640,   646,   652,   665,
     679,   687,   711,   712,   715,   716,   720,   724,   728,   734,
     741,   742,   745,   780,   789,   790,   793,   794,   798,   802,
     806,   821,   827,   834,   835,   838,   880,   889,   890,   893,
     894,   898,   910,   916,   923,   936,   937,   940,   941,   944,
     948,   952,   956,   962,   969,   969,   971,   985,  1005,  1006,
    1009,  1010,  1013,  1017,  1021,  1027,  1031
};
#endif

#if YYDEBUG || YYERROR_VERBOSE
/* YYTNME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals. */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "T_INTERFACE", "T_PREFIX", "T_ROUTE",
  "T_RDNSS", "T_DNSSL", "T_CLIENTS", "T_LOWPANCO", "T_ABRO", "STRING",
  "NUMBER", "SIGNEDNUMBER", "DECIMAL", "SWITCH", "IPV6ADDR", "INFINITY",
  "T_IgnoreIfMissing", "T_AdvSendAdvert", "T_MaxRtrAdvInterval",
  "T_MinRtrAdvInterval", "T_MinDelayBetweenRAs", "T_AdvManagedFlag",
  "T_AdvOtherConfigFlag", "T_AdvLinkMTU", "T_AdvReachableTime",
  "T_AdvRetransTimer", "T_AdvCurHopLimit", "T_AdvDefaultLifetime",
  "T_AdvDefaultPreference", "T_AdvSourceLLAddress", "T_AdvOnLink",
  "T_AdvAutonomous", "T_AdvValidLifetime", "T_AdvPreferredLifetime",
  "T_DeprecatePrefix", "T_DecrementLifetimes", "T_AdvRouterAddr",
  "T_AdvHomeAgentFlag", "T_AdvIntervalOpt", "T_AdvHomeAgentInfo",
  "T_Base6Interface", "T_Base6to4Interface", "T_UnicastOnly",
  "T_HomeAgentPreference", "T_HomeAgentLifetime", "T_AdvRoutePreference",
  "T_AdvRouteLifetime", "T_RemoveRoute", "T_AdvRDNSSPreference",
  "T_AdvRDNSSOpenFlag", "T_AdvRDNSSLifetime", "T_FlushRDNSS",
  "T_AdvDNSSLLifetime", "T_FlushDNSSL", "T_AdvMobRtrSupportFlag",
  "T_AdvContextLength", "T_AdvContextCompressionFlag", "T_AdvContextID",
  "T_AdvLifeTime", "T_AdvContextPrefix", "T_AdvVersionLow",
  "T_AdvVersionHigh", "T_AdvValidLifeTime", "T_Adv6LBRaddress",
  "T_BAD_TOKEN", "'{'", "'}'", "';'", "'/'", "$accept", "grammar",
  "ifacedef", "ifacehead", "name", "ifaceparams", "ifaceparam", "ifaceval",
  "clientslist", "v6addrlist", "prefixdef", "prefixhead",
  "optional_prefixplist", "prefixplist", "prefixparms", "routedef",
  "routehead", "optional_routeplist", "routeplist", "routeparms",
  "rdnssdef", "rdnssaddrs", "rdnssaddr", "rdnsshead",
  "optional_rdnssplist", "rdnssplist", "rdnssparms", "dnssldef",
  "dnsslsuffixes", "dnsslsuffix", "dnsslhead", "optional_dnsslplist",
  "dnsslplist", "dnsslparms", "lowpancodef", "lowpancohead",
  "optional_lowpancoplist", "lowpancoplist", "lowpancoparms", "abrodef",
  "abrohead", "abrohead_new", "abrohead_dep", "optional_abroplist",
  "abroplist", "abroparms", "number_or_infinity", 0
};
#endif

# ifdef YYPRINT
/* YYTOKNUM[YYLEX-NUM] -- Internal token number corresponding to
   token YYLEX-NUM.  */
static const unsigned short yytoknum[] =
{
       0,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,   277,   278,   279,   280,   281,   282,   283,   284,
     285,   286,   287,   288,   289,   290,   291,   292,   293,   294,
     295,   296,   297,   298,   299,   300,   301,   302,   303,   304,
     305,   306,   307,   308,   309,   310,   311,   312,   313,   314,
     315,   316,   317,   318,   319,   320,   321,   123,   125,    59,
      47
};
# endif

/* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const unsigned char yyr1[] =
{
       0,    71,    72,    72,    73,    74,    75,    76,    76,    77,
      77,    77,    77,    77,    77,    77,    77,    78,    78,    78,
      78,    78,    78,    78,    78,    78,    78,    78,    78,    78,
      78,    78,    78,    78,    78,    78,    78,    78,    78,    78,
      78,    79,    80,    80,    81,    82,    83,    83,    83,    84,
      84,    85,    85,    85,    85,    85,    85,    85,    85,    85,
      86,    87,    88,    88,    89,    89,    90,    90,    90,    91,
      92,    92,    93,    94,    95,    95,    96,    96,    97,    97,
      97,    97,    98,    99,    99,   100,   101,   102,   102,   103,
     103,   104,   104,   105,   106,   107,   107,   108,   108,   109,
     109,   109,   109,   110,   111,   111,   112,   113,   114,   114,
     115,   115,   116,   116,   116,   117,   117
};

/* YYR2[YYN] -- Number of symbols composing right hand side of rule YYN.  */
static const unsigned char yyr2[] =
{
       0,     2,     2,     1,     5,     2,     1,     2,     0,     1,
       1,     1,     1,     1,     1,     1,     1,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     5,     2,     3,     3,     4,     0,     2,     3,     2,
       1,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       5,     4,     0,     1,     2,     1,     3,     3,     3,     5,
       2,     1,     1,     2,     0,     1,     2,     1,     3,     3,
       3,     3,     5,     2,     1,     1,     2,     0,     1,     2,
       1,     3,     3,     5,     1,     0,     1,     2,     1,     3,
       3,     3,     3,     5,     1,     1,     2,     4,     0,     1,
       2,     1,     3,     3,     3,     1,     1
};

/* YYDEFACT[STATE-NAME] -- Default rule to reduce with in state
   STATE-NUM when YYTABLE doesn't specify something else to do.  Zero
   means the default is an error.  */
static const unsigned char yydefact[] =
{
       0,     0,     0,     3,     0,     6,     5,     1,     2,     8,
       0,     0,     0,     0,     0,     0,    94,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       7,     9,    11,    10,    46,    12,     0,    13,     0,    14,
       0,    15,     0,    16,     0,   104,   105,     0,     0,    72,
      73,    71,    85,    86,    84,     0,   106,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     4,     0,     0,    62,    74,    87,    95,   108,     0,
       0,    70,    83,     0,     0,     0,    23,    24,    18,    21,
      17,    20,    19,    22,    25,    26,    27,    28,    29,    32,
      30,    31,    33,    36,    34,    35,    39,    37,    38,    40,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    47,
       0,    50,    44,     0,     0,     0,     0,    63,    65,     0,
       0,     0,     0,     0,    75,    77,     0,     0,     0,    88,
      90,     0,     0,     0,     0,     0,    96,    98,     0,     0,
       0,     0,   109,   111,    45,    61,    42,     0,     0,   107,
       0,     0,   115,   116,     0,     0,     0,     0,     0,     0,
       0,    48,    49,     0,     0,     0,     0,    64,     0,     0,
       0,     0,     0,    76,     0,     0,     0,    89,     0,     0,
       0,     0,     0,    97,     0,     0,     0,     0,   110,    43,
      41,    51,    52,    54,    55,    56,    57,    53,    58,    59,
      66,    67,    68,    60,    78,    79,    80,    81,    69,    91,
      92,    82,    99,   100,   101,   102,    93,   112,   113,   114,
     103
};

/* YYDEFGOTO[NTERM-NUM]. */
static const short yydefgoto[] =
{
      -1,     2,     3,     4,     6,    10,    40,    41,    42,   104,
      43,    44,    93,   140,   141,    45,    46,   146,   147,   148,
      47,    60,    61,    48,   153,   154,   155,    49,    63,    64,
      50,   158,   159,   160,    51,    52,   165,   166,   167,    53,
      54,    55,    56,   171,   172,   173,   184
};

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
#define YYPACT_NINF -126
static const short yypact[] =
{
      31,    27,    55,  -126,   -16,  -126,  -126,  -126,  -126,  -126,
      -4,    40,    44,    73,    76,    21,  -126,    74,    77,    78,
      49,    71,    72,    79,    80,    84,    85,    86,    87,    88,
      89,    90,    91,    92,    93,    94,    98,    99,    97,    22,
    -126,  -126,  -126,  -126,    36,  -126,    37,  -126,    46,  -126,
      47,  -126,    48,  -126,    50,  -126,  -126,    51,    52,  -126,
      73,  -126,  -126,    76,  -126,   100,    53,    56,    57,    58,
      59,    60,    61,    62,    63,    64,    65,    66,    67,    68,
      69,    70,    75,    81,    82,    83,    95,    96,   101,   102,
     103,  -126,    11,   104,    25,   -42,    23,   -28,    18,   106,
     107,  -126,  -126,   105,    -9,   108,  -126,  -126,  -126,  -126,
    -126,  -126,  -126,  -126,  -126,  -126,  -126,  -126,  -126,  -126,
    -126,  -126,  -126,  -126,  -126,  -126,  -126,  -126,  -126,  -126,
     109,   125,    16,    16,   126,   127,   128,    27,    27,  -126,
      33,  -126,  -126,   132,    16,   131,   110,    25,  -126,   135,
     133,    16,   134,   111,   -42,  -126,    16,   138,   112,    23,
    -126,   142,   140,   144,   145,   113,   -28,  -126,   146,   147,
     148,   114,    18,  -126,  -126,  -126,  -126,   115,   116,  -126,
     117,   118,  -126,  -126,   119,   120,   121,   122,   123,   124,
     129,  -126,  -126,   130,   136,   137,   139,  -126,   141,   143,
     149,   150,   151,  -126,   152,   153,   154,  -126,   155,   156,
     157,   158,   159,  -126,   160,   161,   162,   163,  -126,  -126,
    -126,  -126,  -126,  -126,  -126,  -126,  -126,  -126,  -126,  -126,
    -126,  -126,  -126,  -126,  -126,  -126,  -126,  -126,  -126,  -126,
    -126,  -126,  -126,  -126,  -126,  -126,  -126,  -126,  -126,  -126,
    -126
};

/* YYPGOTO[NTERM-NUM].  */
static const short yypgoto[] =
{
    -126,  -126,   164,  -126,  -125,  -126,  -126,  -126,  -126,  -126,
    -126,  -126,  -126,  -126,    28,  -126,  -126,  -126,  -126,    14,
    -126,  -126,   173,  -126,  -126,  -126,     8,  -126,  -126,   171,
    -126,  -126,  -126,     4,  -126,  -126,  -126,  -126,     1,  -126,
    -126,  -126,  -126,  -126,  -126,    -3,   -94
};

/* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule which
   number is the opposite.  If zero, do what YYDEFACT says.
   If YYTABLE_NINF, syntax error.  */
#define YYTABLE_NINF -1
static const unsigned char yytable[] =
{
      11,    12,    13,    14,    15,    16,    17,   177,   149,   150,
     151,   152,   189,   190,    18,    19,    20,    21,    22,    23,
      24,    25,    26,    27,    28,    29,    30,    31,   182,   161,
     162,   163,   164,   183,     1,    32,    33,    34,     5,   185,
      35,    36,    37,   130,   131,   132,   133,   134,   135,   136,
     194,     9,    38,   137,   138,     7,    57,   200,     1,   178,
      58,    69,   204,    70,    39,   130,   131,   132,   133,   134,
     135,   136,   143,   144,   145,   137,   138,   156,   157,   139,
     168,   169,   170,    71,    73,    72,    74,    62,    65,    59,
      66,    91,    67,    68,    75,    76,    77,    78,    79,    80,
      81,   191,    82,    92,    94,    83,    84,    85,    86,    87,
      88,    89,    90,    95,    96,    97,   103,    98,   174,   175,
     179,    99,   100,   105,   180,   106,   107,   108,   109,   110,
     111,   112,   113,   114,   115,   116,   117,   118,   119,   120,
     181,   186,   187,   188,   121,   193,   195,   198,   199,   201,
     122,   123,   124,   205,   208,   209,   210,   211,   214,   215,
     216,   197,   203,   207,   125,   126,     8,   213,   192,   218,
     127,   128,   129,   142,   176,     0,     0,     0,   196,   202,
     206,   212,   217,     0,   219,   220,   221,   222,   223,   224,
     225,   226,   227,   228,     0,     0,     0,     0,   229,   230,
       0,     0,     0,     0,     0,   231,   232,     0,   233,     0,
     234,     0,   235,     0,     0,     0,     0,     0,   236,   237,
     238,   239,   240,   241,   242,   243,   244,   245,   246,   247,
     248,   249,   250,   101,   102
};

static const short yycheck[] =
{
       4,     5,     6,     7,     8,     9,    10,    16,    50,    51,
      52,    53,   137,   138,    18,    19,    20,    21,    22,    23,
      24,    25,    26,    27,    28,    29,    30,    31,    12,    57,
      58,    59,    60,    17,     3,    39,    40,    41,    11,   133,
      44,    45,    46,    32,    33,    34,    35,    36,    37,    38,
     144,    67,    56,    42,    43,     0,    16,   151,     3,    68,
      16,    12,   156,    14,    68,    32,    33,    34,    35,    36,
      37,    38,    47,    48,    49,    42,    43,    54,    55,    68,
      62,    63,    64,    12,    12,    14,    14,    11,    67,    16,
      16,    69,    15,    15,    15,    15,    12,    12,    12,    12,
      12,    68,    13,    67,    67,    15,    15,    15,    15,    15,
      12,    12,    15,    67,    67,    67,    16,    67,    12,    12,
      12,    70,    70,    70,    15,    69,    69,    69,    69,    69,
      69,    69,    69,    69,    69,    69,    69,    69,    69,    69,
      15,    15,    15,    15,    69,    13,    15,    12,    15,    15,
      69,    69,    69,    15,    12,    15,    12,    12,    12,    12,
      12,   147,   154,   159,    69,    69,     2,   166,   140,   172,
      69,    69,    69,    69,    69,    -1,    -1,    -1,    68,    68,
      68,    68,    68,    -1,    69,    69,    69,    69,    69,    69,
      69,    69,    69,    69,    -1,    -1,    -1,    -1,    69,    69,
      -1,    -1,    -1,    -1,    -1,    69,    69,    -1,    69,    -1,
      69,    -1,    69,    -1,    -1,    -1,    -1,    -1,    69,    69,
      69,    69,    69,    69,    69,    69,    69,    69,    69,    69,
      69,    69,    69,    60,    63
};

/* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
   symbol of state STATE-NUM.  */
static const unsigned char yystos[] =
{
       0,     3,    72,    73,    74,    11,    75,     0,    73,    67,
      76,     4,     5,     6,     7,     8,     9,    10,    18,    19,
      20,    21,    22,    23,    24,    25,    26,    27,    28,    29,
      30,    31,    39,    40,    41,    44,    45,    46,    56,    68,
      77,    78,    79,    81,    82,    86,    87,    91,    94,    98,
     101,   105,   106,   110,   111,   112,   113,    16,    16,    16,
      92,    93,    11,    99,   100,    67,    16,    15,    15,    12,
      14,    12,    14,    12,    14,    15,    15,    12,    12,    12,
      12,    12,    13,    15,    15,    15,    15,    15,    12,    12,
      15,    69,    67,    83,    67,    67,    67,    67,    67,    70,
      70,    93,   100,    16,    80,    70,    69,    69,    69,    69,
      69,    69,    69,    69,    69,    69,    69,    69,    69,    69,
      69,    69,    69,    69,    69,    69,    69,    69,    69,    69,
      32,    33,    34,    35,    36,    37,    38,    42,    43,    68,
      84,    85,    69,    47,    48,    49,    88,    89,    90,    50,
      51,    52,    53,    95,    96,    97,    54,    55,   102,   103,
     104,    57,    58,    59,    60,   107,   108,   109,    62,    63,
      64,   114,   115,   116,    12,    12,    69,    16,    68,    12,
      15,    15,    12,    17,   117,   117,    15,    15,    15,    75,
      75,    68,    85,    13,   117,    15,    68,    90,    12,    15,
     117,    15,    68,    97,   117,    15,    68,   104,    12,    15,
      12,    12,    68,   109,    12,    12,    12,    68,   116,    69,
      69,    69,    69,    69,    69,    69,    69,    69,    69,    69,
      69,    69,    69,    69,    69,    69,    69,    69,    69,    69,
      69,    69,    69,    69,    69,    69,    69,    69,    69,    69,
      69
};

#if ! defined (YYSIZE_T) && defined (__SIZE_TYPE__)
# define YYSIZE_T __SIZE_TYPE__
#endif
#if ! defined (YYSIZE_T) && defined (size_t)
# define YYSIZE_T size_t
#endif
#if ! defined (YYSIZE_T)
# if defined (__STDC__) || defined (__cplusplus)
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# endif
#endif
#if ! defined (YYSIZE_T)
# define YYSIZE_T unsigned int
#endif

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
      YYPOPSTACK;						\
      goto yybackup;						\
    }								\
  else								\
    { 								\
      yyerror ("syntax error: cannot back up");\
      YYERROR;							\
    }								\
while (0)

#define YYTERROR	1
#define YYERRCODE	256

/* YYLLOC_DEFAULT -- Compute the default location (before the actions
   are run).  */

#ifndef YYLLOC_DEFAULT
# define YYLLOC_DEFAULT(Current, Rhs, N)		\
   ((Current).first_line   = (Rhs)[1].first_line,	\
    (Current).first_column = (Rhs)[1].first_column,	\
    (Current).last_line    = (Rhs)[N].last_line,	\
    (Current).last_column  = (Rhs)[N].last_column)
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
} while (0)

# define YYDSYMPRINT(Args)			\
do {						\
  if (yydebug)					\
    yysymprint Args;				\
} while (0)

# define YYDSYMPRINTF(Title, Token, Value, Location)		\
do {								\
  if (yydebug)							\
    {								\
      YYFPRINTF (stderr, "%s ", Title);				\
      yysymprint (stderr, 					\
                  Token, Value);	\
      YYFPRINTF (stderr, "\n");					\
    }								\
} while (0)

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

#if defined (__STDC__) || defined (__cplusplus)
static void
yy_stack_print (short *bottom, short *top)
#else
static void
yy_stack_print (bottom, top)
    short *bottom;
    short *top;
#endif
{
  YYFPRINTF (stderr, "Stack now");
  for (/* Nothing. */; bottom <= top; ++bottom)
    YYFPRINTF (stderr, " %d", *bottom);
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)				\
do {								\
  if (yydebug)							\
    yy_stack_print ((Bottom), (Top));				\
} while (0)


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

#if defined (__STDC__) || defined (__cplusplus)
static void
yy_reduce_print (int yyrule)
#else
static void
yy_reduce_print (yyrule)
    int yyrule;
#endif
{
  int yyi;
  unsigned int yylno = yyrline[yyrule];
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %u), ",
             yyrule - 1, yylno);
  /* Print the symbols being reduced, and their result.  */
  for (yyi = yyprhs[yyrule]; 0 <= yyrhs[yyi]; yyi++)
    YYFPRINTF (stderr, "%s ", yytname [yyrhs[yyi]]);
  YYFPRINTF (stderr, "-> %s\n", yytname [yyr1[yyrule]]);
}

# define YY_REDUCE_PRINT(Rule)		\
do {					\
  if (yydebug)				\
    yy_reduce_print (Rule);		\
} while (0)

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;
#else /* !YYDEBUG */
# define YYDPRINTF(Args)
# define YYDSYMPRINT(Args)
# define YYDSYMPRINTF(Title, Token, Value, Location)
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
   SIZE_MAX < YYSTACK_BYTES (YYMAXDEPTH)
   evaluated with infinite-precision integer arithmetic.  */

#if defined (YYMAXDEPTH) && YYMAXDEPTH == 0
# undef YYMAXDEPTH
#endif

#ifndef YYMAXDEPTH
# define YYMAXDEPTH 10000
#endif



#if YYERROR_VERBOSE

# ifndef yystrlen
#  if defined (__GLIBC__) && defined (_STRING_H)
#   define yystrlen strlen
#  else
/* Return the length of YYSTR.  */
static YYSIZE_T
#   if defined (__STDC__) || defined (__cplusplus)
yystrlen (const char *yystr)
#   else
yystrlen (yystr)
     const char *yystr;
#   endif
{
  register const char *yys = yystr;

  while (*yys++ != '\0')
    continue;

  return yys - yystr - 1;
}
#  endif
# endif

# ifndef yystpcpy
#  if defined (__GLIBC__) && defined (_STRING_H) && defined (_GNU_SOURCE)
#   define yystpcpy stpcpy
#  else
/* Copy YYSRC to YYDEST, returning the address of the terminating '\0' in
   YYDEST.  */
static char *
#   if defined (__STDC__) || defined (__cplusplus)
yystpcpy (char *yydest, const char *yysrc)
#   else
yystpcpy (yydest, yysrc)
     char *yydest;
     const char *yysrc;
#   endif
{
  register char *yyd = yydest;
  register const char *yys = yysrc;

  while ((*yyd++ = *yys++) != '\0')
    continue;

  return yyd - 1;
}
#  endif
# endif

#endif /* !YYERROR_VERBOSE */



#if YYDEBUG
/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

#if defined (__STDC__) || defined (__cplusplus)
static void
yysymprint (FILE *yyoutput, int yytype, YYSTYPE *yyvaluep)
#else
static void
yysymprint (yyoutput, yytype, yyvaluep)
    FILE *yyoutput;
    int yytype;
    YYSTYPE *yyvaluep;
#endif
{
  /* Pacify ``unused variable'' warnings.  */
  (void) yyvaluep;

  if (yytype < YYNTOKENS)
    {
      YYFPRINTF (yyoutput, "token %s (", yytname[yytype]);
# ifdef YYPRINT
      YYPRINT (yyoutput, yytoknum[yytype], *yyvaluep);
# endif
    }
  else
    YYFPRINTF (yyoutput, "nterm %s (", yytname[yytype]);

  switch (yytype)
    {
      default:
        break;
    }
  YYFPRINTF (yyoutput, ")");
}

#endif /* ! YYDEBUG */
/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

#if defined (__STDC__) || defined (__cplusplus)
static void
yydestruct (int yytype, YYSTYPE *yyvaluep)
#else
static void
yydestruct (yytype, yyvaluep)
    int yytype;
    YYSTYPE *yyvaluep;
#endif
{
  /* Pacify ``unused variable'' warnings.  */
  (void) yyvaluep;

  switch (yytype)
    {

      default:
        break;
    }
}


/* Prevent warnings from -Wmissing-prototypes.  */

#ifdef YYPARSE_PARAM
# if defined (__STDC__) || defined (__cplusplus)
int yyparse (void *YYPARSE_PARAM);
# else
int yyparse ();
# endif
#else /* ! YYPARSE_PARAM */
#if defined (__STDC__) || defined (__cplusplus)
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



/*----------.
| yyparse.  |
`----------*/

#ifdef YYPARSE_PARAM
# if defined (__STDC__) || defined (__cplusplus)
int yyparse (void *YYPARSE_PARAM)
# else
int yyparse (YYPARSE_PARAM)
  void *YYPARSE_PARAM;
# endif
#else /* ! YYPARSE_PARAM */
#if defined (__STDC__) || defined (__cplusplus)
int
yyparse (void)
#else
int
yyparse ()

#endif
#endif
{
  
  register int yystate;
  register int yyn;
  int yyresult;
  /* Number of tokens to shift before error messages enabled.  */
  int yyerrstatus;
  /* Lookahead token as an internal (translated) token number.  */
  int yytoken = 0;

  /* Three stacks and their tools:
     `yyss': related to states,
     `yyvs': related to semantic values,
     `yyls': related to locations.

     Refer to the stacks thru separate pointers, to allow yyoverflow
     to reallocate them elsewhere.  */

  /* The state stack.  */
  short	yyssa[YYINITDEPTH];
  short *yyss = yyssa;
  register short *yyssp;

  /* The semantic value stack.  */
  YYSTYPE yyvsa[YYINITDEPTH];
  YYSTYPE *yyvs = yyvsa;
  register YYSTYPE *yyvsp;



#define YYPOPSTACK   (yyvsp--, yyssp--)

  YYSIZE_T yystacksize = YYINITDEPTH;

  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;


  /* When reducing, the number of symbols on the RHS of the reduced
     rule.  */
  int yylen;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yystate = 0;
  yyerrstatus = 0;
  yynerrs = 0;
  yychar = YYEMPTY;		/* Cause a token to be read.  */

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
     have just been pushed. so pushing a state here evens the stacks.
     */
  yyssp++;

 yysetstate:
  *yyssp = yystate;

  if (yyss + yystacksize - 1 <= yyssp)
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYSIZE_T yysize = yyssp - yyss + 1;

#ifdef yyoverflow
      {
	/* Give user a chance to reallocate the stack. Use copies of
	   these so that the &'s don't force the real ones into
	   memory.  */
	YYSTYPE *yyvs1 = yyvs;
	short *yyss1 = yyss;


	/* Each stack pointer address is followed by the size of the
	   data in use in that stack, in bytes.  This used to be a
	   conditional around just the two extra args, but that might
	   be undefined if yyoverflow is a macro.  */
	yyoverflow ("parser stack overflow",
		    &yyss1, yysize * sizeof (*yyssp),
		    &yyvs1, yysize * sizeof (*yyvsp),

		    &yystacksize);

	yyss = yyss1;
	yyvs = yyvs1;
      }
#else /* no yyoverflow */
# ifndef YYSTACK_RELOCATE
      goto yyoverflowlab;
# else
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
	goto yyoverflowlab;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
	yystacksize = YYMAXDEPTH;

      {
	short *yyss1 = yyss;
	union yyalloc *yyptr =
	  (union yyalloc *) YYSTACK_ALLOC (YYSTACK_BYTES (yystacksize));
	if (! yyptr)
	  goto yyoverflowlab;
	YYSTACK_RELOCATE (yyss);
	YYSTACK_RELOCATE (yyvs);

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

  goto yybackup;

/*-----------.
| yybackup.  |
`-----------*/
yybackup:

/* Do appropriate processing given the current state.  */
/* Read a lookahead token if we need one and don't already have one.  */
/* yyresume: */

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
      YYDSYMPRINTF ("Next token is", yytoken, &yylval, &yylloc);
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

  if (yyn == YYFINAL)
    YYACCEPT;

  /* Shift the lookahead token.  */
  YYDPRINTF ((stderr, "Shifting token %s, ", yytname[yytoken]));

  /* Discard the token being shifted unless it is eof.  */
  if (yychar != YYEOF)
    yychar = YYEMPTY;

  *++yyvsp = yylval;


  /* Count tokens shifted since error; after three, turn off error
     status.  */
  if (yyerrstatus)
    yyerrstatus--;

  yystate = yyn;
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
#line 176 "gram.y"
    {
			dlog(LOG_DEBUG, 4, "%s interface definition ok", iface->props.name);

			iface->next = IfaceList;
			IfaceList = iface;

			iface = NULL;
		}
    break;

  case 5:
#line 186 "gram.y"
    {
			iface = IfaceList;

			while (iface)
			{
				if (!strcmp(yyvsp[0].str, iface->props.name))
				{
					flog(LOG_ERR, "duplicate interface "
						"definition for %s", yyvsp[0].str);
					ABORT;
				}
				iface = iface->next;
			}

			iface = malloc(sizeof(struct Interface));

			if (iface == NULL) {
				flog(LOG_CRIT, "malloc failed: %s", strerror(errno));
				ABORT;
			}

			iface_init_defaults(iface);
			strncpy(iface->props.name, yyvsp[0].str, IFNAMSIZ-1);
			iface->props.name[IFNAMSIZ-1] = '\0';
			iface->lineno = num_lines;
		}
    break;

  case 6:
#line 215 "gram.y"
    {
			/* check vality */
			yyval.str = yyvsp[0].str;
		}
    break;

  case 10:
#line 226 "gram.y"
    { ADD_TO_LL(struct AdvPrefix, AdvPrefixList, yyvsp[0].pinfo); }
    break;

  case 11:
#line 227 "gram.y"
    { ADD_TO_LL(struct Clients, ClientList, yyvsp[0].ainfo); }
    break;

  case 12:
#line 228 "gram.y"
    { ADD_TO_LL(struct AdvRoute, AdvRouteList, yyvsp[0].rinfo); }
    break;

  case 13:
#line 229 "gram.y"
    { ADD_TO_LL(struct AdvRDNSS, AdvRDNSSList, yyvsp[0].rdnssinfo); }
    break;

  case 14:
#line 230 "gram.y"
    { ADD_TO_LL(struct AdvDNSSL, AdvDNSSLList, yyvsp[0].dnsslinfo); }
    break;

  case 15:
#line 231 "gram.y"
    { ADD_TO_LL(struct AdvLowpanCo, AdvLowpanCoList, yyvsp[0].lowpancoinfo); }
    break;

  case 16:
#line 232 "gram.y"
    { ADD_TO_LL(struct AdvAbro, AdvAbroList, yyvsp[0].abroinfo); }
    break;

  case 17:
#line 236 "gram.y"
    {
			iface->MinRtrAdvInterval = yyvsp[-1].num;
		}
    break;

  case 18:
#line 240 "gram.y"
    {
			iface->MaxRtrAdvInterval = yyvsp[-1].num;
		}
    break;

  case 19:
#line 244 "gram.y"
    {
			iface->MinDelayBetweenRAs = yyvsp[-1].num;
		}
    break;

  case 20:
#line 248 "gram.y"
    {
			iface->MinRtrAdvInterval = yyvsp[-1].dec;
		}
    break;

  case 21:
#line 252 "gram.y"
    {
			iface->MaxRtrAdvInterval = yyvsp[-1].dec;
		}
    break;

  case 22:
#line 256 "gram.y"
    {
			iface->MinDelayBetweenRAs = yyvsp[-1].dec;
		}
    break;

  case 23:
#line 260 "gram.y"
    {
			iface->IgnoreIfMissing = yyvsp[-1].num;
		}
    break;

  case 24:
#line 264 "gram.y"
    {
			iface->AdvSendAdvert = yyvsp[-1].num;
		}
    break;

  case 25:
#line 268 "gram.y"
    {
			iface->ra_header_info.AdvManagedFlag = yyvsp[-1].num;
		}
    break;

  case 26:
#line 272 "gram.y"
    {
			iface->ra_header_info.AdvOtherConfigFlag = yyvsp[-1].num;
		}
    break;

  case 27:
#line 276 "gram.y"
    {
			iface->AdvLinkMTU = yyvsp[-1].num;
		}
    break;

  case 28:
#line 280 "gram.y"
    {
			iface->ra_header_info.AdvReachableTime = yyvsp[-1].num;
		}
    break;

  case 29:
#line 284 "gram.y"
    {
			iface->ra_header_info.AdvRetransTimer = yyvsp[-1].num;
		}
    break;

  case 30:
#line 288 "gram.y"
    {
			iface->ra_header_info.AdvDefaultLifetime = yyvsp[-1].num;
		}
    break;

  case 31:
#line 292 "gram.y"
    {
			iface->ra_header_info.AdvDefaultPreference = yyvsp[-1].snum;
		}
    break;

  case 32:
#line 296 "gram.y"
    {
			iface->ra_header_info.AdvCurHopLimit = yyvsp[-1].num;
		}
    break;

  case 33:
#line 300 "gram.y"
    {
			iface->AdvSourceLLAddress = yyvsp[-1].num;
		}
    break;

  case 34:
#line 304 "gram.y"
    {
			iface->mipv6.AdvIntervalOpt = yyvsp[-1].num;
		}
    break;

  case 35:
#line 308 "gram.y"
    {
			iface->mipv6.AdvHomeAgentInfo = yyvsp[-1].num;
		}
    break;

  case 36:
#line 312 "gram.y"
    {
			iface->ra_header_info.AdvHomeAgentFlag = yyvsp[-1].num;
		}
    break;

  case 37:
#line 316 "gram.y"
    {
			iface->mipv6.HomeAgentPreference = yyvsp[-1].num;
		}
    break;

  case 38:
#line 320 "gram.y"
    {
			iface->mipv6.HomeAgentLifetime = yyvsp[-1].num;
		}
    break;

  case 39:
#line 324 "gram.y"
    {
			iface->UnicastOnly = yyvsp[-1].num;
		}
    break;

  case 40:
#line 328 "gram.y"
    {
			iface->mipv6.AdvMobRtrSupportFlag = yyvsp[-1].num;
		}
    break;

  case 41:
#line 334 "gram.y"
    {
			yyval.ainfo = yyvsp[-2].ainfo;
		}
    break;

  case 42:
#line 340 "gram.y"
    {
			struct Clients *new = calloc(1, sizeof(struct Clients));
			if (new == NULL) {
				flog(LOG_CRIT, "calloc failed: %s", strerror(errno));
				ABORT;
			}

			memcpy(&(new->Address), yyvsp[-1].addr, sizeof(struct in6_addr));
			yyval.ainfo = new;
		}
    break;

  case 43:
#line 351 "gram.y"
    {
			struct Clients *new = calloc(1, sizeof(struct Clients));
			if (new == NULL) {
				flog(LOG_CRIT, "calloc failed: %s", strerror(errno));
				ABORT;
			}

			memcpy(&(new->Address), yyvsp[-1].addr, sizeof(struct in6_addr));
			new->next = yyvsp[-2].ainfo;
			yyval.ainfo = new;
		}
    break;

  case 44:
#line 366 "gram.y"
    {
			if (prefix) {
				unsigned int dst;

				if (prefix->AdvPreferredLifetime > prefix->AdvValidLifetime)
				{
					flog(LOG_ERR, "AdvValidLifeTime must be "
						"greater than AdvPreferredLifetime in %s, line %d",
						filename, num_lines);
					ABORT;
				}

				if ( prefix->if6[0] && prefix->if6to4[0]) {
					flog(LOG_ERR, "Base6Interface and Base6to4Interface are mutually exclusive at this time");
					ABORT;
				}

				if ( prefix->if6to4[0] )
				{
					if (get_v4addr(prefix->if6to4, &dst) < 0)
					{
						flog(LOG_ERR, "interface %s has no IPv4 addresses, disabling 6to4 prefix", prefix->if6to4 );
						prefix->enabled = 0;
					} else
					{
                    if(strcmp(nvram_safe_get("ip6_mode"),"6rd") !=0)
                    {
                    		int force_prefix = 1;
	                    	if (strcmp(nvram_safe_get("ip6_mode"),"auto")==0)
	                    	{
								FILE * fp;
								char result[80] = "";
								fp = fopen("/tmp/ipv6_wizard.out", "r");
								if (fp) 
								{
									fscanf(fp, "%s", result);
									if (strcmp(result, "6rd") == 0)
									{
										system("/usr/sbin/echo radvd auto mode result is 6rd so not need force prefix > /dev/console");
										force_prefix = 0;
									}
									fclose(fp);
								}
	                    	}
                    		
                    		if (force_prefix)
                    		{
                    			system("/usr/sbin/echo force prefix since mode is 6to4 or auto mode is 6to4  > /dev/console");
						*((uint16_t *)(prefix->Prefix.s6_addr)) = htons(0x2002);
						memcpy( prefix->Prefix.s6_addr + 2, &dst, sizeof( dst ) );
					}
                    }
					}
				}

				if ( prefix->if6[0] )
				{
#ifndef HAVE_IFADDRS_H
					flog(LOG_ERR, "Base6Interface not supported in %s, line %d", filename, num_lines);
					ABORT;
#else
					struct ifaddrs *ifap = 0, *ifa = 0;
					struct AdvPrefix *next = prefix->next;

					if (prefix->PrefixLen != 64) {
						flog(LOG_ERR, "only /64 is allowed with Base6Interface.  %s:%d", filename, num_lines);
						ABORT;
					}

					if (getifaddrs(&ifap) != 0)
						flog(LOG_ERR, "getifaddrs failed: %s", strerror(errno));

					for (ifa = ifap; ifa; ifa = ifa->ifa_next) {
						struct sockaddr_in6 *s6 = 0;
						struct sockaddr_in6 *mask = (struct sockaddr_in6 *)ifa->ifa_netmask;
						struct in6_addr base6prefix;
						char buf[INET6_ADDRSTRLEN];
						int i;

						if (strncmp(ifa->ifa_name, prefix->if6, IFNAMSIZ))
							continue;

						if (ifa->ifa_addr->sa_family != AF_INET6)
							continue;

						s6 = (struct sockaddr_in6 *)(ifa->ifa_addr);

						if (IN6_IS_ADDR_LINKLOCAL(&s6->sin6_addr))
							continue;

						base6prefix = get_prefix6(&s6->sin6_addr, &mask->sin6_addr);
						for (i = 0; i < 8; ++i) {
							prefix->Prefix.s6_addr[i] &= ~mask->sin6_addr.s6_addr[i];
							prefix->Prefix.s6_addr[i] |= base6prefix.s6_addr[i];
						}
						memset(&prefix->Prefix.s6_addr[8], 0, 8);
						prefix->AdvRouterAddr = 1;
						prefix->AutoSelected = 1;
						prefix->next = next;

						if (inet_ntop(ifa->ifa_addr->sa_family, (void *)&(prefix->Prefix), buf, sizeof(buf)) == NULL)
							flog(LOG_ERR, "%s: inet_ntop failed in %s, line %d!", ifa->ifa_name, filename, num_lines);
						else
							dlog(LOG_DEBUG, 3, "auto-selected prefix %s/%d on interface %s from interface %s",
								buf, prefix->PrefixLen, iface->props.name, ifa->ifa_name);

						/* Taking only one prefix from the Base6Interface.  Taking more than one would require allocating new
						   prefixes and building a list.  I'm not sure how to do that from here. So for now, break. */
						break;
					}

					if (ifap)
						freeifaddrs(ifap);
#endif /* ifndef HAVE_IFADDRS_H */
				}
			}
			yyval.pinfo = prefix;
			prefix = NULL;
		}
    break;

  case 45:
#line 464 "gram.y"
    {
			struct in6_addr zeroaddr;
			memset(&zeroaddr, 0, sizeof(zeroaddr));

			if (!memcmp(yyvsp[-2].addr, &zeroaddr, sizeof(struct in6_addr))) {
#ifndef HAVE_IFADDRS_H
				flog(LOG_ERR, "invalid all-zeros prefix in %s, line %d", filename, num_lines);
				ABORT;
#else
				struct ifaddrs *ifap = 0, *ifa = 0;
				struct AdvPrefix *next = iface->AdvPrefixList;

				while (next) {
					if (next->AutoSelected) {
						flog(LOG_ERR, "auto selecting prefixes works only once per interface.  See %s, line %d", filename, num_lines);
						ABORT;
					}
					next = next->next;
				}
				next = 0;

				dlog(LOG_DEBUG, 5, "all-zeros prefix in %s, line %d", filename, num_lines);

				if (getifaddrs(&ifap) != 0)
					flog(LOG_ERR, "getifaddrs failed: %s", strerror(errno));

				for (ifa = ifap; ifa; ifa = ifa->ifa_next) {
					struct sockaddr_in6 *s6 = (struct sockaddr_in6 *)ifa->ifa_addr;
					struct sockaddr_in6 *mask = (struct sockaddr_in6 *)ifa->ifa_netmask;
					char buf[INET6_ADDRSTRLEN];

					if (strncmp(ifa->ifa_name, iface->props.name, IFNAMSIZ))
						continue;

					if (ifa->ifa_addr->sa_family != AF_INET6)
						continue;

					s6 = (struct sockaddr_in6 *)(ifa->ifa_addr);

					if (IN6_IS_ADDR_LINKLOCAL(&s6->sin6_addr))
						continue;

					prefix = malloc(sizeof(struct AdvPrefix));

					if (prefix == NULL) {
						flog(LOG_CRIT, "malloc failed: %s", strerror(errno));
						ABORT;
					}

					prefix_init_defaults(prefix);
					prefix->Prefix = get_prefix6(&s6->sin6_addr, &mask->sin6_addr);
					prefix->AdvRouterAddr = 1;
					prefix->AutoSelected = 1;
					prefix->next = next;
					next = prefix;

					if (prefix->PrefixLen == 0)
						prefix->PrefixLen = count_mask(mask);

					if (inet_ntop(ifa->ifa_addr->sa_family, (void *)&(prefix->Prefix), buf, sizeof(buf)) == NULL)
						flog(LOG_ERR, "%s: inet_ntop failed in %s, line %d!", ifa->ifa_name, filename, num_lines);
					else
						dlog(LOG_DEBUG, 3, "auto-selected prefix %s/%d on interface %s", buf, prefix->PrefixLen, ifa->ifa_name);
				}

				if (!prefix) {
					flog(LOG_WARNING, "no auto-selected prefix on interface %s, disabling advertisements",  iface->props.name);
				}

				if (ifap)
					freeifaddrs(ifap);
#endif /* ifndef HAVE_IFADDRS_H */
			}
			else {
				prefix = malloc(sizeof(struct AdvPrefix));

				if (prefix == NULL) {
					flog(LOG_CRIT, "malloc failed: %s", strerror(errno));
					ABORT;
				}

				prefix_init_defaults(prefix);

				if (yyvsp[0].num > MAX_PrefixLen)
				{
					flog(LOG_ERR, "invalid prefix length in %s, line %d", filename, num_lines);
					ABORT;
				}

				prefix->PrefixLen = yyvsp[0].num;

				memcpy(&prefix->Prefix, yyvsp[-2].addr, sizeof(struct in6_addr));
			}
		}
    break;

  case 51:
#line 570 "gram.y"
    {
			if (prefix) {
				if (prefix->AutoSelected) {
					struct AdvPrefix *p = prefix;
					do {
						p->AdvOnLinkFlag = yyvsp[-1].num;
						p = p->next;
					} while (p && p->AutoSelected);
				}
				else
					prefix->AdvOnLinkFlag = yyvsp[-1].num;
			}
		}
    break;

  case 52:
#line 584 "gram.y"
    {
			if (prefix) {
				if (prefix->AutoSelected) {
					struct AdvPrefix *p = prefix;
					do {
						p->AdvAutonomousFlag = yyvsp[-1].num;
						p = p->next;
					} while (p && p->AutoSelected);
				}
				else
					prefix->AdvAutonomousFlag = yyvsp[-1].num;
			}
		}
    break;

  case 53:
#line 598 "gram.y"
    {
			if (prefix) {
				if (prefix->AutoSelected && yyvsp[-1].num == 0)
					flog(LOG_WARNING, "prefix automatically selected, AdvRouterAddr always enabled, ignoring config line %d", num_lines);
				else
					prefix->AdvRouterAddr = yyvsp[-1].num;
			}
		}
    break;

  case 54:
#line 607 "gram.y"
    {
			if (prefix) {
				if (prefix->AutoSelected) {
					struct AdvPrefix *p = prefix;
					do {
						p->AdvValidLifetime = yyvsp[-1].num;
						p->curr_validlft = yyvsp[-1].num;
						p = p->next;
					} while (p && p->AutoSelected);
				}
				else {
					prefix->AdvValidLifetime = yyvsp[-1].num;
					prefix->curr_validlft = yyvsp[-1].num;
				}
			}
		}
    break;

  case 55:
#line 624 "gram.y"
    {
			if (prefix) {
				if (prefix->AutoSelected) {
					struct AdvPrefix *p = prefix;
					do {
						p->AdvPreferredLifetime = yyvsp[-1].num;
						p->curr_preferredlft = yyvsp[-1].num;
						p = p->next;
					} while (p && p->AutoSelected);
				}
				else {
					prefix->AdvPreferredLifetime = yyvsp[-1].num;
					prefix->curr_preferredlft = yyvsp[-1].num;
				}
			}
		}
    break;

  case 56:
#line 641 "gram.y"
    {
			if (prefix) {
				prefix->DeprecatePrefixFlag = yyvsp[-1].num;
			}
		}
    break;

  case 57:
#line 647 "gram.y"
    {
			if (prefix) {
				prefix->DecrementLifetimesFlag = yyvsp[-1].num;
			}
		}
    break;

  case 58:
#line 653 "gram.y"
    {
			if (prefix) {
				if (prefix->AutoSelected) {
					flog(LOG_ERR, "automatically selecting the prefix and Base6Interface are mutually exclusive");
					ABORT;
				} /* fallthrough */
				dlog(LOG_DEBUG, 4, "using prefixes on interface %s for prefixes on interface %s", yyvsp[-1].str, iface->props.name);
				strncpy(prefix->if6, yyvsp[-1].str, IFNAMSIZ-1);
				prefix->if6[IFNAMSIZ-1] = '\0';
			}
		}
    break;

  case 59:
#line 666 "gram.y"
    {
			if (prefix) {
				if (prefix->AutoSelected) {
					flog(LOG_ERR, "automatically selecting the prefix and Base6to4Interface are mutually exclusive");
					ABORT;
				} /* fallthrough */
				dlog(LOG_DEBUG, 4, "using interface %s for 6to4 prefixes on interface %s", yyvsp[-1].str, iface->props.name);
				strncpy(prefix->if6to4, yyvsp[-1].str, IFNAMSIZ-1);
				prefix->if6to4[IFNAMSIZ-1] = '\0';
			}
		}
    break;

  case 60:
#line 680 "gram.y"
    {
			yyval.rinfo = route;
			route = NULL;
		}
    break;

  case 61:
#line 688 "gram.y"
    {
			route = malloc(sizeof(struct AdvRoute));

			if (route == NULL) {
				flog(LOG_CRIT, "malloc failed: %s", strerror(errno));
				ABORT;
			}

			route_init_defaults(route, iface);

			if (yyvsp[0].num > MAX_PrefixLen)
			{
				flog(LOG_ERR, "invalid route prefix length in %s, line %d", filename, num_lines);
				ABORT;
			}

			route->PrefixLen = yyvsp[0].num;

			memcpy(&route->Prefix, yyvsp[-2].addr, sizeof(struct in6_addr));
		}
    break;

  case 66:
#line 721 "gram.y"
    {
			route->AdvRoutePreference = yyvsp[-1].snum;
		}
    break;

  case 67:
#line 725 "gram.y"
    {
			route->AdvRouteLifetime = yyvsp[-1].num;
		}
    break;

  case 68:
#line 729 "gram.y"
    {
			route->RemoveRouteFlag = yyvsp[-1].num;
		}
    break;

  case 69:
#line 735 "gram.y"
    {
			yyval.rdnssinfo = rdnss;
			rdnss = NULL;
		}
    break;

  case 72:
#line 746 "gram.y"
    {
			if (!rdnss) {
				/* first IP found */
				rdnss = malloc(sizeof(struct AdvRDNSS));

				if (rdnss == NULL) {
					flog(LOG_CRIT, "malloc failed: %s", strerror(errno));
					ABORT;
				}

				rdnss_init_defaults(rdnss, iface);
			}

			switch (rdnss->AdvRDNSSNumber) {
				case 0:
					memcpy(&rdnss->AdvRDNSSAddr1, yyvsp[0].addr, sizeof(struct in6_addr));
					rdnss->AdvRDNSSNumber++;
					break;
				case 1:
					memcpy(&rdnss->AdvRDNSSAddr2, yyvsp[0].addr, sizeof(struct in6_addr));
					rdnss->AdvRDNSSNumber++;
					break;
				case 2:
					memcpy(&rdnss->AdvRDNSSAddr3, yyvsp[0].addr, sizeof(struct in6_addr));
					rdnss->AdvRDNSSNumber++;
					break;
				default:
					flog(LOG_CRIT, "too many addresses in RDNSS section");
					ABORT;
			}

		}
    break;

  case 73:
#line 781 "gram.y"
    {
			if (!rdnss) {
				flog(LOG_CRIT, "no address specified in RDNSS section");
				ABORT;
			}
		}
    break;

  case 78:
#line 799 "gram.y"
    {
			flog(LOG_WARNING, "ignoring deprecated RDNSS preference");
		}
    break;

  case 79:
#line 803 "gram.y"
    {
			flog(LOG_WARNING, "ignoring deprecated RDNSS open flag");
		}
    break;

  case 80:
#line 807 "gram.y"
    {
			if (yyvsp[-1].num > 2*(iface->MaxRtrAdvInterval))
				flog(LOG_WARNING, "warning: AdvRDNSSLifetime <= 2*MaxRtrAdvInterval would allow stale DNS servers to be deleted faster");
			if (yyvsp[-1].num < iface->MaxRtrAdvInterval && yyvsp[-1].num != 0) {
				flog(LOG_ERR, "AdvRDNSSLifetime must be at least MaxRtrAdvInterval");
				rdnss->AdvRDNSSLifetime = iface->MaxRtrAdvInterval;
			} else {
				rdnss->AdvRDNSSLifetime = yyvsp[-1].num;
			}
			if (yyvsp[-1].num > 2*(iface->MaxRtrAdvInterval))
				flog(LOG_WARNING, "warning: (%s:%d) AdvRDNSSLifetime <= 2*MaxRtrAdvInterval would allow stale DNS servers to be deleted faster", filename, num_lines);

			rdnss->AdvRDNSSLifetime = yyvsp[-1].num;
		}
    break;

  case 81:
#line 822 "gram.y"
    {
			rdnss->FlushRDNSSFlag = yyvsp[-1].num;
		}
    break;

  case 82:
#line 828 "gram.y"
    {
			yyval.dnsslinfo = dnssl;
			dnssl = NULL;
		}
    break;

  case 85:
#line 839 "gram.y"
    {
			char *ch;
			for (ch = yyvsp[0].str;*ch != '\0';ch++) {
				if (*ch >= 'A' && *ch <= 'Z')
					continue;
				if (*ch >= 'a' && *ch <= 'z')
					continue;
				if (*ch >= '0' && *ch <= '9')
					continue;
				if (*ch == '-' || *ch == '.')
					continue;

				flog(LOG_CRIT, "invalid domain suffix specified");
				ABORT;
			}

			if (!dnssl) {
				/* first domain found */
				dnssl = malloc(sizeof(struct AdvDNSSL));

				if (dnssl == NULL) {
					flog(LOG_CRIT, "malloc failed: %s", strerror(errno));
					ABORT;
				}

				dnssl_init_defaults(dnssl, iface);
			}

			dnssl->AdvDNSSLNumber++;
			dnssl->AdvDNSSLSuffixes =
				realloc(dnssl->AdvDNSSLSuffixes,
					dnssl->AdvDNSSLNumber * sizeof(char*));
			if (dnssl->AdvDNSSLSuffixes == NULL) {
				flog(LOG_CRIT, "realloc failed: %s", strerror(errno));
				ABORT;
			}

			dnssl->AdvDNSSLSuffixes[dnssl->AdvDNSSLNumber - 1] = strdup(yyvsp[0].str);
		}
    break;

  case 86:
#line 881 "gram.y"
    {
			if (!dnssl) {
				flog(LOG_CRIT, "no domain specified in DNSSL section");
				ABORT;
			}
		}
    break;

  case 91:
#line 899 "gram.y"
    {
			if (yyvsp[-1].num > 2*(iface->MaxRtrAdvInterval))
				flog(LOG_WARNING, "warning: AdvDNSSLLifetime <= 2*MaxRtrAdvInterval would allow stale DNS suffixes to be deleted faster");
			if (yyvsp[-1].num < iface->MaxRtrAdvInterval && yyvsp[-1].num != 0) {
				flog(LOG_ERR, "AdvDNSSLLifetime must be at least MaxRtrAdvInterval");
				dnssl->AdvDNSSLLifetime = iface->MaxRtrAdvInterval;
			} else {
				dnssl->AdvDNSSLLifetime = yyvsp[-1].num;
			}

		}
    break;

  case 92:
#line 911 "gram.y"
    {
			dnssl->FlushDNSSLFlag = yyvsp[-1].num;
		}
    break;

  case 93:
#line 917 "gram.y"
    {
			yyval.lowpancoinfo = lowpanco;
			lowpanco = NULL;
		}
    break;

  case 94:
#line 924 "gram.y"
    {
			lowpanco = malloc(sizeof(struct AdvLowpanCo));

			if (lowpanco == NULL) {
				flog(LOG_CRIT, "malloc failed: %s", strerror(errno));
				ABORT;
			}

			memset(lowpanco, 0, sizeof(struct AdvLowpanCo));
		}
    break;

  case 99:
#line 945 "gram.y"
    {
			lowpanco->ContextLength = yyvsp[-1].num;
		}
    break;

  case 100:
#line 949 "gram.y"
    {
			lowpanco->ContextCompressionFlag = yyvsp[-1].num;
		}
    break;

  case 101:
#line 953 "gram.y"
    {
			lowpanco->AdvContextID = yyvsp[-1].num;
		}
    break;

  case 102:
#line 957 "gram.y"
    {
			lowpanco->AdvLifeTime = yyvsp[-1].num;
		}
    break;

  case 103:
#line 963 "gram.y"
    {
			yyval.abroinfo = abro;
			abro = NULL;
		}
    break;

  case 106:
#line 972 "gram.y"
    {
			abro = malloc(sizeof(struct AdvAbro));

			if (abro == NULL) {
				flog(LOG_CRIT, "malloc failed: %s", strerror(errno));
				ABORT;
			}

			memset(abro, 0, sizeof(struct AdvAbro));
			memcpy(&abro->LBRaddress, yyvsp[0].addr, sizeof(struct in6_addr));
		}
    break;

  case 107:
#line 986 "gram.y"
    {
			flog(LOG_WARNING
				, "%s:%d abro prefix length deprecated, remove trailing '/%d'"
				, filename
				, num_lines
				, yyvsp[0].num
			);
			abro = malloc(sizeof(struct AdvAbro));

			if (abro == NULL) {
				flog(LOG_CRIT, "malloc failed: %s", strerror(errno));
				ABORT;
			}

			memset(abro, 0, sizeof(struct AdvAbro));
			memcpy(&abro->LBRaddress, yyvsp[-2].addr, sizeof(struct in6_addr));
		}
    break;

  case 112:
#line 1014 "gram.y"
    {
			abro->Version[1] = yyvsp[-1].num;
		}
    break;

  case 113:
#line 1018 "gram.y"
    {
			abro->Version[0] = yyvsp[-1].num;
		}
    break;

  case 114:
#line 1022 "gram.y"
    {
			abro->ValidLifeTime = yyvsp[-1].num;
		}
    break;

  case 115:
#line 1028 "gram.y"
    {
				yyval.num = yyvsp[0].num;
			}
    break;

  case 116:
#line 1032 "gram.y"
    {
				yyval.num = (uint32_t)~0;
			}
    break;


    }

/* Line 1000 of yacc.c.  */
#line 2384 "gram.c"

  yyvsp -= yylen;
  yyssp -= yylen;


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
#if YYERROR_VERBOSE
      yyn = yypact[yystate];

      if (YYPACT_NINF < yyn && yyn < YYLAST)
	{
	  YYSIZE_T yysize = 0;
	  int yytype = YYTRANSLATE (yychar);
	  const char* yyprefix;
	  char *yymsg;
	  int yyx;

	  /* Start YYX at -YYN if negative to avoid negative indexes in
	     YYCHECK.  */
	  int yyxbegin = yyn < 0 ? -yyn : 0;

	  /* Stay within bounds of both yycheck and yytname.  */
	  int yychecklim = YYLAST - yyn;
	  int yyxend = yychecklim < YYNTOKENS ? yychecklim : YYNTOKENS;
	  int yycount = 0;

	  yyprefix = ", expecting ";
	  for (yyx = yyxbegin; yyx < yyxend; ++yyx)
	    if (yycheck[yyx + yyn] == yyx && yyx != YYTERROR)
	      {
		yysize += yystrlen (yyprefix) + yystrlen (yytname [yyx]);
		yycount += 1;
		if (yycount == 5)
		  {
		    yysize = 0;
		    break;
		  }
	      }
	  yysize += (sizeof ("syntax error, unexpected ")
		     + yystrlen (yytname[yytype]));
	  yymsg = (char *) YYSTACK_ALLOC (yysize);
	  if (yymsg != 0)
	    {
	      char *yyp = yystpcpy (yymsg, "syntax error, unexpected ");
	      yyp = yystpcpy (yyp, yytname[yytype]);

	      if (yycount < 5)
		{
		  yyprefix = ", expecting ";
		  for (yyx = yyxbegin; yyx < yyxend; ++yyx)
		    if (yycheck[yyx + yyn] == yyx && yyx != YYTERROR)
		      {
			yyp = yystpcpy (yyp, yyprefix);
			yyp = yystpcpy (yyp, yytname[yyx]);
			yyprefix = " or ";
		      }
		}
	      yyerror (yymsg);
	      YYSTACK_FREE (yymsg);
	    }
	  else
	    yyerror ("syntax error; also virtual memory exhausted");
	}
      else
#endif /* YYERROR_VERBOSE */
	yyerror ("syntax error");
    }



  if (yyerrstatus == 3)
    {
      /* If just tried and failed to reuse lookahead token after an
	 error, discard it.  */

      if (yychar <= YYEOF)
        {
          /* If at end of input, pop the error token,
	     then the rest of the stack, then return failure.  */
	  if (yychar == YYEOF)
	     for (;;)
	       {
		 YYPOPSTACK;
		 if (yyssp == yyss)
		   YYABORT;
		 YYDSYMPRINTF ("Error: popping", yystos[*yyssp], yyvsp, yylsp);
		 yydestruct (yystos[*yyssp], yyvsp);
	       }
        }
      else
	{
	  YYDSYMPRINTF ("Error: discarding", yytoken, &yylval, &yylloc);
	  yydestruct (yytoken, &yylval);
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

#ifdef __GNUC__
  /* Pacify GCC when the user code never invokes YYERROR and the label
     yyerrorlab therefore never appears in user code.  */
  if (0)
     goto yyerrorlab;
#endif

  yyvsp -= yylen;
  yyssp -= yylen;
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

      YYDSYMPRINTF ("Error: popping", yystos[*yyssp], yyvsp, yylsp);
      yydestruct (yystos[yystate], yyvsp);
      YYPOPSTACK;
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  if (yyn == YYFINAL)
    YYACCEPT;

  YYDPRINTF ((stderr, "Shifting error token, "));

  *++yyvsp = yylval;


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

#ifndef yyoverflow
/*----------------------------------------------.
| yyoverflowlab -- parser overflow comes here.  |
`----------------------------------------------*/
yyoverflowlab:
  yyerror ("parser stack overflow");
  yyresult = 2;
  /* Fall through.  */
#endif

yyreturn:
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif
  return yyresult;
}


#line 1037 "gram.y"


static int countbits(int b)
{
	int count;

	for (count = 0; b != 0; count++) {
		b &= b - 1; // this clears the LSB-most set bit
	}

	return (count);
}

static int count_mask(struct sockaddr_in6 *m)
{
	struct in6_addr *in6 = &m->sin6_addr;
	int i;
	int count = 0;

	for (i = 0; i < 16; ++i) {
		count += countbits(in6->s6_addr[i]);
	}
	return count;
}

static struct in6_addr get_prefix6(struct in6_addr const *addr, struct in6_addr const *mask)
{
	struct in6_addr prefix = *addr;
	int i = 0;

	for (; i < 16; ++i) {
		prefix.s6_addr[i] &= mask->s6_addr[i];
	}

	return prefix;
}

static void cleanup(void)
{
	if (iface) {
		free_ifaces(iface);
		iface = 0;
	}

	if (prefix) {
		free(prefix);
		prefix = 0;
	}

	if (route) {
		free(route);
		route = 0;
	}

	if (rdnss) {
		free(rdnss);
		rdnss = 0;
	}

	if (dnssl) {
		int i;
		for (i = 0;i < dnssl->AdvDNSSLNumber;i++)
			free(dnssl->AdvDNSSLSuffixes[i]);
		free(dnssl->AdvDNSSLSuffixes);
		free(dnssl);
		dnssl = 0;
	}

	if (lowpanco) {
		free(lowpanco);
		lowpanco = 0;
	}

	if (abro) {
		free(abro);
		abro = 0;
	}
}

struct Interface * readin_config(char const *path)
{
	FILE * in = fopen(path, "r");
	if (in) {
		filename = path;
		IfaceList = 0;
		num_lines = 1;
		iface = 0;

		yyset_in(in);
		if (yyparse() != 0) {
			free_ifaces(iface);
			iface = 0;
		} else {
			dlog(LOG_DEBUG, 1, "config file, %s, syntax ok", path);
		}
		yylex_destroy();
		fclose(in);
	}

	return IfaceList;
}

static void yyerror(char const * msg)
{
	fprintf(stderr, "%s:%d error: %s\n",
		filename,
		num_lines,
		msg);
}


