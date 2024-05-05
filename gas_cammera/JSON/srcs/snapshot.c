#include "../headers/snapshot.h"
#include<stdio.h>
#include <string.h>
#include <stdlib.h>

void init_snapshot(snapshot_t* snap)
{    

    strcpy(snap->title,"snapshot");
    strcpy(snap->file_name,"snap");
    snap->width=240;
    snap->height=320;
    strcpy(snap->type,"jpg");
}

void read_snapshot(snapshot_t* snap, char*buff){
    char title[TITLE_LENGTH +2],*startp=NULL,*endp ,my_buff[MAX_OBJECT_SIZE];
    sprintf(title,"'%s':{",snap->title);

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
    strcpy(snap->file_name,token);

    token=strtok(NULL,":");
    token=strtok(NULL,",");
    snap->width=atoi(token);

    token=strtok(NULL,":");
    token=strtok(NULL,"\n");
    snap->height=atoi(token);

    token=strtok(NULL,":");
    token=strtok(NULL,"\n");
    strcpy(snap->type,token);

}

char* toString_snapshot(snapshot_t* snap){
    static char cfg_obj[MAX_OBJECT_SIZE];
    sprintf(cfg_obj,"'%s':{\n'file_name':%s,\n'width':%d,\n'height':%d,\n'type':%s\n}",
            snap->title,snap->file_name,snap->width,snap->height,snap->type);
    return cfg_obj;
}
