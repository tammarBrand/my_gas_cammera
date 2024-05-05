#include "stage_lib.h"

struct stage_params* params_head;

void* main_stage_thread(void* arg);
/*
void* create_handler(void* pipe_header){

    stage_handler* my_handler=(stage_handler*)malloc(sizeof (stage_handler));
    my_handler->head=(stage*)pipe_header;
    my_handler->is_pipe_on=NULL;
    return my_handler;

}
void free_handler(void* my_handler){
    stage_handler* handle=(stage_handler*)my_handler;
    if(handle==NULL)
        return;
    free(handle);
}
*/

void* create_stage(Queue* sourse, Queue* dest, function func, void* params ,stage* next){

    stage* my_stage=(stage*)malloc(sizeof(stage));
    my_stage->destQu=dest;
    my_stage->sourseQu=sourse;
    my_stage->func=func;
    my_stage->next=next;
    my_stage->is_active=0;
    my_stage->params=params;
    return my_stage;
}


void free_stage(void* my_stage){

    if(my_stage==NULL)
        return;
    free((stage*)my_stage);
}
/*
void free_stages(void* my_stage){
    stage *prev, *head=(stage*)my_stage;
    prev=head;
    while(prev!=NULL){
        head=prev->next;
        free(prev);
        prev=head;
    }

}
void free_stages_and_queues(void* my_stage){
    stage *prev, *head=(stage*)my_stage;
    prev=head;
    Queue* q;

    while(prev!=NULL){
        head=prev->next;
        q=prev->destQu;
        freeQueue(q);
        free(prev);
        prev=head;
    }

}
*/
void start_pipe(void* head, int stages_number,bool* is_pipe_on){

    stage* head_stage=(stage*)head;
    stage_params *params;
    int i=0;
    head_stage->is_first=1;

    while (head_stage!=NULL&&i<stages_number) {

        i++;

        params=(stage_params*)malloc(sizeof(stage_params));
        params->is_pipe_on=is_pipe_on;
        params->my_stage=head_stage;
        params->next=NULL;
        head_stage->id=i;

        if(i==1)
            head_stage->is_first=1;
        else
            head_stage->is_first=0;
        if(i==stages_number)
            head_stage->is_last=1;
        else
            head_stage->is_last=0;

        pthread_create(&(head_stage->thread),NULL,main_stage_thread,params);
        head_stage=head_stage->next;

    }



    printf("%d stages are active\n",i);

}

void stop_pipe(void* head_stage){

    stage* ptr=((stage*)head_stage);
    //finish stages threads

    while (ptr!=NULL) {
        pthread_join(ptr->thread,NULL);
        ptr=ptr->next;
    }

    stage_params *prev_params, *params=params_head;
    //free stage_params
    prev_params=params;
    while (prev_params!=NULL) {
        params=prev_params->next;
        free(prev_params);
        prev_params=params;
    }

}


/*
 * since all thread have 'do while' loop,
 * means the first thread will create at least 2 matrix,
 * the others won't be stucked in the dequeue setion.
 * */

/*
 * we put value before the loop ,
 * because if we don't the condition to inilize the 'is_active'
 * variable will be asked on a data which has been send , or worse
 * has been freed...
 */

void* main_stage_thread(void* arg){

    printf("stage is action \n");
    bool* is_pipe_on=((stage_params*)arg)->is_pipe_on;
    stage* my_stage=((stage_params*)arg)->my_stage;


    Node *message;// last node to enter the pipeline
    if(my_stage->is_first){
        /* for first stage ->  creating a new message to send*/
        message=createNode(NULL);
        printf("----created----\n");
    }
    else{
        /* for the others stages get message to send from the previous stage's data */
        message=dequeue(my_stage->sourseQu);
    }

    do{

        message->data=my_stage->func(message->data, my_stage->params);

        if(my_stage->is_last){
            /* for last stage finish the message's cycle */
            printf("----freed----\n");
            freeNode(message);
        }
        else{
            /*for the others sending the message */
            enqueu(my_stage->destQu,message);
        }

        if(my_stage->is_first){
            /* for first stage ->  creating a new message to send*/
            message=createNode(NULL);
            printf("----created----\n");
        }
        else{
            /* for the others stages get message to send from the previous stage's data */
            message=dequeue(my_stage->sourseQu);
        }

        if(my_stage->is_first){
            /* first stage condition for stoping is PIPELINE_ON variable */
            my_stage->is_active=*is_pipe_on;
            if(!my_stage->is_active)
                printf("---000---- capture is finishing---0000---\n");
        }
        else{
            /* other stages' condition for stoping is geting NULL data value*/
            my_stage->is_active=message->data!=NULL;
            if(!my_stage->is_active)
                printf("id:  %i---000----finishing---0000---\n",my_stage->id);
        }

    }while(my_stage->is_active);

    if(my_stage->is_last){
        /* for last stage finish the message's cycle */
        freeNode(message);
        printf("----freed----\n");
    }
    else{
        /*for the others sending the message */
        enqueu(my_stage->destQu,message);
    }

}


