# PROYECTO FINAL DE SISTEMAS OPERATIVOS (750001C)

## DOCENTE: John Alexander Sanabria
## UNIVERSIDAD DEL VALLE
## INTEGRANTES:
- David Santiago Guerrero Delgado (2324594-3743)
- Jhonier Mendez Bravo (2372226-3743)
- Cristian Daniel Guaza Mejia (2372225-3743)

En este proyecto nosotros desarrollamos una aplicación cliente/servidor la cual permite a un usuario ejecutar comandos en un computador remoto. Esta aplicación cliente/servidor tiene como cliente al usuario que ejecuta los comandos y el servidor es el computador remoto  donde se ejecutan los comandos.

## CÓMO EJECUTAR EN WINDOWS:

## CÓMO EJECUTAR EN LINUX:

en dos terminales diferentes ejecuta el primer comando en el primer terminal y el otro en la segunda terminal:
```
gcc cliente.c -o cliente
gcc servidor.c -o servidor
```

Primero en la terminal que hiciste gcc al servidor.c para iniciar el programa ejecuta el siguiente comando:
```
./servidor
```

En la otra terminal ejecuta el siguiente comando:
```
./cliente
```

¡Y listo! ya tienes un cliente conectado a un servidor para una shell remota