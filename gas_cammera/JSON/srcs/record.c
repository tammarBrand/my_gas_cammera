#include "../headers/record.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void init_record(record_t* record)
{

    strcpy(record->title,"record");
    record->is_record_on=0;
    strcpy(record->file_name,"file_name.ts");
    record->codec=2;
    record->width=240;
    record->height=320;
    record->fps=-1;
    record->GOP=5;

}

void read_record(record_t* record , char *buff){

    char title[TITLE_LENGTH +2],*startp=NULL,*endp ,my_buff[MAX_OBJECT_SIZE];
    sprintf(title,"'%s':{",record->title);

    /* search for pointer to the start of the object in cfg file*/
    startp=strstr(buff,title);
    if(!startp){
        return;
    }
    /*if existed: find end pointer*/
    endp=strstr(startp,"}");

    /*having the object configuration section*/

    strncpy(my_buff,startp,endp-startp);
    my_buff[strlen(my_buff)-1]='\0';


    char *token=strtok(my_buff,"\n");

    token=strtok(NULL,":");
    token=strtok(NULL,",");
    strcpy(record->file_name,token);

    token=strtok(NULL,":");
    token=strtok(NULL,",");
    record->codec=atoi(token);

    token=strtok(NULL,":");
    token=strtok(NULL,",");
    record->width=atoi(token);

    token=strtok(NULL,":");
    token=strtok(NULL,",");
    record->height=atoi(token);

    token=strtok(NULL,":");
    token=strtok(NULL,",");
    record->GOP=atoi(token);



}

char* toString_record(record_t* record){
    static char cfg_obj [MAX_OBJECT_SIZE];
    sprintf(cfg_obj,"'%s':{\n'file_name':%s,\n'codec':%d,\n'width':%d,\n'height':%d,\n'GOP':%d\n}",
            record->title,record->file_name,record->codec,record->width,record->height, record->GOP);
    return cfg_obj;
}
