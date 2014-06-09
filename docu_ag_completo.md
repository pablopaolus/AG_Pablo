Documentación del trabajo final: Algoritmo Genético completo
============================================================

He creado un nuevo repositorio [AG_Pablo] (https://github.com/pablopaolus/AG_Pablo.git) para albergar el trabajo final de la asignatura. 
El código del algoritmo genético completo en sí se encuentra en la carpeta `src` y se denomina [ag_completo.cpp] (https://github.com/pablopaolus/AG_Pablo/blob/master/src/ag_completo.cpp). 

Recordemos que a lo largo de las anteriores sesiones de trabajo decidí seguir adelante utilizando la clase `<vector>`, debido a la disponibilidad de funciones en esta clase que, además de facilitarnos la vida, ahorrarán mucho tiempo de ejecución. Los cambios "grosso modo" del nuevo código en [ag_completo.cpp] (https://github.com/pablopaolus/AG_Pablo/blob/master/src/ag_completo.cpp) con respecto al último ejercicio de la sesión anterior ([seleccion.cpp] (https://github.com/pablopaolus/repositorio_pablopaolus/blob/master/seleccion.cpp)) son los siguientes:

#####1. Modularización del código 

Se han modularizado todas las funciones del algoritmo (cálculo de fitness, selección basada en rueda de ruleta, crossover, mutación...) de manera que ahora el `main` ha quedado mucho más limpio, organizado y corto. En concreto, las funciones implementadas han sido las siguientes: 

```cpp
/* Función para inicializar la población con nP individuos (cromosomas) con nG (1s y 0s aleatorios) */
void inicializar_poblacion(int nP, int nG, vector< vector<char> > &v)
```

```cpp
/* Función para calcular el número de 1s de cada individuo de la población */
vector< pair<int,int> > calculo_fitness(vector< vector<char> > &v)
```

```cpp
/* Función para calcular el fitness total */
double total_fitness(vector< pair<int,int> > &fitness)
```

```cpp
/* Función que devuelve el vector ruleta para la selección (rueda de ruleta) */
vector<int> seleccion_ruleta(int nP, double media_fitness, vector< pair<int,int> > &fitness)
```

```cpp
/* Función para realizar la mutación */
void mutacion(vector< vector<char> > &v)
```

```cpp
/* Función para realizar el crossover */
vector< vector<char> > crossover(vector< vector<char> > &padres, vector<int> &ruleta)
```

#####2. Implementación del algoritmo genético completo 

Se ha implementado ya un algoritmo genético completo (anteriormente solo se hacía una vez, como si solamente hubiera une generación). Para ello, tras inicializar la población de forma aleatoria, se ha implementado un bucle `for` en el que se realiza la evaluación, selección para reproducción y generación de una nueva población. La condición de terminación del bucle es la ejecución de un determinado número de generaciones. 

#####3. Parámetros de entrada

Ahora, los parámetros de entrada son tres en lugar de dos: 

I. `nPob`: Tamaño de la población, es decir, número de individuos o cromosomas.
II. `nGenes`: Número de genes que contiene cada cromosoma.
III. `nGeneraciones`: número de generaciones (determina la condición de terminación del bucle).

La población está representada por un vector de `nPob` vectores, donde cada uno de ellos es un cromosoma que contiene `nGenes` valores aleatorios 0 o 1.

```cpp
vector< vector<char> > poblacion(nPob, vector<char>(nGenes)); 
```

#####4. Ejecución mediante fichero de configuración

Los parámetros de entrada anteriores ya no se introducen por línea de comandos como hacíamos antes, sino que se hace uso de un fichero de configuración ([config.xml](https://github.com/pablopaolus/AG_Pablo/blob/master/config.xml)) como, por ejemplo:

```xml
<?xml version="1.0"?>
<!DOCTYPE config PUBLIC "" "config.dtd">

<config>
<nPob>128</nPob>
<nGenes>256</nGenes>
<nGeneraciones>10</nGeneraciones>
</config>
```

#####5. Emisión de los resultados

Antes, los resultados se mostraban por pantalla y no se almacenaban en ningún fichero. El usuario tenía que copiarlos manualmente si deseaba guardarlos para cada ejecución. Ahora, los resultados se emiten en un formato estructurado XML que almacena los valores de:

1. Parámetros de entrada:
	1. Tamaño de la población `nPob`
	2. Número de genes `nGenes`
	3. Número de generaciones `nGeneraciones`
2. Para cada generación:
	1. Número de la generación
	2. Fitness total
	3. Media del fitness 
	4. Mejor fitness de la generación
3. Tiempo de ejecución del algoritmo genético

El fichero donde se almacenan estos datos se guarda automátiamente en la carpeta `resultados`. Su nombre es identificativo de la ejecución realizada, ya que contiene los valores de los parámetros de entrada y la hora de la ejecución, siguiendo el formato:

`prefijo-nPob-nGenes-nGeneraciones-hh:mm:ss.xml`,

donde `prefijo` es siempre la cadena `agPablo`. Por ejemplo:

[`agPablo-128-256-10-17:29:58`](https://github.com/pablopaolus/AG_Pablo/blob/master/results/agPablo-128-256-10-17:29:58).

#####6. Compilación, enlazado y ejecución

En todos los ejercicios de las sesiones de trabajo anteriores, para realizar la compilación y enlazado de los códigos había que utilizar la línea de comandos e indicarlo explícitamente:

```bash
g++ file.cpp -o file
```

Ahora se ha creado un [Makefile](https://github.com/pablopaolus/AG_Pablo/blob/master/Makefile) que simplifica y facilita el proceso. Para construir el proyecto tan solo hay que ejecutar la orden `make` en el intérprete de órdenes, situándonos en la raíz del proyecto. Con ello se construye directamente el ejecutable `ag_completo` en la carpeta `bin`, a partir del código fuente de la carpeta `src`. Cabe mencionar que, para ejecutar el algoritmo, hay que situarse en la raíz del proyecto y escribir:

```bash
./bin/ag_completo
```

Es necesario hacerlo desde la raíz porque es aquí donde se encuentra el fichero de configuración ([config.xml](https://github.com/pablopaolus/AG_Pablo/blob/master/config.xml)). Si intentamos ejectuarlo desde la carpeta `bin` dará error (se puede solucionar fácilmente copiando el fichero de configuración a esta carpeta).

También se pueden ejecutar las órdenes `make clean` para borrar ficheros con código objeto y de edición, así como `make mrproper` para borrar todo lo construído con el `make`, y además los ficheros con los resultados. 


Resultados experimentales
--------------------------

ToDo


