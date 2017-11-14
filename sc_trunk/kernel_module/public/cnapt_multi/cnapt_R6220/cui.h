
#ifndef __CUI_H
#define __CUI_H

struct cui_cmd_t {
	int	 cmd;
	char data[0];
};

enum {
	CUI_PM_ADD = 1,
	CUI_PM_DEL,
	CUI_PM_EDIT,
	CUI_PM_FLUSH,
	CUI_PT_ADD,
	CUI_PT_DEL,
	CUI_PT_EDIT,
	CUI_PT_FLUSH,
	CUI_DMZ_CLEAN,
	CUI_CNAPT_FLUSH,
};

#ifdef __KERNEL__

struct cui_cmd_hdl_t {
	int cmd;
	int (*cui_fn)(void*);
	struct cui_cmd_hdl_t *next;
};

extern int cui_init(void);
extern void cui_fini(void);
extern int cui_register_cmd(int cmd, int (*cui_fn)(void*));
extern int cui_unregister_cmd(int cmd);

#endif




#endif
