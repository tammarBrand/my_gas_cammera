#ifndef GAS_CAM_CLI_H
#define GAS_CAM_CLI_H
#include <stdio.h>
#include <assert.h>
#include <dlfcn.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include "gas_cammera.h"
#include "controller.h"
#include "message.h"

#define MAXLINE 1024



void init_gas_cam_cli();
void  free_gas_cam_cli();
void status_app();
int start_record_cli(int n,char** params);
int stop_record_cli(int n,char** params);
int do_snapshot_cli(int n,char** params);
int get_status(int * status);
int get_record_params_cli(int n,char** params);
int set_record_params_cli(int n,char** params);
int get_snapshot_params_cli(int n,char** params);
int set_snapshot_params_cli(int n,char** params);
int turn_off_cli(int n,char** params);
int download_file_cli(int n,char** params);
#endif // GAS_CAM_CLI_H
