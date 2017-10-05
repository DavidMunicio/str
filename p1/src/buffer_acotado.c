#include <stdbool.h>
#include "buffer_acotado.h"
#include <stdio.h> 
#include <signal.h>

int limited_buffer_create  (limited_buffer_t *buffer, int buff_size) {
	bool estado = true;
	int i;

	//Si buff_size > TAMANIO devolver falso
	if ( buff_size > TAMANIO ) {
		estado = false;
		printf("Tamanio superado");
	} else {
		//Inicializar los contadores
		printf("Tamanio bueno\n");
		for(i = 0; i < buff_size; i++ ) {
			buffer->cuenta = 0;
			buffer->primero = 0;
			buffer->ultimo = 0;
		}

		//Inicializar el mutex
		pthread_mutex_init (&buffer->mutex, NULL);

		//Inicializar las variables de condicion
		pthread_cond_init (&buffer->lleno, NULL);
		pthread_cond_init (&buffer->vacio, NULL);
	}

	//Devolver verdadero
	return estado;

}

void limited_buffer_destroy (limited_buffer_t  *buffer) {
	//Destruir mutex
	pthread_mutex_destroy(&buffer->mutex);

	//Destruir las variables de condición
	pthread_cond_destroy(&buffer->lleno);
	pthread_cond_destroy(&buffer->vacio);

}

void limited_buffer_put (limited_buffer_t  *buffer, int  elem) {
	//Acceder a la región crítica a través del mutex
	pthread_mutex_lock(&buffer->mutex);

	//Mientras el buffer esté lleno, bloquearse en la variable de condición lleno
	while(buffer->cuenta == buffer->buff_size) {
		printf("*************************************************************************************\n");
		printf("Buffer lleno, esperando a recibir la senial apropiada para ser despertado y continuar\n");
		printf("*************************************************************************************\n");
		pthread_cond_wait(&buffer->lleno, &buffer->mutex);
	}

	printf("añadiendo el elemento %d a la posicion %d\n", elem, buffer->ultimo);

	buffer->buf[buffer->ultimo] = elem;
	buffer->cuenta = buffer->cuenta +1;
	buffer->ultimo = buffer->ultimo +1;

	if (buffer->ultimo == TAMANIO)
		buffer->ultimo = 0;

	printf("El buffer tiene actualmente %d elementos\n", buffer->cuenta);

	//Liberar el mutex
	pthread_mutex_unlock(&buffer->mutex);

	//Hacer un signal en la variable de condición vacio
	pthread_cond_signal(&buffer->vacio);

}

void limited_buffer_get (limited_buffer_t  *buffer, int *elem) {
	//Acceder a la región crítica a través del mutex
	pthread_mutex_lock(&buffer->mutex);

	//Mientras el buffer esté vacío, bloquearse en la variable de condición vacio
	while(buffer->cuenta == 0) {
		printf("/////////////////////////////////////////////////////////////////////////////////////\n");
		printf("Buffer vacio, esperando a recibir la senial apropiada para ser despertado y continuar\n");
		printf("/////////////////////////////////////////////////////////////////////////////////////\n");
		pthread_cond_wait(&buffer->vacio, &buffer->mutex);
	}

	//Guardar en *elem el primer elemento del buffer y actualizar cuenta y primero
	printf("\t\tobteniendo el elemento %d, de posicion %d habiendo %d elementos\n", buffer->buf[buffer->primero], buffer->primero, buffer->cuenta);


	*elem = *&buffer->buf[buffer->primero];
	buffer->primero = buffer->primero +1;
	buffer->cuenta = buffer->cuenta -1;
	if (buffer->primero == buffer->buff_size) {
		printf("\t*********************\n");
		printf("\treiniciando el buffer\n");
		printf("\t*********************\n");
		buffer->primero = 0;
	}

	printf("\t\tActualmente hay %d elementos\n", buffer->cuenta);

	//Liberar el mutex
	pthread_mutex_unlock(&buffer->mutex);

	//Hacer un signal en la variable de condición lleno
	pthread_cond_signal(&buffer->lleno);

}
