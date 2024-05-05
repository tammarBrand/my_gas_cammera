#pragma once
#ifndef FUNCTIONS_TOOLS_H
#define FUNCTIONS_TOOLS_H
#define SNAPSHOT_HEIGHT 240
#define SNAPSHOT_WIDTH 360
#define MAX_TEMP 30
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <libavcodec/avcodec.h>
#include "./JSON/headers/cfg_doc.h"



static char rgb_per_temp[MAX_TEMP*3];

typedef struct yuv{
    char *y;//[SNAPSHOT_HEIGHT*SNAPSHOT_WIDTH];
    char *u;//[SNAPSHOT_HEIGHT*SNAPSHOT_WIDTH/4];
    char *v;//[SNAPSHOT_HEIGHT*SNAPSHOT_WIDTH/4];
}YUV;

typedef struct record_tools{
    const AVCodec *codec;
    AVCodecContext *c;
    AVFrame *frame;
    AVPacket *pkt;
    int start_time;
    FILE* f;
}record_tools;

typedef struct rgb_converter_params{
    snapshot_t* snap;
    record_t* record;
}rgb_converter_params;


typedef struct encode_params{

    record_t* record;
    record_tools *tools;

}encode_params;

typedef struct write_record_params{
    FILE* f;
}write_record_params;

typedef struct write_record_data{
    uint8_t *data;
     int   size;
}write_record_data;


void init_rgb_matrix();
void randMat(int** matrix_temp, int height, int width);
void covert_to_rgb(char rgb_matrix[],int** matrix, int height, int width);
void convert_to_yuv(YUV* yuv, char rgb_matrix[],   int height, int width);
void free_matrix(int** m ,int height);
void free_rgb_matrix(char * m);
void free_yuv(YUV* yuv);
void free_pkt_data(uint8_t* data);

#endif // FUNCTIONS_TOOLS_H
