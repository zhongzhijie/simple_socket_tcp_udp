#include <sys/types.h>
#include <sys/socket.h>
#include <string.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <unistd.h>

#define MAXLINE 80
#define SERV_PORT 8888

void do_cli(FILE *fp, int sockfd, struct sockaddr *pservAddr, socklen_t servLen)
{
	int recvLen;
	char sendBuf[MAXLINE];
	char recvBuf[MAXLINE];
	
	printf("please input message to send:\n");
	while(fgets(sendBuf, MAXLINE, fp) != NULL)
	{
		sendto(sockfd, sendBuf, strlen(sendBuf), 0, pservAddr, servLen);
		recvLen = recvfrom(sockfd, recvBuf, MAXLINE, 0, NULL, NULL);
		if(recvLen == -1)
		{
			perror("recvfrom error");
			exit(1);
		}
		recvBuf[recvLen] = 0;
		printf("message from server reply:\n");
		fputs(recvBuf, stdout);
	}
}

int main(int argc, char **argv)
{
	int sockfd;
	struct sockaddr_in servAddr;

	if(argc != 2)
	{
		printf("usage: udpclient <IPaddress>\n");
		exit(1);
	}

	bzero(&servAddr, sizeof(servAddr));
	servAddr.sin_family = AF_INET;
	servAddr.sin_port = htons(SERV_PORT);
	if(inet_pton(AF_INET, argv[1], &servAddr.sin_addr) <= 0)
	{
		printf("[%s] is not a valid IPaddress\n", argv[1]);
		exit(1);
	}

	sockfd = socket(AF_INET, SOCK_DGRAM, 0);

	do_cli(stdin, sockfd, (struct sockaddr *)&servAddr, sizeof(servAddr));

	return 0;
}
