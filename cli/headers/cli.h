#ifndef CLI_H
#define CLI_H
#pragma once

#include <math.h>
#include <stdarg.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdio.h>
#include <dlfcn.h>
#include <assert.h>
#include <unistd.h>
#include <signal.h>
#include <pthread.h>
#include <time.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>

#include "./headers/controller.h"
#include "./headers/gas_cam_cli.h"

#define N 20
#define P_LENGTH 100
#define SIGINT 2


#define LENGTH 5

struct func;
typedef int (*genericFunc_cli)(int num,char**);

typedef struct func{
    char* name;
    genericFunc_cli func_cli;
    char* help;
    struct func* next;
}Func;

typedef struct list{
    Func*head;
    Func* tail;
}List;



void getCommandAndActive();

#endif // FUNCTIONS_H
