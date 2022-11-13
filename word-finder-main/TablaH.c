#include <stdio.h>
#include<stdlib.h>
#include <string.h>
#include <time.h>
#include "TablaH.h"
#define TAMANNO_MAXIMO 	1024*1024		// Longitud máxima de la tabla
#define SI			1
#define NO			0
#define ENVIA		1
#define RECIBE		0



/* SE USA EN: {TablaH.c -> annadir_nodo_hash, obtener_datos_hash}
 * Función para devolver valor hash en base a corrimiento de bits
 * @pam [key]: palabra.
 * @pam [key_len]: longitud de la palabra.
 * | Primero se agarra la primera letra de [key] en ASCII
 * | Se hace un corrimiento de bits de 5 unidades hacia la izquierda de la varible "hash"
 * | Se le suma el valor ASCII de la letra y se suma nuevamente "hash" con un corrimiento de 2 unidades hacia la derecha
 * | se realiza un XOR y este proceso se repite para cada letra de la palabra.  
 * @return [has]: unsigned int, .
*/
unsigned int valor_hash(char* key, unsigned int key_len) {
	unsigned int hash = 1315423911;
	unsigned int i = 0;

	for(i = 0; i < key_len; key++, i++)
	{
		hash ^= ((hash << 5) + (*key) + (hash >> 2)); 	//Se hace un XOR en ciclo
	}

	return hash;
}

/* SE USA EN: {Proyecto2.c -> Main}
 * Función para crear una tabla hash
 * | Pide la memoria para crear una tabla que va a contener
 * | 1024*1024 "nodos" dentro de sí. Si no consigue la memoria necesaria
 * | para todos se libera la memoria ocupada.
 * @return [tablilla]: tabla_h*, la tabla vacía si consiguió la memoria suficiente. (o NULL)
*/
tabla_h* crear_tabla(void) {
	tabla_h* tablilla = (tabla_h *)calloc(1, sizeof(tabla_h));	    // Se asigna el espacio y direccion para la tabla
	if (!tablilla)
	{
		printf("No hay memoria suficiente\n");
		return NULL; 
	}
	
	tablilla -> table = calloc(1,sizeof(nodo_h) * TAMANNO_MAXIMO);	// Asigna el espacio para los nodos de la tabla hash
	if (!tablilla -> table) 										//Si no se logra crear el espacio, se elimina la tabla
	{
		printf("No hay suficiente memoria para la tabla\n");
		free(tablilla);
		tablilla = NULL;
		return NULL;
	}
	
	return tablilla;												// Devuelve la tabla
}

/* SE USA EN: {TablaH.c -> annadir_ruta}
 * Función para añadir nodo a la tabla de hash.
 * @param [tablilla]: es la tabla en la que se va a anhadir el nodo.
 * @param [key]: es la llave de un elemento dentro de [tablilla].
 * @param [key_len]: es la longitud de [key].
 * @param [valor]: es la ruta de un documento.
 * | Se revisa que los valores dentro de la tabla no sean nulos. Luego se posiciona en un subindice
 * | determinado dentro de la tabla, una vez ahí revisa que la llave de dicho subindice sea igual
 * | a la del valor que se quiere anhadir, si lo es se actualiza el valor de la posición; si no 
 * | se sigue recorriendo la tabla.
 * | Si se llega a una posición vacía se intenta pedir la memoria para anhadir un nuevo nodo y 
 * | reacomodan los punteros.
 * @return []: int, 0 si funcionó o -1 si falló.
*/
int annadir_nodo_hash(tabla_h* tablilla, char* key, unsigned int key_len, void* valor) {
	if(!tablilla || !key ) 			   //Se verifica que la tabla no este vacia y que se haya dado alguna llave
	{
		printf("Algo está en NULL\n");
		return -1;
	}

	unsigned int i = valor_hash(key, key_len) % TAMANNO_MAXIMO; // Se obtiene el subindice de la tabla en base a su llave

	nodo_h* act = tablilla -> table[i]; //Apunta a el punto actual
	nodo_h* prev = act; 			    //Apunta a el anterior al actual, sin embargo al inicio se empieza en actual
	
	while(act)							// Si hay un nodo hash aqui debemos ir al final
	{
		if (strncmp(key, act -> key, key_len) == 0)	     // Si el valor clave ya existe entonces se actualiza
		{
			if(act -> ocupado) 			//Si el punto esta ocupado
			{
				act -> valor = valor; 	//Entonces el valor de actual es el nuevo valor
				act -> ocupado = 1;		// Y se indica nuevamente que actual esta ocupado
				break;
			}
		}
		prev = act;
		act = act -> sigt;
	}
	
	if(!act)							// Si el punto no esta ocupado o ya se pasó por el ciclo anterior
	{
		nodo_h* nn = (nodo_h *)calloc(1, sizeof(nodo_h)); //Se asigna espacio para un nuevo nodo
		if(!nn) 										  //Si no se logra crear
		{
			printf("No hay suficiente memoria\n");  //Entonces es porque no hay:
			return -1;
		}
		char* nn_key = (char*)calloc(1, key_len+1); //Se le da el valor a la clave
		if(!nn_key) 							    //Si no se logra crear se borra el nodo
		{
			free(nn);
			nn = NULL;
			return -1;
		}
		strncpy(nn_key, key, key_len);  //es como decir, nn_key = key, es decir la llave del nuevo nodo es la dada
		nn -> key = nn_key; 	        //Se le dan los valores al nuevo nodo
		nn -> valor = valor; 
		nn -> ocupado = SI;			    // Se asigna como ocupado
		
		if(prev == NULL)			    // Si el punto anterior no existe o no ha sido ocupado
		{
			tablilla -> table[i] = nn; 	//Se añade la tabla aqui, es decir al inicio
		}
		else							// Si el punto ha sido ocupado y se llega al final
		{
			prev -> sigt = nn;		    // Se asigna el nuevo nodo en act
		}
	}
	
	return 0;
}

/* SE USA EN: {TablaH.c -> imprimirRutas}
 * Función para obtener los datos de la tabla hash, para esto se necesita el valor hash
 * Debe devolver el contenido almacenado a menos que no haya(NULL)
 * @param [tablilla]: es una tabla de hash.
 * @param [key]: es el identifcador de un subindice en la tabla.
 * @param [key_len]: es la longitu de dicho identidcador.
 * | Se revisa que tanto com la tabla y el la llave (key) no sean nulos. Luego se obtiene
 * | el subindice en hash con la [key] y [key_len], se revisa dicho subindice en la tabla
 * | para determinar las llaves sí sean iguales, en caso de serlo se retorna el valor en esa
 * | posición; si no, se recorre la tabla hasta llegar posición nula.
 * @return [act -> valor]: void*, valor de un nodo hash.
*/
void* obtener_datos_hash(tabla_h* tablilla, char* key, unsigned int key_len) {
	if( !tablilla || !key) { //Si no hay tabla o no hay clave, entonces es porque:
		printf("Algo esta en NULL\n");
		return NULL;
	}
	
	int i = valor_hash(key, key_len) % TAMANNO_MAXIMO; //Se obtiene el valor hash
	nodo_h *act = tablilla->table[i];
	
	while(act) {
		if(strncmp(act->key, key, key_len) == 0) {
			return act->valor;
		}
		act = act->sigt;
	}
	return NULL;
}


/* SE USA EN: {Proyecto2.c -> cargar, acceder_archivos}
 * Función para anhadir una dirección a la tabla hash.
 * @param [tablilla]: es la tabla que contiene todas la rutas.
 * @param [file]: el el nombre del archivo.
 * @param [path]: es la ruta hacia el archvio.
 * | Se crea una variable (ruta) de tipo archivo* a la cual se le asigna el nombre de [file] junto con
 * | la dirección del [path], una vez que se tiene esta información en el struct de llama a la función 
 * | "annadir_nodo_hash" para que agregue la a (ruta) en [tablilla].
 * @return []: void. 
*/
void annadir_ruta(tabla_h* tablilla, char file[1000], const char* path) {
	archivo* ruta = (archivo*) calloc(1, sizeof(archivo));

	sprintf(ruta -> nombre, "%s", file);
	ruta -> direccion = path;

	annadir_nodo_hash(tablilla, ruta -> nombre, strlen(ruta -> nombre), ruta);	//Se añade el struct a la tabla hash como un nodo nuevo
}

/* SE USA EN: {Proyecto2.c -> MAIN(Switch: case4)}
 * Función para eliminar toda la tabla hash.
 * @param [nombreArchivo]: es el nombre de un documento.
 * @param [tabla]: es la tabla de hash en la que se encuentran almacenadas las rutas.
 * | Primero se obtienen los datos sobre el documento que están contenidos en la tabla de hash con eso
 * | simplemente se accesa al nombre del documento y la ruta en la que se encuentra dicho archivo
 * @return []: void. 
*/
void imprimirRutas(char* nombreArchivo, tabla_h* tabla) {
    archivo* imprimiendo = obtener_datos_hash(tabla, nombreArchivo, strlen(nombreArchivo));
    printf("%s - %s\n", imprimiendo -> nombre, imprimiendo -> direccion);
}