#ifndef __SC_USB__
#define __SC_USB__

#define USB_PLUS_SYSFILE        "/sys/bus/usb/devices/1-1"
#define USB_PLUS_SYSFILE2        "/sys/bus/usb/devices/1-2"
#define USB_PLUS_SYSFILE3        "/sys/bus/usb/devices/1-3"
#define USB_PLUS_SYSFILE4        "/sys/bus/usb/devices/1-4"

#define MAX_USB_USER 15
#define USB_AUTH_NONE 0
#define USB_AUTH_FAIL 1
#define USB_AUTH_PASS 2
#define USB_NO_AUTH 3
#define USB_AUTH_GUEST	4
struct usb_user_t {
	char remote_ip[16];
	time_t timestamp;
	int auth;
};

int usb_session_check(void);
void usb_auth_check(char *dirname);
int check_valid_request(void);
int is_readable(const char *dir, const char *name);
struct usb_user_t *usb_user_lookup(const char *ip);
int sc_usb_mounted();


#endif
