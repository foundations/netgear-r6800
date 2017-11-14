
/* interface exported to configure cnapt. */

#ifndef __LIBCNAPT_H
#define __LIBCNAPT_H

/* error define, cnapt.h also define it. */
enum napt_err_t {
	ENAPT_SERV_CONFLICT = 1983,
	ENAPT_SYS_CONFLICT = 1984,
};

#define CNAPT_LOCK  "/var/lock/cnapt_lock"

extern int cpm_add(const char *rule);
extern int cpm_del(const char *rule);
extern int cpm_edit(const char *rule, const char *old_rule);
extern void cpm_flush(char *type);

extern int cpt_add(const char *rule);
extern int cpt_del(const char *rule);
extern int cpt_edit(const char *rule, const char *old_rule);
extern int cpt_flush(void);


extern int cdmz_clean(int enable, const char *dmz_ip);

extern void cnapt_flush_bypriv(char *lan_ip, char *lan_mask);

#endif
