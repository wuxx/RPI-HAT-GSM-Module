#ifndef __AT_H__
#define __AT_H__

#include "stdint.h"

int at_request(char *request, char *response, uint32_t response_len, 
        uint32_t force_wait_timeout_ns, uint32_t recv_timeout_ms);


int at_init();

#endif /* __AT_H__ */
