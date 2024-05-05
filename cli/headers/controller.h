#ifndef CONTROLLER_H
#define CONTROLLER_H
#include <pthread.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>
#include "gas_cammera.h"



struct flash{
    int time;
    int is_active;
};
enum STATUS_MACHINE{
    BLINK_ON,
    BLINK_OFF,
    SNAPSHOT_ON,
    SNAPSHOT_OFF
};
enum TIME_LONG{
    RECORD_LONG=200000,
    SNAPSHOT_LONG=500000
};

void* contoller_fun(void* arg);

#endif // CONTROLLER_H
