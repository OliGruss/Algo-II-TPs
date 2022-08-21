#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "heap.h"

#define EXITO 0
#define ERROR -1

heap_t* heap_crear(heap_comparador comparador ,heap_destructor destructor) {
	if (!comparador) {
		return NULL ;
	}

	heap_t* heap = malloc (sizeof (heap_t)) ;
	if (!heap) {
		return NULL ;
	}
	heap->array = NULL ;
	heap->comparador = comparador ;
	heap->destructor = destructor ;
	heap->tope = 0 ;
	return heap ;
}

//Pre: Recibe la posición de un nodo del heap (n siempre es menor al tope del heap).
//Post: Devuelve la posición del padre del nodo.

size_t posicion_padre(size_t n) {
	return (n-1)/2 ;
}

//Pre: Recibe la posición de un nodo del heap (n siempre es menor al tope del heap).
//Post: Devuelve la posición del hijo derecho del nodo.

size_t posicion_hijo_der (size_t n) {
	return (2*n)+2 ;
}

//Pre: Recibe la posición de un nodo del heap (n siempre es menor al tope del heap).
//Post: Devuelve la posición del hijo izquierdo del nodo.

size_t posicion_hijo_izq (size_t n) {
	return (2*n)+1 ;
}

//Pre: Recibe array de un heap válido y 2 posiciones en este heap 
//	(n y m son menores al tope del heap).
//Post: Intercambia los elementos de esas posiciones.

void intercambiar (void** array, size_t n, size_t m) {
	void* aux = array[n] ;
	array[n] = array[m] ;
	array[m] = aux ;
}

//Pre: Recibe un heap y una posición dentro de este (n siempre es menot al tope).
//Post: En caso de que el elemento en la posición n esté desordenado, la función
//		coloca al elemento donde debe estar.

void sift_up (heap_t* heap , size_t n) {
	if (n == 0) {
		return ;
	}

	size_t pos_padre = posicion_padre(n) ;
	bool dejar_de_ascender = false ;

	while (dejar_de_ascender == false && n > 0) {
		if (heap->comparador(heap->array[n] , heap->array[pos_padre]) == ES_MENOR) {
			intercambiar(heap->array, n, pos_padre) ;
			n = pos_padre ;
			pos_padre = posicion_padre(n) ;
		}
		else {
			dejar_de_ascender = true ;
		}
	}
}

int heap_insertar (heap_t* heap, void* elemento) {
	void** array_aux = realloc(heap->array, sizeof(void*) * (heap->tope+1)) ;
	if (!array_aux) {
		return ERROR ;
	}

	
	heap->tope++ ;
	heap->array = array_aux ;
	heap->array[heap->tope-1] = elemento ;

	sift_up (heap, heap->tope-1) ;
	return EXITO ;
}

//Pre: Recibe un heap cuya raiz acaba de ser modificada por el ùltimo elemento.
//Post: coloca al elemento en su lugar adecuado en el heap.

void sift_down (heap_t* heap, size_t n) {
	size_t pos_der = posicion_hijo_der(n) ;
	size_t pos_izq = posicion_hijo_izq(n) ;
	size_t pos_menor = pos_izq ;

	if (pos_izq >= heap->tope) {
		return ;
	}
	if (pos_der < heap->tope) {
		if (heap->comparador(heap->array[pos_der] , heap->array[pos_izq]) == ES_MENOR) {
			pos_menor = pos_der ;
		}
	}

	bool dejar_de_descender = false ;

	while (dejar_de_descender == false && n < heap->tope) {
		if (heap->comparador(heap->array[n], heap->array[pos_menor]) == ES_MAYOR) {
			intercambiar(heap->array, n, pos_menor) ;
			n = pos_menor ;
			pos_der = posicion_hijo_der(n) ;
			pos_izq = posicion_hijo_izq(n) ;
			if (pos_izq >= heap->tope) {
				dejar_de_descender = true ; ;
			}
			if (pos_der < heap->tope) {
				if (heap->comparador(heap->array[pos_der] , heap->array[pos_izq]) == ES_MENOR) {
					pos_menor = pos_der ;
				}
			}
		}
		else {
			dejar_de_descender = true ;
		}
	}
}

void* heap_extraer_raiz (heap_t* heap) {
	if (heap->tope == 0) {
		return NULL ;
	}

	void* raiz = heap->array[0] ;
	heap->array[0] = heap->array[heap->tope - 1] ;
	void** array_aux = realloc(heap->array, sizeof(void*) * heap->tope-1) ;
	if (array_aux == NULL) { 
		return NULL ;
	}
	heap->array = array_aux ;
	heap->tope-- ;
	if (heap->tope > 0) {
		sift_down(heap, 0) ;
	}

	return raiz ;
}

bool heap_vacio(heap_t* heap) {
	if (heap->tope <= 0) {
		return true ;
	}
	return false ;
}

void heap_destruir (heap_t* heap) {
	if (heap) {
		if (heap->array) {
			if (heap->destructor) {
				for (int i= 0 ; i< heap->tope ; i++) {
					heap->destructor(heap->array[i]) ;
				}
			}
			free(heap->array) ;
		}
		free(heap) ;
	}
}


