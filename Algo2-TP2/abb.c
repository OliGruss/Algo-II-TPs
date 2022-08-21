#include <stdio.h>
#include "abb.h"

#define ERROR -1
#define EXITO 0

#define ES_MAYOR 1
#define ES_IGUAL 0 
#define ES_MENOR -1

#define NO_TIENE_HIJOS -1
#define TIENE_HIJO_DERECHO 0
#define TIENE_HIJO_IZQUIERDO 1
#define TIENE_DOS_HIJOS 2

abb_t* arbol_crear(abb_comparador comparador , abb_liberar_elemento destructor) {
	if (!comparador) {
		return NULL ;
	}

	abb_t* abb = malloc (sizeof (abb_t)) ;
	if (!abb) {
		return NULL ;
	}
	abb->nodo_raiz = NULL ;
	abb->comparador = comparador ;
	abb->destructor = destructor ;
	return abb ;
}

nodo_abb_t* nodo_crear (void* elemento) {
	nodo_abb_t* nodo = malloc (sizeof(nodo_abb_t)) ;
	if (!nodo) {
		return NULL ;
	}
	nodo->elemento = elemento ;
	nodo->izquierda = NULL ;
	nodo->derecha = NULL ;
	return nodo ;
}

//Pre: Recibe un abb válido y un nodo válido también.
//Post: Inserta al nodo que recibe en su debida posición.

void insertar_nodo(abb_t* arbol, nodo_abb_t* nodo) {
	bool pudo_insertar = false ;
	nodo_abb_t* nodo_actual = arbol->nodo_raiz ;

	while (pudo_insertar == false) {
		if (arbol->comparador(nodo->elemento , nodo_actual->elemento) > ES_IGUAL) {
			if (nodo_actual->derecha == NULL) {
				nodo_actual->derecha = nodo ;
				pudo_insertar = true ;
			}
			else {
				nodo_actual = nodo_actual->derecha ;
			}
		}
		else {
			if (nodo_actual->izquierda == NULL) {
				nodo_actual->izquierda = nodo ;
				pudo_insertar = true ;
			}
			else {
				nodo_actual = nodo_actual->izquierda ;
			}
		}
	}
}

int arbol_insertar(abb_t* arbol , void* elemento) {
	if (!arbol) {
		return ERROR ;
	}
	if (arbol->comparador == NULL)  {
		return ERROR ;
	}
	nodo_abb_t* nodo = nodo_crear (elemento) ;
	if (!nodo) {
		return ERROR ;
	}
	if (arbol->nodo_raiz == NULL) {
		arbol->nodo_raiz = nodo ;
		return EXITO ;
	}

	insertar_nodo (arbol, nodo) ;
	return EXITO ;
}

//Pre: recibe un nodo válido.
//Post: Devuelve -1 si el nodo no tiene hijos, 0 si tiene un hijo a la derecha, 1 si tiene hijo a la 
// 		izquierda y 2 si tiene 2 hijos.

int nodo_hijos (nodo_abb_t* nodo) {
	if (nodo->derecha && !nodo->izquierda) {
		return TIENE_HIJO_DERECHO ;
	}
	else if (!nodo->derecha && nodo->izquierda) {
		return TIENE_HIJO_IZQUIERDO ;
	}
	else if (nodo->derecha && nodo->izquierda) {
		return TIENE_DOS_HIJOS ;
	}
	else  {
		return NO_TIENE_HIJOS ;
	}
}

//Pre: recibe un nodo valido y un arbol.
//Post: Si el arbol tiene destructor lo invoca con el elemento del nodo y despues libera la memoria
//		reservada para el nodo.

void borrar_nodo (abb_t* arbol, nodo_abb_t* nodo) {
	if (arbol->destructor != NULL && nodo->elemento != NULL) {
		arbol->destructor(nodo->elemento) ;
	}
	free(nodo) ;
}

//Pre: Recibe un arbol valido y un nodo a borrar que será su raíz.
//Post: Libera la memoria reservada por la raiz del arbol y, si es necesatio, reorganiza el arbol
//		para que quede ordenado y con raíz.

void arbol_borrar_raiz (abb_t* arbol, nodo_abb_t* nodo_a_borrar) {
	if (nodo_hijos(nodo_a_borrar) == NO_TIENE_HIJOS) {
			borrar_nodo(arbol, arbol->nodo_raiz) ;
			arbol->nodo_raiz = NULL ;
		}
		else if (nodo_hijos(nodo_a_borrar) == TIENE_HIJO_DERECHO) {
			nodo_abb_t* nodo_derecha = nodo_a_borrar->derecha ;
			borrar_nodo(arbol, arbol->nodo_raiz) ;
			arbol->nodo_raiz = nodo_derecha ;
		}
		else if (nodo_hijos(nodo_a_borrar) == TIENE_HIJO_IZQUIERDO) {
			nodo_abb_t* nodo_izquierda = nodo_a_borrar->izquierda ;
			borrar_nodo(arbol, arbol->nodo_raiz) ;
			arbol->nodo_raiz = nodo_izquierda ;
		}
		else {
			nodo_abb_t* nodo_izquierda = nodo_a_borrar->izquierda ;
			nodo_abb_t* nodo_derecha = nodo_a_borrar->derecha ;
			
			nodo_abb_t* nodo_padre_a_cercano = nodo_a_borrar ;
			nodo_abb_t* nodo_cercano = nodo_izquierda ;

			while (nodo_cercano->derecha != NULL) {
				nodo_padre_a_cercano = nodo_cercano ;
				nodo_cercano = nodo_cercano->derecha ;
			}
			if (nodo_padre_a_cercano != nodo_a_borrar) {
					nodo_padre_a_cercano->derecha = NULL ;
			}
			else {
				nodo_izquierda = nodo_cercano->izquierda ;
			}
			borrar_nodo(arbol, arbol->nodo_raiz) ;
			nodo_cercano->derecha = nodo_derecha ;
			nodo_cercano->izquierda = nodo_izquierda ;
			arbol->nodo_raiz = nodo_cercano ;
		}
}

//Pre: Recibe un arbol válido, un nodo a borrar y su padre.
//Post: Libera la memoria reservada por el nodo a borrar y reorganiza el arbol para que quede ordenado.

void arbol_borrar_nodo (abb_t* arbol, nodo_abb_t* nodo_a_borrar, nodo_abb_t* nodo_padre) {
	if (nodo_hijos(nodo_a_borrar) == NO_TIENE_HIJOS) {
		if (arbol->comparador(nodo_padre->elemento, nodo_a_borrar->elemento) > ES_IGUAL) {
			borrar_nodo(arbol, nodo_padre->izquierda) ;
			nodo_padre->izquierda = NULL ;
		}
		else {
			borrar_nodo(arbol, nodo_padre->derecha) ;
			nodo_padre->derecha = NULL ;
		}
	}
	else if (nodo_hijos(nodo_a_borrar) == TIENE_HIJO_DERECHO) {
		nodo_abb_t* nodo_derecha = nodo_a_borrar->derecha ;
		if (arbol->comparador(nodo_padre->elemento, nodo_a_borrar->elemento) > ES_IGUAL) {
			borrar_nodo(arbol, nodo_padre->izquierda) ;
			nodo_padre->izquierda = nodo_derecha ;
		}
		else {
			borrar_nodo(arbol, nodo_padre->derecha) ;
			nodo_padre->derecha = nodo_derecha ;
		}
	}

	else if (nodo_hijos(nodo_a_borrar) == TIENE_HIJO_IZQUIERDO) {
		nodo_abb_t* nodo_izquierda = nodo_a_borrar->izquierda ;
		if (arbol->comparador(nodo_padre->elemento, nodo_a_borrar->elemento) > ES_IGUAL) {
			borrar_nodo(arbol, nodo_padre->izquierda) ;
			nodo_padre->izquierda = nodo_izquierda ;
		}
		else {
			borrar_nodo(arbol, nodo_padre->derecha) ;
			nodo_padre->derecha = nodo_izquierda ;
		}
	}
	else {
		nodo_abb_t* nodo_izquierda = nodo_a_borrar->izquierda ;
		nodo_abb_t* nodo_derecha = nodo_a_borrar->derecha ;
		
		nodo_abb_t* nodo_padre_a_cercano = nodo_a_borrar ;
		nodo_abb_t* nodo_cercano = nodo_izquierda ;

		while (nodo_cercano->derecha != NULL) {
			nodo_padre_a_cercano = nodo_cercano ;
			nodo_cercano = nodo_cercano->derecha ;
		}

		if (arbol->comparador(nodo_padre->elemento, nodo_a_borrar->elemento) > ES_IGUAL) {
			if (nodo_padre_a_cercano != nodo_a_borrar) {
				nodo_padre_a_cercano->derecha = NULL ;
			}
			else {
				nodo_izquierda = nodo_cercano->izquierda ;
			}
			borrar_nodo(arbol, nodo_padre->izquierda) ;
			nodo_padre->izquierda = nodo_cercano ;
			nodo_cercano->derecha = nodo_derecha ;
			nodo_cercano->izquierda = nodo_izquierda ;
		}
		else {
			if (nodo_padre_a_cercano != nodo_a_borrar) {
				nodo_padre_a_cercano->derecha = NULL ;
			}
			else {
				nodo_izquierda = nodo_cercano->izquierda ;
			}
			borrar_nodo(arbol, nodo_padre->derecha) ;
			nodo_padre->derecha = nodo_cercano ;
			nodo_cercano->derecha = nodo_derecha ;
			nodo_cercano->izquierda = nodo_izquierda ;
		}
	}
}

int arbol_borrar (abb_t* arbol, void* elemento) {
	if (!arbol) {
		return ERROR ;
	}
	if (arbol->nodo_raiz == NULL) {
		return ERROR ;
	}
	if (arbol->comparador == NULL)  {
		return ERROR ;
	}

	nodo_abb_t* nodo_padre = arbol->nodo_raiz ;
	nodo_abb_t* nodo_a_borrar = arbol->nodo_raiz ;
	bool pudo_encontrar = false ;

	while (pudo_encontrar == false && nodo_a_borrar != NULL) {
		if (arbol->comparador(elemento , nodo_a_borrar->elemento) > ES_IGUAL) {
			nodo_padre = nodo_a_borrar ;
			nodo_a_borrar = nodo_a_borrar->derecha ;
		}
		else if (arbol->comparador(elemento , nodo_a_borrar->elemento) < ES_IGUAL) {
			nodo_padre = nodo_a_borrar ;
			nodo_a_borrar = nodo_a_borrar->izquierda ;
		}
		else {
			pudo_encontrar = true ;
		}
	}
	if (pudo_encontrar == false) {
		return ERROR ;
	}

	if (nodo_a_borrar == arbol->nodo_raiz) {
		arbol_borrar_raiz(arbol, nodo_a_borrar) ;
		return EXITO ;
	}
	else {
		arbol_borrar_nodo (arbol, nodo_a_borrar, nodo_padre) ;
		return EXITO ;
	}
}


//Pre: Recibe un abb válido, que tenga comparador y con al menos un elemento.
//Post: Busca y devuelve algún nodo que contenga al elemento que recibe la función. Si no lo encuentra
//		devuelve NULL.

nodo_abb_t* buscar_nodo (abb_t* arbol, void* elemento) {
	nodo_abb_t* nodo_actual = arbol->nodo_raiz ;
	bool pudo_encontrar = false ;

	while (pudo_encontrar == false && nodo_actual != NULL) {
		if (arbol->comparador(elemento , nodo_actual->elemento) > ES_IGUAL) {
			nodo_actual = nodo_actual->derecha ;
		}
		else if (arbol->comparador(elemento , nodo_actual->elemento) < ES_IGUAL) {
			nodo_actual = nodo_actual->izquierda ;
		}
		else {
			pudo_encontrar = true ;
		}
	}
	if (pudo_encontrar == false) {
		return NULL ;
	}
	else {
		return nodo_actual ;
	}
}

void* arbol_buscar(abb_t* arbol, void* elemento) {
	if (!arbol) {
		return NULL ;
	}
	if (arbol->nodo_raiz == NULL) {
		return NULL ;
	}
	if (arbol->comparador == NULL)  {
		return NULL ;
	}

	nodo_abb_t* nodo_a_buscar = buscar_nodo(arbol, elemento) ;
	if (nodo_a_buscar == NULL) {
		return NULL ;
	}
	else {
		return nodo_a_buscar->elemento ;
	}
}

void* arbol_raiz(abb_t* arbol) {
	if (!arbol) {
		return NULL ;
	}
	if (arbol->nodo_raiz == NULL) {
		return NULL ;
	}
	else {
		return arbol->nodo_raiz->elemento ;
	}
}

bool arbol_vacio(abb_t* arbol) {
	if (!arbol) {
		return true ;
	}
	if (arbol->nodo_raiz == NULL) {
		return true ;
	}
	else {
		return false ;
	}
}

//Pre: Recibe un nodo valido, un array, su tope y la cantidad de elementos insertados.
//Post: Funcion recursiva que recorre el arbol de forma inorden e inserta los elementos de este en el
//		array. Se detiene cuando los elementos insertados sean mayores o iguales al tope.

void insertar_inorden(nodo_abb_t* nodo_actual, void** array, size_t tamanio_array,
						 size_t* elementos_insertados) {
	if (nodo_actual == NULL || (*elementos_insertados) >= tamanio_array) {
		return ;
	}

	if (nodo_actual->izquierda != NULL) {
		insertar_inorden(nodo_actual->izquierda, array, tamanio_array, elementos_insertados) ;
	}
	if(nodo_actual && (*elementos_insertados) < tamanio_array) {
		array[*elementos_insertados] = nodo_actual->elemento ;
		(*elementos_insertados)++ ;
	}
	if (nodo_actual->derecha) {
		insertar_inorden(nodo_actual->derecha, array, tamanio_array, elementos_insertados) ;
	}
}

size_t arbol_recorrido_inorden(abb_t* arbol, void** array, size_t tamanio_array) {
	size_t elementos_insertados = 0 ;
	insertar_inorden(arbol->nodo_raiz, array, tamanio_array, &elementos_insertados) ;
	return elementos_insertados ;
}

//Pre: Recibe un nodo valido, un array, su tope y la cantidad de elementos insertados.
//Post: Funcion recursiva que recorre el arbol de forma preorden e inserta los elementos de este en el
//		array. Se detiene cuando los elementos insertados sean mayores o iguales al tope.

void insertar_preorden(nodo_abb_t* nodo_actual, void** array, size_t tamanio_array, 
						size_t* elementos_insertados) {
	if (nodo_actual == NULL || (*elementos_insertados) >= tamanio_array) {
		return ;
	}

	if(nodo_actual && (*elementos_insertados) < tamanio_array) {
		array[*elementos_insertados] = nodo_actual->elemento ;
		(*elementos_insertados)++ ;
	}
	if (nodo_actual->izquierda != NULL) {
		insertar_preorden(nodo_actual->izquierda, array, tamanio_array, elementos_insertados) ;
	}
	if (nodo_actual->derecha != NULL) {
		insertar_preorden(nodo_actual->derecha, array, tamanio_array, elementos_insertados) ;
	}
}

size_t arbol_recorrido_preorden(abb_t* arbol, void** array, size_t tamanio_array) {
	size_t elementos_insertados = 0;
	insertar_preorden(arbol->nodo_raiz, array, tamanio_array, &elementos_insertados) ;
	return elementos_insertados ;
}

//Pre: Recibe un nodo valido, un array, su tope y la cantidad de elementos insertados.
//Post: Funcion recursiva que recorre el arbol de forma postorden e inserta los elementos de este en el
//		array. Se detiene cuando los elementos insertados sean mayores o iguales al tope.

void insertar_postorden(nodo_abb_t* nodo_actual, void** array, size_t tamanio_array, 
						size_t* elementos_insertados) {
	if (nodo_actual == NULL || (*elementos_insertados) >= tamanio_array) {
		return ;
	}

	if (nodo_actual->izquierda != NULL) {
		insertar_postorden(nodo_actual->izquierda, array, tamanio_array, elementos_insertados) ;
	}
	if (nodo_actual->derecha != NULL) {
		insertar_postorden(nodo_actual->derecha, array, tamanio_array, elementos_insertados) ;
	}
	if(nodo_actual && (*elementos_insertados) < tamanio_array) {
		array[*elementos_insertados] = nodo_actual->elemento ;
		(*elementos_insertados)++ ;
	}
}

size_t arbol_recorrido_postorden(abb_t* arbol, void** array, size_t tamanio_array) {
	size_t elementos_insertados = 0;
	insertar_postorden(arbol->nodo_raiz, array, tamanio_array, &elementos_insertados) ;
	return elementos_insertados ;
}

//Pre: Recibe un arbol existente y un nodo.
//Post: Borra todos los elementos de el arbol de forma postorden.

void borrar_postorden (abb_t* arbol, nodo_abb_t* nodo_actual) {
	if (nodo_actual == NULL) {
		return ;
	}

	if (nodo_actual->izquierda != NULL) {
		borrar_postorden(arbol, nodo_actual->izquierda) ;
	}
	if (nodo_actual->derecha != NULL) {
		borrar_postorden(arbol, nodo_actual->derecha) ;
	}
	if(nodo_actual) {
		borrar_nodo(arbol, nodo_actual) ;
	}
}

void arbol_destruir(abb_t* arbol) {
	if (arbol == NULL) {
		return ;
	}
	borrar_postorden(arbol, arbol->nodo_raiz) ;
	free(arbol) ;
}

void recorrer_inorden_it_in(nodo_abb_t* nodo_actual, bool (*funcion)(void*, void*), void* extra, 
						size_t* elementos_iterados, bool* dejar_de_iterar) {
	if (nodo_actual == NULL || (*dejar_de_iterar) == true) {
		return ;
	}

	if (nodo_actual->izquierda != NULL && (*dejar_de_iterar) == false) {
		recorrer_inorden_it_in(nodo_actual->izquierda, funcion, extra, elementos_iterados, dejar_de_iterar) ;
	}
	if(nodo_actual && (*dejar_de_iterar) == false) {
		if (funcion(nodo_actual->elemento, extra) == false) {
			(*elementos_iterados)++ ;
		}
		else {
			(*elementos_iterados)++ ;
			(*dejar_de_iterar) = true ;
			return  ;
		}
	}
	if (nodo_actual->derecha != NULL && (*dejar_de_iterar) == false) {
		recorrer_inorden_it_in(nodo_actual->derecha, funcion, extra, elementos_iterados, dejar_de_iterar) ;
	}
}

void recorrer_preorden_it_in(nodo_abb_t* nodo_actual, bool (*funcion)(void*, void*), void* extra, 
						 size_t* elementos_iterados, bool* dejar_de_iterar) {
	if (nodo_actual == NULL || (*dejar_de_iterar) == true) {
		return ;
	}

	if(nodo_actual && (*dejar_de_iterar) == false) {
		if (funcion(nodo_actual->elemento, extra) == false) {
			(*elementos_iterados)++ ;
		}
		else {
			(*elementos_iterados)++ ;
			(*dejar_de_iterar) = true ;
			return  ;
		}
	}
	if (nodo_actual->izquierda != NULL && (*dejar_de_iterar) == false) {
		recorrer_preorden_it_in(nodo_actual->izquierda, funcion, extra, elementos_iterados, dejar_de_iterar) ;
	}
	if (nodo_actual->derecha != NULL && (*dejar_de_iterar) == false) {
		recorrer_preorden_it_in(nodo_actual->derecha, funcion, extra, elementos_iterados, dejar_de_iterar) ;
	}
}

void recorrer_postorden_it_in(nodo_abb_t* nodo_actual, bool (*funcion)(void*, void*), void* extra, 
						 size_t* elementos_iterados, bool* dejar_de_iterar) {
	if (nodo_actual == NULL || (*dejar_de_iterar) == true) {
		return ;
	}

	if (nodo_actual->izquierda != NULL && (*dejar_de_iterar) == false) {
		recorrer_postorden_it_in(nodo_actual->izquierda, funcion, extra, elementos_iterados, dejar_de_iterar) ;
	}
	if (nodo_actual->derecha != NULL && (*dejar_de_iterar) == false) {
		recorrer_postorden_it_in(nodo_actual->derecha, funcion, extra, elementos_iterados, dejar_de_iterar) ;
	}

	if(nodo_actual && (*dejar_de_iterar) == false) {
		if (funcion(nodo_actual->elemento, extra) == false) {
			(*elementos_iterados)++ ;
		}
		else {
			(*elementos_iterados)++ ;
			(*dejar_de_iterar) = true ;
			return  ;
		}
	}
}

size_t abb_con_cada_elemento(abb_t* arbol, int recorrido, bool (*funcion)(void*, void*), void* extra) {
	size_t elementos_iterados = 0 ;
	bool dejar_de_iterar = false ;

	if (arbol_vacio(arbol) == true) {
		return elementos_iterados ;
	}
	if (funcion == NULL){
		return elementos_iterados ;
	}

	if (recorrido == ABB_RECORRER_INORDEN) {
		recorrer_inorden_it_in (arbol->nodo_raiz, funcion, extra, &elementos_iterados, &dejar_de_iterar) ;
	}
	else if (recorrido == ABB_RECORRER_PREORDEN) {
		recorrer_preorden_it_in (arbol->nodo_raiz, funcion, extra, &elementos_iterados, &dejar_de_iterar) ;
	}
	else if (recorrido == ABB_RECORRER_POSTORDEN) {
		recorrer_postorden_it_in (arbol->nodo_raiz, funcion, extra, &elementos_iterados, &dejar_de_iterar) ;
	}
	return elementos_iterados ;
}