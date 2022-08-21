#include "batallas.h"
#include "funciones_varias.h"

//Función que compara la velocidad de pokemon. Ganará aquel que sea más rápido.
//Si ambas velocidades son iguales ganará el segundo pokemon.

int funcion_batalla_1(void* pkm_1, void* pkm_2) {
	pokemon_t* pokemon_1 = pkm_1 ;
	pokemon_t* pokemon_2 = pkm_2 ;

	if (pokemon_1->velocidad > pokemon_2->velocidad) {
		return GANO_PRIMERO ;
	}
	else {
		return GANO_SEGUNDO ;
	}
}

//Función que compara el ataque de pokemon. Ganará aquel que tenga más ataque.
//Si ambos ataques son iguales ganará el segundo pokemon.

int funcion_batalla_2(void* pkm_1, void* pkm_2) {
	pokemon_t* pokemon_1 = pkm_1 ;
	pokemon_t* pokemon_2 = pkm_2 ;

	if (pokemon_1->ataque > pokemon_2->ataque) {
		return GANO_PRIMERO ;
	}
	else {
		return GANO_SEGUNDO ;
	}
}

//Función que compara la defensa de pokemon. Ganará aquel que tenga más defensa.
//Si ambas defensas son iguales ganará el segundo pokemon.

int funcion_batalla_3(void* pkm_1, void* pkm_2) {
	pokemon_t* pokemon_1 = pkm_1 ;
	pokemon_t* pokemon_2 = pkm_2 ;

	if (pokemon_1->defensa > pokemon_2->defensa) {
		return GANO_PRIMERO ;
	}
	else {
		return GANO_SEGUNDO ;
	}
}

//Función que compara el promedio entre el ataque y la defensa de cada pokemon.
//El pokemon con mejor promedio será el ganador. Si ambos tienen el mismo promedio,
//ganará el segundo pokemon.

int funcion_batalla_4 (void* pkm_1, void* pkm_2) {
	pokemon_t* pokemon_1 = pkm_1 ;
	pokemon_t* pokemon_2 = pkm_2 ;

	int promedio_at_def_1 = (pokemon_1->ataque + pokemon_1->defensa)/2 ;
	int promedio_at_def_2 = (pokemon_2->ataque + pokemon_2->defensa)/2 ;

	if (promedio_at_def_1 > promedio_at_def_2) {
		return GANO_PRIMERO ;
	}
	else {
		return GANO_SEGUNDO ;
	}
}

//Función que compara la suma de estadísticas totales (ataque+defensa+velocidad)
//de ambos pokemon. Quien tenga mejores estadísticas será el ganador. Si ambos
//tienen estadísticas iguales ganará el segundo pokemon.

int funcion_batalla_5 (void* pkm_1, void* pkm_2) {
	pokemon_t* pokemon_1 = pkm_1 ;
	pokemon_t* pokemon_2 = pkm_2 ;

	int suma_stats_1 = pokemon_1->ataque + pokemon_1->defensa + pokemon_1->velocidad ;
	int suma_stats_2 = pokemon_2->ataque + pokemon_2->defensa + pokemon_2->velocidad ;

	if (suma_stats_1 > suma_stats_2) {
		return GANO_PRIMERO ;
	}
	else {
		return GANO_SEGUNDO ;
	}
}