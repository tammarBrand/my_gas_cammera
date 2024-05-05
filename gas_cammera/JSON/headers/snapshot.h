#ifndef WINDOW_H
#define WINDOW_H
#define TYPE_LENGTH 10

#include "cfg_obj.h"


typedef struct snapshot_t{
    char title[TITLE_LENGTH];
    bool is_snap_on;
    char file_name[FILE_NAME_LENGTH];
    int width;
    int height;
    char type[TYPE_LENGTH];
    char *data;
}snapshot_t;


void init_snapshot(snapshot_t* snap);
void read_snapshot(snapshot_t* snap, char*buff);
char* toString_snapshot(snapshot_t* snap);


#endif // WINDOW_H
