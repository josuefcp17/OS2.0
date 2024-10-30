// Pinit.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/types.h>

#define SHM_SIZE 1024  // Tamaño del segmento de memoria compartida

int main() {
    key_t key;
    int shmid;
    char *data;

    // Generar una clave única para la memoria compartida
    key = ftok("Pinit.c", 'R');

    // Crear el segmento de memoria compartida
    if ((shmid = shmget(key, SHM_SIZE, 0644 | IPC_CREAT)) == -1) {
        perror("Error al crear la memoria compartida");
        exit(1);
    }

    // Adjuntar el segmento de memoria compartida
    data = shmat(shmid, (void *)0, 0);
    if (data == (char *)(-1)) {
        perror("Error al adjuntar la memoria compartida");
        exit(1);
    }

    // Escribir un número y "init" en la memoria compartida
    int number = 42;  // Puedes cambiar este número si lo deseas
    memcpy(data, &number, sizeof(int));  // Copiar el número
    strcpy(data + sizeof(int), "init");  // Copiar "init" después del número

    printf("Pinit: Número %d y 'init' escritos en la memoria compartida.\n", number);

    // Desadjuntar la memoria compartida
    if (shmdt(data) == -1) {
        perror("Error al desadjuntar la memoria compartida");
        exit(1);
    }

    return 0;
}
