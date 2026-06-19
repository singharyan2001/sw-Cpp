
#include "log.h"
#include "ring_buffer.h"

/* ============= RING BUFFER DECLARATIONS ============= */
constexpr int max_buffer_size = 10;

int rbuffer[max_buffer_size] = {0};

rbuf_handle_t rbuf_handle = {0};


int main(){
    log_msg("============= MINI-PROJECT 001: RING BUFFER =============");
    
    int length = max_buffer_size;

    // 1. Initialize Ring Buffer
    log_msg("<<<<<<<<<<<<<<< RING BUFFER INIT TEST >>>>>>>>>>>>>>>>>>>");
    rbuf_status_t ret = RBUF_OK;
    ret = rbuf_init(rbuf_handle, rbuffer,length);
    if(ret != RBUF_OK){
        log_msg("RING BUFFER INITIALIZE - FAILED");
    }
    else{
        log_msg("RING BUFFER INITIALIZE - SUCCESS");
    }

    // 2. Ring Buffer Push Operation Test
    log_msg("<<<<<<<<<<<<<<< RING BUFFER PUSH TEST >>>>>>>>>>>>>>>>>>>");
    int value = 10;
    for(int i=0; i<5; i++){
        rbuf_push(rbuf_handle, value++);
        log_data("RING BUFFER PUSHED VALUE: ", value);
    }
    rbuf_print_status(rbuf_handle);
    
    // 3. Ring Buffer Pop Operation Test
    log_msg("<<<<<<<<<<<<<<< RING BUFFER POP TEST >>>>>>>>>>>>>>>>>>>");
    int data = 0;
    for(int i=0; i<5; i++){
        rbuf_pop(rbuf_handle, data);
        log_data("RING BUFFER POPED VALUE: ", data);
    }
    rbuf_print_status(rbuf_handle);

    // 4. Ring Buffer Empty Condition Test
    log_msg("<<<<<<<<<<<<<<< RING BUFFER EMPTY CONDITION TEST >>>>>>>>>>>>>>>>>>>");
    ret = rbuf_pop(rbuf_handle, data);
    if(ret == RBUF_EMPTY){
        log_msg("RING BUFFER STATUS - EMPTY - TEST SUCCESS");
    }
    else{
        log_msg("RING BUFFER EMPTY CONDITION TEST FAILED");
    }

    // 5. Ring Buffer Overflow Condition Test
    log_msg("<<<<<<<<<<<<<<< RING BUFFER OVERFLOW CONDITION TEST >>>>>>>>>>>>>>>>>>>");
    for(int i=0; i<11; i++){
        ret = rbuf_push(rbuf_handle, value++);
        if(rbuf_handle.rbuf_overflow == true){
            log_msg("RING BUFFER OVERFLOW CONDITION DETECTED - EXITING LOOP");
            break;
        }
    }
    rbuf_print_status(rbuf_handle);

    // 6. Clear Ring Buffer
    log_msg("<<<<<<<<<<<<<<< EMPTYING EXISTING RING BUFFER BY POP OPERATION >>>>>>>>>>>>>>>>>>>");
    ret = RBUF_OK;
    while(ret != RBUF_OK){
        ret = rbuf_pop(rbuf_handle, data);
        log_data("RING BUFFER POPED VALUE: ", data);
    }
    log_msg("RING BUFFER NOW EMPTY");

}
