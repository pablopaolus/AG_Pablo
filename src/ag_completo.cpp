#include <boost/property_tree/xml_parser.hpp>
#include <boost/property_tree/ptree.hpp>
#include <iostream>
#include <stdio.h>
#include <stdlib.h> 
#include <cstdio>
#include <cstdlib>
#include <time.h> 
#include <ctime>
#include <sys/time.h>
#include <sys/resource.h>
#include <vector>


using namespace std;

/* ********************************************************************************** */

/* Función para inicializar la población con nP individuos (cromosomas) con nG (1s y 0s aleatorios) */
void inicializar_poblacion(int nP, int nG, vector< vector<char> > &v) {

	// Vector de vectores con la población. Cada vector es un cromosoma con nGenes
	vector< vector<char> >::iterator itPob;
	vector<char>::iterator itCrom;
	
	// Inicialización aleatoria de cada vector cromosoma (habrá nP vectores) con 0 o 1
	//printf("\nPOBLACIÓN INICIAL:\n");
	for(itPob = v.begin(); itPob != v.end(); ++itPob) {
		for(itCrom = (*itPob).begin(); itCrom != (*itPob).end(); ++itCrom) {
			*itCrom = rand() % 2;
			//printf("%d ", *itCrom);
		}
		//printf("\n");
	}
}

/* ********************************************************************************** */

/* Funcion usada por qsort para ordenar por fitness a los individuos */
int compare(const void *ind1, const void *ind2) {

	return ( ((pair<int,int>*)ind2)->second - ((pair<int,int>*)ind1)->second );
}

/* ********************************************************************************** */

/* Función para calcular el número de 1s de cada individuo de la población */
vector< pair<int,int> > calculo_fitness(vector< vector<char> > &v) {

	int nP = (int) v.capacity(); // Tamaño de la población (nº cromosomas)
	int nG = (int) v[0].capacity(); // Nº de genes de cada cromosoma
	vector< pair<int,int> > ones(nP); // Vector para almacenar indice de individuo y su fitness (nº de unos)
	long ones_count = 0; // Variable para almacenar el número de unos contados en cada cromosoma

	//printf("\nFITNESS:\n");
	for(int i = 0; i < nP; i++) {
		ones_count = 0;
		for(int j = 0; j < nG; j++) {
			if(v[i][j] == 1)
				ones_count++;
		}
	ones[i] = make_pair(i, ones_count);
	//printf("Cromosoma %d --> Fitness: %d\n", ones[i].first, ones[i].second);
	}

	// Ordenamos el vector fitness según el número de ones (de mayor a menor) para ver qué cromosomas tienen mejor fitness
	qsort(&ones[0], nP, sizeof(pair<int,int>), compare);
	//printf("\nVector de fitness ordenado:\n");
	//for(int i = 0; i < nP; i++) 
	//	printf("Cromosoma %d --> Fitness: %d\n", ones[i].first, ones[i].second);
	
	return ones;	
}

/* ********************************************************************************** */

/* Función para calcular la media del fitness de todos los individuos */
double media_fitness(vector< pair<int,int> > &fitness) {

	long int total_ones = 0; // Variable para almacenar el número total de unos de todos los cromosomas de la población
	double media = 0; // Variable para almacenar la media del fitness
	int nP = fitness.size(); // Tamaño de la población (número de parejas en fitness)

	for(int i = 0; i < nP; i++)
		total_ones += fitness[i].second;
	media = ((double) total_ones / (double) nP);
	//printf("Fitness total: %ld\nAverage Fitness: %f\n", total_ones, media);

	return media;
}

/* ********************************************************************************** */

/* Función para calcular el fitness total */
double total_fitness(vector< pair<int,int> > &fitness) {

	long int total_ones = 0; // Variable para almacenar el número total de unos de todos los cromosomas de la población
	int nP = fitness.size(); // Tamaño de la población (número de parejas en fitness)

	for(int i = 0; i < nP; i++)
		total_ones += fitness[i].second;

	return total_ones;
}

/* ********************************************************************************** */

/* Función que devuelve el vector ruleta para la selección (rueda de ruleta) */
vector<int> seleccion_ruleta(int nP, double media_fitness, vector< pair<int,int> > &fitness) {

	// Vector ruleta donde almacenamos los índices de los cromosomas en proporción a su fitness (más copias cuanto mayor sea su fitness)
	vector<int> ruleta(nP);	
	int copias=0;
	int indice_ruleta=0;

	//printf("\nSELECCIÓN:\n");
	for(int i = 0; i < nP; i++) {
		copias=	((double) fitness[i].second / (double) media_fitness);	
		//printf("Copias de cromosoma %d = %d\n", fitness[i].first, copias); 
		for(int j = indice_ruleta; j < copias + indice_ruleta; j++) {
			ruleta[j] = fitness[i].first;
			//printf("ruleta[%d] = %d\n", j, ruleta[j]); 
		}
		indice_ruleta += copias;
		//printf("indice_ruleta: %d\n", indice_ruleta); 
	}

	// Si el número de copias no llega a nP, rellenamos lo restante con una copia extra de cada cromosoma
	// respetando la prioridad según su fitness. 
	if(indice_ruleta < nP) {
		int indice_fitness=0;
		for(int i = indice_ruleta; i < nP; i++) {
			ruleta[i] = fitness[indice_fitness].first;
			indice_fitness++;
		}
	}

	/*
	// Información
	printf("Indices de cromosomas en la ruleta: ");
	for(int i = 0; i < nP; ++i)
		printf("%d ", ruleta[i]);
	printf("\n");
	*/

	return ruleta;
}

/* ********************************************************************************** */

/* Función para realizar la mutación */
void mutacion(vector< vector<char> > &v) {

	/* Alteración aleatoria (normalmente con probabilidad pequeña) de cada gen componente del cromosoma 
	de forma que obtengamos un individuo mutado. */
	bool mutar;
	vector< vector<char> >::iterator itPob;
	vector<char>::iterator itCrom;

	for(itPob = v.begin(); itPob != v.end(); ++itPob) {
		for(itCrom = (*itPob).begin(); itCrom != (*itPob).end(); ++itCrom) {
			mutar = (rand() % 100) < 10;
			if(mutar==true) 
				*itCrom=~*itCrom+2;
		}
	}	
}

/* ********************************************************************************** */

/* Función para realizar el crossover */
vector< vector<char> > crossover(vector< vector<char> > &padres, vector<int> &ruleta) {

	// El operador de cruce coge dos padres e intercambia los genes comprendidos entre dos puntos de cruce escogidos al azar, 
	// obteniendo por tanto dos individuos hijos

	int nP = (int) padres.capacity(); // Tamaño de la población (nº cromosomas)
	int nG = (int) padres[0].capacity(); // Nº de genes de cada cromosoma

	// Dado que hay que coger parejas aleatorias de cromosomas para cruzarlas, el número de cruces será igual al tamaño 
	// de la población dividido entre 2, lo cual podemos calcular simplemente desplazando un bit a la derecha:
	int nCruces = nP >> 1; // Número de cruces a realizar
	//printf("\nCROSSOVER:\nNúmero de cruces: %d\n", nCruces);
	vector< vector<char> > hijos(nP); // Creamos un vector de hijos (vector de vectores) de tamaño nP

	// Bucle para realizar los nCruces
	for(int i = 0; i < nCruces; ++i) {
		//printf("\nCRUCE Nº %d\n",i+1);
		// Elijo aleatoriamente dos individuos de la poblacion. Sus índices dentro del vector de vectores poblacion son crom1 y crom2
		int crom1 = rand() % nP;
		int crom2 = rand() % nP;
		// Elijo aleatoriamente los puntos de corte para el cruce de cadenas entre los dos vectores con índices crom1 y crom2
		int p1 = rand() % nG;
		int p2 = rand() % nG;
		// Los puntos anteriores pueden no haber salido ordenados. Necesitamos pc1 (punto de cruce 1) < pc2 (punto de cruce 2)
		int pc1 = (p1<p2) ? p1 : p2;
		int pc2 = (p1>p2) ? p1 : p2;
		//printf("Puntos de cruce: pc1=%d y pc2=%d\n", pc1, pc2);
		// Copiamos dos cromosomas padre (los que haya en el vector ruleta en los índices crom1 y crom2) en sendos cromosomas hijos.
		// Dado que en el vector ruleta hay más índices de los cromosomas con más fitness, habra más probabilidad 
		// de que el índice seleccionado pertenezca a cromosomas con más fitness. 
		hijos[i<<1] = padres[ruleta[crom1]];
		hijos[(i<<1)+1] = padres[ruleta[crom2]];
		//printf("Índices de los cromosomas que se cruzarán: %d y %d\n", ruleta[crom1], ruleta[crom2]);
		// Realizamos el crossover basado en dos puntos
		for(int k=pc1; k<=pc2; k++) {
			hijos[(i<<1)][k] = padres[ruleta[crom2]][k];
       			hijos[(i<<1)+1][k] = padres[ruleta[crom1]][k];
    		}
	}
		
	return hijos;
}

/* ********************************************************************************** */

/* Función para imprimir los valores de un vector de vectores de char */
void print_vector(vector< vector<char> > &v) {

	vector< vector<char> >::iterator itPob;
	vector<char>::iterator itCrom;

	for(itPob = v.begin(); itPob != v.end(); ++itPob) {
		for(itCrom = (*itPob).begin(); itCrom != (*itPob).end(); ++itCrom) {
			printf("%d ", *itCrom);
		}
		printf("\n");
	}

}

/* ********************************************************************************** */

int main(int argc, char *argv[]) {

	// Creación de un objeto árbol para el xml
	using boost::property_tree::ptree;
	ptree ptin, ptout;

	// Lectura de los parámetros de entrada desde el fichero de configuración
	read_xml("config.xml", ptin);
	int nPob = ptin.get<int>("config.nPob");
	int nGenes = ptin.get<int>("config.nGenes");
	int nGeneraciones = ptin.get<int>("config.nGeneraciones");

	// Datos de salida
	ptout.put("resultados.nPob", nPob);
	ptout.put("resultados.nGenes", nGenes);
	ptout.put("resultados.nGeneraciones", nGeneraciones);

	struct timeval timeMeas;
	
	vector< vector<char> > poblacion(nPob, vector<char>(nGenes));
	vector< pair<int,int> > fitness(nPob); // Vector para almacenar indice de individuo y su fitness (nº de unos)
	long int fitness_total = 0; // Variable para almacenar el fitness total de cada generación
	double average_fitness = 0; // Variable para almacenar la media de los fitness de la población de cada generación
	vector<int> ruleta(nPob); // Vector ruleta donde almacenamos los índices de los cromosomas en proporción a su fitness
	vector< vector<char> > hijos(nPob); // Creamos un vector de hijos (vector de vectores) de tamaño nPob

	gettimeofday(&timeMeas, NULL);
	double tstart = timeMeas.tv_sec+(timeMeas.tv_usec/1000000.0);

	/*** POBLACIÓN INICIAL***/
	srand (time(NULL)); // initialize random seed
	inicializar_poblacion(nPob, nGenes, poblacion);

	// El algoritmo genético finaliza cuando se generan nGeneraciones
	for(int i = 0; i < nGeneraciones; ++i) {

		char NGeneracion[100];
		sprintf(NGeneracion, "resultados.generacion%d", i); // Dato de salida: número de generación

		/*** FITNESS ***/
		fitness = calculo_fitness(poblacion);
		fitness_total = total_fitness(fitness);
		average_fitness = ((double) fitness_total / (double) nPob);

		char aux[100];
		sprintf(aux, "%s.fitnessTotal", NGeneracion);
		ptout.put(aux, fitness_total);
		sprintf(aux, "%s.fitnessMedia", NGeneracion);
		ptout.put(aux, average_fitness); // Dato de salida: media del fitness
		sprintf(aux, "%s.fitnessMejor", NGeneracion);
		ptout.put(aux, fitness[0].second); // Dato de salida: mejor fitness (el vector fitness está ordenado)
		
		/*** SELECCIÓN: RUEDA DE RULETA ***/
		// Se crea un pool genético formado por cromosomas de la generación actual, en una cantidad proporcional a su fitness.
		ruleta = seleccion_ruleta(nPob, average_fitness, fitness);

		/*** MUTACIÓN ***/
		mutacion(poblacion);

		/*** CROSSOVER ***/
		hijos = crossover(poblacion, ruleta);

		/*** INSERCIÓN ***/
		// Si nPob es impar, el nº de hijos generados es uno menos, ya que el nº de cruces se ha calculado como 
		// la parte entera de la división entre 2 de nPob.Para completar la población, añadimos al cromosoma padre con mas fitness 
		if(nPob % 2 == 1)
			hijos[nPob-1] = poblacion[fitness[0].first]; // El vector fitness lo tenemos ya ordenado
		// Se insertan los nuevos individuos en la población, sustituyendo la población anterior
		swap(hijos, poblacion);
	}

	gettimeofday(&timeMeas, NULL);
	double tend = timeMeas.tv_sec+(timeMeas.tv_usec/1000000.0);
	ptout.put("resultados.tiempo", tend - tstart);

	time_t tSac = time(NULL); // instante actual
	struct tm *tmP = localtime(&tSac);

	char nombre[100];
	sprintf(nombre, "results/agPablo-%d-%d-%d-%d:%d:%d", nPob, nGenes, nGeneraciones, tmP->tm_hour, tmP->tm_min, tmP->tm_sec);
	write_xml(nombre, ptout);

	return 0;

}
