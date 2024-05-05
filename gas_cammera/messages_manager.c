#include "messages_manager.h"

Node* message_pull;
bool *is_in_use;
int *messages_to_use;
int give, get, size, capacity, used_to_give;

sem_t locker;

void init(int pull_capacity){

    message_pull=(Node*)calloc(capacity , sizeof(Node));
    if(message_pull==NULL){
        printf("error allocatimg message pull\n");
        exit(0);
    }
    is_in_use=(bool*)calloc(capacity,sizeof(bool));
    if(is_in_use==NULL){
        printf("error allocatimg bool pull \n");
        exit(0);
    }
    messages_to_use=(int*)calloc(capacity,sizeof(int));
    if(is_in_use==NULL){
        printf("error allocatimg to use pull \n");
        exit(0);
    }
    give=0;
    size=0;
    capacity=pull_capacity;
    used_to_give=-1;
    sem_init(&locker,0,1);

}

Node* get_new_messages(void* data){

    Node* message;

    sem_wait(&locker);

    if(size==capacity)
        return NULL;

    //  while(!is_in_use[give]) give=(give+1)%capacity;

    if(used_to_give==-1){
        is_in_use[give]=1;
        message= message_pull+give;
        give++;//give is inilized to the index-to-use pointer
    }
    else{
        message= message_pull+messages_to_use[used_to_give];
        used_to_give--;
    }

    sem_post(&locker);
    message->data=data;

    return message;

}

void free_message(Node* message){

    sem_wait(&locker);

    if(size==0)
        return;
    int index=message-message_pull;
    is_in_use[index]=0;
    messages_to_use[used_to_give++]=index;

    sem_post(&locker);

}

void free_message_pull(){

    if(message_pull!=NULL)
        free(message_pull);
    if(is_in_use!=NULL)
        free(is_in_use);

}
