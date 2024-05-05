#include "yuv_converter.h"

void* yuv_converter(void* data, void* params){

    printf("yuv\n");

    if(data==NULL)
        return NULL;

    record_t* record=(record_t*)params;
    char* rgb_matrix=(char*)data;

    YUV * yuv=(YUV*)malloc(sizeof(YUV));
    yuv->y=(char*)malloc((record->width)*(record->height));
    if(yuv->y==NULL)
    {
        printf("error allocating y\n");
        exit(0);
    }
    yuv->u=(char*)malloc((record->width)*(record->height)/4);
    if(yuv->u==NULL)
    {
        printf("error allocating u\n");
        exit(0);
    }
    yuv->v=(char*)malloc((record->width)*(record->height)/4);

    if(yuv->v==NULL)
    {
        printf("error allocating v\n");
        exit(0);
    }convert_to_yuv(yuv,rgb_matrix, record->height, record->width);
    free_rgb_matrix(rgb_matrix);

    return yuv;


}
