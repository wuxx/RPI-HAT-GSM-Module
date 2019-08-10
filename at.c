#include "at.h"
#include "serial.h"


int at_request(char *request, char *response, uint32_t response_len, uint32_t timeout_ms)
{

     serial_send(request, strlen(request));
     serial_recv(response, response_len, timeout_ms);

     return 0;
}


int at_init()
{
    int r;

    r = serial_init();

    return r;
}

