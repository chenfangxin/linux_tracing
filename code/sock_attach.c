#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <linux/filter.h>
#include <linux/if_ether.h>

/* From the example : tcpdump -i em1 tcp dst port 80 -dd */
struct sock_filter code[]={
    { 0x28, 0, 0, 0x0000000c },
    { 0x15, 0, 4, 0x000086dd },
    { 0x30, 0, 0, 0x00000014 },
    { 0x15, 0, 11, 0x00000006 },
    { 0x28, 0, 0, 0x00000038 },
    { 0x15, 8, 9, 0x00000050 },
    { 0x15, 0, 8, 0x00000800 },
    { 0x30, 0, 0, 0x00000017 },
    { 0x15, 0, 6, 0x00000006 },
    { 0x28, 0, 0, 0x00000014 },
    { 0x45, 4, 0, 0x00001fff },
    { 0xb1, 0, 0, 0x0000000e },
    { 0x48, 0, 0, 0x00000010 },
    { 0x15, 0, 1, 0x00000050 },
    { 0x6, 0, 0, 0x00040000 },
    { 0x6, 0, 0, 0x00000000 },
};

struct sock_fprog bpf = {
    .len = sizeof(code)/sizeof(code[0]),
    .filter = code,
};

int main(void)
{
    int sock;
    int ret;

    sock = socket(PF_PACKET, SOCK_RAW, htons(ETH_P_ALL));
    if(sock<0){
        printf("Can not open socket.\n");
        return -1;
    }

    ret = setsockopt(sock, SOL_SOCKET, SO_ATTACH_FILTER, &bpf, sizeof(bpf));
    if(ret<0){
        printf("Can not setsockopt: errno=%d.\n", errno);
        close(sock);
        return -1;
    }

    close(sock);

    return 0;
}
