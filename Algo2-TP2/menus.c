#include "menus.h"
#include "batallas.h"

#define ROJO "\033[1m\033[31m"
#define VERDE "\033[0m\033[32m"
#define REINICIAR_COLOR "\033[0m"

#define MAX_RUTA 50

#define VICTORIA 1
#define DERROTA -1

#define INGRESAR_PERSONAJE 'E'
#define INGRESAR_GIMNASIO 'A'
#define INICIAR_PARTIDA 'I'
#define SIMULAR_PARTIDA 'S'

#define MOSTRAR_PERSONAJE 'E'
#define MOSTRAR_GIMNASIO 'G'
#define CAMBIAR_POKEMON 'C'
#define INICIAR_BATALLA 'B'

#define SIGUIENTE_BATALLA 'N'

#define REINTENTAR 'R'
#define FINALIZAR_PARTIDA 'F'

#define ROBAR_POKEMON 'T'
#define SIGUIENTE_GIMNASIO 'N'

juego_t* inicializar_juego () {
	juego_t* juego = malloc(sizeof(juego_t)) ;
	if (juego == NULL) {
		return NULL ;
	}
	juego->heap_gimnasios = NULL ;
	juego->personaje_principal = NULL ;

	return juego ;
}

//Pre: Recibe un juego válido.
//Post: Pide al usuario una ruta a un archivo de gimnasios. Además, imprime por pantalla
//		si el archivo se leyó correctamente o si hubo algún error.

void ingresar_gimnasios (juego_t* juego) {
	char ruta_archivo[MAX_RUTA] ;
	printf ("Ingresa la ruta al archivo de gimnasios.\n") ;
	scanf ("%s" , ruta_archivo) ;
	juego->heap_gimnasios = cargar_gimnasios(juego->heap_gimnasios , ruta_archivo) ;
	if (juego->heap_gimnasios == NULL) {
		printf (ROJO"Error: los gimnasios no se cargaron exitosamente. Puede ser"
			"que la ruta al archivo se haya ingresado mal o que la memoria necesaria"
			"para al menos un gimnasio no es la suficiente.\n"REINICIAR_COLOR) ;
	}
	else {
		printf (VERDE"Gimnasios cargados exitosamente.\n"REINICIAR_COLOR) ;
	}
}

//Pre: Recibe un juego válido.
//Post: Pide al usuario una ruta a un archivo de personaje. Además, imprime por pantalla
//		si el archivo se leyó correctamente o si hubo algún error.

void ingresar_personaje(juego_t* juego) {
	char ruta_archivo[MAX_RUTA] ;
	printf ("Ingresa la ruta al archivo del personaje.\n") ;
	scanf ("%s" , ruta_archivo) ;
	juego->personaje_principal = leer_personaje(ruta_archivo) ;
	if (juego->personaje_principal == NULL) {
		printf (ROJO"Error: el personaje no se cargo exitosamente. Puede ser"
			"que la ruta al archivo se haya ingresado mal o que la memoria necesaria"
			"para el personaje no es la suficiente.\n"REINICIAR_COLOR) ;
	}
	else {
		printf (VERDE"Personaje cargado exitosamente.\n"REINICIAR_COLOR) ;
	}
}

//Pre: Recibe un juego válido.
//Post: Ayuda al jugador a cambiar entre los pokemon a combatir con los pokemon obtenidos.

void cambiar_pokemon(juego_t* juego) {
	int input ;
	printf ("A continuacion se mostraran los pokemon que tenes para combatir, deberas ingresar un"
		" numero para seleccionar el pokemon que deseas quitar (aquel pokemon que se encuentre mas"
		" arriba sera el pokemon 1, el que este abajo sera el 2, etc.).\n") ;
	printf ("\n") ;
	lista_con_cada_elemento (juego->personaje_principal->pokemon_combates , mostrar_pokemon , NULL) ;
	scanf ("%i", &input) ;
	while (input < 1 || input > lista_elementos(juego->personaje_principal->pokemon_combates)) {
		printf (ROJO"Error, el numero que ingreses no puede ser menor que 1 ni mayor"
			"que la cantidad de pokemon de tu equipo.\n"REINICIAR_COLOR) ;
		scanf ("%i" , &input) ;
	}

	pokemon_t* pokemon_extraido = lista_elemento_en_posicion(juego->personaje_principal->pokemon_combates, ((size_t)input)-1) ;
	pokemon_extraido->esta_en_equipo = false ;

	printf ("Ahora se mostraran todos tus pokemon, esta vez, deberás escribir su nombre (tal y como se muestra)"
			"para agregarlo a tu equipo. Asegurate que no tengas ese pokemon en tu equipo ya.\n") ;

	abb_con_cada_elemento(juego->personaje_principal->pokemon_obtenidos, ABB_RECORRER_INORDEN,
						 	mostrar_pokemon_abb, NULL) ;

	pokemon_t pokemon_a_buscar ;

	scanf ("%s", pokemon_a_buscar.especie) ;
	pokemon_t* pokemon_encontrado = arbol_buscar(juego->personaje_principal->pokemon_obtenidos , 
												&pokemon_a_buscar) ;
	
	while (pokemon_encontrado == NULL || pokemon_encontrado->esta_en_equipo == true) {
		printf (ROJO"Error, el pokemon que insertaste no esta (puede ser por error de"
					"ortografia) o ya esta en el equipo, en ese caso proba con otro.\n"REINICIAR_COLOR) ;
		scanf ("%s", pokemon_a_buscar.especie) ;
		pokemon_encontrado = arbol_buscar(juego->personaje_principal->pokemon_obtenidos , &pokemon_a_buscar) ;
	}

	if (lista_insertar(juego->personaje_principal->pokemon_combates, pokemon_encontrado) == ERROR) {
	 	printf (ROJO"Error, no se pudo reservar memoria en el equipo para tu pokemon. No se agrego.\n"REINICIAR_COLOR) ;
	}
	else {
		pokemon_encontrado->esta_en_equipo = true ;
		printf (VERDE"Se substituyo a %s por %s.\n"REINICIAR_COLOR, pokemon_extraido->especie, pokemon_encontrado->especie) ;
	}
		//No puede fallar porque siempre recibe una lista con al menos un elemento.
	lista_borrar_de_posicion(juego->personaje_principal->pokemon_combates, ((size_t)input)-1) ;
}

//Declaro la función antes para que el compilador no me tire error.

void menu_batalla(juego_t* juego, gimnasio_t* gimnasio_actual) ;

//Declaro la función antes para que el compilador no me tire error.

void menu_gimnasios(juego_t* juego) ;

//Pre: Recibe un juego y un gimnasio en el que se perdió la partida.
//Post: Muestra el menú derrota y todas las opciones que este ofrece. Solo se puede abandonar
//		este menu si se reintenta la partida o si se finaliza.

void menu_derrota (juego_t* juego, gimnasio_t* gimnasio_actual) {
	char input ;

	printf (ROJO"PERDISTE.\n"REINICIAR_COLOR) ;
	printf ("Utiliza 'C' para cambiar tus pokemon de batalla.\n") ;
	printf ("Utiliza 'R' para reintentar el gimnasio desde el entrenador en que te quedaste.\n") ;
	printf ("Utiliza 'F' para finalizar la partida.\n") ;

	scanf (" %c", &input) ;
	while (input  != CAMBIAR_POKEMON && input != REINTENTAR && input != FINALIZAR_PARTIDA) {
		printf(ROJO"Error, debes usar 'C' si queres cambiar tus pokemon de batalla,"
			"'R' si queres reintentar el gimnasio o 'F' si queres finalizar la partida."
			"Los caracteres deben estar en mayusculas.\n"REINICIAR_COLOR) ;
		scanf (" %c", &input) ;
	}

	while (input != REINTENTAR && input != FINALIZAR_PARTIDA) {
		system("clear") ;
		if (input == CAMBIAR_POKEMON) {
			cambiar_pokemon(juego) ;
		}

		printf ("Utiliza 'C' para cambiar tus pokemon de batalla.\n") ;
		printf ("Utiliza 'R' para reintentar el gimnasio desde el entrenador en que te quedaste.\n") ;
		printf ("Utiliza 'F' para finalizar la partida.\n") ;

		scanf (" %c", &input) ;
		while (input  != CAMBIAR_POKEMON && input != REINTENTAR && input != FINALIZAR_PARTIDA) {
			printf(ROJO"Error, debes usar 'C' si queres cambiar tus pokemon de batalla,"
				"'R' si queres reintentar el gimnasio o 'F' si queres finalizar la partida."
				"Los caracteres deben estar en mayusculas.\n"REINICIAR_COLOR) ;
			scanf (" %c", &input) ;
		}
	}
	if (input == REINTENTAR) {
		menu_batalla(juego, gimnasio_actual) ;
	}
	if (input == FINALIZAR_PARTIDA) {
		destructor_gimnasios(gimnasio_actual) ;
		return ;
	}
}

//Pre: Recibe un juego válido y el gimnasio que se acaba de ganar.
//Post: Le permite al usuario tomar un pokemon del lider del gimnasio.

void robar_pokemon(juego_t* juego, gimnasio_t* gimnasio_actual) {
	int input ;
	entrenador_t* lider = lista_tope(gimnasio_actual->pila_entrenadores) ;
	printf ("A continuacion se mostraran los pokemon de lider del %s, deberas ingresar un "
		"numero para seleccionar el pokemon que desees (aquel pokemon que se encuentre mas"
		" arriba sera el pokemon 1, el que este abajo sera el 2, etc.).\n" , gimnasio_actual->nombre) ;
	printf ("\n") ;
	lista_con_cada_elemento (lider->lista_pokemon , mostrar_pokemon , NULL) ;
	scanf ("%i", &input) ;
	while (input < 1 || input > lista_elementos(lider->lista_pokemon)) {
		printf (ROJO"Error, el numero que ingreses no puede ser menor que 1 ni mayor"
			"que la cantidad de pokemon del lider.\n"REINICIAR_COLOR) ;
		scanf ("%i" , &input) ;
	}

	pokemon_t* pokemon_robado = lista_elemento_en_posicion(lider->lista_pokemon, ((size_t)input)-1) ;
	pokemon_t* pokemon_nuevo = malloc(sizeof(pokemon_t)) ;
	if (pokemon_nuevo== NULL) {
		printf (ROJO"Error, no se pudo reservar la memoria para tu nuevo pokemon. Tu pokemon"
			"no se ha agregado.\n"REINICIAR_COLOR) ;
		return  ;
	}

	*pokemon_nuevo = *pokemon_robado ;

	if (arbol_insertar(juego->personaje_principal->pokemon_obtenidos, pokemon_nuevo) == ERROR) {
		printf (ROJO"Error, no se pudo reservar la memoria para tu nuevo pokemon. Tu pokemon"
			"no se ha agregado.\n"REINICIAR_COLOR) ;
		destructor_pokemon(pokemon_nuevo) ;
		return ;
	}
	else {
		printf (VERDE"%s ha sido agregado a tu conjunto de pokemon obtenidos.\n"REINICIAR_COLOR ,
				pokemon_nuevo->especie) ;
	}
}

//Pre: Recibe un juego válido y un gimnasio que se acaba de ganar.
//Post: Muestra el menú victoria y todas sus opciones. Si el gimnasio que se ganó era el
//		último, le informa al usuario que ganó el juego.

void menu_victoria(juego_t* juego, gimnasio_t* gimnasio_actual) {
	if (heap_vacio(juego->heap_gimnasios) == true) {
		printf ("\n") ;
		printf (VERDE"Felicidades, te convertiste en maestro pokemon.\n"REINICIAR_COLOR) ;
		printf ("\n") ;
		destructor_gimnasios(gimnasio_actual) ;
		return ;
	}
	
	char input ;
	bool se_robo = false ;

	printf (VERDE"Felicidades, completaste el %s.\n"REINICIAR_COLOR, gimnasio_actual->nombre) ;
	printf ("Utiliza 'C' para cambiar tus pokemon de batalla.\n") ;
	printf ("Utiliza 'T' para tomar prestado (no robar) un pokemon del lider de gimnasio.\n") ;
	printf ("Utiliza 'N' para ir al siguiente gimnasio.\n") ;
	scanf (" %c", &input) ;

	while (input != CAMBIAR_POKEMON && input != ROBAR_POKEMON && input != SIGUIENTE_GIMNASIO) {
		printf (ROJO"Error, debes usar 'C' si queres cambiar tus pokemon de batalla"
			", 'T' si queres tomar prestado un pokemon del lider o 'N' si queres ir al"
			"siguiente gimnasio. Los caracteres deben estar en mauyscula.\n"REINICIAR_COLOR) ;
		scanf (" %c", &input) ;
	}

	while (input != SIGUIENTE_GIMNASIO) {
		if (input == ROBAR_POKEMON && se_robo == true) {
			printf(ROJO"Error, ya robaste, perdon, ya tomaste prestado un pokemon del lider.\n"REINICIAR_COLOR) ;
		}
		if (input == ROBAR_POKEMON && se_robo == false) {
			se_robo = true ;
			robar_pokemon(juego, gimnasio_actual) ;
		}
		if (input == CAMBIAR_POKEMON) {
			cambiar_pokemon(juego) ;
		}

		printf ("Utiliza 'C' para cambiar tus pokemon de batalla.\n") ;
		printf ("Utiliza 'T' para tomar prestado (no robar) un pokemon del lider de gimnasio.\n") ;
		printf ("Utiliza 'N' para ir al siguiente gimnasio.\n") ;
		scanf (" %c", &input) ;

		while (input != CAMBIAR_POKEMON && input != ROBAR_POKEMON && input != SIGUIENTE_GIMNASIO) {
			printf (ROJO"Error, debes usar 'C' si queres cambiar tus pokemon de batalla"
				", 'T' si queres tomar prestado un pokemon del lider o 'N' si queres ir al"
				"siguiente gimnasio. Los caracteres deben estar en mauyscula.\n"REINICIAR_COLOR) ;
			scanf (" %c", &input) ;
		}
	}
	if (input == SIGUIENTE_GIMNASIO) {
		destructor_gimnasios(gimnasio_actual) ;
		menu_gimnasios(juego) ;
	}
}

//Pre: Recibe un juego válido y un gimnasio válido.
//Post: Realiza todas las batallas del gimnasio e informa al usuario si estas se ganan o se pierden.

void menu_batalla(juego_t* juego, gimnasio_t* gimnasio_actual) {
	pokemon_t* pokemon_propio ;
	pokemon_t* pokemon_entrenador ;
	entrenador_t* entrenador_actual = lista_tope(gimnasio_actual->pila_entrenadores) ;
	char input ;
	size_t i= 0 ;
	size_t j = 0 ;
	int resultado_batalla ;
	bool se_perdio = false ;

	while (se_perdio == false && entrenador_actual != NULL) {
		while (i < lista_elementos (juego->personaje_principal->pokemon_combates) &&
				j < lista_elementos(entrenador_actual->lista_pokemon)) {
			pokemon_propio = lista_elemento_en_posicion(juego->personaje_principal->pokemon_combates, i) ;
			pokemon_entrenador = lista_elemento_en_posicion(entrenador_actual->lista_pokemon, j) ;
			if (gimnasio_actual->id_funcion == 1) {
				resultado_batalla = funcion_batalla_1(pokemon_propio, pokemon_entrenador) ;
			}
			if (gimnasio_actual->id_funcion == 2) {
				resultado_batalla = funcion_batalla_2(pokemon_propio, pokemon_entrenador) ;
			}
			if (gimnasio_actual->id_funcion == 3) {
				resultado_batalla = funcion_batalla_3(pokemon_propio, pokemon_entrenador) ;
			}
			if (gimnasio_actual->id_funcion == 4) {
				resultado_batalla = funcion_batalla_4(pokemon_propio, pokemon_entrenador) ;
			}
			if (gimnasio_actual->id_funcion == 5) {
				resultado_batalla = funcion_batalla_5(pokemon_propio, pokemon_entrenador) ;
			}

			if (resultado_batalla == VICTORIA) {
				printf ("Combate contra: %s\n", entrenador_actual->nombre) ;
				printf ("Tu pokemon: %s. Velocidad: %i. Ataque: %i. Defensa:%i.\n", pokemon_propio->especie,
						pokemon_propio->velocidad, pokemon_propio->ataque, pokemon_propio->defensa) ;
				printf ("Pokemon contrario: %s. Velocidad: %i. Ataque: %i. Defensa: %i.\n", pokemon_entrenador->especie,
						pokemon_entrenador->velocidad, pokemon_entrenador->ataque, pokemon_entrenador->defensa) ;
				printf ("Resultado:"VERDE" VICTORIA.\n"REINICIAR_COLOR) ;
				if (pokemon_propio->bonificador < 63) {
					pokemon_propio->velocidad++ ;
					pokemon_propio->ataque++ ;
					pokemon_propio->defensa++ ;
					pokemon_propio->bonificador++ ;
				}
				j++;
			}
			if (resultado_batalla == DERROTA) {
				printf ("Combate contra: %s\n", entrenador_actual->nombre) ;
				printf ("Tu pokemon: %s. Velocidad: %i. Ataque: %i. Defensa:%i.\n", pokemon_propio->especie,
						pokemon_propio->velocidad, pokemon_propio->ataque, pokemon_propio->defensa) ;
				printf ("Pokemon contrario: %s. Velocidad: %i. Ataque: %i. Defensa: %i.\n", pokemon_entrenador->especie,
						pokemon_entrenador->velocidad, pokemon_entrenador->ataque, pokemon_entrenador->defensa) ;
				printf ("Resultado:"ROJO" DERROTA.\n"REINICIAR_COLOR) ;
				i++ ;
			}
			printf ("\n") ;
			printf ("Utiliza 'N' para continuar.\n") ;
			scanf (" %c", &input) ;
			system("clear") ;
			while (input != SIGUIENTE_BATALLA) {
				printf (ROJO"Error, debes ingresar 'N' en mayuscula.\n"REINICIAR_COLOR) ;
				scanf (" %c", &input) ;
			}
		}
		if (i >= lista_elementos (juego->personaje_principal->pokemon_combates)) {
			se_perdio = true ;
		}
		if (j >= lista_elementos(entrenador_actual->lista_pokemon)) {
			printf ("Venciste al entrenador: %s.\n", entrenador_actual->nombre) ;
			if (lista_elementos(gimnasio_actual->pila_entrenadores) <= 1) {
				entrenador_actual = NULL ;
			}
			if (lista_elementos(gimnasio_actual->pila_entrenadores) > 1) {
				lista_desapilar(gimnasio_actual->pila_entrenadores) ;
				entrenador_actual = lista_tope(gimnasio_actual->pila_entrenadores) ;
			}
			if (entrenador_actual !=NULL) {
				printf("Tu siguiente rival sera: %s.\n", entrenador_actual->nombre) ;
				printf ("Utiliza 'N' para continuar.\n") ;
				scanf (" %c", &input) ;
				system("clear") ;
				while (input != SIGUIENTE_BATALLA) {
					printf (ROJO"Error, debes ingresar 'N' en mayuscula.\n"REINICIAR_COLOR) ;
					scanf (" %c", &input) ;
				}
			}
		}
		i = 0 ;
		j = 0 ;
	}
	if (se_perdio == true) {
		menu_derrota(juego, gimnasio_actual) ;
	}
	else {
		menu_victoria(juego, gimnasio_actual) ;
	}
}

//Pre: Recibe un juego válido.
//Post: Muestra el menu gimnasio y todas sus opciones. Este menú solo se puede abandonar
//		si se inicia la batalla contra el gimnasio actual. 

void menu_gimnasios(juego_t* juego) {
	system("clear") ;
	char input ;
	gimnasio_t* gimnasio_actual = heap_extraer_raiz(juego->heap_gimnasios) ;
	printf("Bienvenido al menú de gimnasios. Deberas ingresar un caracter para utilizar"
			"una opcion, los caracteres deben estar en mayúscula.\n") ;
	printf ("Utiliza 'E' para ver detalles sobre tu personaje.\n") ;
	printf ("Utiliza 'G' para ver detalles sobre el gimnasio a enfrentar.\n") ;
	printf ("Utiliza 'C' si queres cambiar algunos de tus pokemon.\n") ;
	printf ("Utiliza 'B' para iniciar la siguiente batalla del gimnasio.\n") ;

	scanf (" %c", &input) ;
	while (input != MOSTRAR_PERSONAJE && input != MOSTRAR_GIMNASIO && input != CAMBIAR_POKEMON
			&& input != INICIAR_BATALLA) {
		printf (ROJO"Error, los caracteres que debes utilizar son la 'E' para mostrar tu personaje"
				", la 'G' para mostrar el gimnasio, la 'C' para cambiar pokemon de tu equipo"
				"o la 'B' para iniciar la batalla.\n"REINICIAR_COLOR) ;
		scanf(" %c", &input) ;
	}

	while (input != INICIAR_BATALLA) {
		system("clear") ;
		if (input == MOSTRAR_PERSONAJE) {
			mostrar_personaje(juego->personaje_principal) ;
		}
		if (input == MOSTRAR_GIMNASIO) {
			mostrar_gimnasio(gimnasio_actual) ;
		}
		if (input == CAMBIAR_POKEMON) {
			cambiar_pokemon(juego) ;
		}
		if (input != INICIAR_BATALLA) {
			printf ("Utiliza 'E' para ver detalles sobre tu personaje.\n") ;
			printf ("Utiliza 'G' para ver detalles sobre el gimnasio a enfrentar.\n") ;
			printf ("Utiliza 'C' si queres cambiar algunos de tus pokemon.\n") ;
			printf ("Utiliza 'B' para iniciar la primera batalla del gimnasio.\n") ;

			scanf (" %c", &input) ;
			while (input != MOSTRAR_PERSONAJE && input != MOSTRAR_GIMNASIO && input != CAMBIAR_POKEMON
					&& input != INICIAR_BATALLA) {
				printf (ROJO"Error, los caracteres que debes utilizar son la 'E' para mostrar tu personaje"
						", la 'G' para mostrar el gimnasio, la 'C' para cambiar pokemon de tu equipo"
						"o la 'B' para iniciar la batalla.\n"REINICIAR_COLOR) ;
				scanf(" %c", &input) ;
			}
		}
	}
	if (input == INICIAR_BATALLA) {
		menu_batalla(juego, gimnasio_actual) ;
	}
}

//Pre: Recibe un juego válido con gimnasios y personaje inicializados.
//Post: Juega la partida sin necesidad de ningun input del ususario y le informa si se ganó
//		o se perdió.

void simular_partida(juego_t* juego) {
	gimnasio_t* gimnasio_actual = heap_extraer_raiz(juego->heap_gimnasios) ;
	entrenador_t* entrenador_actual = lista_tope(gimnasio_actual->pila_entrenadores) ;
	pokemon_t* pokemon_propio ;
	pokemon_t* pokemon_entrenador ;
	bool se_perdio = false ;
	size_t i = 0 ;
	size_t j = 0;
	int resultado_batalla ;

	while (se_perdio == false && heap_vacio(juego->heap_gimnasios) == false) {

		destructor_gimnasios(gimnasio_actual) ;
		gimnasio_actual = heap_extraer_raiz(juego->heap_gimnasios) ;
		if (gimnasio_actual) {
			entrenador_actual = lista_tope(gimnasio_actual->pila_entrenadores) ;
		}

		while (se_perdio == false && lista_elementos(gimnasio_actual->pila_entrenadores) > 0) {
			while (i < lista_elementos (juego->personaje_principal->pokemon_combates) &&
					j < lista_elementos(entrenador_actual->lista_pokemon)) {
				pokemon_propio = lista_elemento_en_posicion(juego->personaje_principal->pokemon_combates, i) ;
				pokemon_entrenador = lista_elemento_en_posicion(entrenador_actual->lista_pokemon, j) ;
				if (gimnasio_actual->id_funcion == 1) {
					resultado_batalla = funcion_batalla_1(pokemon_propio, pokemon_entrenador) ;
				}
				if (gimnasio_actual->id_funcion == 2) {
					resultado_batalla = funcion_batalla_2(pokemon_propio, pokemon_entrenador) ;
				}
				if (gimnasio_actual->id_funcion == 3) {
					resultado_batalla = funcion_batalla_3(pokemon_propio, pokemon_entrenador) ;
				}
				if (gimnasio_actual->id_funcion == 4) {
					resultado_batalla = funcion_batalla_4(pokemon_propio, pokemon_entrenador) ;
				}
				if (gimnasio_actual->id_funcion == 5) {
					resultado_batalla = funcion_batalla_5(pokemon_propio, pokemon_entrenador) ;
				}

				if (resultado_batalla == VICTORIA) {
					if (pokemon_propio->bonificador < 63) {
						pokemon_propio->velocidad++ ;
						pokemon_propio->ataque++ ;
						pokemon_propio->defensa++ ;
						pokemon_propio->bonificador++ ;
					}
					j++;
				}
				if (resultado_batalla == DERROTA) {
					i++ ;
				}
			}
			if (i >= lista_elementos (juego->personaje_principal->pokemon_combates)) {
				se_perdio = true ;
			}
			if (j >= lista_elementos(entrenador_actual->lista_pokemon)) {
				lista_desapilar(gimnasio_actual->pila_entrenadores) ;
				entrenador_actual = lista_tope(gimnasio_actual->pila_entrenadores) ;
			}
			i = 0 ;
			j = 0 ;
		}
	}

	if (se_perdio == true) {
		printf (ROJO"PERDISTE.\n" REINICIAR_COLOR) ;
		printf ("Llegaste hasta: %s.\n" , gimnasio_actual->nombre) ;
		printf ("Fuiste derrotado por: %s.\n", entrenador_actual->nombre) ;
		destructor_gimnasios(gimnasio_actual) ;
		return ;
	}
	else {
		printf (VERDE"Felicidades, te convertiste en maestro pokemon.\n"REINICIAR_COLOR) ;
		destructor_gimnasios(gimnasio_actual) ;
		return ;
	}
}

void menu_inicio(juego_t* juego) {
	char input ;
	printf("Bienvenido al menú inicio. Deberas ingresar un caracter para utilizar"
			"una opcion, los caracteres deben estar en mayúscula.\n") ;
	printf ("Utiliza 'E' si queres ingresar un archivo de entrenador principal.\n") ;
	printf ("Utiliza 'A' si queres ingresar un archivo de gimnasios.\n") ;
	printf ("Utiliza 'I' si queres iniciar una partida.\n") ;
	printf ("Utiliza 'S' si queres simular una partida.\n") ;

	scanf(" %c" , &input) ;
	while (input != INGRESAR_PERSONAJE && input != INGRESAR_GIMNASIO &&
			input != INICIAR_PARTIDA && input != SIMULAR_PARTIDA) {
		printf (ROJO"Error. Los caracteres que debés usar son 'E' para ingresar personaje,"
				"'A' para ingresar gimnasios, 'I' para iniciar partida o 'S' para simularla."
				"Los caracteres deben estar en mayuscula.\n"REINICIAR_COLOR) ;
		scanf(" %c", &input) ;
	}

	while ((input != INICIAR_PARTIDA && input != SIMULAR_PARTIDA) ||
			(juego->heap_gimnasios == NULL || juego->personaje_principal == NULL)) {
		system("clear") ;
		if (input == INGRESAR_GIMNASIO) {
			ingresar_gimnasios(juego) ;
		}
		if (input == INGRESAR_PERSONAJE && juego->personaje_principal != NULL) {
			printf (ROJO"Error, ya se ingreso un personaje anteriormente.\n"REINICIAR_COLOR) ;
		}
		if (input == INGRESAR_PERSONAJE && juego->personaje_principal == NULL) {
			ingresar_personaje(juego) ;
		}
		if (input == INICIAR_PARTIDA && (juego->heap_gimnasios == NULL || juego->personaje_principal == NULL)) {
			printf (ROJO"Error, no podes iniciar partida sin personaje o sin gimnasios.\n"REINICIAR_COLOR) ;
		}
		if (input == SIMULAR_PARTIDA && (juego->heap_gimnasios == NULL || juego->personaje_principal == NULL)) {
			printf (ROJO"Error, no podes simular partida sin personaje o sin gimnasios.\n"REINICIAR_COLOR) ;
		}

		if ((input != INICIAR_PARTIDA && input != SIMULAR_PARTIDA) ||
			(juego->heap_gimnasios == NULL || juego->personaje_principal == NULL)) {
			printf ("Utiliza 'E' si queres ingresar un archivo de entrenador principal.\n") ;
			printf ("Utiliza 'A' si queres ingresar un archivo de gimnasios.\n") ;
			printf ("Utiliza 'I' si queres iniciar una partida.\n") ;
			printf ("Utiliza 'S' si queres simular una partida.\n") ;

			scanf(" %c" , &input) ;
			while (input != INGRESAR_PERSONAJE && input != INGRESAR_GIMNASIO &&
					input != INICIAR_PARTIDA && input != SIMULAR_PARTIDA) {
				printf (ROJO"Error. Los caracteres que debés usar son 'E' para ingresar personaje,"
						"'A' para ingresar gimnasios, 'I' para iniciar partida o 'S' para simularla."
						"Los caracteres deben estar en mayuscula.\n"REINICIAR_COLOR) ;
				scanf(" %c", &input) ;
			}
		}
	}
	if (input == INICIAR_PARTIDA) {
		menu_gimnasios(juego) ;
	}
	if (input == SIMULAR_PARTIDA) {
		simular_partida(juego) ;
	}
}

void destruir_juego(juego_t* juego) {
	if (juego) {
		heap_destruir(juego->heap_gimnasios) ;
		destruir_personaje(juego->personaje_principal) ;
		free(juego) ;
	}
}