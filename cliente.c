#include <netinet/in.h> //estructura para guardar la información de dirección
#include <stdio.h>
#include <string.h> //para la función strcmp()
#include <stdlib.h>
#include <sys/socket.h> //para la APIs de socket
#include <sys/types.h>
#include <unistd.h> //para la función close()

int main(int argc, char const* argv[])
{
	//Conectarse
	char comando[255]; //Array dónde se va a guardar el comando

	int sockD = socket(AF_INET, SOCK_STREAM, 0);

	struct sockaddr_in servAddr;

	servAddr.sin_family = AF_INET;
	servAddr.sin_port = htons(9001); //usar un puerto que no se esté usando
	servAddr.sin_addr.s_addr = INADDR_ANY;

	//Read / Write
	
	//si connect devuelve -1 no hubo conexión, si devuelve 0 si se conectó exitosamente
	int connectStatus = connect(sockD, (struct sockaddr*)&servAddr,	sizeof(servAddr));

	if(connectStatus == -1){
		printf("Error conectando con el servidor...\n"); 
		return 0;
	}

	while(connectStatus == 0){ //si connecStatus es -1 (no hay conexión con el servidor) nunca entra al while
		
		printf("\nEscriba el comando que le va a mandar al servidor: \n");
		scanf("%s", comando); //registra el comando escrito por el cliente en la array comando
		send(sockD, comando, sizeof(comando), 0); //le envia la array de comando al servidor

		char strData[2048];
		recv(sockD, strData, sizeof(strData), 0);

		printf("\nMessage: %s\n", strData);

		if (strcmp(comando, "salida") == 0){
			connectStatus = -1;
		}
	}

	printf("Cerrando la conexión con el servidor...\n"); 
	close(sockD); //cerrar el socket
	return 0;
}