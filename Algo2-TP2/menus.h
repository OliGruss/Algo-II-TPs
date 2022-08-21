#ifndef __MENUS_H__
#define __MENUS_H__

#include "funciones_varias.h"

typedef struct juego {
	heap_t* heap_gimnasios ;
	personaje_t* personaje_principal ;
} juego_t ;

//Pre: No recibe nada.
//Post: Reserva memoria para el juego y lo inicializa. 
//		Devuelve NULL si no hay memoria suficiente.

juego_t* inicializar_juego() ;

//Pre: Recibe un puntero a un juego válido.
//Post: Muestra el menú inicio y todas las opciones que este ofrece. No se podrá
//		abandonar este menú hasta que se haya cargado exitosamente un gimnasio y un
//		personaje principal. Una vez se haya abandonado este menu, se iran llamando el
//		resto de funciones que hacen el juego posible.

void menu_inicio(juego_t* juego) ;

//Pre: Recibe un puntero a un juego.
//Post: Libera la memoria reservada por el juego y todas sus estructuras.

void destruir_juego(juego_t* juego) ;


#endif /*__MENUS_h__*/