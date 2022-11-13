/* =============================================================================
 *  Asignacion:  Proyecto 2
 *
 *  Version:  1.0.0
 *  Fecha: 11 de noviembre de 2022
 *  Compilador:  gcc
 *
 * Autor:  Yosward García Tellez, Jorge Esteban Benavides Castro, Yurgen Cambronero Mora
 * Curso:  Estructuras de Datos
 * =============================================================================*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>
#include <sys/types.h>
#include <dirent.h>
#include "arbolB.c"
#include "arbolChaf.c"
#include "TablaH.c"


#define TAMANNO_MAXIMO 	1024*1024 // Longitud máxima de la tabla
#define SI			1
#define NO			0
#define ENVIA		1
#define RECIBE		0
#define GRADO_BTREE 5


/* 
 * Borra el salto de línea para hacer la lectura.
 * @param [void]: .
 * | Va a recorrer el input del usuario hasta al final y una vez ahí quita el "\n"
 * | para hacer la comparación con las palabras de los archivos
 * @return []: void.
*/
void consumirNuevaLinea(void){
    int c;
    do{
    	c = getchar();
    } while (c != EOF && c != '\n');
}


/*
 * Funcion para leer las palabras dentro de un archivo
 * @param [nombreArchivo]: El nombre del archivo del cual se leerá las palabras.
 * @param [arbol]: Arbol B donde se insertara las palabras del archivo.
 * @param [path]: Ruta en la que se encuentra el archivo.
 * @param [almacenar]: Lista donde se almacenaran las palabras con su respectivo archivo y ruta.
 * | Se lee palabra por palabra hasta que llegue al final del archivo.
 * | Cada vez que lea una palabra se crea una nueva llave y se inserta en el arbol B.
 * | Ademas, se inserta en la lista donde se guardaran las palabras con su respectiva informacion
 * @return []: void.
*/
void acceder_archivos(char* nombreArchivo, arbol_b* arbol, const char* path, listilla* almacenar, tabla_h* rutasH){

	FILE* archivo;
	archivo = fopen(nombreArchivo, "r");
	char palabraActual[1024];

	// Cada palabra tendra como maximo 1023 caracteres

	while(fscanf(archivo, "%1023s", palabraActual)==1){
		/*llave* nueva_llave = crear_llave(palabraActual, nombreArchivo);
		printf("\nvalor: %d\n", nueva_llave->valor);
		nodo_b* nodo_insertar = buscar_nodo_insertar(arbol->raiz, nueva_llave);*/

		annadir_ruta(rutasH, nombreArchivo, path);

		char guardando[3024] = {0};
		printf("%s\n", palabraActual);
		sprintf(guardando, "us`%s`%s`%s`", palabraActual, nombreArchivo, path);
		insertar_al_final(almacenar, guardando);

		/*int resp = insertar_llave(nodo_insertar, nueva_llave);
		if(resp == 1){
			reorganizarArbolB(nodo_insertar, arbol);
		} */

		//printf("valor: %d\n", arbol->raiz->llaves[0]->valor);
	}
	
	
	if (archivo != NULL){
		fclose(archivo);
	}
}


/*
 * Funcion para leer_archivos
 * @param [directorio]: Ruta donde se encuentran los archivos a ser leidos
 * @param [lista]: Lista donde se almacenaran las palabras con su respectivo archivo y ruta.
 * @param [arbol]: Arbol B donde se insertaran las palabras
 * | Se iterara por la carpeta obteniendo cada uno de los archivos dentro de la ruta.
 * | Si se encuentra un carpeta dentro de la carpeta principal, se llama a la funcion de manera 
 * | recursiva pero con la subcarpeta.
 * | Si lo que lee readdir() es un archivo, se lee el contenido de dicho archivo.
 * @return []: void.
*/
int leer_archivos(const char* directorio, listilla* lista, arbol_b* arbol, tabla_h* rutas) {
	DIR* filedirectory = opendir(directorio);
	if(filedirectory == NULL){
		printf("Directory no encontrado");
		return -1;
	}

	struct dirent* entity;
	entity = readdir(filedirectory);
	while(entity != NULL){
		if(entity->d_type == DT_DIR && strcmp(entity->d_name, ".") && strcmp(entity->d_name, "..")){
			char path[1000] = {0};
			strcat(path, directorio);
			strcat(path, "/");
			strcat(path, entity->d_name);
			leer_archivos(path, lista, arbol, rutas);
		}
		if (entity->d_type != DT_DIR && strcmp(entity->d_name, ".") && strcmp(entity->d_name, "..")){
			char path[1000] = {0};
			strcat(path, directorio);
			strcat(path, "/");
			strcat(path, entity->d_name);
			acceder_archivos(path, arbol, directorio, lista, rutas);
		}
		entity = readdir(filedirectory);
	}
	printf("\nraiz: %s\n", arbol->raiz->llaves[0]->palabra);
	closedir(filedirectory);
	return 0;
}


/*
 * Pasa un string al heap para compararlo.
 * @param []: es el input del usuario.
 * | Recibe un input del usuario tipo string, aparte el espacio de memoria
 * | y lo pasa al heap para realizar las comparaciones.
 * @retunr [string]: char*, el texto ya procesado.
*/
char* input_texto(){
	/* Declaracion de variables */
	char texto[1024]; 	//Genera espacio para texto
	fgets(texto, sizeof(texto), stdin); 	//Recibe el texto
	
	/* Traspaso al Heap */
	int tamaño_char = strlen(texto);
	char* string = calloc(tamaño_char, sizeof(char));
	strcpy(string, strtok(texto, "\n")); 		// Convierte el texto de pila a heap.
	return string;		//Devuelve el string ya convertido o pasado.
}


/*
 * Guarda un archivo(nombre, ruta, palabra) en una lista para guardarlo en el disco.
 * @param [guardados]: es la lista en que se va a guardar los documentos([archivo]).
 * @param [archivo]: es el docuemnto que se quiere guardar en la lista([guardados]).
 * | Revisa que la lista de archivos y el documento no estén nulos. 
 * | Crea un array de 0s en el que se va a guardar el nombre del [archivo] y escribe esa
 * | información el archivo, esto lo hace a lo largo de toda la lista. 
 * @return []: void.
*/
void guardar(listilla* guardados, FILE* archivo){
    if (guardados -> inicio == NULL){
        printf("No hay nada para guardar!\n");
    } else {
        nodito* act = guardados -> inicio;
        int lenLista = longitud_lista(guardados);
        int n = 0;

		if (archivo == NULL) {
			printf("Para guardar cree un archivo llamado index.txt en la ruta indicada!\n");
		} else {
			while (n != lenLista && act != NULL){
            	char dato[3024] = {0};             //pone 0 en las 3024 posiciones
				sprintf(dato, "%s",act -> nombre); //se guarda el "nombre" de "actual" en "dato".
            	fprintf(archivo, "%s\n", dato);    //mete la información de "dato" en el "archivo".
				act = act -> sigt;
            	n ++;
			}
        }
    }
}


/*
 * 
 * @param [guardado]: es el archivo al que se quiere acceder.
 * @param [tablaCargada]: es la tabala en la que se quiere agregar el archivo.
 * @param [arbolCargado]: Arbol B.
 * | Se define un valor ASCII poco comun (`) como un separador. Se recorre toda una linea
 * | del archivo hasta encontrar el separador, si todo lo que está antes del separador es: "us" indicaría
 * | que el archivo es nuestro, por lo que seguíriamos leyendolo hasta conseguir: la palabra con la que se 
 * | relaciona el achivo, el nombre del archivo, la ruta en la que se encuentra y una valor en base a "palabra".
 * @return []: int, retorna 0 sí todo funcionó bien.
*/
int cargar(FILE* guardado, tabla_h* tablaCargada, arbol_b* arbolCargado){
	char* ignore;    //identificador de  archivos
    char* palabrita; //palabra
    char* archivo;   //archivo
    char* path;      //ruta del archivo
    int valor;       // para el arbol B

    const char separador[2] = "`";  //ascii poco comun
    char linea[4000];				//pa'que entre todod
	while (fgets(linea, 4000, guardado)){  //recorre todo e; archivo
		ignore = strtok(linea, separador); //recoore toda la linea hasta encontrar "`"
		if (strcmp(ignore, "us") == 0){    //compara que ignore sea "us"  [Antes: strcmp(ignore, "us`")]
			continue;
		} else {
			return -1;
		}
        palabrita = strtok(NULL, separador); //desde done quedó hasta el separador
        archivo = strtok(NULL, separador); //desde donde quedó hasta el separador
        path = strtok(NULL, separador); //desde donde quedó nuevamente hasta el ultimo separador
        valor = funcion_hash(palabrita, strlen(palabrita)); //se saca el valor hash en base a la palabra

        //insertar b
    	annadir_ruta(tablaCargada, archivo, path);
	}
	return 0;
}


/*  =================================== */
/*================ M A I N ================*/
/*  =================================== */
int main(){
	listilla* listaGuardados = calloc(1, sizeof(listilla));
	int menu = 1;
	int opcion = 0;
	arbol_b* indiceTerminos = calloc(1, sizeof(arbol_b));
	indiceTerminos->grado = 5;
	nodo_b* root = crear_nodo_b();
	indiceTerminos->raiz = root;
	arbolitoc* cache = calloc(11, sizeof(arbolitoc));
	tabla_h* indiceRutas = crear_tabla();
	listilla* busqueda = calloc(1, sizeof(listilla));

	printf("\n\tBIENVENIDO AL BUSCADOR DE PALABRAS EN ARCHIVOS\n\n");

	int a1 = 1;
	int a2 = 1;
	

	while(menu) {
		printf("\no-------o-------o-------o-------o-------o-------o-------o\n");
		printf("\n\t\t MENU\n\n");
		printf("\t1. Crear índice\n\t2. Cargar índice\n\t3. Guardar índice\n\t4. Buscar\n\t5. Imprimir Arbol chaflaneados\n\t6. Salir\n");

		printf("\nSeleccione una opción a realizar: ");
		scanf("%d", &opcion);
		consumirNuevaLinea(); 	//Consume el \n para solucionar errores
		int a = 1;

		
		switch(opcion){
			//Carga el directorio con los archivos en los que se quiere buscar la palabra.
			case 1:
				if(a1 == 1){
					while (a1 == 1) {
						printf("Inserte la ruta donde se encuentran sus archivos: ");
						char* pathArchivos = input_texto();

						if (leer_archivos("./ayer_en_el_tec", listaGuardados, indiceTerminos, indiceRutas) == -1) {
							printf("Inserte una ruta válida!\n");
							continue;
						} else {
							printf("Se han creado los índices!\n");
							a1 = 0;
							break;
						}
					}
				} else{
					printf("\nEl índice ya fue creado\n");
				}
				break;

			//Carga el índice de palabras desde el disco a través de la ruta.	
			case 2:
				printf("Recordatorio: solo puedes cargar un índice del disco!\n");
				FILE* cargado;
				while (a2 == 1){
					printf("Inserte la ruta del archivo a cargar: ");
					char* pathCargar = input_texto();
					cargado = fopen(pathCargar, "r");
					if (cargado == NULL) {
						printf("Inserte un archivo válido!\n");
					} else {
						a2 = 0;
						break;
					}
				}
				int funciona = cargar(cargado, indiceRutas, indiceTerminos);
				if (funciona == -1) {
					printf("Inserte un archivo válido!\n");
					a2 = 1;
				} else {
					a2 = 0;
					printf("Indice cargado!\n");
				}
				break;

			//Va a guardar el indice en un ruta especificada en el disco.
			case 3:
				char* pathG;
				while(a == 1){
					printf("Ingrese a continuación la ruta donde desea guardar el indice en disco: ");
					pathG = input_texto();
					FILE* archivoAGuardar = fopen(("%s/indice.txt", pathG), "w");
					if (archivoAGuardar == NULL) {
						printf("La ruta no existe!\n");
					} else {
						guardar(listaGuardados, archivoAGuardar);
						printf("Se ha guardado con éxito el índice en disco en la ruta %s!\n", pathG);
						a = 0;
						break;
					}
				}
				break;
			
			//Solicita la palabra a buscar
			case 4:
				printf("Ingrese la palabra a buscar: ");
				char* palabra = input_texto();
				nodito* actual = listaGuardados -> inicio;
				while (actual != NULL) {
					char ignore[5];
					char archivo[1000];
					char palabraAct[1000];
					char separador[2] = "`";
					sprintf(ignore, "%s",strtok(actual -> nombre, separador));
					sprintf(palabraAct, "%s",strtok(NULL, separador));
					sprintf(archivo, "%s",strtok(NULL, separador));
					if(strcmp(palabra, palabraAct) == 0) {
						imprimirRutas(archivo, indiceRutas);
						actual -> sigt;
					} else {
						actual = actual -> sigt;
					}
				}
				/* noditoc* nodo = buscar_nodo_c(cache, palabra);
				int n;
				int lenRutas;
				if(nodo == NULL){
					int valor = funcion_hash(palabra, strlen(palabra));
					llave* encontrado = buscar_nodo_b(palabra, indiceTerminos->raiz, valor);
					if(encontrado != NULL){
						insertar_chaflaneado(cache, encontrado->palabra, encontrado->archivos);
						printf("Palabra: %s\n", cache->raiz->terminos);
						printf("Archivos: \n");
						imprimir_lista(cache->raiz->archivos);
						lenRutas = longitud_lista(encontrado -> archivos) ;
						n = 0;
						nodito* act = encontrado -> archivos -> inicio;
						while(n != lenRutas && act != NULL) {
							imprimirRutas(act -> nombre, indiceRutas);
							act = act -> sigt;
							n ++;
						}
					}
					else{
						printf("La palabra ingresada no está en ningún archivo\n");
					}
					
				}
				else{
					printf("Palabra: %s\n", nodo->terminos);
					printf("Archivos: \n");
					imprimir_lista(nodo->archivos);
					lenRutas = longitud_lista(nodo -> archivos);
					n = 0;
					nodito* act = nodo -> archivos -> inicio;
					while(n != lenRutas && act != NULL) {
						imprimirRutas(act -> nombre, indiceRutas);
						act = act -> sigt;
						n ++;
					}
				} */
				printf("Hola\n");
				break;

			//Imprime la memoria caché en preorden.
			case 5:
				if(cache->raiz != NULL){
					imprimirPreorden(cache->raiz);
				}
				else{
					printf("\nEl árbol chaflaneado está vacío\n");
				}
				break;

			//Salir
			case 6:
				printf("\nGracias por usar nuestro buscador\n");
				menu = 0;
				break;
			
			//Por sí se pasan de graciosos
			default:
				printf("\nIngrese una opción válida\n");
				break;
		}

	}
	
	return 0;
}