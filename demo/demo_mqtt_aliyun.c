#include <stdio.h>
#include <stdint.h>
#include "at.h"

static char buf_send[1024];
static char buf_recv[1024];

#define CLIENT_ID   "test123|securemode=3,signmethod=hmacsha1,timestamp=1234567|"
#define USERNAME    "testdevice&a1Gzzy5Vewc"
#define PASSWORD    "455A6E0F72DA0003A6B9239DFCEB475692D36CF7"

#define HOST        "a1Gzzy5Vewc.iot-as-mqtt.cn-shanghai.aliyuncs.com"
#define PORT        1883

int32_t demo_aliyun()
{

#if 0
    at_request("AT+HTTPINIT\r\n", buf_recv, sizeof(buf_recv), 0, 1000);

    at_request("AT+HTTPPARA=\"URL\",\"https://iot-auth.cn-shanghai.aliyuncs.com/auth/devicename\"\r\n", 
            buf_recv, sizeof(buf_recv), 0, 1000);
#endif

    snprintf(buf_send, sizeof(buf_send), "AT+MCONFIG=\"%s\",\"%s\",\"%s\"\r\n",
            CLIENT_ID, USERNAME, PASSWORD);

    at_request(buf_send, buf_recv, sizeof(buf_recv), 0, 1000);

    snprintf(buf_send, sizeof(buf_send), "AT+MIPSTART=\"%s\",\"%d\"\r\n",
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
