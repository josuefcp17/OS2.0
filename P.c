// P.c
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

    // Generar la misma clave que en Pinit.c
    key = ftok("Pinit.c", 'R');

    // Localizar el segmento de memoria compartida
    if ((shmid = shmget(key, SHM_SIZE, 0644)) == -1) {
        perror("Error al acceder a la memoria compartida");
        exit(1);
    }

    // Adjuntar el segmento de memoria compartida
    data = shmat(shmid, (void *)0, 0);
    if (data == (char *)(-1)) {
        perror("Error al adjuntar la memoria compartida");
        exit(1);
    }

    // Leer el número de la memoria compartida
    int shm_number;
    memcpy(&shm_number, data, sizeof(int));

    // Obtener el número y la cadena del usuario
    int user_number;
    char user_string[SHM_SIZE - sizeof(int) - 3];  // Ajustar el tamaño si es necesario
    printf("Ingresa un número: ");
    scanf("%d", &user_number);
    printf("Ingresa una cadena: ");
    scanf("%s", user_string);

    // Comparar los números
    if (user_number == shm_number) {
        printf("Los números son iguales.\n");

        // Escribir la cadena en la memoria compartida después de los primeros 3 bytes
        int offset = 3;  // Después del tercer byte
        strcpy(data + offset, user_string);

        printf("Cadena '%s' escrita en la memoria compartida.\n", user_string);
    } else {
        printf("Los números no son iguales.\n");
    }

    // Desadjuntar la memoria compartida
    if (shmdt(data) == -1) {
        perror("Error al desadjuntar la memoria compartida");
        exit(1);
    }

    return 0;
}
