#include <netinet/in.h> //structure for storing address information
#include <stdio.h>
#include <string.h> //para la función strcmp()
#include <stdlib.h>
#include <sys/socket.h> //for socket APIs
#include <sys/types.h>
#include <unistd.h> //para la función close()

int main(int argc, char const* argv[])
{
	// create server socket similar to what was done in
	// client program
	int servSockD = socket(AF_INET, SOCK_STREAM, 0);

	// define server address
	struct sockaddr_in servAddr;

	servAddr.sin_family = AF_INET;
	servAddr.sin_port = htons(9001);
	servAddr.sin_addr.s_addr = INADDR_ANY;

	// bind socket to the specified IP and port
	bind(servSockD, (struct sockaddr*)&servAddr, sizeof(servAddr));

	// listen for connections
	listen(servSockD, 1);

	// integer to hold client socket.
	int clientSocket = accept(servSockD, NULL, NULL);

	// send's messages to client socket
	char strData[255];
	
	int comprobacion = 1;

	while(comprobacion){
		recv(clientSocket, strData, sizeof(strData), 0);

		if (strcmp(strData, "salir") == 0){
			char salir[255] = "Gracias por usar nuestra shell remota";
			send(clientSocket, salir, sizeof(salir), 0);
			close(clientSocket);
			close(servSockD);
			comprobacion = 0;
		}
		else if (strcmp(strData, "ls") == 0){
			char ls[255] = "Este es el comando ls";
			send(clientSocket, ls, sizeof(ls), 0);
		}
		else { 
			char response[255] = "Comando no reconocido"; 
			send(clientSocket, response, sizeof(response), 0);
		}
	}
	return 0;
}
