#include <stdio.h>
#include <stdint.h>

#include "serial.h"


char buf_send[1024];
char buf_recv[1024];

int main()
{
    at_init();

#if 0
    serial_send("AT\r\n", 4);

    serial_recv(buf_recv, sizeof(buf_recv), 1000);

    printf("recv [%s]\r\n", buf_recv);
#endif

    at_request("AT\r\n", buf_recv, sizeof(buf_recv), 1000);
    printf("recv [%s]\r\n", buf_recv);

    return 0;
}
