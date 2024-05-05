#ifndef GAS_CAMMERA_H
#define GAS_CAMMERA_H
#define FILE_NAME_LENGTH 50
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

typedef enum Image_type{
    JPG=1,
    BMP=2
}Image_type;
struct handler;

typedef struct name_and_gop{
    char name[FILE_NAME_LENGTH];
    int gop;
}name_and_gop;

typedef struct name_and_img_type{
    char name[FILE_NAME_LENGTH];
    Image_type type;
}name_and_img_type;

typedef struct {
    void* (*create_gas_handler)();
    void (*free_handler)(void* handle);
    int (*start_record)(void* handler);
    int (*stop_record)(void* handler);
    int (*do_snapshot)(void*);
    int* (*get_status)(void*handle);
    name_and_gop (*get_record_params)(void* handle);
    int (*GAS_API_set_record_params)(void* handle,name_and_gop n);
    int (*set_record_params)(void* handle,name_and_gop n);
    name_and_img_type (*get_snapshot_params)(void* handle);
    int (*set_snapshot_params)(void* handle,name_and_img_type n_a_i);
}gazapi_t;


#endif // GAS_CAMMERA_H
