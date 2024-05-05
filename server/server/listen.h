#ifndef LISTEN_H
#define LISTEN_H
#include <assert.h>
#include <dlfcn.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdbool.h>
#include <time.h>


#include "message.h"

#define PORT     8080
#define MAXLINE 1024

#include "gas_cammera.h"




void listen_to_client();
void action();


#endif // LISTEN_H
