/*
######################################################################
# # DISCREPTION   : tcp-server
# # AUTHOR        : lu.liu <vivianliulu@126.com>
# # CREATED DATE  : 2017-11-08
# # MODIFIED DATE : 
# ######################################################################
*/
#include "server.h"

void usage(char *proc)
{
	printf("usage : %s Please input IP and port\n",proc);
}

int init_socket(char* ip, uint16_t port)
{
	int listenfd, connfd;

	if((listenfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
	{
		printf("socket error:%d, %s\n",errno,strerror(errno));
		exit(1);
	}

	struct sockaddr_in servaddr;
	memset(&servaddr, 0, sizeof(servaddr));
	servaddr.sin_family = AF_INET;//ipv4
	//servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	inet_aton(ip ,&servaddr.sin_addr);
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servaddr.sin_port = htons(port);

	if((bind(listenfd, (struct sockaddr*)&servaddr, sizeof(servaddr))) < 0)
	{
		printf("bind error:%d,%s\n",errno,strerror(errno));
		exit(1);
	}

	if((listen(listenfd, _BACK_LOG_))<0)
	{
		printf("listen error:%d,%s\n",errno,strerror(errno));
		exit(1);
	}

	return listenfd;
}

static void wait_child(int signal)
{
	pid_t exit_id;
	while((exit_id = waitpid(-1,NULL,WNOHANG)) > 0)
	{
		printf("one child had exit,pid is %d\n",exit_id);
	}
}

static void* thread_run(int arg)
{
	printf("create a new thread, thread_id is: %ld\n",pthread_self());

	int new_client = arg;
	char buf[1024];
	memset(buf,'\0',sizeof(buf));
	while(1)
	{
		//reading from new_client and store to buf
		ssize_t  _size = read(new_client,buf,sizeof(buf) - 1);
		if(_size < 0)
		{
			perror("read");
			break;
		}
		else if(_size == 0)
		{
			printf("client release!\n");
			break;
		}
		else
		{
			buf[_size] = '\0';
			printf("client: ID:%ld require:-> %s",pthread_self(), buf);
			server_response(new_client, buf);

		}
	}
	close(new_client);
}

int main(int argc, char *argv[])
{
	if (argc != 3)
	{
		usage(argv[0]);
		exit(1);
	}

	g_controller_id = 0;
	char * ip = argv[1];
	uint16_t port = atoi(argv[2]);

	int listenfd = init_socket(ip, port);
	signal(SIGCHLD, wait_child);

	struct sockaddr_in client_info; //output val
	socklen_t client_len = 0; //output val
	struct sockaddr_in cliaddr;
	pid_t childpid;

	while(1){
		//accept return a new socket,client's ip and port
		int new_client = accept(listenfd, (struct sockaddr*)&client_info, &client_len);
		if(new_client < 0)
		{
			perror("accept");
			continue;
		}
		printf("connect success\n");

		pthread_t tid;
		pthread_create(&tid, NULL,thread_run, (void*)new_client);
		pthread_detach(tid);

	}

	close(listenfd);
	return 0;
}