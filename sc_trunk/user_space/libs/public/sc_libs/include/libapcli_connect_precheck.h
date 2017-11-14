#ifndef __LIBAPCLI_CONNECT__
#define __LIBAPCLI_CONNECT__

enum
{
	APCLI_CONNECT_NOT_READY,
	APCLI_CONNECT_NEED_RESTARTWIFI,
	APCLI_CONNECT_READY_FOR_CONNECT,
};

int apcli_connect_init();
int apcli_connect_ready_for_connect(int which_band);
int apcli_connect_data_ok(int which_band);

int apcli_connect_mark_rc_ready();
int apcli_connect_if_rc_ready();

#endif
