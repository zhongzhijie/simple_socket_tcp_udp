#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <netinet/in.h>

#define TCP_SERVER_MAXCON       32
#define TCP_BUFFER_SIZE         128
#define TCP_SERVER_PORTNUM      6000

void str_echo(int sockfd)
{
	int recvLen;
	char recvBuf[TCP_BUFFER_SIZE];
	char sendBuf[TCP_BUFFER_SIZE] = "receive success,thank you,client!";
	while(recvLen <= 0)
	{
		recvLen = recv(sockfd,recvBuf,TCP_BUFFER_SIZE,0);
		if(recvLen > 0 )
		{
			recvBuf[recvLen] = 0;
			printf("message from client is:\n%s\n",recvBuf);
			write(sockfd,sendBuf,strlen(sendBuf));
			recvLen = 0;
		}
	}
}

int main(int argc,char **argv)
{
	struct sockaddr_in clientAddr;
	struct sockaddr_in serverAddr;
	int serverSocket,clientFd;
	int cliLen;
	int childpid;

	bzero(&serverAddr,sizeof(serverAddr));
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(TCP_SERVER_PORTNUM);
	serverAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	
	if((serverSocket=socket(AF_INET,SOCK_STREAM,0)) == -1)
	{
		perror("socket");
		exit(1);
	}
	printf("\nsocket success! socket = %d\n",serverSocket);

	if(bind(serverSocket,(struct sockaddr *)&serverAddr,sizeof(serverAddr)) == -1)
	{
		perror("bind");
		exit(1);
	}
	printf("bind success!\n");

	if(listen(serverSocket,TCP_SERVER_MAXCON) == -1)
	{
		perror("listen");
		exit(1);
	}
	printf("server listening......\n");
	
	while(1)
	{
		cliLen = sizeof(clientAddr);

		clientFd=accept(serverSocket,(struct sockaddr *)&clientAddr,&cliLen);
		if((childpid = fork()) == 0)
		{
			close(serverSocket);
			str_echo(clientFd);
			exit(0);
		}
		close(clientFd);
	}
	return 0;
}
