#ifndef GAS_CAMMERA_H
#define GAS_CAMMERA_H
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

enum status_bits{
    INIT=1,
    RECORD_START=2,
    RECORD_STOP=4,
    SNAPSHOT=8,
    CAPTURE=16,
    RGB_CONVERTER=32,
    YUV_CONVERTER=64,
    ENCODE=128,
    WRITE_RECORD=256
};

struct handler;


typedef struct {
    void* (*create_gas_handler)();
    void (*free_handler)(void* handle);
    int (*start_record)(void* handler);
    int (*stop_record)(void* handler);
    int (*do_snapshot)(void*);
    /* int (*start_streaming)(streaming_t*,char * file_name);
     int (*stop_streamig)(streaming_t*);*/
    char* (*get_dll_version)();
    char* (*get_video_statics)();
    int* (*get_status)(void*handle);
}gazapi_t;


#endif // GAS_CAMMERA_H
