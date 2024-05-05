#include "../headers/gas_cam_cli.h"



header message_header;

void init_gas_cam_cli(){

    init_cli();

}

void status_app(){
    pthread_t controller;
    //int status;
    //get_status(&status);
    pthread_create(&controller,NULL,contoller_fun,(void*)get_status);
    // while(status) get_status(&status);
  /*  start_record_cli(0,NULL);
    sleep(5);

    stop_record_cli(0,NULL);*/
    pthread_join(controller,NULL);
}





//client gas_csam functions
int start_record_cli(int n,char** params){
    if(n!=0){
        printf("error not fiten number of arguments\n");
        return 0;
    }

    message_header.header_size=sizeof(header);
    message_header.type=Operation;
    message_header.opcode=Start_record;
    message_header.data_size=20;
    message_header.reserve=0;
    strcpy(message_header.data,"start_record");
    message_header.Checksum=get_checksum(message_header.data);

    send_message_header(&message_header);
    get_message_header(&message_header);

    if(check_checksum(message_header.Checksum,message_header.data)&&
            message_header.type==Acknowledge&&
            message_header.opcode==Start_record)
    {
        if(message_header.data[Ack_index]==Ack)
            printf("got right\n");
        else
            switch(message_header.data[1]){
            case Command_Error: return 0;
                break;
            case Invalid_Massage:
                start_record_cli(0,NULL);
                break;
            case Failed:
                printf("error accour: %d\n",message_header.data[2]);
                break;
            }

    }
    return 1;
}

int stop_record_cli(int n,char** params){
    if(n!=0){
        printf("error not fiten number of arguments\n");
        return 0;
    }


    message_header.header_size=sizeof(header);
    message_header.type=Operation;
    message_header.opcode=Stop_record;
    message_header.data_size=20;
    message_header.reserve=0;
    strcpy(message_header.data,"stop_record");
    message_header.Checksum=get_checksum(message_header.data);


    send_message_header(&message_header);
    get_message_header(&message_header);
    if(check_checksum( message_header.Checksum,message_header.data)&&
            message_header.type==Acknowledge&&
            message_header.opcode==Stop_record)
    {
        if(message_header.data[Ack_index]==Ack)
            printf("got right\n");
        else
            switch(message_header.data[1]){
            case Command_Error: return 0;
                break;
            case Invalid_Massage:
                stop_record_cli(0,NULL);
                break;
            case Failed:
                printf("error accour: %d\n",message_header.data[2]);
                break;
            }
    }
    return 1;
}

int do_snapshot_cli(int n,char** params){
    if(n!=0){
        printf("error not fiten number of arguments\n");
        return 0;
    }

    message_header.header_size=sizeof(header);
    message_header.type=Operation;
    message_header.opcode=Snapshot;
    message_header.data_size=20;
    message_header.reserve=0;
    strcpy(message_header.data,"do_snapshot");
    message_header.Checksum=get_checksum(message_header.data);

    send_message_header(&message_header);
    get_message_header(&message_header);
    if(check_checksum( message_header.Checksum,message_header.data)&&
            message_header.type==Acknowledge &&
            message_header.opcode==Snapshot)
    {
        if(message_header.data[Ack_index]==Ack)
            printf("got right\n");
        else
            switch(message_header.data[1]){
            case Command_Error: return 0;
                break;
            case Invalid_Massage:
                do_snapshot_cli(0,NULL);
                break;
            case Failed:
                printf("error accour: %d\n",message_header.data[2]);
                break;
            }
    }

    return 1;
}

int get_status(int * status){

    message_header.header_size=sizeof(header);
    message_header.type=Get_parameters;
    message_header.opcode=Get_status;
    message_header.data_size=0;
    message_header.reserve=0;
    message_header.data[0]='\0';
    message_header.Checksum=get_checksum(message_header.data);

    send_message_header(&message_header);
    get_message_header(&message_header);
    if(check_checksum( message_header.Checksum,message_header.data)&&
            message_header.type==Reply_massage &&
            message_header.opcode==Get_status)
    {
        if(message_header.data[Ack_index]==Ack)
        {
            *status=(int)message_header.data[Data_index];
            printf("got right\n");
        }
        else
            switch(message_header.data[Nack_states_index]){
            case Command_Error:
                return 0;
                break;
            case Invalid_Massage:
                return get_status(status);
                break;
            case Failed:
                printf("error accour: %d\n",message_header.data[Error_number_index]);
                break;
            }
    }

    return 1;

}

int get_record_params_cli(int n,char** params){
    if(n!=0){
        printf("error not fiten number of arguments\n");
        return 0;
    }
    message_header.header_size=sizeof(header);
    message_header.type=Get_parameters;
    message_header.opcode=Start_record;
    message_header.data_size=0;
    message_header.reserve=0;
    message_header.data[0]='\0';
    message_header.Checksum=get_checksum(message_header.data);

    send_message_header(&message_header);
    get_message_header(&message_header);
    if(check_checksum( message_header.Checksum,message_header.data)&&
            message_header.type==Reply_massage &&
            message_header.opcode==Start_record)
    {
        if(message_header.data[Ack_index]==Ack)
        {
            name_and_gop p;
            //memcpy(&p,message_header.data+Data_index,sizeof(p));
            sscanf(message_header.data+Data_index,"%s%d",p.name,&(p.gop));

            printf("record:file_name:%s\n GOP:%d\n",p.name,p.gop);
            printf("got right\n");
        }
        else
            switch(message_header.data[Nack_states_index]){
            case Command_Error:
                return 0;
                break;
            case Invalid_Massage:
                return get_record_params_cli(0,NULL);
                break;
            case Failed:
                printf("error accour: %d\n",message_header.data[Error_number_index]);
                break;
            }
    }

    return 1;

}

int set_record_params_cli(int n,char** params){
    if(n!=2){
        printf("error not fiten number of arguments\n");
        return 0;
    }
    name_and_gop p;
    strcpy(p.name,params[0]);
    p.gop=atoi(params[1]);


    message_header.header_size=sizeof(header);
    message_header.type=Set_parameters;
    message_header.opcode=Start_record;
    message_header.data_size=sizeof(p);
    message_header.reserve=0;
    memcpy(message_header.data,&p,sizeof(p));
    message_header.Checksum=get_checksum(message_header.data);

    send_message_header(&message_header);
    get_message_header(&message_header);
    if(check_checksum( message_header.Checksum,message_header.data)&&
            message_header.type==Acknowledge &&
            message_header.opcode==Start_record)
    {
        if(message_header.data[Ack_index]==Ack)
        {
            printf("got right\n");
        }
        else
            switch(message_header.data[Nack_states_index]){
            case Command_Error:
                return 0;
                break;
            case Invalid_Massage:
                return set_record_params_cli(0,NULL);
                break;
            case Failed:
                printf("error accour: %d\n",message_header.data[Error_number_index]);
                break;
            }
    }

    return 1;

}

int get_snapshot_params_cli(int n,char** params){
    if(n!=0){
        printf("error not fiten number of arguments\n");
        return 0;
    }
    message_header.header_size=sizeof(header);
    message_header.type=Get_parameters;
    message_header.opcode=Snapshot;
    message_header.data_size=0;
    message_header.reserve=0;
    message_header.data[0]='\0';
    message_header.Checksum=get_checksum(message_header.data);

    send_message_header(&message_header);
    get_message_header(&message_header);
    if(check_checksum( message_header.Checksum,message_header.data)&&
            message_header.type==Reply_massage &&
            message_header.opcode==Snapshot)
    {
        if(message_header.data[Ack_index]==Ack)
        {
            name_and_img_type n_a_i;
            memcpy(&n_a_i,message_header.data+Data_index,sizeof(n_a_i));
            char t[10];
            if(n_a_i.type==JPG)
                    strcpy(t,"jpg");
            if(n_a_i.type==BMP)
                    strcpy(t,"bmp");

            printf("snapshot:file_name:%s\n type:%s\n",n_a_i.name,t);
            printf("got right\n");
        }
        else
            switch(message_header.data[Nack_states_index]){
            case Command_Error:
                return 0;
                break;
            case Invalid_Massage:
                return get_snapshot_params_cli(0,NULL);
                break;
            case Failed:
                printf("error accour: %d\n",message_header.data[Error_number_index]);
                break;
            }
    }

    return 1;

}


int set_snapshot_params_cli(int n,char** params){
    if(n!=2){
        printf("error not fiten number of arguments\n");
        return 0;
    }
    name_and_img_type n_a_i;
    strcpy(n_a_i.name,params[0]);
    if(strcmp(params[1],"jpg"))
            n_a_i.type=JPG;
    if(strcmp(params[1],"jpg"))
            n_a_i.type=BMP;


    message_header.header_size=sizeof(header);
    message_header.type=Set_parameters;
    message_header.opcode=Snapshot;
    message_header.data_size=sizeof(n_a_i);
    message_header.reserve=0;
    memcpy(message_header.data,&n_a_i,sizeof(n_a_i));
    message_header.Checksum=get_checksum(message_header.data);

    send_message_header(&message_header);
    get_message_header(&message_header);
    if(check_checksum( message_header.Checksum,message_header.data)&&
            message_header.type==Acknowledge &&
            message_header.opcode==Snapshot)
    {
        if(message_header.data[Ack_index]==Ack)
        {
            printf("got right\n");
        }
        else
            switch(message_header.data[Nack_states_index]){
            case Command_Error:
                return 0;
                break;
            case Invalid_Massage:
                return set_snapshot_params_cli(0,NULL);
                break;
            case Failed:
                printf("error accour: %d\n",message_header.data[Error_number_index]);
                break;
            }
    }

    return 1;

}

int turn_off_cli(int n,char** params){
    if(n!=0){
        printf("error not fiten number of arguments\n");
        return 0;
    }

    message_header.header_size=sizeof(header);
    message_header.type=Control;
    message_header.opcode=Turn_off;
    message_header.data_size=0;
    message_header.reserve=0;
    message_header.data[0]='\0';
    message_header.Checksum=get_checksum(message_header.data);

    send_message_header(&message_header);
    get_message_header(&message_header);
    if(check_checksum( message_header.Checksum,message_header.data)&&
            message_header.type==Acknowledge &&
            message_header.opcode==Turn_off)
    {
        if(message_header.data[Ack_index]==Ack)
        {
            printf("got right\n");
            return -1;
        }
        else
            switch(message_header.data[Nack_states_index]){
            case Command_Error:
                return 0;
                break;
            case Invalid_Massage:
                return turn_off_cli(0,NULL);
                break;
            case Failed:
                printf("error accour: %d\n",message_header.data[Error_number_index]);
                break;
            }
    }

    return 1;

}

int download_file_cli(int n,char** params){

    if(n!=2){
        printf("error not fiten number of arguments\n");
        return 0;
    }
    char* file_name=params[0];

    message_header.header_size=sizeof(header);
    message_header.type=Get_parameters;
    message_header.opcode=Download_file;
    message_header.data_size=strlen(message_header.data)+1;
    message_header.reserve=0;

    strcpy(message_header.data,file_name);
    char offset[2];
    offset[0]=atoi(params[1]);
    offset[1]='\0';
    strcat(message_header.data,offset);

    message_header.data[message_header.data_size]='\0';
    message_header.Checksum=get_checksum(message_header.data);

    send_message_header(&message_header);
    get_message_header(&message_header);
    if(check_checksum( message_header.Checksum,message_header.data)&&
            message_header.type==Reply_massage &&
            message_header.opcode==Download_file)
    {
        if(message_header.data[Ack_index]==Ack)
        {
            printf("got right\n");
            /*save file*/
            FILE *fd;
            fd = fopen(file_name, "wb");
                if (!fd) {
                    fprintf(stderr, "Could not open %s\n",file_name);
                    exit(1);
                }

            fwrite(message_header.data+Data_index, 1, sizeof(message_header.data+Data_index),fd);
            fclose(fd);
            return 1;
        }
        else
            switch(message_header.data[Nack_states_index]){
            case Command_Error:
                return 0;
                break;
            case Invalid_Massage:
                return download_file_cli(0,NULL);
                break;
            case Failed:
                printf("error accour: %d\n",message_header.data[Error_number_index]);
                break;
            }
    }

    return 1;

}
