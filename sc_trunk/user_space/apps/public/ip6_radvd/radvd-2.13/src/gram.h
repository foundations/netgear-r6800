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
/* Line 1231 of yacc.c.  */
#line 184 "gram.h"
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
# define YYSTYPE_IS_TRIVIAL 1
#endif

extern YYSTYPE yylval;



