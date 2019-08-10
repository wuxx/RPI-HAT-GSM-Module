#include <stdio.h>
#include <stdint.h>

#include "serial.h"


char buf_recv[1024];

int main()
{
    serial_init();

    serial_send("AT\r\n", 4);

    serial_recv(buf_recv, sizeof(buf_recv), 1000);

    printf("recv [%s]\r\n", buf_recv);

    return 0;
}
