#include <stdlib.h>
#include "lista.h"

#define ERROR -1
#define EXITO 0

#define LISTA_VACIA 0 
#define UN_ELEMENTO 1

#define PRIMER_NODO 0

lista_t* lista_crear(lista_destructor destructor) {
	lista_t* lista = malloc(sizeof(lista_t)) ;
	if (!lista) {
		return NULL ;
	}
	lista->nodo_inicio = NULL ;
	lista->nodo_fin = NULL ;
	lista->cantidad = 0 ;
	lista->destructor = destructor ;

	return lista ;
}

//Pre: Recibe un elemento.
//Post: Crea un nodo reservando la memoria necesaria agregandole el elemento y un NULL en diguiente. Si
//		se crea exitosamente devuelve el nodo sino devuelve NULL.

nodo_t* crear_nodo (void* elemento) {
	nodo_t* nodo = malloc(sizeof(nodo_t)) ;
	if (!nodo) {
		return NULL ;
	}
	nodo->elemento = elemento ;
	nodo->siguiente = NULL ;
	return nodo ;
}

int lista_insertar(lista_t* lista, void* elemento){
	if (!lista) {
		return ERROR ;
	}
	nodo_t* nodo = crear_nodo(elemento) ;
	if (!nodo) {
		return ERROR ;
	}
	

	if (lista->cantidad == LISTA_VACIA) {
		lista->nodo_inicio = nodo ;
		lista->nodo_fin = nodo ;
		lista->cantidad++ ;
		return EXITO ;
	}
	else {
		lista->nodo_fin->siguiente = nodo ;
		lista->nodo_fin = nodo ;
		lista->cantidad++ ;
		return EXITO ;
	}
}

int lista_insertar_en_posicion(lista_t* lista, void* elemento, size_t posicion) {
	if (!lista) {
		return ERROR ;
	}
	if (lista->cantidad <= posicion) {
		return lista_insertar(lista, elemento) ;
	}

	nodo_t* nodo = crear_nodo(elemento) ;
	if (!nodo) {
		return ERROR ;
	}

	if (posicion == PRIMER_NODO) {
		nodo->siguiente = lista->nodo_inicio ;
		lista->nodo_inicio = nodo ;
		lista->cantidad++ ;
		return EXITO ;
	}
	else {
		nodo_t* nodo_anterior = lista->nodo_inicio ;
		for (int i= 1 ; i< posicion ; i++) {
			nodo_anterior = nodo_anterior->siguiente ;
		}
		nodo->siguiente = nodo_anterior->siguiente ;
		nodo_anterior->siguiente = nodo ;
		lista->cantidad++ ;
		return EXITO ;
	}
}

int lista_borrar(lista_t* lista) {
	if (!lista) {
		return ERROR ;
	}
	if (lista->cantidad == LISTA_VACIA) {
		return ERROR ;
	}
	if (lista->cantidad == UN_ELEMENTO) {
		if (lista->destructor != NULL) {
			lista->destructor(lista->nodo_fin->elemento) ;
		}
		free(lista->nodo_fin) ;
		lista->nodo_fin = NULL ;
		lista->nodo_inicio = NULL ;
		lista->cantidad-- ;
		return EXITO ;
	}
	else {
		nodo_t* ante_ultimo_nodo = lista->nodo_inicio ;
		for (int i= 1 ; i< (lista->cantidad - 1) ; i++) {
			ante_ultimo_nodo = ante_ultimo_nodo->siguiente ;
		}
		if (lista->destructor != NULL) {
			lista->destructor(lista->nodo_fin->elemento) ;
		}
		free(lista->nodo_fin) ;
		ante_ultimo_nodo->siguiente = NULL ;
		lista->nodo_fin = ante_ultimo_nodo ;
		lista->cantidad-- ;
		return EXITO ;
	}
}

int lista_borrar_de_posicion(lista_t* lista, size_t posicion) {
	if (!lista) {
		return ERROR ;
	}
	if (lista->cantidad == LISTA_VACIA) {
		return ERROR ;
	}
	if (lista->cantidad == UN_ELEMENTO) {
		if (lista->destructor != NULL) {
			lista->destructor(lista->nodo_fin->elemento) ;
		}
		free(lista->nodo_fin) ;
		lista->nodo_fin = NULL ;
		lista->nodo_inicio = NULL ;
		lista->cantidad-- ;
		return EXITO ;
	}
	if (posicion == PRIMER_NODO) {
		nodo_t* nodo_a_borrar = lista->nodo_inicio ;
		lista->nodo_inicio = nodo_a_borrar->siguiente ;
		if (lista->destructor != NULL) {
			lista->destructor(nodo_a_borrar->elemento) ;
		}
		free(nodo_a_borrar) ;
		lista->cantidad-- ;
		return EXITO ;
	}
	if (posicion >= lista->cantidad) {
		return lista_borrar(lista) ;
	}
	else {
		nodo_t* nodo_a_borrar = lista->nodo_inicio->siguiente ;
		nodo_t* nodo_anterior = lista->nodo_inicio ;
		for (int i= 1 ; i< posicion ; i++) {
			nodo_a_borrar = nodo_a_borrar->siguiente ;
			nodo_anterior = nodo_anterior->siguiente ;
		}
		nodo_anterior->siguiente = nodo_a_borrar->siguiente ;
		if (lista->destructor != NULL) {
			lista->destructor(nodo_a_borrar->elemento) ;
		}
		free (nodo_a_borrar) ;
		lista->cantidad-- ;
		return EXITO ;
	}
}

void* lista_devolver_elemento (lista_t* lista, size_t posicion) {
	if (!lista) {
		return NULL ;
	}
	if (lista->cantidad == LISTA_VACIA) {
		return NULL ;
	}
	if (lista->cantidad == UN_ELEMENTO) {
		void* elemento = lista->nodo_fin->elemento ;
		free(lista->nodo_fin) ;
		lista->nodo_fin = NULL ;
		lista->nodo_inicio = NULL ;
		lista->cantidad-- ;
		return elemento ;
	}
	if (posicion == PRIMER_NODO) {
		nodo_t* nodo_a_borrar = lista->nodo_inicio ;
		lista->nodo_inicio = nodo_a_borrar->siguiente ;
		void* elemento = nodo_a_borrar->elemento ;
		free(nodo_a_borrar) ;
		lista->cantidad-- ;
		return elemento ;
	}
	if (posicion >= lista->cantidad) {
		nodo_t* ante_ultimo_nodo = lista->nodo_inicio ;
		for (int i= 1 ; i< (lista->cantidad - 1) ; i++) {
			ante_ultimo_nodo = ante_ultimo_nodo->siguiente ;
		}
		void* elemento = lista->nodo_fin->elemento ;
		free(lista->nodo_fin) ;
		ante_ultimo_nodo->siguiente = NULL ;
		lista->nodo_fin = ante_ultimo_nodo ;
		lista->cantidad-- ;
		return elemento ;
	}
	else {
		nodo_t* nodo_a_borrar = lista->nodo_inicio->siguiente ;
		nodo_t* nodo_anterior = lista->nodo_inicio ;
		for (int i= 1 ; i< posicion ; i++) {
			nodo_a_borrar = nodo_a_borrar->siguiente ;
			nodo_anterior = nodo_anterior->siguiente ;
		}
		void* elemento = nodo_a_borrar->elemento ;
		free (nodo_a_borrar) ;
		lista->cantidad-- ;
		return elemento ;
	}
}

void* lista_elemento_en_posicion(lista_t* lista, size_t posicion) {
	if (!lista) {
		return NULL ;
	}
	if (posicion >= lista->cantidad) {
		return NULL ;
	}
	else {
		nodo_t* nodo_a_devolver = lista->nodo_inicio ;
		for (int i= 0 ; i< posicion ; i++) {
			nodo_a_devolver = nodo_a_devolver->siguiente ;
		}
		return nodo_a_devolver->elemento ;
	}
}

void* lista_ultimo(lista_t* lista) {
	if (!lista) {
		return NULL ;
	}
	if (lista->cantidad == LISTA_VACIA) {
		return NULL ;
	}
	else {
		return lista->nodo_fin->elemento ;
	}
}

bool lista_vacia(lista_t* lista) {
	if (!lista) {
		return true ;
	}
	return !(lista->cantidad) ;
}

size_t lista_elementos(lista_t* lista) {
	if (!lista) {
		return LISTA_VACIA ;
	}
	return lista->cantidad ;
}

int lista_apilar(lista_t* lista, void* elemento) {
	return lista_insertar(lista, elemento) ;
}

int lista_desapilar(lista_t* lista) {
	return lista_borrar(lista) ;
}

void* lista_tope(lista_t* lista) {
	return lista_ultimo(lista) ;
}

int lista_encolar(lista_t* lista, void* elemento) {
	return lista_insertar(lista, elemento) ;
}

int lista_desencolar(lista_t* lista){
	if (!lista) {
		return ERROR ;
	}
	if (lista->cantidad == LISTA_VACIA) {
		return ERROR ;
	}
	if (lista->cantidad == UN_ELEMENTO) {
		if (lista->destructor != NULL) {
			lista->destructor(lista->nodo_fin->elemento) ;
		}
		free(lista->nodo_fin) ;
		lista->nodo_fin = NULL ;
		lista->nodo_inicio = NULL ;
		lista->cantidad-- ;
		return EXITO ;
	}	
	else {
		nodo_t* nodo_a_borrar = lista->nodo_inicio ;
		lista->nodo_inicio = nodo_a_borrar->siguiente ;
		if (lista->destructor != NULL) {
			lista->destructor(nodo_a_borrar->elemento) ;
		}
		free(nodo_a_borrar) ;
		lista->cantidad-- ;
		return EXITO ;
	}
}

void* lista_primero(lista_t* lista) {
	if (!lista) {
		return NULL ;
	}
	if (lista->cantidad == LISTA_VACIA) {
		return NULL ;
	}
	else {
		return lista->nodo_inicio->elemento ;
	}
}

void lista_destruir(lista_t* lista) {
	if (lista) {
		while (lista_vacia(lista) == false){
			lista_borrar(lista) ;
		}
		free(lista) ;
	}
}

lista_iterador_t* lista_iterador_crear(lista_t* lista) {
	if (!lista) {
		return NULL ;
	}
	lista_iterador_t* iterador = malloc (sizeof (lista_iterador_t)) ;
	if (!iterador) {
		return NULL ;
	}
	iterador->corriente = lista->nodo_inicio ;
	iterador->lista = lista ;
	return iterador ;
}

bool lista_iterador_tiene_siguiente(lista_iterador_t* iterador) {
	if (!iterador) {
		return false;
	}
	if (lista_vacia(iterador->lista)) {
		return false ;
	}
	if (iterador->corriente == NULL) {
		return false ;
	}
	else {
		return true ;
	}
}

bool lista_iterador_avanzar(lista_iterador_t* iterador) {
	if (!iterador) {
		return false ;
	}
	if (lista_vacia (iterador->lista)) {
		return false ;
	}
	if (iterador->corriente->siguiente == NULL) {
		iterador->corriente = NULL ;
		return false;
	}
	else {
		iterador->corriente = iterador->corriente->siguiente ;
		return true ;
	}
}

void* lista_iterador_elemento_actual(lista_iterador_t* iterador) {
	if (!iterador) {
		return NULL ;
	}
	if (lista_vacia(iterador->lista)) {
		return NULL ;
	}
	if (iterador->corriente == NULL) {
		return NULL ;
	}
	else {
		return iterador->corriente->elemento ;
	}
}

void lista_iterador_destruir(lista_iterador_t* iterador) {
	if (iterador) {
		free (iterador) ;
	}
}

size_t lista_con_cada_elemento(lista_t* lista, bool (*funcion)(void*, void*), void *contexto) {
	if (!lista || funcion == NULL) {
		return 0 ;
	}
	nodo_t* nodo_actual = lista->nodo_inicio ;
	size_t contador = 0 ;

		while (nodo_actual && funcion(nodo_actual->elemento, contexto) == true) {
			contador++ ;
			nodo_actual = nodo_actual->siguiente ;
		}
	return contador ;
}