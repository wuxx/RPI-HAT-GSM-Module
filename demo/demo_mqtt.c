#include <stdio.h>
#include <stdint.h>
#include "at.h"

static char buf_send[1024];
static char buf_recv[1024];

#define CLIENT_ID   "mqtt_user-123"
#define USERNAME    "mqtt_user"
#define PASSWORD    "mqttpw123"

#define HOST        "123.56.12.240"
#define PORT        1883

int32_t demo_mqtt()
{

#if 0
    at_request("AT+HTTPINIT\r\n", buf_recv, sizeof(buf_recv), 0, 1000);

    at_request("AT+HTTPPARA=\"URL\",\"https://iot-auth.cn-shanghai.aliyuncs.com/auth/devicename\"\r\n", 
            buf_recv, sizeof(buf_recv), 0, 1000);
#endif

    snprintf(buf_send, sizeof(buf_send), "AT+MCONFIG=\"%s\",\"%s\",\"%s\"\r\n",
            CLIENT_ID, USERNAME, PASSWORD);

    at_request(buf_send, buf_recv, sizeof(buf_recv), 0, 1000);

    snprintf(buf_send, sizeof(buf_send), "AT+MIPSTART=\"%s\",%d\r\n",
            HOST, PORT);

    at_request(buf_send, buf_recv, sizeof(buf_recv), 0, 1000);

    at_request("AT+MCONNECT=1,80", buf_recv, sizeof(buf_recv), 3000, 1000);

#if 0
    at_request("AT+MSUB=\"/a1Gzzy5Vewc/testdevice/user/get\",0\r\n", 
            buf_recv, sizeof(buf_recv), 3000, 1000);

    at_request("AT+MPUB=\"/sys/a1Gzzy5Vewc/testdevice/thing/model/up_raw\",1,0,\"test-msg\"\r\n", 
            buf_recv, sizeof(buf_recv), 3000, 1000);
#endif


    return 0;
}
