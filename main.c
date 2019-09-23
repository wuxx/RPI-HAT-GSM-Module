#include <stdio.h>
#include <stdint.h>

#include "serial.h"
#include "at.h"


char buf_send[1024];
char buf_recv[1024];

extern int32_t demo_mqtt();
extern int32_t demo_mqtt_aliyun();
extern int32_t demo_tcp();

int main()
{
    at_init();

#if 0
    serial_send("AT\r\n", 4);

    serial_recv(buf_recv, sizeof(buf_recv), 1000);

    printf("recv [%s]\r\n", buf_recv);
#endif

    at_request("AT\r\n", buf_recv, sizeof(buf_recv), 0, 1000);

    printf("manufacturer:\r\n");
    at_request("AT+CGMI\r\n", buf_recv, sizeof(buf_recv), 0, 1000);

    printf("model name:\r\n");
    at_request("AT+CGMM\r\n", buf_recv, sizeof(buf_recv), 0, 1000);

    printf("model version:\r\n");
    at_request("AT+CGMR\r\n", buf_recv, sizeof(buf_recv), 0, 1000);

    printf("imei:\r\n");
    at_request("AT+CGSN\r\n", buf_recv, sizeof(buf_recv), 0, 1000);

    printf("ccid:\r\n");
    at_request("AT+CCID\r\n", buf_recv, sizeof(buf_recv), 0, 1000);

    printf("imsi:\r\n");
    at_request("AT+CIMI\r\n", buf_recv, sizeof(buf_recv), 0, 1000);

    

    at_request("AT+CGREG?\r\n", buf_recv, sizeof(buf_recv), 0, 1000);

    at_request("AT+CGATT?\r\n", buf_recv, sizeof(buf_recv), 0, 1000);

    at_request("AT+CSTT=\"CMNET\"\r\n", buf_recv, sizeof(buf_recv), 0, 1000);

    /* request ip */
    at_request("AT+CIICR\r\n", buf_recv, sizeof(buf_recv), 0, 1000);

    printf("ip:\r\n");
    at_request("AT+CIFSR\r\n", buf_recv, sizeof(buf_recv), 0, 1000);

    printf("dns-server:\r\n");
    at_request("AT+CDNSCFG?\r\n", buf_recv, sizeof(buf_recv), 0, 1000);

    /* FIXME: */
    printf("dns resolve:\r\n");
    at_request("AT+CDNSGIP=\"www.baidu.com\"\r\n", buf_recv, sizeof(buf_recv), 3000, 1000);

    demo_mqtt();

    //demo_mqtt_aliyun();

    //demo_tcp();

    return 0;
}
