#ifndef __HEAP_H__
#define __HEAP_H__

#define EXITO 0
#define ERROR -1

#define ES_MAYOR 1
#define ES_IGUAL 0
#define ES_MENOR -1

//Comparador de elementos. Recibe dos elementos del heap y devuelve
//0 en caso de ser iguales, 1 si el primer elemento es mayor o
//-1 si el segundo elemento es el mayor. */

typedef int (*heap_comparador)(void*, void*);

// Destructor de elementos. Cada vez que un elemento deja el heap
// se invoca al destructor pasandole el elemento. 

typedef void (*heap_destructor)(void*);

typedef struct heap {
	void** array ;
	size_t tope ;
	heap_comparador comparador ;
	heap_destructor destructor ;
} heap_t ;


//Crea el heap reservando la memoria necesaria para este. Utiliza el
//comparador para ordenar los elementos y el destructor para eliminarlos
//(este último es opcional). Si no recibe un array, el heap se creará vacio.
//Devuelve NULL si hubo algún error o no se pudo crear el heap.

heap_t* heap_crear(heap_comparador comparador ,heap_destructor destructor) ;

//Inserta al elemento dentro del heap y lo pone en su debida posición. 
//Devuelve 0 si se insertó exitosamente o -1 si hubo algún error.

int heap_insertar(heap_t* heap, void* elemento) ;

//Extrae la raiz del heap y la devuelve. Si hubo algún error devuelve NULL.

void* heap_extraer_raiz (heap_t* heap) ;

//Devuelve true si el heap está vacío.

bool heap_vacio (heap_t* heap) ;

//Destruye el heap eliminando todos los elementos y liberando su espacio en 
//memoria. Si es necesario llama al destructor en cada elemento.

void heap_destruir(heap_t* heap) ;

#endif /* __HEAP_H__ */