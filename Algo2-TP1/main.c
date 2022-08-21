#include <stdio.h>
#include <string.h>
#include "evento_pesca.h"

#define PESO_MAGIKARP_ADULTO 20
#define PESO_POKEMON_LIVIANOS 10 
#define VELOCIDAD_POKEMON_RAPIDOS 100

#define ROJO "\033[1m\033[31m"
#define VERDE "\033[0m\033[32m"
#define AZUL "\033[1m\033[34m"
#define AMARILLO "\033[1m\033[33m"
#define REINICIAR_COLOR "\033[0m"

/*Una parte del evento de pesca sera sobre pescar magikarps, debido a esto se deben buscar pokemon de esta
especie en el arrecife. Sin embargo, para no despoblar al arrecife de magikarps, solo se buscaran magikarps
adultos (se considera adulto a cualquier magikarp que pese 20 kilos o mas)*/

//Pre: Recibe un puntero que lleva a un Pokemon con todos sus valores validos.
//Post: Devuelve true si el pokemon es de la especie magikarp y pesa 20 kilos o mas.

bool seleccionar_magikarps_adultos (pokemon_t* pokemon) {
	if (strcmp ((*pokemon).especie , "Magikarp") == 0 || strcmp ((*pokemon).especie , "magikarp") == 0) {
		if ((*pokemon).peso >= PESO_MAGIKARP_ADULTO) {
			return true ;
		}
	}
	return false ;
}

/*Debido a que no todos los pescadores tienen cañas de calidad, misty decidio llevar pokemon livianos asi
las cañas de los pescadores no se rompian durante el evento.*/

//Pre: Recibe un puntero que lleva a un Pokemon con todos sus valores validos.
//Post: Devuelve true si el peso del pokemon es menor o igual a 10 kilos.

bool seleccionar_pokemon_livianos(pokemon_t* pokemon) {
	if ((*pokemon).peso <= PESO_POKEMON_LIVIANOS) {
		return true ;
	}
	return false ;
}

/*Para que el evento sea desafiante y entretenido, misty decidio llevar unos pocos pokemon que sean muy
veloces y el atraparlos sea un gran desafio.*/

//Pre: Recibe un puntero que lleva a un Pokemon con todos sus valores validos.
//Post: Devuelve true si la velocidad del pokemon es mayor o igual a 100.

bool seleccionar_pokemon_rapidos (pokemon_t* pokemon) {
	if ((*pokemon).velocidad >= VELOCIDAD_POKEMON_RAPIDOS) {
		return true ;
	} 
	return false ;
}

/*Para el evento principal, misty queria conseguir un milotic, sin embargo esta especie es muy rara y dificil
de encontrar.*/

//Pre: Recibe un puntero que lleva a un Pokemon con todos sus valores validos.
//Post: Devuelve true si la especie del pokemon es Milotic.

bool seleccionar_milotic (pokemon_t* pokemon) {
	if (strcmp ((*pokemon).especie , "Milotic") == 0 || strcmp ((*pokemon).especie , "milotic") == 0) {
		return true ;
	}
	return false ;
}

/*Siendo Kyogre el pokemon mas grande de todos. Misty penso que llevar uno al evento llamaria mucho la atencion.*/

//Pre: Recibe un puntero que lleva a un Pokemon con todos sus valores validos.
//Post: Devuelve true si el pokemon es de la especie Kyogre.

bool seleccionar_kyogre (pokemon_t* pokemon) {
	if (strcmp ((*pokemon).especie , "Kyogre") == 0 || strcmp ((*pokemon).especie , "kyogre") == 0) {
		return true ;
	}
	return false ;
}

//Pre: Recibe un puntero que lleva a un Pokemon con todos sus valores validos.
//Post: Imprime por pantalla las caracteristicas de ese pokemon.

void mostrar_pokemon (pokemon_t* pokemon) {
	printf ("Especie: %s. Velocidad: %i. Peso: %i. Color: %s.\n" , pokemon->especie, pokemon->velocidad, pokemon->peso , pokemon->color) ;
}

//Pre: Recibe un puntero que lleva a un Pokemon con todos sus valores validos.
//Post: Imprime por pantalla las caracteristicas de ese pokemon con caracteres color rojo.

void mostrar_pokemon_rojo (pokemon_t* pokemon) {
	printf (ROJO"Especie: %s. Velocidad: %i. Peso: %i. Color: %s.\n"REINICIAR_COLOR , pokemon->especie, pokemon->velocidad, pokemon->peso , pokemon->color) ;
}

//Pre: Recibe un puntero que lleva a un Pokemon con todos sus valores validos.
//Post: Imprime por pantalla las caracteristicas de ese pokemon con caracteres color verde.

void mostrar_pokemon_verde (pokemon_t* pokemon) {
	printf (VERDE"Especie: %s. Velocidad: %i. Peso: %i. Color: %s.\n"REINICIAR_COLOR , pokemon->especie, pokemon->velocidad, pokemon->peso , pokemon->color) ;
}

//Pre: Recibe un puntero que lleva a un Pokemon con todos sus valores validos.
//Post: Imprime por pantalla las caracteristicas de ese pokemon con caracteres color azul.

void mostrar_pokemon_azul (pokemon_t* pokemon) {
	printf (AZUL "Especie: %s. Velocidad: %i. Peso: %i. Color: %s.\n" REINICIAR_COLOR , pokemon->especie, pokemon->velocidad, pokemon->peso , pokemon->color) ;
}

//Pre: Recibe un puntero que lleva a un Pokemon con todos sus valores validos.
//Post: Imprime por pantalla las caracteristicas de ese pokemon con caracteres color amarillo.

void mostrar_pokemon_amarillo (pokemon_t* pokemon) {
	printf (AMARILLO"Especie: %s. Velocidad: %i. Peso: %i. Color: %s.\n"REINICIAR_COLOR , pokemon->especie, pokemon->velocidad, pokemon->peso , pokemon->color) ;
}

int main (int argc , char* argv[]) {
	if (argc < 3) {
		printf ("Error. Este programa debe recibir 2 archivos por linea de comandos (primero el del arrecife y despues el que se usa para agregar los pokemon del acuario).\n") ;
		return -1 ;
	}

	arrecife_t* arrecife = crear_arrecife(argv[1]) ;
	if (arrecife == NULL) {
		liberar_arrecife(arrecife) ;
		return -1 ;
	}

	acuario_t* acuario = crear_acuario() ;
	if (acuario == NULL) {
		liberar_arrecife(arrecife) ;
		liberar_acuario(acuario) ;
		return -1 ;
	}

	trasladar_pokemon (arrecife, acuario , seleccionar_magikarps_adultos , 10) ;
	printf ("Arrecife despues del 1º traslado:\n") ;
	censar_arrecife (arrecife, mostrar_pokemon) ;
	trasladar_pokemon (arrecife, acuario , seleccionar_pokemon_livianos , 10) ;
	printf ("Arrecife despues del 2º traslado:\n") ;
	censar_arrecife (arrecife, mostrar_pokemon_rojo) ;
	trasladar_pokemon(arrecife, acuario, seleccionar_pokemon_rapidos, 2) ;
	printf ("Arrecife despues del 3º traslado:\n") ;
	censar_arrecife (arrecife, mostrar_pokemon_verde) ;
	trasladar_pokemon(arrecife, acuario, seleccionar_milotic, 1) ;
	printf ("Arrecife despues del 4º traslado:\n") ;
	censar_arrecife(arrecife, mostrar_pokemon_azul) ;
	trasladar_pokemon(arrecife, acuario, seleccionar_kyogre, 1) ;
	printf ("Arrecife despues del ultimo traslado:\n") ;
	censar_arrecife(arrecife, mostrar_pokemon_amarillo) ;
	guardar_datos_acuario(acuario , argv[2]) ;
	liberar_arrecife (arrecife) ;
	liberar_acuario (acuario) ;
 	return 0;
 }