/***************************************************************
*             Copyright (C) 2007 by SerComm Corp.
*                    All Rights Reserved.
*
*      Use of this software is restricted to the terms and
*      conditions of SerComm's software license agreement.
*
*                        www.sercomm.com
****************************************************************/

#define EXPORT_SYMTAB

#if defined(CONFIG_MODVERSIONS) && !defined(MODVERSIONS)
#define MODEVERSIONS
#endif

#ifdef MODVERSIONS
#include <linux/modversions.h>
#endif

#include <linux/module.h>
#include <sc/sc_ct_mgr.h>


void (*sc_hook_destroy_conntrack)(struct nf_conn *ct) = NULL;
int (*sc_hook_init_conntrack_test)(const struct nf_conntrack_tuple *tuple, struct nf_conntrack_l3proto *l3proto)=NULL;
unsigned int (*sc_hook_init_conntrack_mask)(const struct nf_conntrack_tuple *tuple, struct nf_conntrack_l3proto *l3proto)=NULL;
void (*sc_hook_init_conntrack_add)(struct nf_conn *ct)=NULL;



EXPORT_SYMBOL(sc_hook_destroy_conntrack);
EXPORT_SYMBOL(sc_hook_init_conntrack_test);
EXPORT_SYMBOL(sc_hook_init_conntrack_add);
EXPORT_SYMBOL(sc_hook_init_conntrack_mask);

/***************************************************************
*             Copyright (C) 2007 by SerComm Corp.
*                    All Rights Reserved.
*
*      Use of this software is restricted to the terms and
*      conditions of SerComm's software license agreement.
*
*                        www.sercomm.com
****************************************************************/

#define EXPORT_SYMTAB

#if defined(CONFIG_MODVERSIONS) && !defined(MODVERSIONS)
#define MODEVERSIONS
#endif

#ifdef MODVERSIONS
#include <linux/modversions.h>
#endif

#include <linux/module.h>
#include <sc/sc_ct_mgr.h>


void (*sc_hook_destroy_conntrack)(struct nf_conn *ct) = NULL;
int (*sc_hook_init_conntrack_test)(const struct nf_conntrack_tuple *tuple, struct nf_conntrack_l3proto *l3proto)=NULL;
unsigned int (*sc_hook_init_conntrack_mask)(const struct nf_conntrack_tuple *tuple, struct nf_conntrack_l3proto *l3proto)=NULL;
void (*sc_hook_init_conntrack_add)(struct nf_conn *ct)=NULL;



EXPORT_SYMBOL(sc_hook_destroy_conntrack);
EXPORT_SYMBOL(sc_hook_init_conntrack_test);
EXPORT_SYMBOL(sc_hook_init_conntrack_add);
EXPORT_SYMBOL(sc_hook_init_conntrack_mask);

