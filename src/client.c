#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<errno.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>


#define MAXLINE 1024

static void usage(char *proc)
{
	printf("client : %s Please input server's IP and port\n",proc);
}

int main(int argc,char *argv[])
{
	if(argc != 3)
	{
		usage(argv[0]);
		exit(1);
	}
	char * ip = argv[1];
	uint16_t port = atoi(argv[2]);

	//int socket(int domain.int type,int protocol)
	int sock = socket(AF_INET,SOCK_STREAM,0);
	if(sock == -1)
	{
		printf("socket error:%d,%s\n",errno,strerror(errno));
		exit(1);
	}

	struct sockaddr_in remote;
	remote.sin_family = AF_INET; //ipv4
	remote.sin_port = htons(port);//host->net short
	//int inet_aton(const char* strptr,struct in_addr *addrptr)
	inet_aton(ip ,&remote.sin_addr);//char* -> inaddr
	//remote.sin_addr.s_addr = htonl(ip);
	
	int ret = connect(sock,(struct sockaddr*)&remote,sizeof(remote));

	if(ret < 0)
	{
		perror("connect");
		exit(1);
	}
	printf("connect success\n");

	char buf[1024];
	memset(buf,'\0',sizeof(buf));
	while(1)
	{
		//read from terminal
		ssize_t ret = read(0,buf,sizeof(buf) - 1);
		if(ret < 0)
		{
			perror("read");
			break;
		}
		buf[ret] = '\0';
		//ssize_t =  singed size_t
		ssize_t  _size = write(sock,buf,strlen(buf));
		if(_size < 0)
		{
			perror("write");
			break;
		}

	char buf_read[1024];
		ret = read(sock,buf_read,sizeof(buf_read) - 1);
		if(ret < 0)
		{
			perror("read");
			break;
		}
		else if(ret == 0)
		{
			printf("client release!\n");
			break;
		}
		else
		{
			buf_read[ret] = '\0';
			if (!strcmp(buf_read, "keep_alive_echo"))
			{
				n = write(sock,"keep_alive_ack",15);
			}
			else
				printf("server response:-> %s",buf_read);
		}

	}
	close(sock);
	printf("connect release\n");
	return 0;
}
