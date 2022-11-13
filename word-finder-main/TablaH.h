#ifndef TablaH_h
#define TablaH_h

#define TAMANNO_MAXIMO 	1024*1024		// Longitud máxima de la tabla
#define SI			1
#define NO			0
#define ENVIA		1
#define RECIBE		0

/*Struct para los valores de la tabla Hash*/
typedef struct nodo_hash
{
	struct nodo_hash* sigt;	// Puntero a siguiente
	char* key;				//Palabra clave o hash
	void* valor;			//valor
	char ocupado;	// ¿Está ocupado?
}nodo_h;

/*Struct para la tabla hash*/
typedef struct tabla_hash
{
	nodo_h** table;
}tabla_h;

/*Struct para guardar la ruta de los documentos*/
typedef struct ruta_archivo
{
    char nombre[1000];
    const char* direccion;
}archivo;

/* SE USA EN: {TablaH.c -> annadir_nodo_hash, obtener_datos_hash}
 * Función para devolver valor hash en base a corrimiento de bits
 * @pam [key]: palabra.
 * @pam [key_len]: longitud de la palabra.
 * | Primero se agarra la primera letra de [key] en ASCII
 * | Se hace un corrimiento de bits de 5 unidades hacia la izquierda de la varible "hash"
 * | Se le suma el valor ASCII de la letra y se suma nuevamente "hash" con un corrimiento de 2 unidades hacia la derecha
 * | se realiza un XOR y este proceso se repite para cada letra de la palabra.  
 * @return [has]: unsigned int, .
*/gned int valor_hash(char* key, unsigned int key_len);


/* SE USA EN: {Proyecto2.c -> Main}
 * Función para crear una tabla hash
 * | Pide la memoria para crear una tabla que va a contener
 * | 1024*1024 "nodos" dentro de sí. Si no consigue la memoria necesaria
 * | para todos se libera la memoria ocupada.
 * @return [tablilla]: tabla_h*, la tabla vacía si consiguió la memoria suficiente. (o NULL)
*/
tabla_h* crear_tabla(void);


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
int annadir_nodo_hash(tabla_h* tablilla, char* key, unsigned int key_len, void* valor);


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
void* obtener_datos_hash(tabla_h* tablilla, char* key, unsigned int key_len);


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
void annadir_ruta(tabla_h* tablilla, char* file, const char* path);


/* SE USA EN: {Proyecto2.c -> MAIN(Switch: case4)}
 * Función para eliminar toda la tabla hash.
 * @param [nombreArchivo]: es el nombre de un documento.
 * @param [tabla]: es la tabla de hash en la que se encuentran almacenadas las rutas.
 * | Primero se obtienen los datos sobre el documento que están contenidos en la tabla de hash con eso
 * | simplemente se accesa al nombre del documento y la ruta en la que se encuentra dicho archivo
 * @return []: void. 
*/
void imprimirRutas(char* nombreArchivo, tabla_h* tabla);


#endif