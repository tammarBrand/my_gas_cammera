#ifndef CFG_DOC_H
#define CFG_DOC_H
#define FILE_NAME "config.log"

#include <stdio.h>
#include <stdlib.h>

#include "record.h"
#include "snapshot.h"


typedef struct cfg_doc{

    char title[TITLE_LENGTH];
    char file_path[FILE_NAME_LENGTH];
    FILE *fp;
    record_t record;
    snapshot_t snap;

}cfg_doc;

void init_cfg_doc(cfg_doc* obj,char* path);
void read_file(cfg_doc* obj);//updating objects
void write_file(cfg_doc* obj); //update the doc
record_t* get_record(cfg_doc* obj);
snapshot_t* get_snap(cfg_doc* obj);


#endif // CFG_DOC_H
