#include<sys/types.h>
#include<sys/socket.h>
#include<string.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>

#define MAXLINE 80
#define SERV_PORT 8888

void do_echo(int sockfd, struct sockaddr * cliAddr, socklen_t cliLen)
{
	int recvLen;
	int sockLen;
	char recvBuf[MAXLINE];
	char sendBuf[MAXLINE] = "server reply message!";

	while(1)
	{
		sockLen = cliLen;

		recvLen = recvfrom(sockfd, recvBuf, MAXLINE, 0, cliAddr, &sockLen);
		if(recvLen > 0)
		{
			recvBuf[recvLen] = 0;
			printf("message from client is:\n%s\n",recvBuf);
			sendto(sockfd, sendBuf, strlen(sendBuf), 0, cliAddr, sockLen);
		}
	}
}

int main(void)
{
	int sockfd;
	struct sockaddr_in servAddr;
	struct sockaddr_in cliAddr;

	sockfd = socket(AF_INET, SOCK_DGRAM, 0);

	bzero(&servAddr, sizeof(servAddr));
	servAddr.sin_family = AF_INET;
	servAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servAddr.sin_port = htons(SERV_PORT);

	if(bind(sockfd, (struct sockaddr *)&servAddr, sizeof(servAddr)) == -1)
	{
		perror("bind error");
		exit(1);
	}

	do_echo(sockfd, (struct sockaddr *)&cliAddr, sizeof(cliAddr));

	return 0;
}
