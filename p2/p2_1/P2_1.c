//Compilación por consola: gcc p2_1.c barrera.c -o p2_1 -lpthread
//Configuración en eclipse: botón derecho sobre el proyecto -> Properties -> C/C++ Build -> Settins -> Cross GCC Linker -> Libraries -> Añadir pthread

#include "barrera.h"
#include "stdlib.h"

typedef struct {
	barrera_t *b;
	int idhilo; //Nos servirá para que el usuario pueda identificar los hilos
} hilo_t;

void HiloEnBarrera (hilo_t *h) {
	printf("Hilo %d entrando en la barrera\n", h->idhilo);
	//Invocar a la barrera
	sincronizar(h->b);

	printf("Hilo %d saliendo de la barrera\n", h->idhilo);
}

int main (int argc, char *argv[]) {
	if (argc!=2) {
		printf("Error en la ejecución. El formato correcto es p2_1 num_hilos\n");
		return 0;
	}
	int num_hilos=atoi(argv[1]), i;
	//Crear un array de identificadores de hilos de tipo pthread_t de tamaño num_hilos
	pthread_t arrayid[num_hilos];

	//Crear una barrera e inicializarla con num_hilos
	barrera_t barrera;
	crea ( &barrera, num_hilos);

	//Crear un array de datos de tipo hilo_t de tamaño num_hilos
	hilo_t arraydata[num_hilos];
	//Para cada hilo
	for(i = 0; i < num_hilos; i++) {
		//Asignar en la posición correspondiente del array de datos un identificador (único para cada hilo)
		arraydata[i].idhilo = i;
		//Asignar en la posición correspondiente del array de datos la dirección de memoria de la barrera (mismo valor para todos los hilos)
		arraydata[i].b = &barrera;
		//Crear un hilo usando la posición correspondiente del array de identificadores, la función HiloEnBarrera y
		//la posición correspondiente del array de datos
		pthread_create(&arrayid[i], NULL, (void *) HiloEnBarrera, (void *) &arraydata[i] );
	}

	//Esperar por todos los hilos
	for (i = 0; i < num_hilos; i++) {
		pthread_join(arrayid[i],NULL);
	}

	//Destruir la barrera
	destruye(&barrera);
}
