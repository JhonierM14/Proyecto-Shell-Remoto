#include <netinet/in.h> //estructura para guardar la información de dirección
#include <stdio.h>
#include <string.h> //para la función strcmp()
#include <stdlib.h>
#include <sys/socket.h> //para la APIs de socket
#include <sys/types.h>
#include <unistd.h> //para la función close()
#include <dirent.h> //para el comando ls

int main(int argc, char const* argv[])
{
	// create server socket similar to what was done in client program
	int servSockD = socket(AF_INET, SOCK_STREAM, 0);

	//definir la dirección del servidor
	struct sockaddr_in servAddr;

	servAddr.sin_family = AF_INET;
	servAddr.sin_port = htons(9001);
	servAddr.sin_addr.s_addr = INADDR_ANY;

	// bind socket to the specified IP and port
	bind(servSockD, (struct sockaddr*)&servAddr, sizeof(servAddr));

	//escuchar para conexiones
	listen(servSockD, 1);

	// integer to hold client socket.
	int clientSocket = accept(servSockD, NULL, NULL);

	char strData[255];

	while(1){
		recv(clientSocket, strData, sizeof(strData), 0); //recibe el comando del cliente

		if (strcmp(strData, "salida") == 0){
			char salir[255] = "Gracias por usar nuestra shell remota"; //definimos un array para guardar la respuesta del servidor
			send(clientSocket, salir, sizeof(salir), 0); //manda al cliente el mensaje de despedida
			close(clientSocket); //cierra el socket del cliente
			close(servSockD); //cierra el socket del servidor
			break; //sale del while
		}
		else if (strcmp(strData, "ls") == 0){
			char ls[1024] = {0}; //definimos un array para guardar la respuesta del servidor

			DIR *d; //puntero a la estructura DIR
			struct dirent *de; //puntero a la estructura dirent
			d = opendir("."); //abre el directorio actual

			while(de = readdir(d)){ //un while que recorre el directorio actual hasta que no haya más archivos
				strcat(ls, de->d_name); //concatena el nombre del archivo al array ls
				strcat(ls, "\n"); //concatena un salto de línea al array ls
			}

			send(clientSocket, ls, sizeof(ls), 0); //manda al cliente la cadena de caracteres del resultado del comando ls
		}
		else { 
			char response[255] = "Comando no reconocido"; //definimos un array para guardar la respuesta del servidor
			send(clientSocket, response, sizeof(response), 0); //manda al cliente el mensaje comando no reconocido
		}
	}
	return 0;
}