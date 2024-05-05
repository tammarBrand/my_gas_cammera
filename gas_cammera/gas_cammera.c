#include "gas_cammera.h"
#include "capture.h"
#include "rgb_converter.h"
#include "yuv_converter.h"
#include "encode.h"
#include "write_record.h"
#include "stage_lib.h"
#include "JSON/headers/cfg_doc.h"
void* libHandle;

typedef struct handler{

    int status_bits;
    int stages_number;
    stage* stage_head;
    cfg_doc sys_obj;
    record_tools tools;

}handler;


void  GAS_API_init(handler* my_handler){

    init_rgb_matrix();
    init_cfg_doc(&(my_handler->sys_obj),"/home/tammar/sambashare/gas_cammera/JSON");

    void* q1=createQueue(10);
    void* q2=createQueue(10);
    void* q3=createQueue(10);
    void* q4=createQueue(10);

    rgb_converter_params * rgb_converter_p=(rgb_converter_params *)malloc(sizeof(rgb_converter_params));
    if(rgb_converter_p==NULL){
        printf("error mallocing\n");
        exit(0);
    }
    rgb_converter_p->record=&(my_handler->sys_obj.record);
    rgb_converter_p->snap=&(my_handler->sys_obj.snap);

    encode_params * encode_p=(encode_params*)malloc(sizeof(encode_params));
    if(encode_p==NULL){
        printf("error mallocing\n");
        exit(0);
    }
    encode_p->record=&(my_handler->sys_obj.record);
    encode_p->tools=&(my_handler->tools);

    write_record_params* write_record_p=(write_record_params*)malloc(sizeof(write_record_params));
    if(write_record_p==NULL){
        printf("error mallocing\n");
        exit(0);
    }
    write_record_p->f=my_handler->tools.f;
    //TODO: hide queues
    void* st5=create_stage(q4,NULL,write_record,(void*)(write_record_p),NULL);
    void* st4=create_stage(q3,q4,encode,(void*)(encode_p),st5);
    void* st3=create_stage(q2,q3,yuv_converter,(void*)&(my_handler->sys_obj.record),st4);
    void* st2=create_stage(q1,q2,rgb_converter,(void*)rgb_converter_p,st3);
    void* st1=create_stage(NULL,q1,capture,(void*)&(my_handler->sys_obj.record),st2);

    my_handler->stage_head=st1;
    my_handler->stages_number=5;
    my_handler->sys_obj.record.is_record_on=0;
    my_handler->sys_obj.snap.is_snap_on=0;
    my_handler->status_bits=0;
    my_handler->status_bits|=INIT;

    //my_handler->stage_handle=create_handler(st1);
}

void* GAS_API_create_gas_handler(){
    handler* my_handler=(handler*)malloc(sizeof(handler));
    if(my_handler==NULL){
        printf("error mallocing\n");
        exit(0);
    }
    my_handler->status_bits=0;
    GAS_API_init(my_handler);
    return (void*)my_handler;
}

void free_stages_and_queues_api(void* my_stage){
    stage *prev, *head=(stage*)my_stage;
    prev=head;
    Queue* q;

    while(prev!=NULL){
        head=prev->next;
        q=prev->destQu;
        freeQueue(q);
        free_stage(prev);
        prev=head;
    }

}

void GAS_API_free_handler(void* handle){
    handler* my_handler=(handler*)(handle);

    if(my_handler==NULL)
        return;
    my_handler->status_bits&=~INIT;
    write_file(&(my_handler->sys_obj));
    free_stages_and_queues_api(my_handler->stage_head);
    free(my_handler);
}



void init_tools(record_tools* tools, record_t *record){

    int ret;

    tools->codec=avcodec_find_encoder(AV_CODEC_ID_MPEG2VIDEO);
    if (!tools->codec) {
        fprintf(stderr, "Codec not found\n");
        exit(1);
    }
    tools->c = avcodec_alloc_context3(tools->codec);
    if (!tools->c) {
        fprintf(stderr, "Could not allocate video codec context\n");
        exit(1);
    }

    tools->pkt = av_packet_alloc();
    if (!tools->pkt)
        exit(1);

    // put sample parameters
    tools->c->bit_rate = 400000;
    // resolution must be a multiple of two
    tools->c->width = record->width;
    tools->c->height = record->height;
    //frames per second
    tools->c->time_base = (AVRational){1, 25};
    tools->c->framerate = (AVRational){25, 1};
    tools->c->gop_size = record->GOP;
    tools->c->max_b_frames = 1;
    // c->pix_fmt = AV_PIX_FMT_YUV420P;
    tools->c->pix_fmt=AV_PIX_FMT_YUV420P;
    ret = avcodec_open2(tools->c, tools->codec, NULL);
    if (ret < 0) {
        //fprintf(stderr, "Could not open codec: %s\n", av_err2str(ret));
        exit(1);
    }
    tools->f = fopen(record->file_name, "wb");
    if (!tools->f) {
        fprintf(stderr, "Could not open %s\n",record->file_name);
        exit(1);
    }
    tools->frame = av_frame_alloc();
    if (!tools->frame) {
        fprintf(stderr, "Could not allocate video frame\n");
        exit(1);
    }
    tools->frame->format = tools->c->pix_fmt;
    tools->frame->width  = tools->c->width;
    tools->frame->height = tools->c->height;

    ret = av_frame_get_buffer(tools->frame, 0);
    if (ret < 0) {
        fprintf(stderr, "Could not allocate the video frame data\n");
        exit(1);
    }

    tools->start_time=0;
    return;
}

int GAS_API_start_record(void* handle){

    printf ("====start_record!====\n");
    handler* my_handler=(handler*)(handle);

    //update app handler
    my_handler->status_bits|=RECORD_START;

   // strcpy(my_handler->sys_obj.record.file_name,"/home/tammar/sambashare/my record.ts");
    init_tools(&(my_handler->tools), &(my_handler->sys_obj.record));

    //update stage handler
    my_handler->sys_obj.record.is_record_on=1;

    int stages_number=5;
    bool *is_pipe_on=&(my_handler->sys_obj.record.is_record_on);

    start_pipe(my_handler->stage_head, stages_number,is_pipe_on);


    return 1;

}

void close_record_tool(record_tools* tools){

    uint8_t endcode[] = { 0, 0, 1, 0xb7 };
    fwrite(endcode, 1, sizeof(endcode),tools->f);
    fclose(tools->f);
    avcodec_free_context(&(tools->c));
    av_frame_free(&(tools->frame));
    av_packet_free(&(tools->pkt));
}

int GAS_API_stop_record(void* handle){

    printf ("====stop_record====\n\r");

    handler* my_handler=(handler*)(handle);

    my_handler->sys_obj.record.is_record_on=0;
    my_handler->status_bits|=RECORD_STOP;
    my_handler->sys_obj.record.is_record_on=0;
    stop_pipe(my_handler->stage_head);


    close_record_tool(&(my_handler->tools));

    my_handler->status_bits&=~RECORD_START;
    my_handler->status_bits&=~RECORD_STOP;

    return 0;
}

int GAS_API_do_snapshot(void* handle){
    printf("GAZ_API_do_snapshot\n");
    handler* my_handler=(handler*)(handle);
    my_handler->status_bits|=SNAPSHOT;

    time_t t=time(NULL);
    struct tm tm=*localtime(&t);
    int n=50;
    char* name=(char*)malloc(n+5);
    sprintf(name,"./photos/%d-%02d-%02d %02d:%02d:%02d.", tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec);
    strcat(name,my_handler->sys_obj.snap.type);
    my_handler->sys_obj.snap.is_snap_on=1;
    strcpy(my_handler->sys_obj.snap.file_name,name);
    if((my_handler->status_bits&RECORD_START )&& !(my_handler->status_bits&RECORD_STOP))
    {
        my_handler->status_bits&=~SNAPSHOT;
        return 1;
    }
    int stages_number=2;
    bool is_pipe=0;


    start_pipe(my_handler->stage_head,stages_number,&is_pipe);
    my_handler->status_bits&=~SNAPSHOT;
    return 1;
}



int* GAS_API_get_status(void* handle){
    printf("GAZ_API_get_status\n");
    handler* my_handler=(handler*)(handle);
    return &(my_handler->status_bits);
}

name_and_gop GAS_API_get_record_params(void* handle){
    printf("GAZ_API_get_record_p\n");
    handler* my_handler=(handler*)(handle);
    name_and_gop n;
    strcpy(n.name,my_handler->sys_obj.record.file_name);
    n.gop=my_handler->sys_obj.record.GOP;
    return n;
}
int GAS_API_set_record_params(void* handle,name_and_gop n){
    printf("GAZ_API_set_record_p\n");
    handler* my_handler=(handler*)(handle);

    strcpy(my_handler->sys_obj.record.file_name,n.name);
    my_handler->sys_obj.record.GOP=n.gop;
    return 1;
}

name_and_img_type GAS_API_get_snapshot_params(void* handle){
    printf("GAZ_API_get_snapshot_p\n");
    handler* my_handler=(handler*)(handle);
    name_and_img_type n_a_i;
    strcpy(n_a_i.name,my_handler->sys_obj.snap.file_name);
    if(!strcmp(my_handler->sys_obj.snap.type,"jpg"))
            n_a_i.type=JPG;
    if(!strcmp(my_handler->sys_obj.snap.type,"bmp"))
            n_a_i.type=BMP;
    return n_a_i;
}

int GAS_API_set_snapshot_params(void* handle,name_and_img_type n_a_i){
    printf("GAZ_API_set_snapshot_p\n");
    handler* my_handler=(handler*)(handle);

    strcpy(my_handler->sys_obj.snap.file_name,n_a_i.name);
    if(n_a_i.type==JPG)
            strcpy(my_handler->sys_obj.snap.type,"jpg");
    if(n_a_i.type==BMP)
            strcpy(my_handler->sys_obj.snap.type,"BMP");
    return 1;
}

gazapi_t p_gaz_api= {
    .create_gas_handler=GAS_API_create_gas_handler,
    .free_handler=GAS_API_free_handler,
    .start_record=GAS_API_start_record,
    .stop_record=GAS_API_stop_record,
    .do_snapshot=GAS_API_do_snapshot,
    .get_status=GAS_API_get_status,
    .get_record_params=GAS_API_get_record_params,
    .set_record_params=GAS_API_set_record_params,
    .get_snapshot_params=GAS_API_get_snapshot_params,
    .set_snapshot_params=GAS_API_set_snapshot_params
};

