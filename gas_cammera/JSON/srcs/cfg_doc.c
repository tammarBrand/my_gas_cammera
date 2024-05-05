#include "../headers/cfg_doc.h"
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#define MAX_FILE_SIZE 500



void init_cfg_doc(cfg_doc* obj,char* path)
{

    sprintf(obj->file_path,"%s/%s\0",path,FILE_NAME);
    strcpy(obj->title,"shell");
    init_record(&(obj->record));
    init_snapshot(&(obj->snap));
    obj->fp=NULL;
    read_file(obj);

}



/* activated when system is on
  * reading the config file and update the system object when neede
*/
void read_file(cfg_doc* obj){

    /*open file*/
    char* mode="r";
    obj->fp=fopen(obj->file_path,mode);
    if(obj->fp==NULL){
        printf("error\n");
        //fclose(obj->fp);
        return;
    }

    /*read file*/
    struct stat st;
    stat(obj->file_path, &st);
    int size = st.st_size;
    char buff[MAX_FILE_SIZE];
    fread(buff,sizeof(char),size,obj->fp);
    fclose(obj->fp);
    buff[strlen(buff)-1]='\0';
    read_record(&(obj->record), buff);
    read_snapshot(&(obj->snap),buff);

}


void write_file(cfg_doc* obj){
    char *arr[3];
    int i=0, sum=0;

    arr[0]=toString_record(&(obj->record));
    arr[1]=toString_snapshot(&(obj->snap));

    static char cfg_to_write[MAX_FILE_SIZE]={'\0'};
    sprintf(cfg_to_write,"'%s':{\n",obj->title);

    for(int j=0;j<2;j++){
        strcat(cfg_to_write,arr[j]);
        if(j<i-1){
            strcat(cfg_to_write,",");
        }
        strcat(cfg_to_write,"\n");
    }

    strcat(cfg_to_write,"}\n\0");
    char* mode="w";
    obj->fp=fopen(obj->file_path,mode);
    if(!obj->fp){
        printf("error opening\n");
        return;
    }

    fwrite(cfg_to_write,sizeof(char),strlen(cfg_to_write) ,obj->fp);
    fclose(obj->fp);

}


record_t* get_record(cfg_doc* obj){

    return  &(obj->record);
}

snapshot_t* get_snap(cfg_doc* obj){

    return  &(obj->snap);

}
