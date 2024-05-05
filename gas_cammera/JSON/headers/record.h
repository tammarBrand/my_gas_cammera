#ifndef IMAGE_H
#define IMAGE_H
#include "cfg_obj.h"

typedef struct  record_t{

    char title[TITLE_LENGTH];
    bool is_record_on;
    char file_name[FILE_NAME_LENGTH];
    int codec;
    int width;
    int height;
    int fps;
    int GOP;

}record_t;

void init_record(record_t*);
void read_record(record_t*,char* buff);
char* toString_record(record_t*);


#endif // IMAGE_H
