#include "at.h"
#include "serial.h"


int at_request(char *request, char *response, uint32_t response_len, uint32_t force_wait_timeout_ms, uint32_t recv_timeout_ms)
{

    serial_send(request, strlen(request));

    if (force_wait_timeout_ms) {
        usleep(force_wait_timeout_ms * 1000);
    }

    serial_recv(response, response_len, recv_timeout_ms);

    printf("===========================\r\n");
    printf("at send [%s]; recv [%s]\r\n", request, response);
    printf("===========================\r\n");
    return 0;
}


int at_init()
{
    int r;

    r = serial_init();

    return r;
}

