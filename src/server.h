//server.h
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<errno.h>
#include<unistd.h>
#include<signal.h>
#include<pthread.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>

#define _BACK_LOG_  10
#define MAXLINE 1024
#define MAX_CLIENT_NUM 100

void server_response(int socketfd, char* input);

pthread_t g_controller_id;
