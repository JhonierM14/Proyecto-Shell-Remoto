#include <netinet/in.h> //structure for storing address information
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h> //for socket APIs
#include <sys/types.h>

int main(int argc, char const* argv[])
{
	//Conectarse
	char comando[255]; //Dónde se va a guardar el comando

	int sockD = socket(AF_INET, SOCK_STREAM, 0);

	struct sockaddr_in servAddr;

	servAddr.sin_family = AF_INET;
	servAddr.sin_port = htons(9001); // use some unused port number
	servAddr.sin_addr.s_addr = INADDR_ANY;

	int connectStatus = connect(sockD, (struct sockaddr*)&servAddr,	sizeof(servAddr));
	
	//Read / Write
	printf("Escriba el comando que le va a mandar al servidor: \n");
	scanf("%s", comando);
	send(sockD, comando, sizeof(comando), 0);

	if (connectStatus == -1) {
		printf("Error...\n");
		return 0;
	}

	else {
		char strData[255];

		recv(sockD, strData, sizeof(strData), 0);

		printf("Message: %s\n", strData);
	}
}
