/*
######################################################################
# # DISCREPTION   : handle client require
	客户端请求处理逻辑：
	1.　"read"请求：所有用户都响应Success
	2.　"write"请求：只有controller用户响应Success，其他用户响应Failure
	3. "controller"请求：表示请求获取controller权限.
		当前没有controller用户响应Success，client为controller用户响应Success;
		否则响应Failure;
	4. 其他请求：除上述三种请求外都响应Failure
# # AUTHOR        : lu.liu <vivianliulu@126.com>
# # CREATED DATE  : 2017-11-08
# # MODIFIED DATE : 
# ######################################################################
*/

#include "server.h"

extern pthread_t g_controller_id;

void keep_alive()
{
	n = write(socketfd,"keep_alive_echo\n", 16);
}

int handle_require(char* input)
{
	int response = -1;
	if (!strcmp(input, "controller\n"))
	{
		if (g_controller_id == 0)
		{
			g_controller_id = pthread_self();
			response = 1;
		}
		else if (g_controller_id == pthread_self())
		{
			response = 1;
		}
		else
			response = 0;
	}
	else if (!strcmp(input, "write\n"))
	{

		if (g_controller_id == pthread_self())
			response = 1;
		else
			response = 0;
	}
	else if (!strcmp(input, "read\n"))
		response = 1;
	else
		response = 0;

	return response;
}

void server_response(int socketfd, char* input)
{
	ssize_t n = -1;
	int response = handle_require(input);

	if (response == 0)
		n = write(socketfd,"Failure\n", 8);
	else if (response == 1)
		n = write(socketfd,"Success\n", 8);
	else
	{
		printf("%s\n", "handle_require error");
		exit(1);
	}

	if (n<0)
	{
		perror("write");
		exit(1);
	}

	return;
}