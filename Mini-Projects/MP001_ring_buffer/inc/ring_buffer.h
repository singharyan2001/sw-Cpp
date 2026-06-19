#ifndef RING_BUFFER_H
#define RING_BUFFER_H

#include <stdint.h>

/* ============= RING BUFFER ENUMS ============= */
enum rbuf_status{
    RBUF_OK,
    RBUF_EMPTY,
    RBUF_FULL,
    RBUF_OVERFLOW,
    RBUF_IN_ERROR
};

/* ============= RING BUFFER STRUCTS ============= */
struct rbuf_handle{
    int* rbuf_addr;
    int rbuf_max_length;
    int head_index;
    int tail_index;
    int bytes_written;
    bool rbuf_overflow;
};

/* ============= RING BUFFER FUNCTION DECLARATIONS ============= */

rbuf_status rbuf_init(rbuf_handle& rbuf, int* buf, int& buffer_length);

rbuf_status rbuf_push(rbuf_handle& rbuf, int value);

rbuf_status rbuf_pop(rbuf_handle& rbuf, int& out_value);

void rbuf_print_status(const rbuf_handle& rbuf);


#endif // RING_BUFFER_H