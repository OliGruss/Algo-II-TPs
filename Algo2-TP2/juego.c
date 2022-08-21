#include "menus.h"

int main() {
	juego_t* juego = inicializar_juego() ;
	if (juego == NULL)  {
		printf ("Error; no se pudo reservar memoria para el juego.\n") ;
		return -1 ;
	}
	menu_inicio(juego) ;
	destruir_juego(juego) ;
	return 0 ;
}