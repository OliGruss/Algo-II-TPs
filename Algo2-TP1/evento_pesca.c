#include <stdio.h>
#include <stdlib.h>
#include "evento_pesca.h"

#define FORMATO_ARCHIVO_LECTURA "%100[^;];%i;%i;%50[^\n]\n"
#define FORMATO_ARCHIVO_ESCRITURA "%s;%i;%i;%s\n"
#define ERROR -1
#define EXITO 0

arrecife_t* crear_arrecife (const char* ruta_archivo) {
	FILE* archivo = fopen(ruta_archivo , "r") ;
	if (archivo == NULL) {
		printf ("Error. No existe el archivo de arrecife.\n") ;
		return NULL ;
	}
	arrecife_t* arrecife = malloc(sizeof(arrecife_t)) ;
	if (arrecife == NULL) {
		fclose(archivo) ;
		printf ("Error, no se pudo reservar memoria para el arrecife.\n") ;
		return NULL ;
	}

	pokemon_t pokemon_archivo ;
	int leidos = fscanf(archivo , FORMATO_ARCHIVO_LECTURA, pokemon_archivo.especie, &pokemon_archivo.velocidad, 
												&pokemon_archivo.peso, pokemon_archivo.color) ;
	if (leidos != 4) {
		printf ("Error. No se pudo leer ningun pokemon valido dentro del archivo del arrecife.\n") ;
		free (arrecife) ;
		fclose(archivo) ;
		return NULL ;
	}

	(*arrecife).cantidad_pokemon = 0 ;
	(*arrecife).pokemon = NULL ;
	(*arrecife).pokemon = realloc((*arrecife).pokemon , sizeof(pokemon_t) * (size_t) ((*arrecife).cantidad_pokemon + 1)) ;

	if ((*arrecife).pokemon == NULL) {
		printf ("Error. No se pudo reservar memoria suficiente para los Pokemon.\n") ;
		free (arrecife) ;
		fclose(archivo) ;
		return NULL ;
	}

	while((leidos == 4) && ((*arrecife).pokemon != NULL)) {
		*((*arrecife).pokemon + ((*arrecife).cantidad_pokemon)) = pokemon_archivo ;
		(*arrecife).cantidad_pokemon++ ;
		leidos = fscanf(archivo , FORMATO_ARCHIVO_LECTURA, pokemon_archivo.especie, &pokemon_archivo.velocidad, 
												&pokemon_archivo.peso, pokemon_archivo.color) ;
		if (leidos == 4) {
			(*arrecife).pokemon = realloc((*arrecife).pokemon , sizeof(pokemon_t) * (size_t) ((*arrecife).cantidad_pokemon + 1)) ;
		}
	}

	fclose (archivo) ;
	return arrecife ;
}

acuario_t* crear_acuario() {
	acuario_t* acuario = malloc(sizeof(acuario_t)) ;
	if (acuario == NULL) {
		return NULL ;
	}
	(*acuario).cantidad_pokemon = 0 ;
	(*acuario).pokemon = NULL ;
	return acuario ;
}

//Pre: Recibe un puntero que lleva a un arrecife valido y una funcion booleana que determina si un pokemon es trasladable o no.
//Post: Recorre el arrecife y devuelve la cantidad de Pokemon que son trasladables al acuario.

int pokemon_trasladables (arrecife_t* arrecife, bool (*seleccionar_pokemon)(pokemon_t*)) {
	int cant_pokemon_trasladables = 0 ;
	for (int i= 0 ; i< (*arrecife).cantidad_pokemon ; i++) {
		if (seleccionar_pokemon((*arrecife).pokemon + i) == true) {
			cant_pokemon_trasladables++ ;
		}
	}
	return cant_pokemon_trasladables ;
}

//Pre: Recibe un puntero que lleva a un arrecife valido y una posicion del pokemon a eliminar.
//Post: Elimina al pokemon que este en la posicion dada del arrecife.

void eliminar_pokemon (arrecife_t* arrecife , int posicion) {
	for (int i = posicion ; i< ((*arrecife).cantidad_pokemon -1); i++) {
		(*arrecife).pokemon[i] = (*arrecife).pokemon[i + 1] ;
	}
	(*arrecife).cantidad_pokemon-- ;
	(*arrecife).pokemon = realloc((*arrecife).pokemon, sizeof(pokemon_t) * (size_t)((*arrecife).cantidad_pokemon)) ;
}

int trasladar_pokemon (arrecife_t* arrecife , acuario_t* acuario , bool (*seleccionar_pokemon)(pokemon_t*), int cant_seleccion) {
	int pokemon_a_trasladar = pokemon_trasladables(arrecife , seleccionar_pokemon) ;
	if (pokemon_a_trasladar < cant_seleccion) {
		printf ("La cantidad de pokemon pedidos que cumplen las condiciones es mayor a la cantidad de estos que hay en el arrecife. Para conservarlos, no se trasladara ninguno.\n") ;
		return ERROR ;
	}
	if (pokemon_a_trasladar <= 0) {
		printf ("Ningun pokemon cumple las condiciones pedidas.\n") ;
		return EXITO ;
	}

	(*acuario).pokemon = realloc((*acuario).pokemon , (sizeof(pokemon_t) * (size_t) ((*acuario).cantidad_pokemon + 1))) ;
	if ((*acuario).pokemon == NULL) {
		printf ("Error. No se pudo reservar la memoria requerida para el primer pokemon del acuario.\n") ;
		return ERROR ;
	}

	int i= 0 ;
	int pokemon_trasladados = 0 ;
	while ((i < (*arrecife).cantidad_pokemon) && ((*acuario).pokemon != NULL)) {
		if ((seleccionar_pokemon((*arrecife).pokemon + i) == true) && (pokemon_trasladados < cant_seleccion)) {
			*((*acuario).pokemon + (*acuario).cantidad_pokemon) = (*arrecife).pokemon[i] ;
			(*acuario).cantidad_pokemon++;
			eliminar_pokemon(arrecife, i) ;
			(*acuario).pokemon = realloc((*acuario).pokemon , sizeof(pokemon_t) * (size_t) ((*acuario).cantidad_pokemon + 1)) ;
			pokemon_trasladados++ ;
		}
		else {
			i++ ;
		}
	}
	return EXITO ;
}

void censar_arrecife (arrecife_t* arrecife , void (*mostrar_pokemon)(pokemon_t*)) {
	for (int i= 0 ; i< (*arrecife).cantidad_pokemon ; i++) {
		mostrar_pokemon((*arrecife).pokemon + i) ;
	}
	printf ("\n") ;
}

int guardar_datos_acuario(acuario_t* acuario , const char* nombre_archivo) {
	FILE* archivo = fopen (nombre_archivo , "w") ;
	if (archivo == NULL) {
		printf ("Error, no se pudo abrir el archivo para poner los Pokemon.\n") ;
		return ERROR ;
	}
	for (int i= 0 ; i< (*acuario).cantidad_pokemon ; i++) {
		fprintf(archivo , FORMATO_ARCHIVO_ESCRITURA , (*acuario).pokemon[i].especie , (*acuario).pokemon[i].velocidad , (*acuario).pokemon[i].peso, (*acuario).pokemon[i].color) ;
	}
	fclose(archivo) ;
	return EXITO ;
}

void liberar_arrecife (arrecife_t* arrecife) {
	if (arrecife != NULL) {
		if (arrecife->pokemon != NULL) {
			free (arrecife->pokemon) ;
		}
		free (arrecife) ;
	}	
}

void liberar_acuario (acuario_t* acuario) {
	if (acuario != NULL) {
		if (acuario->pokemon != NULL) {
			free (acuario->pokemon) ;
		}
		free (acuario) ;
	}
}