#include "buffer_acotado.h"
#include <stdio.h>
#include "pthread.h"
#include <signal.h>

#define REPETICIONES 50
#define NUM_HILOS 5

void hilo_productor(limited_buffer_t *b) {
	//Llamar REPETICIONES veces a limited_buffer_put mostrando un mensaje con el número que se introducirá
	for(int i = 0; i < REPETICIONES; i++) {
		limited_buffer_put(b, i);
		printf("En hilo productor llamando a limited_buffer_put para introducir el numero %d\n", i);
	}
}

void hilo_consumidor(limited_buffer_t *b) {
	//Llamar REPETICIONES veces a limited_buffer_get mostrando un mensaje con el número leído
	for(int i = 0; i < REPETICIONES; i++) {
		limited_buffer_get(b, &i);
		printf("leido el elemento de posicion %d \n", i);
	}
}


int main (void) {
	//Definir una variable limited_buffer_t
	limited_buffer_t buffer;
	int i;

	//Inicializar el buffer (si hay error, mostrar un mensaje y terminar)
	if ( !limited_buffer_create(&buffer, NUM_HILOS) ) {
		printf("error al construir el buffer");
	} else {
		printf("Sin error al construir el buffer\n");
		//Crear dos vectores idP e idC de tipo pthread_t y de tamaño NUM_HILOS para almacenar los identificadores de los productores y de los consumidores
		pthread_t idP[NUM_HILOS], idC[NUM_HILOS];

		//Lanzar los NUM_HILOS productores y consumidores
		for (i = 0; i < NUM_HILOS; i++) {
			pthread_create(&idP[i], NULL, (void *)&hilo_productor, (void *)&buffer);
			pthread_create(&idC[i], NULL, (void *)&hilo_consumidor, (void *)&buffer);
		}
		//Esperar a que terminen todos los productores y los consumidores
		for (i = 0; i < NUM_HILOS; i++) {
			pthread_join(idP[i], NULL);
			pthread_join(idC[i], NULL);
		}
	}

	//Destruir el buffer
	limited_buffer_destroy(&buffer);

	return 0;
}

