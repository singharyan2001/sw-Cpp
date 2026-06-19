#include <stdint.h>

#include "log.h"

#include "ring_buffer.h"

/* ============= RING BUFFER FUNCTION DECLARATIONS ============= */

rbuf_status rbuf_init(rbuf_handle& rbuf, int* buf, int& buffer_length){
    // Intialize referenced rung buffer structure
    rbuf.rbuf_addr = buf;
    rbuf.rbuf_max_length = buffer_length;
    rbuf.rbuf_overflow = false;
    rbuf.head_index = 0,
    rbuf.tail_index = 0,
    rbuf.bytes_written = 0;
    return RBUF_OK;
}

rbuf_status rbuf_push(rbuf_handle& rbuf, int value){
    // Check for Full Condition
    if(rbuf.bytes_written == rbuf.rbuf_max_length){
        rbuf.rbuf_overflow = true;
        return RBUF_FULL;
    }
    // Perform Push Operation if buffer has space
    rbuf.rbuf_addr[rbuf.head_index] = value;
    // Safe Warp Around based
    rbuf.head_index = (rbuf.head_index + 1) % rbuf.rbuf_max_length;
    rbuf.bytes_written++;
    return RBUF_OK;
}

rbuf_status rbuf_pop(rbuf_handle& rbuf, int& out_value){
    // Check for Empty Condition
    if(rbuf.head_index == rbuf.tail_index){
        rbuf.rbuf_overflow = false;
        return RBUF_EMPTY;
    }
    // Perform pop operation on the buffer
    out_value = rbuf.rbuf_addr[rbuf.tail_index];
    rbuf.tail_index = (rbuf.tail_index + 1) % rbuf.rbuf_max_length;
    rbuf.bytes_written--;
    return RBUF_OK;
}

void rbuf_print_status(const rbuf_handle& rbuf){
    log_msg("============= RING BUFFER INFO [START] =============");
    log_data("RING BUFFER ADDRESS: ", rbuf.rbuf_addr);
    log_data("RING BUFFER SIZE: ", rbuf.rbuf_max_length);
    log_data("RING BUFFER BYTES WRITTEN: ", rbuf.bytes_written);
    log_data("RING BUFFER HEAD INDEX: ", rbuf.head_index);
    log_data("RING BUFFER TAIL INDEX: ", rbuf.tail_index);
    log_data("RING BUFFER OVERFLOW: ", rbuf.rbuf_overflow);
    log_msg("============= RING BUFFER INFO [END] =============");
}
