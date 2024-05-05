#include "encode.h"
 
static int start_time=0;

void* encode_it(AVCodecContext *enc_ctx, AVFrame *frame, AVPacket *pkt, FILE* f);
uint64_t GetMHClock(void);
void* encode(void* data, void* arg){
    if(data==NULL)
        return NULL;

    printf("encode\n");
    YUV* yuv=(YUV*)data;
    encode_params* params=(encode_params*) arg;
    int ret;

    ret = av_frame_make_writable(params->tools->frame);
    if (ret < 0)
        exit(1);
    //init frame->y
    for (int j, i=0;i<params->tools->frame->height;i++)
        for ( j=0;j<params->tools->frame->width;j++)
            params->tools->frame->data[0][i*params->tools->frame->linesize[0]+j]=yuv->y[i*params->tools->frame->width+j/2];
    //init frame->y , frame->v
    for(int i=0;i<params->tools->frame->height/2;i++)
        for(int j=0;j<params->tools->frame->width/2;j++){
            params->tools->frame->data [1][i*params->tools->frame->linesize[1]+j]= yuv->u[(i/2)*params->tools->frame->width+j/2];
            params->tools->frame->data [2][i*params->tools->frame->linesize[2]+j]= yuv->v[(i/2)*params->tools->frame->width+j/2];
        }

    if (params->tools->start_time==0){
        params->tools->frame->pts=0;
        params->tools->start_time=GetMHClock();
    }
    else
        params->tools->frame->pts = GetMHClock()-params->tools->start_time;


    encode_it(params->tools->c, params->tools->frame, params->tools->pkt, params->tools->f);


    return data;


}

void* encode_it(AVCodecContext *enc_ctx, AVFrame *frame, AVPacket *pkt, FILE* f)
{

    int ret;
    ret = avcodec_send_frame(enc_ctx, frame);

    if (ret < 0) {
        fprintf(stderr, "Error sending a frame for encoding\n");
        exit(1);
    }
    while (ret >= 0){
        ret = avcodec_receive_packet(enc_ctx, pkt);
        if (ret == AVERROR(EAGAIN) || ret == AVERROR_EOF)
            return NULL;
        else if (ret < 0) {
            fprintf(stderr, "Error during encoding\n");
            exit(1);
        }

        fwrite(pkt->data, 1,pkt->size, f);
        av_packet_unref(pkt);
    }
}
uint64_t GetMHClock(void){
    struct timeval tv;
    gettimeofday(&tv,NULL);
    return (uint64_t)tv.tv_sec * 1000000L + (uint64_t)tv.tv_usec;
}
