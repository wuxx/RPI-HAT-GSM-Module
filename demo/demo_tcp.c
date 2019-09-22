#include <stdio.h>
#include <stdint.h>
#include "at.h"

static char buf_send[1024];
static char buf_recv[1024];


/* 
    on your server 
    $nc -l 9999 
*/

#define HOST    "220.181.38.148"
#define PORT    9999

int32_t demo_tcp()
{

    snprintf(buf_send, sizeof(buf_send), "AT+CIPSTART=\"TCP\",\"%s\",\"%d\"\r\n", 
            HOST, PORT);

    at_request(buf_send, buf_recv, sizeof(buf_recv), 5000, 1000);

    at_request("AT+CIPSEND=4\r\n", buf_recv, sizeof(buf_recv), 0, 1000);

    at_request("qwer\r\n", buf_recv, sizeof(buf_recv), 5000, 1000);


    return 0;
}
