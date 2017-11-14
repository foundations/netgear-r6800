#ifndef __SC_GUEST_MANAGEMENT__
#define __SC_GUEST_MANAGEMENT__

#define MAX_GUEST_USER 8
#define GUEST_AUTH_FILE ".htpasswd_guest"
#define HTTPD_GUEST_TIMEOUT_FLAG "/tmp/httpd_guest_timeout"
struct guest_user_t {
	char guest_ip[16];
	time_t guest_access_time;
};
void guest_user_load(void);
void guest_user_save(void);
struct guest_user_t *guest_user_lookup(const char *ip);
struct guest_user_t *guest_user_add(const char *ip);
void handle_multi_login(int sig);

#endif
