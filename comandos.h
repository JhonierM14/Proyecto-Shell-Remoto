#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <dirent.h> //para el comando ls
#include <errno.h>
#include <sys/stat.h>
#include <time.h>

// Variable global para guardar el tiempo de inicio del servidor
time_t tiempo_inicio;

// muestra el contenido de un directorio
void ejecutar_ls(char *response) {
    DIR *d = opendir(".");
    if (d) {
        struct dirent *de;
        while ((de = readdir(d)) != NULL) {
            strcat(response, de->d_name);
            strcat(response, "\n");
        }
        closedir(d);
    } else {
        strcpy(response, "Error: No se pudo abrir el directorio actual\n");
    }
}

// permite desplazarse entre archivos
void ejecutar_cd(const char *ruta, char *response) {
    while (*ruta == ' ') ruta++;
        if (strlen(ruta) == 0) {
            strcpy(response, "Error: No se especificó una ruta válida\n");
        } else if (chdir(ruta) == 0) {
            snprintf(response, 2048, "Directorio cambiado a: %s\n", ruta); //cambie sizeof(response) por el tamaño del array por un warning
        } else {
            snprintf(response, 2048, "Error: No se pudo cambiar al directorio: %s\n", ruta);
        }
}

// permite visualizar el contenido de un archivo
void ejecutar_cat(const char *filename, char *response) {
    FILE* file = fopen(filename, "r");

    if (file) {
        fread(response, 1, 2048 - 1, file);
        fclose(file);
    } else {
        snprintf(response, 2048, "Error abriendo archivo: %s", strerror(errno));
    }
}

// retorna la direccion de un directorio
void ejecutar_pwd(char *response) {
    getcwd(response, 1024);
}

// muestra la estrucutura de un directorio
void ejecutar_tree(const char *path, char *response, int level) {
    DIR *dir = opendir(path);
    if (dir == NULL) {
        snprintf(response, 2048, "Error: No se pudo abrir el directorio %s\n", path);
        return;
    }

    struct dirent *entry;
    while ((entry = readdir(dir)) != NULL) {
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) {
            continue;
        }

        // Formatear espacios según el nivel de profundidad
        for (int i = 0; i < level; i++) {
            strcat(response, "  ");
        }
        strcat(response, "|- ");
        strcat(response, entry->d_name);
        strcat(response, "\n");

        // Construir la ruta completa
        char subPath[1024];
        snprintf(subPath, sizeof(subPath), "%s/%s", path, entry->d_name);

        // Verificar si es un directorio
        struct stat st;
        if (stat(subPath, &st) == 0 && S_ISDIR(st.st_mode)) {
            ejecutar_tree(subPath, response, level + 1);
        }
    }
    closedir(dir);
}

//muestra las primeras lineas de un archivo
void ejecutar_head(const char *filename, char *response, int num_lines) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        snprintf(response, 2048, "Error abriendo archivo: %s\n", strerror(errno));
        return;
    }

    char line[1024];
    int count = 0;
    response[0] = '\0'; // limpiar la respuesta

    while (fgets(line, sizeof(line), file) != NULL) {
        strcat(response, line);
        count++;
        if (count >= num_lines) break; // Mostrar solo las primeras `num_lines` líneas
    }

    fclose(file);
}

// muestra las ultimas lineas de un archivo
// void ejecutar_tail(const char *filename, char *response, int num_lines) {
//     FILE *file = fopen(filename, "r");
//     if (file == NULL) {
//         snprintf(response, 2048, "Error abriendo archivo: %s\n", strerror(errno));
//         return;
//     }

//     char lines[num_lines][1024]; // Buffer circular para almacenar las últimas líneas
//     int count = 0;
//     response[0] = '\0'; // Asegurarse de limpiar la respuesta

//     while (fgets(lines[count % num_lines], sizeof(lines[0]), file) != NULL) {
//         count++;
//     }

//     int start = (count >= num_lines) ? count % num_lines : 0;
//     int displayed = (count < num_lines) ? count : num_lines;

//     for (int i = 0; i < displayed; i++) {
//         strcat(response, lines[(start + i) % num_lines]);
//     }

//     fclose(file);
// }

// Inicializa el tiempo de inicio del servidor
void inicializar_tiempo_servidor() {
    tiempo_inicio = time(NULL);
}

// tiempo que lleva el servidor en funcionamiento
void ejecutar_uptime(char *response) {
    time_t tiempo_actual = time(NULL);
    double segundos_transcurridos = difftime(tiempo_actual, tiempo_inicio);

    int horas = segundos_transcurridos / 3600;
    int minutos = (segundos_transcurridos - (horas * 3600)) / 60;
    int segundos = (int)segundos_transcurridos % 60;

    snprintf(response, 2048, "El servidor ha estado en funcionamiento por: %d horas, %d minutos y %d segundos.\n",
             horas, minutos, segundos);
}

// fecha y hora
void ejecutar_date(char *response) {
    time_t now = time(NULL);
    struct tm *t = localtime(&now);
    strftime(response, 1024, "Fecha y hora actual: %Y-%m-%d %H:%M:%S\n", t);
}

// hace un calculo sencillo
void ejecutar_calc(const char *input, char *response) {
    char operator;
    double num1, num2, result;

    if (sscanf(input, "%lf %c %lf", &num1, &operator, &num2) != 3) {
        strcpy(response, "Error: Formato incorrecto. Usa: calc <num1> <operador> <num2>\n");
        return;
    }

    switch (operator) {
        case '+':
            result = num1 + num2;
            break;
        case '-':
            result = num1 - num2;
            break;
        case '*':
            result = num1 * num2;
            break;
        case '/':
            if (num2 == 0) {
                strcpy(response, "Error: División por cero.\n");
                return;
            }
            result = num1 / num2;
            break;
        default:
            strcpy(response, "Error: Operador no válido. Usa +, -, *, /.\n");
            return;
    }

    snprintf(response, 2048, "Resultado: %.2lf\n", result);
}