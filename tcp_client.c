#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <netinet/in.h>

#define TCP_BUFFER_SIZE         128
#define TCP_SERVER_PORTNUM      6000

/*****************************************************************************/
void str_cli(FILE *fp,int sockfd)
{
	char sendBuf[TCP_BUFFER_SIZE];
	char recvBuf[TCP_BUFFER_SIZE+1];
	int recvLen;
	printf("please input message:\n");
	
	while(fgets(sendBuf,TCP_BUFFER_SIZE,fp) != NULL)
	{
		if(send(sockfd,sendBuf,strlen(sendBuf),0) == -1)
		{
			perror("send");
			exit(1);
		}

		recvLen = read(sockfd,recvBuf,TCP_BUFFER_SIZE);
		if(recvLen <= 0)
		{
			perror("read");
			exit(1);
		}
		recvBuf[recvLen] = 0;
		printf("message from server is:\n");
		fputs(recvBuf,stdout);
	}
}

int main(int argc,char **argv)
{
	struct sockaddr_in serverAddr;  
	int clientSocket;

	if(argc != 2)
	{
		printf("usage:tcpClient <IPaddress>\n");
		exit(1);
	}

	if((clientSocket=socket(AF_INET,SOCK_STREAM,0)) == -1)
	{
		perror("socket");
		exit(1);
	}

	bzero(&serverAddr, sizeof(serverAddr));
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(TCP_SERVER_PORTNUM);
	inet_pton(AF_INET,argv[1],&serverAddr.sin_addr);

	if(connect(clientSocket,(struct sockaddr *)&serverAddr,sizeof(serverAddr)) == -1)
	{
		perror("connect");
		exit(1);
	}

	str_cli(stdin,clientSocket);

	return 0;
}


