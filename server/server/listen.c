#include "listen.h"
#include <stdlib.h>

#define HEADER_SIZE_LENGTH 4


header message_header;
enum server_statuses{GET_MESSAGE_HEADER=1,
                     ACTION=2,
                     REPLAY=3,
                     TURN_OFF=4
                    };

void action();
int active_func();
void start_record_server();
void stop_record_server();
void do_snapshot_server();
void get_status_server();
void get_record_params_server();
void set_record_params_server();
void get_snapshot_params_server();
void set_snapshot_params_server();
void turn_off_server();
void download_file_server();

gazapi_t *p_gaz;//gas library functions object
void* handle;//gas library stages object

void connect_to_library(){

    //conect to library

    void* libHandle= dlopen("/home/tammar/sambashare/build-gas_cammera-Desktop-Debug/libgas_cammera.so.1.0.0",RTLD_LAZY);
    assert(libHandle);
    p_gaz = (gazapi_t*)dlsym(libHandle,"p_gaz_api");
    handle=p_gaz->create_gas_handler();

}
void diconnect_to_library(){
    p_gaz->free_handler(handle);
}


void action(){
    connect_to_library();
    init_server();
    int ans;
    int status=GET_MESSAGE_HEADER;
    while(status!=TURN_OFF){
        switch (status) {
        case GET_MESSAGE_HEADER:
            get_message_header(&message_header);
            status=ACTION;
            break;
        case ACTION:
            ans=active_func();
            status=REPLAY;

            break;
        case REPLAY:
            send_message_header(&message_header);
            if(ans)
               status=GET_MESSAGE_HEADER;
            else
                status=TURN_OFF;
            break;
        }
    }
    diconnect_to_library();

}

int active_func(){

    p_gaz;
    switch (message_header.type) {

    case Operation:
        switch (message_header.opcode) {

        case Start_record:
            start_record_server();
            break;
        case Stop_record:
            stop_record_server();
            break;
        case Snapshot:
            do_snapshot_server();
            break;
        }
        break;

    case Get_parameters:
        switch(message_header.opcode){
        case Get_status:
            get_status_server();
            break;
        case Start_record:
            get_record_params_server();
            break;
        case Snapshot:
            get_snapshot_params_server();
            break;
        case Download_file:
            download_file_server();
            break;
        }
        break;
    case Set_parameters:
        switch(message_header.opcode){
        case Start_record:
            set_record_params_server();
            break;
        case Snapshot:
            set_snapshot_params_server();
            break;
        }
        break;

    case Control:
        switch(message_header.opcode){
        case Turn_off:
            turn_off_server();
            return 0;
            break;
        }
        break;
    }
    return 1;
}
void start_record_server(){
    message_header.header_size=sizeof(header);
    message_header.type=Acknowledge;
    message_header.opcode=Start_record;
    message_header.data_size=3;
    message_header.reserve=0;

    if(!check_checksum(message_header.Checksum,message_header.data))
    {

        message_header.data[0]=Nack;
        message_header.data[1]=Invalid_Massage;
        message_header.data[2]='\0';

    }
    else{
        if(!p_gaz->start_record(handle)){
            message_header.data[0]=Nack;
            message_header.data[1]=Command_Error;
            message_header.data[2]='\0';
        }
        else{
            message_header.data[0]=Ack;
            message_header.data[1]='\0';
        }
    }

    message_header.Checksum=get_checksum(message_header.data);

   // send_message_header(&message_header);
}
void stop_record_server(){
    message_header.header_size=sizeof(header);
    message_header.type=Operation;
    message_header.opcode=Stop_record;
    message_header.data_size=20;
    message_header.reserve=0;


    if(!check_checksum(message_header.Checksum,message_header.data))
    {

        message_header.data[0]=Nack;
        message_header.data[1]=Invalid_Massage;
        message_header.data[2]='\0';

    }
    else{
        if(!p_gaz->stop_record(handle)){
            message_header.data[0]=Nack;
            message_header.data[1]=Command_Error;
            message_header.data[2]='\0';
        }
        else{
            message_header.data[0]=Ack;
            message_header.data[1]='\0';
        }
    }

    message_header.Checksum=get_checksum(message_header.data);

 //   send_message_header(&message_header);

}
void do_snapshot_server(){

    message_header.header_size=sizeof(header);
    message_header.type=Acknowledge;
    message_header.opcode=Snapshot;
    message_header.data_size=3;
    message_header.reserve=0;

    if(!check_checksum(message_header.Checksum,message_header.data))
    {

        message_header.data[0]=Nack;
        message_header.data[1]=Invalid_Massage;
        message_header.data[2]='\0';

    }
    else{
        if(!p_gaz->do_snapshot(handle)){
            message_header.data[0]=Nack;
            message_header.data[1]=Command_Error;
            message_header.data[2]='\0';
        }
        else{
            message_header.data[0]=Ack;
            message_header.data[1]='\0';
        }
    }

    message_header.Checksum=get_checksum(message_header.data);

   // send_message_header(&message_header);

}
void get_status_server(){

    message_header.header_size=sizeof(header);
    message_header.type=Reply_massage;
    message_header.opcode=Get_status;
    message_header.data_size=3;
    message_header.reserve=0;

    if(!check_checksum(message_header.Checksum,message_header.data))
    {
        message_header.data[0]=Nack;
        message_header.data[1]=Invalid_Massage;
        message_header.data[2]='\0';

    }
    else{
        int* s=p_gaz->get_status(handle);
        if(s==NULL){
            message_header.data[0]=Nack;
            message_header.data[1]=Command_Error;
            message_header.data[2]='\0';
        }
        else{
            message_header.data[0]=1;
            message_header.data[1]=*s;
        }
    }

    message_header.Checksum=get_checksum(message_header.data);

   // send_message_header(&message_header);

}

void get_record_params_server(){

    message_header.header_size=sizeof(header);
    message_header.type=Reply_massage;
    message_header.opcode=Start_record;
    message_header.reserve=0;

    if(!check_checksum(message_header.Checksum,message_header.data))
    {
        message_header.data[Ack_index]=Nack;
        message_header.data[Nack_states_index]=Invalid_Massage;

    }
    else{
        name_and_gop p=p_gaz->get_record_params(handle);
        if(p.name[0]=='\0'){
            message_header.data[Ack_index]=Nack;
            message_header.data[Nack_states_index]=Command_Error;
        }
        else{
            message_header.data[Ack_index]=1;
            memcpy(&(*(message_header.data+Data_index)),&p,sizeof(p));
        /*int name_len=sizeof(p.name);
        strncpy(message_header.data+Data_index,p.name,name_len);
        */
            sprintf(message_header.data+Data_index,"%s\0%d",p.name,p.gop);



        }
    }

    message_header.Checksum=get_checksum(message_header.data);
//    send_message_header(&message_header);
}

void set_record_params_server(){

    message_header.header_size=sizeof(header);
    message_header.type=Acknowledge;
    message_header.opcode=Start_record;
    message_header.reserve=0;


    if(!check_checksum(message_header.Checksum,message_header.data))
    {
        message_header.data[Ack_index]=Nack;
        message_header.data[Nack_states_index]=Invalid_Massage;

    }
    else{
        name_and_gop p;
        memcpy(&p,message_header.data,sizeof(p));
        int ans;
        ans=p_gaz->set_record_params(handle,p);

        if(!ans){
            message_header.data[Ack_index]=Nack;
            message_header.data[Nack_states_index]=Command_Error;
        }
        else{
            message_header.data[Ack_index]=Ack;
        }
    }

    message_header.Checksum=get_checksum(message_header.data);
    //send_message_header(&message_header);
}
void get_snapshot_params_server(){

    message_header.header_size=sizeof(header);
    message_header.type=Reply_massage;
    message_header.opcode=Snapshot;
    message_header.reserve=0;

    if(!check_checksum(message_header.Checksum,message_header.data))
    {
        message_header.data[Ack_index]=Nack;
        message_header.data[Nack_states_index]=Invalid_Massage;

    }
    else{
        name_and_img_type n_a_i=p_gaz->get_snapshot_params(handle);
        if(n_a_i.name[0]=='\0'){
            message_header.data[Ack_index]=Nack;
            message_header.data[Nack_states_index]=Command_Error;
        }
        else{
            message_header.data[Ack_index]=Ack;
            memcpy(message_header.data+Data_index,&n_a_i,sizeof(n_a_i));

        }
    }

    message_header.Checksum=get_checksum(message_header.data);
//    send_message_header(&message_header);
}

void set_snapshot_params_server(){

    message_header.header_size=sizeof(header);
    message_header.type=Acknowledge;
    message_header.opcode=Snapshot;
    message_header.reserve=0;


    if(!check_checksum(message_header.Checksum,message_header.data))
    {
        message_header.data[Ack_index]=Nack;
        message_header.data[Nack_states_index]=Invalid_Massage;

    }
    else{

        name_and_img_type n_a_i;

        memcpy(&n_a_i,message_header.data,sizeof(n_a_i));
        int ans;
        ans=p_gaz->set_snapshot_params(handle,n_a_i);

        if(!ans){
            message_header.data[Ack_index]=Nack;
            message_header.data[Nack_states_index]=Command_Error;
        }
        else{
            message_header.data[Ack_index]=Ack;
        }
    }

    message_header.Checksum=get_checksum(message_header.data);
    //send_message_header(&message_header);
}

void download_file_server(){

    message_header.header_size=sizeof(header);
    message_header.type=Reply_massage;
    message_header.opcode=Download_file;
    message_header.reserve=0;


    if(!check_checksum(message_header.Checksum,message_header.data))
    {
        message_header.data[Ack_index]=Nack;
        message_header.data[Nack_states_index]=Invalid_Massage;

    }
    else{
        char * file_name=(char*)malloc(message_header.data_size);
        strncpy(file_name,message_header.data,message_header.data_size-1);
        file_name[message_header.data_size-1]='\0';
        int offset=message_header.data[message_header.data_size-1];

        FILE* fd;
        fd = fopen(file_name, "r");

        if (!fd) {
            fprintf(stderr, "Could not open %s\n",file_name);
            message_header.data[Ack_index]=Nack;
            message_header.data[Nack_states_index]=Command_Error;

        }
        else{

            int file_size=MAXLINE;
            /*fsick*/
            /*f_size*/
            fread(message_header.data+Data_index,sizeof(char),file_size-offset,fd);
            message_header.data[Ack_index]=Ack;
            fclose(fd);
        }
    }

    message_header.Checksum=get_checksum(message_header.data);

}
void turn_off_server(){

    message_header.header_size=sizeof(header);
    message_header.type=Acknowledge;
    message_header.opcode=Turn_off;
    message_header.reserve=0;


    if(!check_checksum(message_header.Checksum,message_header.data))
    {
        message_header.data[Ack_index]=Nack;
        message_header.data[Nack_states_index]=Invalid_Massage;

    }
    else{

            message_header.data[Ack_index]=Ack;
    }

    message_header.Checksum=get_checksum(message_header.data);

}

