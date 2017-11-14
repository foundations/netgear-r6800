#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include "referral.h"

/*Indicate the beginning of *.cap file*/
struct pcap_file_header{
    unsigned int magic;     //Libpcap magic number
    unsigned short version_major;   //Libpcap major version
    unsigned short version_minor;   //Libpcap minor version
    int thiszone;           //Gmt to local correction
    unsigned int sigfig;    //Accuracy of timestamps
    unsigned int snaplen; //Length of the max save portion of each packet
    unsigned int linktype;  //Data link type
};

/*The prefix of each packet we captured*/
struct pcap_pkthdr{
    struct timeval ts;      //time stamp
    unsigned int caplen;    //length of portion present
    unsigned int len;       //length of packet
};

int main(int argc, char *argv[])
{
    int num;
    int pkt;
    struct pcap_pkthdr pkthdr;
    char server[512];
    unsigned char msg[1500];
    
    if (argc < 3)
    {
        printf("test packet.pkt number\n");
        exit(-1);
    }
    num = atoi(argv[2]);
    if (num <1)
    {
        printf("number must greater than 1\n");
        exit(-1);
    }
    if ((pkt = open(argv[1], O_RDONLY)) < 0)
    {
        printf("fail to open %s\n", argv[1]);
        exit(-1);
    }
    lseek(pkt, sizeof(struct pcap_file_header), SEEK_SET);
    while (--num)
    {
        if (read(pkt, &pkthdr, sizeof(struct pcap_pkthdr)) > 0)
            lseek(pkt, pkthdr.caplen, SEEK_CUR);
        else
        {
            printf("unexpected end, fail to read packet\n");
            goto quit;
        }
        
    }
    if (read(pkt, &pkthdr, sizeof(struct pcap_pkthdr)) > 0)
    {
        int i;
        
        printf("find packet, length %d\n", pkthdr.caplen);
        lseek(pkt, 42, SEEK_CUR);
        read(pkt, msg, pkthdr.caplen - 42);
        printf("char pattern[] = {");
        for (i = 0; i < pkthdr.caplen - 42; i++)
        {
            if ((i % 8) == 0)
                printf("\n");
            
            printf("0x%2.2x,", msg[i]);
        
        }
        printf("\n};\n");
        pickup_referral(msg, pkthdr.caplen - 42, server, sizeof(server));
        printf("get server <%s>\n", server);
    }
    else
        printf("unexpected end, fail to read packet\n");
    
 quit:
    close(pkt);
    exit(0);
}
