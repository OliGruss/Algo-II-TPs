#include "funciones_varias.h"

#define GIMNASIO 'G'
#define LIDER 'L'
#define ENTRENADOR 'E'
#define POKEMON 'P'

#define ROJO "\033[1m\033[31m"
#define AMARILLO "\033[1m\033[33m"
#define VERDE "\033[0m\033[32m"
#define REINICIAR_COLOR "\033[0m"

#define MAX_POKEMON_COMBATE 6

//Pre: Recibe 2 punteros a gimnasios válidos.
//Post: compara ambos gimnasios según su nivel de dificultad. Si el primer
//		gimnasio es más dificil que el segundo devuelve 1, caso contrario devuelve
//		-1 y si son iguales devuelve 0.

int comparador_gimnasios (void* gym_1 , void* gym_2) {
	gimnasio_t* gimnasio_1 = gym_1 ;
	gimnasio_t* gimnasio_2 = gym_2 ;

	if (gimnasio_1->dificultad > gimnasio_2->dificultad) {
		return ES_MAYOR ;
	}
	if (gimnasio_1->dificultad < gimnasio_2->dificultad) {
		return ES_MENOR ;
	}
	else {
		return ES_IGUAL ;
	}
}

//Pre: Recibe un puntero a un gimnasio.
//Post: Libera la memoria del gimnasio y de todas las estructuras que tenga dentro.

void destructor_gimnasios (void* elemento) {
	gimnasio_t* gimnasio = elemento;
	if (gimnasio) {
		lista_destruir(gimnasio->pila_entrenadores) ;
		free(gimnasio) ;
	}
}

//Pre: Recibe un puntero a un entrenador.
//Post: Libera la memoria del entrenador y todas las estructuras que tenga dentro.

void destructor_entrenadores (void* elemento) {
	entrenador_t* entrenador = elemento ;
	if (entrenador) {
		lista_destruir(entrenador->lista_pokemon) ;
		free(entrenador) ;
	}
}

//Pre: Recibe un puntero a un pokemon.
//Post: Libera la memoria reservada del pokemon.

void destructor_pokemon (void* elemento) {
	pokemon_t* pokemon = elemento ;
	if (pokemon) {
		free(pokemon) ;
	}
}

//Pre: Recibe un archivo de gimnasios.
//Post: Lee el archivo y devuelve un pokemon, además, reserva memoria para
//		este. Si hay algún error devuelve NULL.

pokemon_t* leer_pokemon (FILE* archivo) {
	pokemon_t* pokemon = malloc(sizeof(pokemon_t)) ;
	if (pokemon == 	NULL) {
		return NULL ;
	}

	int leidos ;
	
	leidos = fscanf(archivo, "%50[^;];%i;%i;%i\n" , pokemon->especie , &pokemon->velocidad
					, &pokemon->ataque, &pokemon->defensa) ;
	if (leidos != 4) {
		destructor_pokemon(pokemon) ;
		return NULL ;
	}

	pokemon->bonificador = 0 ;
	pokemon->esta_en_equipo = false ;

	return pokemon ;
}

//Pre: Recibe un archivo de gimnasios.
//Post: Lee el archivo y devuelve un entrenador, además, reserva memoria para
//		este y todas sus estructuras. Si hay algún error devuelve NULL.
//		La función devolverá al entrenador siempre y cuando este tenga al menos
//		un pokemon, si alguno de los pokemon no se lee bien, devuelve al entrenador
//		con cuantos pokemon haya leido exitosamente.

entrenador_t* leer_entrenador (FILE* archivo, char* primer_caracter) {
	entrenador_t* entrenador = malloc(sizeof(entrenador_t)) ;
	if (entrenador == NULL) {
		return NULL ;
	}
	entrenador->lista_pokemon = NULL ;

	int leidos ;

	leidos = fscanf(archivo , "%100[^\n]\n" , entrenador->nombre) ;
	if (leidos != 1) {
		destructor_entrenadores(entrenador) ;
		return NULL ;
	}

	entrenador->lista_pokemon = lista_crear(destructor_pokemon) ;
	if (!entrenador->lista_pokemon) {
		destructor_entrenadores(entrenador) ;
		return NULL ;
	}

	leidos = fscanf(archivo , "%c;" , primer_caracter) ;
	if (leidos != 1 || *primer_caracter != POKEMON) {
		destructor_entrenadores(entrenador) ;
		return NULL ;
	}

	pokemon_t* pokemon = leer_pokemon(archivo) ;
	if (pokemon == NULL) {
		destructor_entrenadores(entrenador) ;
		return NULL ;
	}

	if (lista_insertar(entrenador->lista_pokemon , pokemon) == ERROR) {
		destructor_entrenadores(entrenador) ;
		return NULL ;
	}

	leidos = fscanf(archivo , "%c;" , primer_caracter) ;
	if (leidos != 1 || *primer_caracter != POKEMON) {
		return entrenador ;
	}

	bool hubo_algun_error = false ;

	while(*primer_caracter == POKEMON && hubo_algun_error == false) {
		pokemon = leer_pokemon(archivo) ;
		if (pokemon == NULL) {
			hubo_algun_error = true ;
		}
		else {
			if (lista_insertar(entrenador->lista_pokemon , pokemon) == ERROR) {
				hubo_algun_error = true ;
			}
			else {
				leidos = fscanf(archivo , "%c;" , primer_caracter) ;
			}
		}
	}

	return entrenador ;
}

//Pre: Recibe un archivo de gimnasio.
//Post: Lee el archivo y devuelve un gimnasio, además, reserva memoria para
//		este y todas sus estructuras. Si hay algún error devuelve NULL.
//		La función devolverá al gimnasio siempre y cuando este tenga al menos
//		un entrenador, si alguno de los entrenadores no se lee bien, devuelve al 
//		gimnasio con cuantos entrenadores haya leido exitosamente.

gimnasio_t* leer_gimnasio (FILE* archivo, char* primer_caracter) {
	gimnasio_t* gimnasio = malloc(sizeof(gimnasio_t)) ;
	if (gimnasio == NULL) {
		return NULL ;
	}
	gimnasio->pila_entrenadores = NULL ;

	int leidos ;

	leidos = fscanf(archivo , "%100[^;];%i;%i\n" , gimnasio->nombre , &gimnasio->dificultad
					, &gimnasio->id_funcion) ;
	if (leidos != 3) {
		destructor_gimnasios (gimnasio) ;
		return NULL ;
	}

	gimnasio->pila_entrenadores = lista_crear(destructor_entrenadores) ;
	if (!gimnasio->pila_entrenadores) {
		destructor_gimnasios(gimnasio) ;
		return NULL ;
	}

	leidos = fscanf(archivo , "%c;" , primer_caracter) ;
	if (leidos != 1 || *primer_caracter != LIDER) {
		destructor_gimnasios (gimnasio) ;
		return NULL ;
	}

	entrenador_t* entrenador = leer_entrenador(archivo , primer_caracter) ;
	if (entrenador == NULL) {
		destructor_gimnasios(gimnasio) ;
		return NULL ;
	}

	if (lista_apilar(gimnasio->pila_entrenadores, entrenador) == ERROR) {
		destructor_gimnasios(gimnasio) ;
		return NULL ;
	}

	bool hubo_algun_error = false ;

	while (*primer_caracter == ENTRENADOR && hubo_algun_error == false) {
		entrenador = leer_entrenador(archivo , primer_caracter) ;
		if (entrenador == NULL) {
			hubo_algun_error = true ;
		}
		else {
			if (lista_apilar(gimnasio->pila_entrenadores , entrenador) == ERROR) {
				hubo_algun_error = true ;
			}
		}
	}

	return gimnasio;
}

heap_t* cargar_gimnasios (heap_t* heap , const char* ruta_archivo) {
	FILE* archivo = fopen(ruta_archivo , "r") ;
	if (archivo == NULL) {
		return NULL ;
	}

	if (heap == NULL) {
		heap = heap_crear(comparador_gimnasios, destructor_gimnasios) ;
		if (heap == NULL) {
			fclose(archivo) ;
			return NULL ;
		}
	}

	char primer_caracter ;
	int leidos ;
	leidos = fscanf(archivo , "%c;" , &primer_caracter) ;
	if (leidos != 1 || primer_caracter != GIMNASIO) {
		heap_destruir(heap) ;
		fclose(archivo) ;
		return NULL ;
	}

	gimnasio_t* gimnasio = leer_gimnasio(archivo, &primer_caracter) ;
	if (gimnasio == NULL) {
		heap_destruir(heap) ;
		fclose(archivo) ;
		return NULL ;
	}

	if (heap_insertar(heap, gimnasio) == ERROR) {
		heap_destruir(heap) ;
		fclose(archivo) ;
		return NULL ;
	}

	bool hubo_algun_error = false ;

	while (primer_caracter == GIMNASIO && hubo_algun_error == false) {
		gimnasio = leer_gimnasio(archivo, &primer_caracter) ;
		if (gimnasio == NULL) {
			hubo_algun_error = true ;
		}
		else {
			if (heap_insertar(heap, gimnasio) == ERROR) {
				hubo_algun_error = true ;
			}
		}
	}


	fclose (archivo) ;
	return heap ;
}

//Pre: Recibe como primer elemento un pokemon válido (el segundo elemento no es
//		necesario y puede recibirse cualquier cosa ahí).
//Post: Imprime por pantalla la información sobre el pokemon recibido.

bool mostrar_pokemon (void* elemento, void* contexto) {
	pokemon_t* pokemon = elemento ;
	printf("Especie: %s.  Velocidad: %i. Ataque: %i. Defensa: %i.\n" , pokemon->especie, 
			pokemon->velocidad, pokemon->ataque, pokemon->defensa) ;
	return true ;
}

//Pre: Recibe como primer elemento un entrenador válido (el segundo elemento no es
//		necesario y puede recibirse cualquier cosa ahí).
//Post: Imprime por pantalla la información sobre el entrenador recibido.

bool mostrar_entrenador (void* elemento, void* contexto) {
	entrenador_t* entrenador = elemento ;
	printf (AMARILLO"%s.\n"REINICIAR_COLOR , entrenador->nombre) ;
	printf("Pokemon:\n") ;
	lista_con_cada_elemento (entrenador->lista_pokemon , mostrar_pokemon , NULL) ;
	return true ;
}

void mostrar_gimnasio (gimnasio_t* gimnasio) {
	printf(ROJO"%s.\n"REINICIAR_COLOR, gimnasio->nombre) ;
	printf ("Entrenadores:\n") ;
	lista_con_cada_elemento (gimnasio->pila_entrenadores, mostrar_entrenador, NULL) ;
	printf ("\n") ;
}

void destruir_personaje(personaje_t* personaje) {
	if (personaje) {
		arbol_destruir(personaje->pokemon_obtenidos) ;
		lista_destruir(personaje->pokemon_combates) ;
		free(personaje) ;
	}
}

//Pre: Recibe 2 pokemon válidos.
//Post: Compara la especie de ambos pokemon y devuelve 1 si el primer pokemon
// 		es alfabéticamente mayor al segundo, -1 en caso contrario y 0 si ambos
//		pokemon son de la misma especie.

int comparador_pokemon(void* elemento_1 , void* elemento_2) {
	pokemon_t* pokemon_1 = elemento_1 ;
	pokemon_t* pokemon_2 = elemento_2 ;

	if (strcmp(pokemon_1->especie, pokemon_2->especie) > 0) {
		return ES_MAYOR ;
	}
	if (strcmp(pokemon_1->especie, pokemon_2->especie) < 0) {
		return ES_MENOR ;
	}
	else {
		return ES_IGUAL ;
	}

}

personaje_t* leer_personaje(const char* ruta_archivo) {
	FILE* archivo = fopen(ruta_archivo , "r") ;
	if (archivo == NULL) {
		return NULL ;
	}

	personaje_t* personaje = malloc(sizeof(personaje_t)) ;
	if (personaje == NULL) {
		fclose(archivo) ;
		return NULL ;
	}
	personaje->pokemon_combates = NULL ;
	personaje->pokemon_obtenidos = NULL ;

	char primer_caracter ;
	int leidos ;
	leidos = fscanf(archivo, "%c;" , &primer_caracter) ;
	if (leidos != 1 || primer_caracter != ENTRENADOR) {
		fclose(archivo) ;
		destruir_personaje(personaje) ;
		return NULL ;
	}

	leidos = fscanf(archivo, "%100[^\n]\n" , personaje->nombre) ;
	if (leidos != 1) {
		fclose(archivo) ;
		destruir_personaje(personaje) ;
		return NULL ;
	}

	leidos = fscanf(archivo, "%c;" , &primer_caracter) ;
	if (leidos != 1 || primer_caracter != POKEMON) {
		fclose(archivo) ;
		destruir_personaje(personaje) ;
		return NULL ;
	}

	pokemon_t* pokemon = leer_pokemon(archivo) ;
	if (pokemon == NULL) {
		fclose(archivo) ;
		destruir_personaje(personaje) ;
		return NULL ;
	}
	pokemon->esta_en_equipo = true ;


	//Cabe destacar que, en este caso no le paso destructor cuando creo la cola
	//de pokemon a combatir. Esto es porque, aunque los pokemon se almacenen en el
	// heap, los mismos pokemon de la lista estarán también en el abb de pokemon obtenidos
	//y, el abb ya recibe la función destructora por ende, liberará la memoria.
	//Si le paso la función destructora de pokemon a esta lista, me dará problemas
	//con valgrind ya que estaría liberando 2 veces los mismos pokemon (una en esta
	//lista y otra en el abb).
	
	personaje->pokemon_combates = lista_crear(NULL) ;
	if (personaje->pokemon_combates == NULL) {
		fclose(archivo) ;
		destruir_personaje(personaje) ;
		return NULL ;
	}

	personaje->pokemon_obtenidos = arbol_crear(comparador_pokemon ,destructor_pokemon) ;
	if (personaje->pokemon_obtenidos == NULL) {
		fclose(archivo) ;
		destruir_personaje(personaje) ;
		return NULL ;
	}

	if (lista_insertar(personaje->pokemon_combates, pokemon) == ERROR) {
		fclose(archivo) ;
		destruir_personaje(personaje) ;
		return NULL ;
	}

	if (arbol_insertar(personaje->pokemon_obtenidos, pokemon) == ERROR) {
		fclose(archivo) ;
		destruir_personaje(personaje) ;
		return NULL ;
	}

	int contador = 1;
	bool hubo_algun_error = false ;
	leidos = fscanf(archivo, "%c;" , &primer_caracter) ;

	while(primer_caracter == POKEMON && hubo_algun_error == false) {
		pokemon = leer_pokemon(archivo) ;
		if (pokemon == NULL) {
			hubo_algun_error = true ;
		}
		else {
			if (contador < MAX_POKEMON_COMBATE) {
				if (lista_insertar(personaje->pokemon_combates, pokemon) == ERROR) {
					hubo_algun_error = true ;
				}
				pokemon->esta_en_equipo = true ;
				contador++ ;
			}
			if (hubo_algun_error == false) {
				if (arbol_insertar(personaje->pokemon_obtenidos, pokemon) == ERROR) {
					hubo_algun_error = true ;
				}
			}
			if (hubo_algun_error == false) {
				leidos = fscanf(archivo, "%c;", &primer_caracter) ;
			}
		}
	}

	fclose(archivo) ;
	return personaje ;
}

//Función exactamente igual a mostrar_pokemon pero devuelve false así no detiene
//el iterador interno del abb.

bool mostrar_pokemon_abb (void* elemento, void* contexto) {
	pokemon_t* pokemon = elemento ;
	printf("Especie: %s.  Velocidad: %i. Ataque: %i. Defensa: %i.\n" , pokemon->especie, 
			pokemon->velocidad, pokemon->ataque, pokemon->defensa) ;
	return false ;
}

void mostrar_personaje(personaje_t* personaje) {
	printf (VERDE"%s\n"REINICIAR_COLOR, personaje->nombre) ;
	printf ("\n") ;
	printf ("Pokemon para combatir:\n") ;
	lista_con_cada_elemento(personaje->pokemon_combates, mostrar_pokemon, NULL) ;
	printf ("\n") ;
	printf ("Pokemon obtenidos:\n") ;
	abb_con_cada_elemento(personaje->pokemon_obtenidos, ABB_RECORRER_INORDEN,
						 mostrar_pokemon_abb, NULL) ;
	printf ("\n") ;
}
