#include "./headers/cli.h"
List* list;


int func1(int x, int y);
int func2();

int start_record_cli(int n,char** params);
int stop_record_cli(int n,char** param);
int do_snapshot_cli(int n,char** params);
int func1_cli(int n,char**);
int func2_cli(int n,char**);

List* createList();
void free_all(List*list);
void init(List* list);
void insert(List*list, char*name,genericFunc_cli func_cli,char* help);
char** getParams(char* cmd);
Func* getFunc(List*list,char*name);
int isInt(char*);





void handle_sigint(int signal){
    printf("--going to stop--\n");
    sleep(1);
    int status;
    get_status(&status);
    if((status&RECORD_START) && !(status&RECORD_STOP))
        stop_record_cli(0,"");
    free_all(list);
    printf("exiting\n");
    exit(1);

}

void getCommandAndActive(){

    char cmd[N+P_LENGTH+1], **params;
    Func* function;
    list=createList();
    init(list);
    init_gas_cam_cli();
    //status_app();
    int n, ans;
    bool condition=true;


    while(condition){
        gets(cmd);
        params=getParams(cmd);
        n=atoi(params[0]);
        if(!(function=getFunc(list,params[1]))){
            printf("no such a command\n");
        }
        else{
            if(!isHelp(params, function))
            {
                if(!(ans=function->func_cli(n,params+2)))
                {                    
                    printf("faild, help:");
                    puts(function->help);
                }


            }
        }

        for(int i=0;i<n;i++)
            free(params[i]);
        free(params);

        condition=ans!=-1;
        //gets(cmd);
        //&&strcmp(cmd,"end");
    }
    free_all(list);

}


char** getParams(char* cmd){
    char**params=(char **)calloc(N,sizeof(char*));
    if(params==NULL){
        printf("error mallocing\n");
        exit(0);
    }
    int len=N, i=1;
    char* token;
    token=strtok(cmd," ");
    while(token!=NULL){
        if(!strcmp(token," ")){
            token=strtok(NULL," ");
            continue;
        }
        if(i==len){
            params=(char**)realloc(params,len*1.5);
            if(params==NULL){
                printf("error mallocing\n");
                exit(0);
            }
            len*=1.5;
        }
        params[i++]=strdup(token);
        token=strtok(NULL," ");
    }
    char str[4];
    params[0]=(char*)malloc(sizeof(char)*4);
    sprintf(str,"%d",i-2);
    params[0]=strdup(str);
    return params;
}
Func* getFunc(List* list,char* name){

    if(name==NULL)
        return NULL;
    Func* ptr=list->head;

    while(ptr!=NULL){

        char* tmp=ptr->name;
        if(!strcmp(tmp,name))
            return ptr;

        ptr=ptr->next;
    }
    return NULL;
}
int isHelp(char** params,Func* function)
{
    if(!strcmp(params[0],"1")&&!strcmp(params[2],"-help"))
    {   puts(function->help);
        return 1;
    }
    return 0;
}


List* createList(){
    List* list=(List*)malloc(sizeof(List));
    if(list==NULL){
        printf("error mallocing\n");
        exit(0);
    }
    list->head=list->tail=NULL;
    return list;
}
void free_all(List* list){
    Func* f;
    while(list->head!=NULL){
        f=list->head;
        list->head=list->head->next;
        free(f->help);
        free(f->name);
        free(f);
    }
    free(list);


}
void init(List* list){


    insert(list,"func1",func1_cli,"func1 does nothing \n function: func1 <int> <int>\n");
    insert(list,"func2",func2_cli,"func2 does nothing function\n");
    insert(list,"start_record",start_record_cli,"start_record start recording degrees snapshots \n function: start_record\n");
    insert(list,"stop_record",stop_record_cli,"stop_record stop recording degrees snapshots \n function: stop_record\n");
    insert(list,"do_snapshot",do_snapshot_cli,"do_snapshot sreating a snapshot \n function: do_snapshot\n");
    insert(list,"get_record_params",get_record_params_cli,"prints filename and gop \n function: get_record_params [char*name] [int gop]\n");
    insert(list,"set_record_params",set_record_params_cli,"set filename and gop \n function: set_record_params [char*name] [int gop]\n");
    insert(list,"get_snapshot_params",get_snapshot_params_cli,"prints filename and img type \n function: get_snapshot_params\n");
    insert(list,"set_snapshot_params",set_snapshot_params_cli,"set filename andimg type \n function: set_snapshot_params [char*name] [char* type]\n");
    insert(list,"turn_off",turn_off_cli,"turning off the system \n function: turn_off\n");
    insert(list,"download_file",download_file_cli,"download asked file \n function: download_file [char* file_name] [int offset] \n");

     if(signal(SIGINT, handle_sigint)==SIG_ERR)
        printf("error\n");

}
void insert(List*list, char*name,genericFunc_cli func_cli,char* help){
    Func* n=(Func*)malloc(sizeof(Func));
    if(n==NULL)
    {   printf("error mallocing\n");
        exit(0);
    }
    n->func_cli=func_cli;
    n->help=strdup(help);
    n->name=strdup(name);
    n->next=NULL;

    if(list->head==NULL){
        list->head=list->tail=n;
        return;
    }
    list->tail->next=n;
    list->tail=n;
}

int isInt(char* params){
    for(int i=0;i<strlen(params);i++){
        if(!isdigit(params[i]))
            return 0;
    }
    return 1;
}
int func1(int x, int y){
    printf("int func1 x=%d, y=%d \n", x,y);
}
int func2(){
    printf("func2\n");
}
int func1_cli(int n,char** params){
    if(n!=2){
        printf("error not fiten number of arguments\n");
        return 0;
    }
    int x=0, y=0;
    if(isInt(params[0]))
        x=atoi(params[0]);
    else
    { printf("unexpected type\n");
        return 0;}
    if(isInt(params[1]))
        y=atoi(params[1]);
    else
    {
        printf("unexpected type\n");
        return 0;
    }
    func1(x,y);

    return 1;
}
int func2_cli(int n,char** params){printf("in func2_cli\n");}


