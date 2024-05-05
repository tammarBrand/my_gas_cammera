#ifndef STAGE_LIB_H
#define STAGE_LIB_H

#pragma once
#include "Queue.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <pthread.h>

struct handler;

typedef void* (*function)(void* data, void* handler);

typedef struct stage{
    function func;
    pthread_t thread;
    Queue* sourseQu;
    Queue* destQu;
    bool is_active;
    struct stage* next;
    void* params;
    int id;
    bool is_first;
    bool is_last;
}stage;

struct stage_params;

typedef struct stage_params{
    stage* my_stage;
    bool* is_pipe_on;
    struct stage_params *next;
}stage_params;


 /* void* create_handler(void* pipe_header);
    void free_handler(void* my_handler);

    void free_stages(void* my_stage);
    void free_stages_and_queues(void* my_stage);*/

    void* create_stage(Queue* sourse, Queue* dest, function func, void* params ,stage* next);
    void free_stage(void* my_stage);
    void start_pipe(void* head_stage, int stages_number, bool* is_pipe_on);
    void stop_pipe(void* my_handler);
    void* main_stage_thread(void* arg);



#endif // STAGE_LIB_H
