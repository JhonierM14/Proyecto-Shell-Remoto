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
	//crear un servidor socket similar a lo que se hizo en el programa cliente
	int servSockD = socket(AF_INET, SOCK_STREAM, 0);

	//definir la dirección del servidor
	struct sockaddr_in servAddr;

	//llenar la estructura con la información del servidor
	servAddr.sin_family = AF_INET;
	servAddr.sin_port = htons(9001);
	servAddr.sin_addr.s_addr = INADDR_ANY;
	
	//bind socket a la IP y puerto especificados 
	bind(servSockD, (struct sockaddr*)&servAddr, sizeof(servAddr));

	//escuchar para conexiones
	listen(servSockD, 1);

	//numero entero para guardar el socket del cliente
	int clientSocket = accept(servSockD, NULL, NULL);

	char strData[255]; //definimos un array para guardar el comando del cliente

	while(1){ //un while que se ejecuta infinitamente hasta que se reciba el comando de salida
		// recv(clientSocket, strData, sizeof(strData), 0); //recibe el comando del cliente

		memset(strData, 0, sizeof(strData)); // Limpiar buffer
        recv(clientSocket, strData, sizeof(strData) - 1, 0);

        // Eliminar saltos de línea o espacios finales
        strData[strcspn(strData, "\n")] = '\0';

		if (strcmp(strData, "salida") == 0){
			char salir[] = "Gracias por usar nuestra shell remota"; //definimos un array para guardar la respuesta del servidor
			send(clientSocket, salir, strlen(salir), 0);
			close(clientSocket); //cierra el socket del cliente
			close(servSockD); //cierra el socket del servidor
			break; //sale del while
		}
		else if (strcmp(strData, "comandos") == 0){
			//definimos un array para guardar la respuesta del servidor
			char comandos[] = "Comandos disponibles:\n"
			"1. salida: para cerrar la conexión con el servidor\n"
			"2. ls: para ver los archivos en el directorio actual\n"
			"3. cd: para desplazarse entre los archivos"; 
			send(clientSocket, comandos, strlen(comandos), 0);
		}
		else if (strcmp(strData, "ls") == 0){
			char ls[2048] = {0}; //definimos un array para guardar la respuesta del servidor

			DIR *d; //puntero a la estructura DIR
			// struct dirent *de; //puntero a la estructura dirent
			d = opendir("."); //abre el directorio actual

			// while(de = readdir(d)){ //un while que recorre el directorio actual hasta que no haya más archivos
			// 	strcat(ls, de->d_name); //concatena el nombre del archivo al array ls
			// 	strcat(ls, "\n"); //concatena un salto de línea al array ls
			// }

			if (d) {
                struct dirent *de;
                while ((de = readdir(d)) != NULL) {
                    strcat(ls, de->d_name);
                    strcat(ls, "\n");
                }
                closedir(d);
            } else {
                strcpy(ls, "Error: No se pudo abrir el directorio actual\n");
            }

			send(clientSocket, ls, strlen(ls), 0);
		} 
		else if (strncmp(strData, "cd ", 3) == 0) {
            char response[1024] = {0};
            char *ruta = strData + 3;

            // Eliminar espacios iniciales
            while (*ruta == ' ') ruta++;
            if (strlen(ruta) == 0) {
                strcpy(response, "Error: No se especificó una ruta válida\n");
            } else if (chdir(ruta) == 0) {
                snprintf(response, sizeof(response), "Directorio cambiado a: %s\n", ruta);
            } else {
                snprintf(response, sizeof(response), "Error: No se pudo cambiar al directorio: %s\n", ruta);
            }
            send(clientSocket, response, strlen(response), 0);
        }
		else { 
			char response[] = "Comando no reconocido"; //definimos un array para guardar la respuesta del servidor
			send(clientSocket, response, strlen(response), 0); //manda al cliente el mensaje comando no reconocido
		}
	}
	return 0;
}