#ifndef arbolB_h
#define arbolB_h

#define GRADO_BTREE 5
//#include "arbolChaf.c"

/*Struct para las llaves de las palabras en la tabla hash*/
typedef struct llave{
	char palabra[1024];     // palabra per se
	int valor; 				// valor ascii de la palabra
	struct lista* archivos;
}llave;

/*Struct para el arból B*/
typedef struct arbol_b{
	int grado;
	struct nodo_b* raiz;
}arbol_b;

/*Struct para los nodos del árbol B*/
typedef struct nodo_b{
	struct llave** llaves; // Arreglo de llaves
	int min_llaves; 	   // numero minimo de llave
	int ocupado;           // numero actual de llaves
	struct nodo_b* papa;
	struct nodo_b** ramas; // Rammas
}nodo_b;

/*
 * Funcion crear nodo de un arbol B
 * | Se pide memoria para el nuevo nodo, se rellenan los diferentes campos
 * | del struct y se pide memoria para el arreglo de llavez y ramas.
 * @return [nn]: nodo_b*, devuelve el nodo ya creado.
*/
nodo_b* crear_nodo_b();


/*
 * Funcion para hallar la sumas de los valores ASCII de todos los caracteres
 * @param [llave]: sera la palabra a convertir
 * @param [llave_tammanno]: longitud de la palabra
 * @return [suma]: int, la suma de todos los valores ASCII
*/
int funcion_hash(char* llave, int llave_tamanno);


/*// FALTARIA AGREGAR ARCHIVOS [EDIT: en teoría ya no]
 *Funcion que crea un "struct" que contiene una palabra y la lista de archivos en los que aparece.
 *@param [palabra]: es una palabra leída de un archivo.
 *@param [archivo]: es el archivo en el cual se leyó la palabra.
 *@return [nueva_llave]: llave*, con la [palabra] y la lista de archivos(nueva_llave->archivos) con [archivo] al final.
*/
llave* crear_llave(char* palabra, char* archivo);


/*
 * Funcion para insertar una llave en un nodo de un arbol B
 * @param [nodoDondeInsertar]: el nodo en el cual se va a insertar la nueva llave
 * @param [llaveAInsertar]: llave que se ingresara en el nodo.
 * | Se determina si la nueva palabra ya se encuentra en el nodo y en 
 * | el caso de que asi sea, se determinar si ya se encuentra en la lista de archivos
 * | el archivo de donde se está leyendo.
 * | Se inserta la llave en la primera posicion disponible del arreglo de llaves
 * | del nodo. Se incrementa el contador de llaves en el nodo.
 * | Se reordenar el nodo para que siga el mismo orden del arbol.
 * | Se verifica si es necesario un reacomodo en nodo.
 * @return []: int, devolvera un 1 si es necesario hacer una division del nodo
*/
int insertar_llave(nodo_b* nodoDondeInsertar, llave* llaveAInsertar);


/*
 * Funcion para buscar el nodo donde se va a insertar
 * @param [nodo_actual]: sera el nodo actual por el cual se este recorriendo
 * @param [llaveInsertar]: es la llave que se buscara insertara
 * | Se recorre el nodo actual y se baja hacia las ramas segun sea el valor
 * | de la llave a insertar. Cada vez que se baje por una rama se hara 
 * | una llamada recursiva de la funcion.
 * @return [nodo_actual]: nodo_b*, nodo encontrado
*/
nodo_b* buscar_nodo_insertar(nodo_b* nodo_actual, llave* llaveInsertar);


/*
 * Funcion para buscar un nodo en el arbol B
 * @param [palabra]: será el termino que se buscara en el arbol
 * @param [raiz]: representara el nodo raiz del arbol
 * | En base al valor ASCII de la palabra a buscar, se recorre
 * | el arbol bajando por las ramas hasta encontrar el nodo con la 
 * | llave correspondiente
 * @return []: llave*, devuelve la llave de la palabra buscada. 
 * (o posible NULL)
*/
llave* buscar_nodo_b(char* palabra, nodo_b* raiz, int valor);


/*
 * Función para reorganizar el arbol al insertar una llave
 * @param [nodoArreglar]: sera el nodo al que se le realizara la division
 * @param [arbol]: sera el arbol B en el que esta los nodos
 * | Cuando el nodo contiene 5 llaves, se sube al nodo padre la llave del medio y se 
 * | genera un nuevo nodo con las ultimas doa llaves del nodo separado.
 * | En caso de que el nodo padre tambien necesite ser reorganizado, se volvera
 * | a llamar a al funcion.
 * | Si el nodo que supera la capacidad es la raiz, se crea un nuevo nodo que sera la raiz 
 * | del arbol con la llave del medio del nodo separado.
 * @return []: void.
*/
void reorganizarArbolB(nodo_b* nodoArreglar, arbol_b* arbol);

#endif