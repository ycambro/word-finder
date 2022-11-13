#ifndef arbolChaf_h
#define arbolChaf_h
//#include "arbolB.c"

/*Struct para los nodos del árbol chaflaneado.*/
typedef struct nodo_c{
	struct lista* archivos;	//saber en que archivos está
	char* terminos;			//palabra dentro del nodo
	int valor;				//valor ascii
	struct nodo_c* izq;
	struct nodo_c* der;
}noditoc;
 

/*Struct para el árbol chaflaneado.*/
typedef struct arbol_c{
	struct nodo_c* raiz;
}arbolitoc;


/*Struct para las listas simples*/
typedef struct nodo{
	char nombre[3024]; // Nombre archivo
	save* guardar;
	struct nodo* sigt;
}nodito;

/*Struct para hacer una lista*/
typedef struct lista{
	struct nodo* inicio;
}listilla;


/* SE USA EN: {arbolChaf.c -> insertar_al_final}
 * Función para crear un nodo de una lista simple
 * @param [texto]: será un char* el cual representará el contenido del nodo.
 * | Se pide memoria para el nodo y se rellena el campo de valor con el texto.
 * @return [nn]: nodito*, se devuelve el nodo creado.
*/
nodito* crear_nodo(char* texto);


/* SE USA EN: {arbolChaf.c -> insertar_chaflaneado, }
 * Función para crear un nodo de un árbol chaflaneado/Splay
 * @param [termino]: será la palabra encontrada en los archivos
 * @param [archivo]: representa la lista de los archivos en lo que se encuentra el [termino].
 * | Se pide memoria para el nodo y se rellena sus respectivos campos.
 * | Para el campo de valor se suma los valores ASCII de todos los caracteres del [termino].
 * @return [nn]: noditoc*, se devuelve el nodo ya creado
*/
noditoc* crear_nodo_c(char* termino, listilla* archivo);


/* SE USA EN: {arbolB.c -> insertar_llave, crear_llave}
 * Función para insertar al final de una lista
 * @param [lista]: representa la lista a la cual se inserta el nodo
 * @param [annadir]: será el texto que se buscará añadir a la lista
 * | Se crea un nodo que contenga el texto añadir y si la lista está vacía 
 * | lo añade al inicio, sino recorre la lista hasta el final y lo añade.
 * @return []: void.
*/
int insertar_al_final(listilla* lista, char* annadir);


/* SE USA EN: {Proyecto2.c}
 * Función para hallar la longitud de una lista.
 * @param [lista]: es una lista.
 * | Se recorre la lista y se lleva un contador de nodos.
 * @return [elementos]: int, el número de nodos en la lista.
 */
int longitud_lista(listilla* lista);


/* SE USA EN: {Proyecto2.c}
 * Funcion para imprimir una lista simple
 * @param [lista]: será la lista que se imprimira
 * | Se recorre la lista y se imprime nodo por nodo hasta llegar al final
 * @return []: int, devuelve 0 para indicar que todo ocurrió correctamente
*/
int imprimir_lista(listilla* lista);


/* NO SE USA???
 * Funcion para imprimir un arbol splay con un recorrido de preorden.
 * @param [nodoActual]: representará al nodo que se esté imprimiendo actualmente.
 * | Cuando el nodo sea diferente a NULL, se imrpimira primero la raiz y 
 * | se procedera a llamar a la funcion de forma recursiva primero para el subarbol izquierdo
 * | y luego para el derecho
 * @return []: void.
*/
void imprimirPreorden(noditoc* nodoActual);


/* SE USA EN: {arbolChaf.c -> reacomodar}
 * Funcion para rotar hacia la derecha
 * @param [n]: representará el nodo padre
 * @param [n1]: representará el nodo hijo
 * | El subarbol derecho del nodo hijo se procede a colocarlo
 * | en el subarbol izquierdo del nodo padre. Se mueve el nodo padre
 * | al subarbol derecho del hijo. Se cambia el puntero de n por n1.
 * @return []: void.
*/
void rotar_der(noditoc* n, noditoc* n1);


/* SE USA EN: {arbolChaf.c -> reacomodar}
 * Funcion para rotar hacia la izquierda
 * @param [n]: representará el nodo padre
 * @param [n1]: representará el nodo hijo
 * | El subarbol izquierdo del nodo hijo se procede a colocarlo
 * | en el subarbol derecho del nodo padre. Se mueve el nodo padre
 * | al subarbol izquierdo del hijo. Se cambia el puntero de n por n1.
 * @return []: void.
*/
void rotar_izq(noditoc* n, noditoc* n1);


/* SE USA EN: {arbolChaf.c -> reacomodar}.
 * Funcion que nos permite hallar el nodo de padre de otro nodo
 * @param [raiz]: representa la raiz del arbol que se va a recorrer.
 * @param [nodo]: sera el nodo al que se le va a buscar el padre.
 * | En caso de que el arbol este vacio o el elemento hijo sea la raiz
 * | se devolvera NULL. Mientras que si vemos que la raiz del subarbol actual
 * | tiene como hijo derecho o izquierdo al nodo pasado como parametro se 
 * | devolvera dicha raiz.
 * | Si el valor del nodo es menor al valor de la raiz actual se llama a la
 * | misma funcion pero con el subarbol izquierdo. 
 * | Si el valor del nodo es mayor al valor de la raiz actual se llama a la
 * | misma funcion pero con el subarbol derecho. 
 * @return [raiz]: noditoc*, devuelve el nodo padre encontrado. (o NULL)
*/
noditoc* padre(noditoc* raiz, noditoc* nodo);


/* SE USA EN: {arbolChaf.c -> buscar_nodo_c, insertar_chaflaneado}
 * Funcion de acomodo para subir el ultimo nodo visitado o insertado
 * @param [nodo]: sera el ultimo nodo que fue insertado o visitado
 * @param [arbol]: representara el arbol que se este recorriendo
 * | Se busca el nodo padre y el nodo abuelo del ultimo nodo vistado o insertado.
 * | Según la psociones en la que se encuentre, se realizaran u conjunto de rotaciones
 * | para lograr subir el nodo.
 * @return []: void.
*/
void reacomodar(noditoc* nodo, arbolitoc* arbol);


/* SE USA EN: {arbolCHaf.c -> insertar_chaflaneado}
 * Función auxiliar para insertar en un arbol chaflaneado / Splay
 * @param [raiz]: nodo raiz del subarbol actual
 * @param [nn]: sera el nodo que se insertara en el arbol
 * | Se recorre el arbol verificando si los valores respetan 
 * | la busqueda binaria del mismo. Se insertara el nodo cuando
 * | la raiz del subárbol actual no cuente con algun nodo hijo.
 * @return []: void.
*/
void insertar_chaflaneado_auxiliar(noditoc* raiz, noditoc* nn);


/* SE USA EN: {Proyecto2.c -> Main(Switch: case4)}
 * Funcion para insertar en un arbol chaflaneado/splay
 * @param [arbol]: sera el arbol donde se insertara el nodo
 * @param [termino]: representa la palabra que contendra el nodo a insertar
 * @param [lista]: sera la lista de archivos que tendra el nodo
 * | Se crea el nuevo nodo, si el arbol esta vacio se inserta como raiz.
 * | Si no esta vacio se llamara a la funcion auxiliar de insertar y se realiza la verificacion si 
 * | el nodo insertado se encuentra en la raiz. Se hará un reacomodo 
 * | hasta lograr subir el nodo insertado a la raíz.
*/
void insertar_chaflaneado(arbolitoc* arbol, char* termino, listilla* lista);


/* SE USA EN: {arbolChaf.c -> buscar_nodo}
 * Funcion auxiliar para buscar un nodo en el arbol splay
 * @param [nodo]: sera el nodo actual que se validara
 * @param [valor]: representara el valor que compararemos para buscar el nodo
 * | Se ira bajando el arbol segun sea el valor a buscar y la raiz del subarbol actual.
 * | En caso que el termino no se encuentre en el arbol se devolvera NULL.
 * @return [nodo]: noditoc*, nodo encontrado luego de hacer la busqueda. (o NULL)
*/
noditoc* buscar_nodo_aux(noditoc* nodo, int valor, char* palabra);


/* SE USA EN: {Proyecto2.c -> Main(Switch: case4)}
 * Funcion para buscar el nodo del arbol que contenga la palabra ingresada
 * @param [arbol]: sera la estructura a recorrer para buscar el nodo.
 * @param [termino]: palabra a buscar en el arbol.
 * | Se halla el valor de la palabra ingresada llamando la funcion auxiliar y con respecto a ese valor
 * | se busca el nodo solicitado. Al hallar dicho nodo, por definicion de los arboles splay, 
 * | se llamara a la funcion de reacomodar para subir a la raiz el nodo encontrado.
 * @return [nodo_encontrado]: noditoc*, sera el nodo encontrado.
*/
noditoc* buscar_nodo_c(arbolitoc* arbol, char* termino);

#endif