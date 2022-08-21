#ifndef __FUNCIONES_VARIAS_H__
#define __FUNCIONES_VARIAS_H__

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "heap.h"
#include "lista.h"
#include "abb.h"

#define MAX_ESPECIE 50
#define MAX_NOMBRE 100

typedef struct pokemon {
	char especie[MAX_ESPECIE] ;
	int velocidad ;
	int ataque ;
	int defensa ;
	int bonificador ;
	bool esta_en_equipo ;
} pokemon_t ;

typedef struct entrenador {
	char nombre[MAX_NOMBRE] ;
	lista_t* lista_pokemon ;
} entrenador_t ;

typedef struct gimnasio {
	char nombre[MAX_NOMBRE] ;
	int dificultad ;
	int id_funcion ;
	lista_t* pila_entrenadores ;
} gimnasio_t ;

typedef struct personaje {
	char nombre[MAX_NOMBRE] ;
	lista_t* pokemon_combates ;
	abb_t* pokemon_obtenidos ;
} personaje_t ;

//Pre: Puede recibir un heap y recibe un string que es la ruta a un archivo.
//Post: Lee el archivo y devuelve carga los gimnasios en el heap. Además, 
//		reserva memoria para este y todas sus estructuras. Si hay algún error devuelve NULL.
//		La función devolverá el heap siempre y cuando este tenga al menos
//		un gimnasio, si alguno de los gimnasios no se lee bien, devuelve al 
//		heap con cuantos gimnasios haya leido exitosamente.

heap_t* cargar_gimnasios (heap_t* heap , const char* ruta_archivo) ;

//Pre: Recibe como primer elemento un pokemon válido (el segundo elemento no es
//		necesario y puede recibirse cualquier cosa ahí).
//Post: Imprime por pantalla la información sobre el pokemon recibido y devuelve true.

bool mostrar_pokemon (void* elemento, void* contexto) ;

//Función exactamente igual a mostrar_pokemon pero devuelve false así no detiene
//el iterador interno del abb.

bool mostrar_pokemon_abb (void* elemento, void* contexto) ;

//Pre: Recibe un gimnasio válido.
//Post: Imprime por pantalla datos importantes sobre el gimnasio 
//		que recibe como parámetro.

void mostrar_gimnasio (gimnasio_t* gimnasio) ;

//Pre: Recibe un puntero a un pokemon.
//Post: Libera la memoria reservada del pokemon.

void destructor_pokemon (void* elemento) ;

//Libera la memoria reservada por el gimnasio y cualquier estructura dentro de 
//este y destruye el gimnasio.

void destructor_gimnasios (void* elemento) ;

//Pre: Recibe un string que es la ruta a un archivo.
//Post: Lee el archivo y devuelve un puntero a un personaje. Además, reserva
//		la memoria necesaria para este y todas sus estructuras. Si hay algún
//		error la función devolverá al personaje siempre y cuando este haya cargado
//		por lo menos un pokemon, caso contrario devolverá NULL. Si algún pokemon
//		del personaje no se lee bien se devuelve al personaje con cuantos pokemon
//		se hayan leido exitosamente.

personaje_t* leer_personaje(const char* ruta_archivo) ;

//Pre: Recibe un personaje válido.
//Post: Imprime por pantalla la información importante del personaje.

void mostrar_personaje(personaje_t* personaje) ;

//Pre: Recibe un personaje.
//Post: Libera la memoria reservada por el personaje y todas sus estructuras.

void destruir_personaje(personaje_t* personaje) ;

#endif /* __FUNCIONES_VARIAS_H__ */