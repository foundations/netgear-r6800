#include "upgrade_flash.h"


#define ZIP_FILENAME        "keys.zip"
#define OPENVPN_DIR_BASE   "/tmp/openvpn"
#define CA_FILE 		"ca.crt"
#define SERVER_FILE 	"server.crt"
#define CLIENT_FILE	"client.crt"

#define CERT_DIR_BASE	"/tmp/etc/openvpn/"
#define MTD_SIZE  128*1024

struct openvpn_s
{
	int len;
	char md5[40];                                 
};
int get_openvpn_info(char *mtd, struct openvpn_s *openvpn_s) ;
int decompress_openvpn_cert_file(char *mtd);
int openvpn_cert_file_verify();
int generate_openvpn_cert();
int write_cert_to_mtd(char *mtd);
