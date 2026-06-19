#ifndef RING_BUFFER_H
#define RING_BUFFER_H

#include <stdint.h>

/* ============= RING BUFFER ENUMS ============= */
typedef enum{
    RBUF_OK,
    RBUF_EMPTY,
    RBUF_FULL,
    RBUF_OVERFLOW,
    RBUF_IN_ERROR
} rbuf_status_t;

/* ============= RING BUFFER STRUCTS ============= */
typedef struct{
    int* rbuf_addr;
    int rbuf_max_length;
    int head_index;
    int tail_index;
    int bytes_written;
    bool rbuf_overflow;
} rbuf_handle_t;

/* ============= RING BUFFER FUNCTION DECLARATIONS ============= */

rbuf_status_t rbuf_init(rbuf_handle_t& rbuf, int* buf, int& buffer_length);

rbuf_status_t rbuf_push(rbuf_handle_t& rbuf, int value);

rbuf_status_t rbuf_pop(rbuf_handle_t& rbuf, int& out_value);

void rbuf_print_status(const rbuf_handle_t& rbuf);


#endif // RING_BUFFER_H