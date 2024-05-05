#include "functions_tools.h"

void init_rgb_matrix(){

    int j=0;

    unsigned char z=255,x=0;

    while(j!=MAX_TEMP*3)
    {
        //rand numbers
        rgb_per_temp[j++]=z;//red->blue
        rgb_per_temp[j++]=x;
        rgb_per_temp[j++]=x;//blue->red
    }
    rgb_per_temp[9]=x;//4=blue
    rgb_per_temp[10]=x;
    rgb_per_temp[11]=z;
    rgb_per_temp[81]=x;//27=green
    rgb_per_temp[82]=x;
    rgb_per_temp[83]=z;
}

void randMat(int** matrix_temp, int height, int width){
    int i,j,new_temp,height_pos,width_pos,num_iterations=0.1*width*height;
    for(i=0;i<height;i++){
        for(j=0;j<width;j++){
            matrix_temp[i][j]=10;
        }
    }


    for(i=0;i<num_iterations;i++)
    {
        height_pos=rand() % height;
        width_pos=rand() % width;
        new_temp=rand()%(MAX_TEMP+1);
        matrix_temp[
                height_pos][width_pos]=new_temp;
    }
}
void covert_to_rgb(char rgb_matrix[],int** matrix, int height, int width){

    int k=0, t;
    for(int i=0;i<height;i++)
        for(int j=0;j<width;j++)
        {



            t=matrix[i][j]*3;//temp=5 t=15
            rgb_matrix[k++]=rgb_per_temp[t];
            int y=rgb_matrix[k-1];
            int x=(int)y;
            rgb_matrix[k++]=rgb_per_temp[t+1];
            rgb_matrix[k++]=rgb_per_temp[t+2];
        }


}

void convert_to_yuv(YUV* yuv, char rgb_matrix[],   int height, int width){

    unsigned char R,G,B;
    for(int j, i=0;i<height;i++)
    {
        for(j=0;j<width;j++)
        {
            R=*(rgb_matrix+(i*width+j)*3);
            G=*(rgb_matrix+(i*width+j)*3+1);
            B=*(rgb_matrix+(i*width+j)*3+2);
            yuv->y[width*i+j]= (0.257 * R) + (0.504 * G) + (0.098 * B) + 16;
            if(i%2==0&&j%2==0)
            {
                yuv->u[(width*i+j)/4]= (0.439 * R) - (0.368 * G) - (0.071 * B) + 128;
                yuv->v[(width*i+j)/4]= -(0.148 * R) - (0.291 * G) + (0.439 * B) + 128;
            }

        }
    }
}


void free_matrix(int** m ,int height){
    for(int i=0;i<height;i++){
        if(m[i]==NULL)
        {
            printf("-----double free----\n");
            continue;
        }
        free(m[i]);
        m[i]=NULL;
    }
    m=NULL;
}
void free_rgb_matrix(char * m){
    if(m==NULL)
    {
        printf("-----double free----\n");
        return;
    }
    free(m);

}
void free_yuv(YUV* yuv){
    if(yuv==NULL)
    {
        printf("-----double free----\n");
        return;
    }
    if(yuv->y!=NULL)
        free(yuv->y);
    if(yuv->u!=NULL)
        free(yuv->u);
    if(yuv->v!=NULL)
        free(yuv->v);
    free(yuv);
}
void free_pkt_data(uint8_t* data){
    if(data==NULL)
    {
        printf("-----double free----\n");
        return;
    }
    free(data);
}


