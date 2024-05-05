#include "../headers/message.h"
#define magic 0x10203040
#define MAXLINE 1024


enum statuses{READ=1,
              CHECK_MAGIC=2,
              GET_HEADER_SIZE=3,
              GET_HEADER=4,
              FINISH=7
             };

struct sockaddr_in source_add , dest_add;
int sockfd;
int dest_len;
char buffer[MAXLINE];
int buffer_index;


bool check_magic();
bool get_header();



void init_cli(){



    if ( (sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0 ) {
        perror("socket creation failed");
        exit(EXIT_FAILURE);
    }
    memset(&dest_add, 0, sizeof(dest_add));

    // Filling server information
    dest_add.sin_family = AF_INET;
    dest_add.sin_port = htons(PORT);
    dest_add.sin_addr.s_addr = INADDR_ANY;

    dest_len=sizeof(dest_add);

}

void init_server()
{

    // Creating socket file descriptor
    if ( (sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0 ) {
        perror("socket creation failed");
        exit(EXIT_FAILURE);
    }

    memset(&source_add, 0, sizeof(source_add));
    memset(&dest_add, 0, sizeof(dest_add));

    // Filling server information
    source_add.sin_family    = AF_INET; // IPv4
    source_add.sin_addr.s_addr = INADDR_ANY;
    source_add.sin_port = htons(PORT);

    // Bind the socket with the server address
    if ( bind(sockfd, (const struct sockaddr *)&source_add,
              sizeof(source_add)) < 0 )
    {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }
    dest_len = sizeof(dest_add);  //len is value/result
}


void read_from(){

    int n;
    n = recvfrom(sockfd,buffer,  MAXLINE ,
                 MSG_WAITALL, ( struct sockaddr *) &dest_add,(socklen_t*)&dest_len);
    if(n==-1){
        printf("cant read!\n");
        exit(0);
    }
    buffer[n] = '\0';

}

void get_message_header(header* message_header){
    int status;
    status=READ;
    while(status!=FINISH){
        switch (status) {
        case READ:
            read_from();
            status=CHECK_MAGIC;
            break;
        case CHECK_MAGIC:
            if(check_magic())
                status=GET_HEADER;
            else
                status=READ;
            break;
        case GET_HEADER:
            if(get_header(message_header))
                status=FINISH;
            else
                exit(0);
        }
    }
}


void send_message_header(header* message_header){

    int index=0;
    buffer[index++]=0x10;
    buffer[index++]=0x20;
    buffer[index++]=0x30;
    buffer[index++]=0x40;

    memcpy(&(buffer[index]),message_header,sizeof(header));

    sendto(sockfd, (const char *)buffer, MAXLINE,
           MSG_CONFIRM, (const struct sockaddr *) &dest_add,(socklen_t)dest_len);

}
bool check_magic(){

    char x=0x10;
    while(1){
        if(buffer_index==MAXLINE)
            return false;
        if(x==0x50){
            printf("index=%d\n",buffer_index);

            return true;
        }
        if((int)buffer[buffer_index]==x)
        {
            x+=0x10;
            printf("yes\n");
        }
        else
        {
            x=0x10;
        }
        buffer_index++;
    }


}

bool get_header(header* message_header){

    memcpy(message_header,buffer+buffer_index,sizeof(header));
    buffer_index+=sizeof(header);
    return true;
}


int get_checksum(char data[]){
    int sum=0;
    for(int i=0;i<(int)strlen(data);i++){
        sum+=(int)data[i];
    }
    return sum;
}

bool check_checksum(int sum,char data[]){

    int new_sum=get_checksum(data);
    return sum==new_sum;

}
