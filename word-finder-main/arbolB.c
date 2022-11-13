#include <stdio.h>
#include<stdlib.h>
#include <string.h>
#include <math.h>

#include "arbolB.h"
#include "arbolChaf.h"

/*
 * Funcion crear nodo de un arbol B
 * | Se pide memoria para el nuevo nodo, se rellenan los diferentes campos
 * | del struct y se pide memoria para el arreglo de llavez y ramas.
 * @return [nn]: nodo_b*, devuelve el nodo ya creado.
*/
nodo_b* crear_nodo_b(){
	nodo_b* nn = calloc(1, sizeof(nodo_b));
	nn->min_llaves = 2;
	nn->ocupado = 0;
	nn->llaves = calloc(5, sizeof(llave*));
	nn->ramas = calloc(5, sizeof(nodo_b*));
	return nn;
}


/*
 * Funcion para hallar la sumas de los valores ASCII de todos los caracteres
 * @param [llave]: sera la palabra a convertir
 * @param [llave_tammanno]: longitud de la palabra
 * @return [suma]: int, la suma de todos los valores ASCII
*/
int funcion_hash(char* llave, int llave_tamanno){
    int suma = 0;
    for(int i = 0; i<llave_tamanno; i++){
        suma += llave[i];
    }
    return suma;
}


/*
 *Funcion que crea un "struct" que contiene una palabra y la lista de archivos en los que aparece.
 *@param [palabra]: es una palabra leída de un archivo.
 *@param [archivo]: es el archivo en el cual se leyó la palabra.
 *@return [nueva_llave]: llave*, con la [palabra] y la lista de archivos(nueva_llave->archivos) con [archivo] al final.
*/
llave* crear_llave(char* palabra, char* archivo){

	llave* nueva_llave = calloc(1, sizeof(llave));         //pide memoria

	sprintf(nueva_llave -> palabra, "%s",palabra);         //guarda la palabra en el nodo
	//nueva_llave->palabra = pal;
	int llaveTamanno = strlen(nueva_llave -> palabra);     //consigue el tamanho de la llave en base a la long. de la palabra

	nueva_llave -> archivos = calloc(1, sizeof(listilla)); //pide la memoria para la lista de archivos en los que está la palabra 
	insertar_al_final(nueva_llave -> archivos, archivo);   //agrega el nuevo archivo a la lista de archivos existente

	nueva_llave->valor = funcion_hash(nueva_llave->palabra, llaveTamanno);  //consigue el valor ascii de la palabra
	return nueva_llave;
}


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
int insertar_llave(nodo_b* nodoDondeInsertar, llave* llaveAInsertar){
	//printf("\nword: %s\n", llaveAInsertar->palabra);
	int cantidad_llaves = nodoDondeInsertar->ocupado;
	int flag = 0;
	int meter = 1;
	if (nodoDondeInsertar -> ocupado > 0){
			for(int i = 0; i < cantidad_llaves; i++){
			if(!strcmp(nodoDondeInsertar->llaves[i]->palabra,llaveAInsertar->palabra)){
				nodito* act = nodoDondeInsertar->llaves[i]->archivos->inicio;
				while(act != NULL){
					if(!strcmp(act->nombre, llaveAInsertar->archivos->inicio->nombre)){
						act = act->sigt;
						flag = 0;
					}
					else{
						act = act->sigt;
						flag = 1;
						
					}
				}
				if(flag == 1){
					insertar_al_final(nodoDondeInsertar->llaves[i]->archivos, llaveAInsertar->archivos->inicio->nombre);
				}
				meter = 0;
				break;
			}
			meter = 1;
		}
		if(meter == 1){
			printf("\nEntrooo\n");
			printf("\nse metio: %s", llaveAInsertar->palabra);
			nodoDondeInsertar->llaves[cantidad_llaves] = llaveAInsertar;
			nodoDondeInsertar->ocupado++;

			cantidad_llaves = nodoDondeInsertar->ocupado;
		}	
	} else {
		nodoDondeInsertar -> llaves[0] = llaveAInsertar;
		nodoDondeInsertar -> ocupado ++;
		cantidad_llaves = nodoDondeInsertar -> ocupado;
	}

	llave* temp;

	// Para ordenar el nodo
	for(int i = cantidad_llaves-1; i >= 1; i--){
		if(nodoDondeInsertar->llaves[i]->valor < nodoDondeInsertar->llaves[i-1]->valor){
			temp = nodoDondeInsertar->llaves[i-1];
			nodoDondeInsertar->llaves[i-1] = nodoDondeInsertar->llaves[i];
			nodoDondeInsertar->llaves[i] = temp;
			temp = NULL;
		}
	}
	if(nodoDondeInsertar->ocupado >= GRADO_BTREE){
		return 1; // Para validar si se necesita hacer un reacomodo del árbol
	}
	return 0;
}


/*
 * Funcion para buscar el nodo donde se va a insertar
 * @param [nodo_actual]: sera el nodo actual por el cual se este recorriendo
 * @param [llaveInsertar]: es la llave que se buscara insertara
 * | Se recorre el nodo actual y se baja hacia las ramas segun sea el valor
 * | de la llave a insertar. Cada vez que se baje por una rama se hara 
 * | una llamada recursiva de la funcion.
 * @return [nodo_actual]: nodo_b*, nodo encontrado
*/
nodo_b* buscar_nodo_insertar(nodo_b* nodo_actual,llave* llaveInsertar){

	if(nodo_actual->ocupado == 0){
		return nodo_actual;
	}

	if(nodo_actual->llaves[nodo_actual->ocupado - 1]->valor < llaveInsertar->valor && nodo_actual->ocupado == 4){
		if(nodo_actual->ramas[nodo_actual->ocupado] != NULL){
			return buscar_nodo_insertar(nodo_actual->ramas[nodo_actual->ocupado], llaveInsertar);
		}
		return nodo_actual;
		
	}

	for(int i = 0; i <= nodo_actual->ocupado-1; i++){
		if(nodo_actual->llaves[i]->valor > llaveInsertar->valor){
			if(nodo_actual->ramas[i] != NULL){
				return buscar_nodo_insertar(nodo_actual->ramas[i], llaveInsertar);
			}
			return nodo_actual;	
		}
	}

	if(nodo_actual->llaves[nodo_actual->ocupado-1]->valor <= llaveInsertar->valor){
		if(nodo_actual->ramas[nodo_actual->ocupado] != NULL){
			return buscar_nodo_insertar(nodo_actual->ramas[nodo_actual->ocupado],llaveInsertar);
		}
		return nodo_actual;
	}
}


/*
 * Funcion para buscar un nodo en el arbol B
 * @param [palabra]: sera el termino que se buscara en el arbol
 * @param [raiz]: representara el nodo raiz del arbol
 * | En base al valor ASCII de la palabra a buscar, se recorre
 * | el arbol bajando por las ramas hasta encontrar el nodo con la 
 * | llave correspondiente.
 * @return [pal]: llave*, devuelve la llave de la palabra buscada. (o NULL)
*/
llave* buscar_nodo_b(char* palabra, nodo_b* raiz, int valor){
	
	nodo_b* act = raiz;
	int val_palabra = valor;
	if (act -> llaves[0] != NULL && val_palabra < act -> llaves[0] -> valor) {
		return buscar_nodo_b(palabra, raiz -> ramas[0], val_palabra);
	} else if ( act -> llaves[3] != NULL && val_palabra >= act -> llaves[0] -> valor && val_palabra <= act -> llaves[3] -> valor) {
		llave* pal = act -> llaves[0];
		int i;
		for (i = 0; val_palabra != pal -> valor || i < 3; i++){
			pal = act -> llaves[i];
			if (val_palabra != pal -> valor && val_palabra < act -> llaves[i+1]->valor && val_palabra > act -> llaves[i]->valor && act -> ramas != NULL){
				return buscar_nodo_b(palabra, raiz -> ramas[i+1], val_palabra);
				break;
			} else {
				continue;
			}
		}
		if (pal -> valor == val_palabra && strcmp(pal -> palabra, palabra) == 0) {
			return pal;
		} else if (pal -> valor == val_palabra && act -> llaves[i+1] != NULL && act -> llaves[i+1] -> valor == val_palabra && strcmp(palabra, act -> llaves[i+1] -> palabra) == 0) {
			return act -> llaves[i+1];
		} else if (pal -> valor == val_palabra && act -> llaves[i+2] != NULL && act -> llaves[i+2] -> valor == val_palabra && strcmp(palabra, act -> llaves[i+2] -> palabra) == 0) {
			return act -> llaves[i+2];
		} else if (pal -> valor == val_palabra && act -> llaves[i+3] != NULL && act -> llaves[i+3] -> valor == val_palabra && strcmp(palabra, act -> llaves[i+3] -> palabra) == 0) {
			return act -> llaves[i+3];
		} else {
			return NULL;
		}
	} else if (act -> llaves[3] != NULL && val_palabra > act -> llaves[0] -> valor && val_palabra > act -> llaves[3] -> valor) {
		return buscar_nodo_b(palabra, raiz -> ramas[raiz -> ocupado],val_palabra);
	} else if (act -> llaves[0] != NULL && val_palabra < act -> llaves[0] -> valor) {
		return buscar_nodo_b(palabra, raiz -> ramas[0], val_palabra);
	} else {
		return NULL;
	} 
}


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
void reorganizarArbolB(nodo_b* nodoArreglar, arbol_b* arbol){

	nodo_b* papa = nodoArreglar -> papa;
	int posMedio = ceil(nodoArreglar->ocupado / 2);
	printf("%d", posMedio);
	llave* medio = nodoArreglar->llaves[posMedio];
	int sennal = 0;

	if(papa != NULL){
		int resp = insertar_llave(papa, medio);

		nodo_b* nn = crear_nodo_b();
		nn->papa = papa;
		insertar_llave(nn,nodoArreglar->llaves[3]);
		insertar_llave(nn,nodoArreglar->llaves[4]);
		papa->ramas[papa->ocupado] = nn;
		nodoArreglar->ocupado = nodoArreglar->ocupado - 3;
		nodoArreglar->llaves[2] = NULL;
		nodoArreglar->llaves[3] = NULL;
		nodoArreglar->llaves[4] = NULL;
		for(int i = 0; i < 6; i++){
			if(nodoArreglar->ramas[i] != NULL){
				sennal = 1;
			}
			else{
				sennal = 0;
			}
		}
		if(sennal){
			if (nodoArreglar -> ramas[3] && nodoArreglar -> ramas[4] && nodoArreglar -> ramas[5]){
				nn->ramas[0] = nodoArreglar->ramas[3];
				nn->ramas[1] = nodoArreglar->ramas[4];
				nn->ramas[2] = nodoArreglar->ramas[5];
				nn->ramas[0]->papa = nn;
				nn->ramas[1]->papa = nn;
				nn->ramas[2]->papa = nn;
				nodoArreglar->ramas[3] = NULL;
				nodoArreglar->ramas[4] = NULL;
				nodoArreglar->ramas[5] = NULL;
			} else if (nodoArreglar -> ramas[3] && !nodoArreglar -> ramas[4] && !nodoArreglar -> ramas[5]){
				nodoArreglar->ramas[3]->papa = nn;
				nn->ramas[0] = nodoArreglar->ramas[3];
				nn->ramas[1] = nodoArreglar->ramas[4];
				nn->ramas[2] = nodoArreglar->ramas[5];
				nodoArreglar->ramas[3] = NULL;
				nodoArreglar->ramas[4] = NULL;
				nodoArreglar->ramas[5] = NULL;
			} else if (nodoArreglar -> ramas[3] && nodoArreglar -> ramas[4] && !nodoArreglar -> ramas[5]){
				nodoArreglar->ramas[3]->papa = nn;
				nodoArreglar->ramas[4]->papa = nn;
				nn->ramas[0] = nodoArreglar->ramas[3];
				nn->ramas[1] = nodoArreglar->ramas[4];
				nn->ramas[2] = nodoArreglar->ramas[5];
				nodoArreglar->ramas[3] = NULL;
				nodoArreglar->ramas[4] = NULL;
				nodoArreglar->ramas[5] = NULL;
			} else {
				
			}
		}
		if(resp == 1){
			reorganizarArbolB(papa, arbol);
		}
	} else{
		nodo_b* raiz = crear_nodo_b();
		nodo_b* nn = crear_nodo_b();
		nn->papa = raiz;
		arbol->raiz = raiz;
		nodoArreglar->papa = raiz;
		int resp = insertar_llave(nn,nodoArreglar->llaves[3]);
		resp = insertar_llave(nn,nodoArreglar->llaves[4]);
		resp = insertar_llave(raiz, medio);
		raiz->ramas[raiz->ocupado] = nn;
		nodoArreglar->ocupado = nodoArreglar->ocupado - 3;
		nodoArreglar->llaves[2] = NULL;
		nodoArreglar->llaves[3] = NULL;
		nodoArreglar->llaves[4] = NULL;
		for(int i = 0; i < 6; i++){
			if(nodoArreglar->ramas[i] != NULL){
				sennal = 1;
			}
			else{
				sennal = 0;
			}
		}
		if(sennal){
			if (nodoArreglar -> ramas[3] && nodoArreglar -> ramas[4] && nodoArreglar -> ramas[5]){
				nn->ramas[0] = nodoArreglar->ramas[3];
				nn->ramas[1] = nodoArreglar->ramas[4];
				nn->ramas[2] = nodoArreglar->ramas[5];
				nn->ramas[0]->papa = nn;
				nn->ramas[1]->papa = nn;
				nn->ramas[2]->papa = nn;
				nodoArreglar->ramas[3] = NULL;
				nodoArreglar->ramas[4] = NULL;
				nodoArreglar->ramas[5] = NULL;
			} else if (nodoArreglar -> ramas[3] && !nodoArreglar -> ramas[4] && !nodoArreglar -> ramas[5]){
				nodoArreglar->ramas[3]->papa = nn;
				nn->ramas[0] = nodoArreglar->ramas[3];
				nn->ramas[1] = nodoArreglar->ramas[4];
				nn->ramas[2] = nodoArreglar->ramas[5];
				nodoArreglar->ramas[3] = NULL;
				nodoArreglar->ramas[4] = NULL;
				nodoArreglar->ramas[5] = NULL;
			} else if (nodoArreglar -> ramas[3] && nodoArreglar -> ramas[4] && !nodoArreglar -> ramas[5]){
				nodoArreglar->ramas[3]->papa = nn;
				nodoArreglar->ramas[4]->papa = nn;
				nn->ramas[0] = nodoArreglar->ramas[3];
				nn->ramas[1] = nodoArreglar->ramas[4];
				nn->ramas[2] = nodoArreglar->ramas[5];
				nodoArreglar->ramas[3] = NULL;
				nodoArreglar->ramas[4] = NULL;
				nodoArreglar->ramas[5] = NULL;
			} else {

			}
		}
		


	}
}
