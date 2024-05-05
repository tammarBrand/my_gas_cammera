#ifndef MESSAGE_H
#define MESSAGE_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdbool.h>

#define data_length 50
#define PORT     8080
#define MAGIC 0x10203040
#define FILE_NAME_LENGTH 50

typedef enum Type{
    Control=1,
    Operation=2,
    Get_parameters=3,
    Set_parameters=4,
   /*Get_status =5,*/
    Acknowledge =6,
    Reply_massage=7
}Type;
typedef enum Opcode {
    Turn_off=1,
    Start_record =10,
    Stop_record=11,
    Snapshot=21,
 /* Get_record_params=60,
    Set_record_params=61,
    Get_snapshot_params=62,
    Set_snapshot_params=63,*/
    Get_file_name_by_index=80,
    Download_file=81,
    Get_status=95,
}Opcode;

typedef enum Acknowledge_states{
    Ack=1,
    Nack=2
}Acknowledge_states;

typedef enum Nack_states{
    Command_Error=1,
    Invalid_Massage=2,
    Failed=3,
}Nack_states;

typedef enum Data_indexes{

    Ack_index=0,
    Nack_states_index=1,
    Error_number_index=2,
    Data_index=1,

}Data_indexes;
/*
typedef enum Image_type{
    JPG=1,
    BMP=2
}Image_type;

typedef struct name_and_gop{
    char name[FILE_NAME_LENGTH];
    int gop;
}name_and_gop;

typedef struct name_and_img_type{
    char name[FILE_NAME_LENGTH];
    Image_type type;
}name_and_img_type;
*/
typedef struct header{
    int magic;
    int header_size;
    Type type;
    Opcode opcode;
    int data_size;
    int Checksum;
    long reserve;
    char  data[data_length];
}header;



void init_cli();
void init_server();
void get_message_header(header* message_header);
void send_message_header(header* message_header);
int get_checksum(char data[]);
bool check_checksum(int sum,char data[]);

#endif // MESSAGE_H
