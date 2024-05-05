#include "rgb_converter.h"


void* rgb_converter(void* data, void* params){

    printf("rgb\n");

    if(data==NULL)
        return NULL;

    record_t* record=((rgb_converter_params*)params)->record;
    snapshot_t* snap=((rgb_converter_params*)params)->snap;

    int** matrix=(int**)data;
    char *rgb_matrix=NULL;

    rgb_matrix=(char *)malloc(sizeof(char)*((record->height)*(record->width)*3));
    if(rgb_matrix==NULL){
        printf("error allocating rgb_matrix \n");
        exit(0);
    }

    covert_to_rgb(rgb_matrix,matrix, record->height, record->width);
    free_matrix(matrix, record->height);

    if(snap->is_snap_on)
    {   save_snapshot(snap,rgb_matrix);
        snap->is_snap_on=0;
        if(! (record->is_record_on)){
            free_rgb_matrix(rgb_matrix);
            rgb_matrix=NULL;
        }

    }

    return rgb_matrix;

}




