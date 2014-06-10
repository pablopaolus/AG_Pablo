Documentación del trabajo final: Algoritmo Genético completo
============================================================

He creado un nuevo repositorio [AG_Pablo] (https://github.com/pablopaolus/AG_Pablo.git) para albergar el trabajo final de la asignatura. 
El código del algoritmo genético completo en sí se encuentra en la carpeta `src` del repositorio y se denomina [ag_completo.cpp] (https://github.com/pablopaolus/AG_Pablo/blob/master/src/ag_completo.cpp).
El proyecto se distribuye bajo [licencia GPLv2](https://github.com/pablopaolus/AG_Pablo/blob/master/LICENSE). 

Recordemos que, a lo largo de las anteriores sesiones de trabajo, decidí seguir adelante utilizando la clase `<vector>`, debido a la disponibilidad de funciones en esta clase que, además de facilitarnos la vida, ahorrarán mucho tiempo de ejecución. Los cambios "grosso modo" del nuevo código en [ag_completo.cpp] (https://github.com/pablopaolus/AG_Pablo/blob/master/src/ag_completo.cpp) con respecto al último ejercicio de la sesión anterior ([seleccion.cpp] (https://github.com/pablopaolus/repositorio_pablopaolus/blob/master/seleccion.cpp)) son los siguientes:

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

Se ha implementado ya un algoritmo genético completo (anteriormente solo se hacía el proceso una vez, como si solamente hubiera una generación). Para ello, tras inicializar la población de forma aleatoria, se ha implementado un bucle `for` en el que se realiza la evaluación, selección para reproducción y generación de una nueva población. La condición de terminación del bucle es la ejecución de un determinado número de generaciones. 

#####3. Parámetros de entrada

Ahora, los parámetros de entrada son tres en lugar de dos: 

 - `nPob`: Tamaño de la población, es decir, número de individuos o cromosomas.
 - `nGenes`: Número de genes que contiene cada cromosoma.
 - `nGeneraciones`: número de generaciones (determina la condición de terminación del bucle).

La población está representada por un vector de `nPob` vectores, donde cada uno de ellos es un cromosoma que contiene `nGenes` valores binarios aleatorios (0 o 1).

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

Ahora se ha creado un [Makefile](https://github.com/pablopaolus/AG_Pablo/blob/master/Makefile) que simplifica y facilita el proceso (teniendo en cuenta además que es necesario indicar opciones extra para la compilación con el fin de poder utilizar el fichero de configuración). Para construir el proyecto, tan solo hay que ejecutar la orden `make` en el intérprete de órdenes, situándonos en la raíz del proyecto. Con ello se construye directamente el ejecutable `ag_completo` en la carpeta `bin`, a partir del código fuente de la carpeta `src`. Cabe mencionar que, para ejecutar el algoritmo, hay que situarse en la raíz del proyecto y escribir:

```bash
./bin/ag_completo
```

Es necesario hacerlo desde la raíz del proyecto porque es aquí donde se encuentra el fichero de configuración ([config.xml](https://github.com/pablopaolus/AG_Pablo/blob/master/config.xml)). Si intentamos ejectuarlo desde la carpeta `bin` dará error (se puede solucionar fácilmente copiando el fichero de configuración a esta carpeta).

También se pueden ejecutar las órdenes `make clean` para borrar ficheros con código objeto y de edición, así como `make mrproper` para borrar todo lo construído con el `make`, y además los ficheros con los resultados. 


Resultados experimentales
--------------------------

En este apartado llevaremos a cabo la evaluación del algoritmo genético programado, en términos del tiempo de ejecución para diversos valores de los tres parámetros de entrada. Construiremos así tres tablas, en cada una de las cuales variaremos uno de los parámetros y mantendremos fijos los otros dos. Todos los ficheros de salida con los resultados (donde también aparecen los datos de fitness de cada generación) se encuentran en la carpeta [resultados](https://github.com/pablopaolus/AG_Pablo/tree/master/results). 

En primer lugar, veamos qué ocurre al variar el número de genes por cromosoma, manteniendo fijo el tamaño de la población a 128 individuos y ejecutando el algoritmo para 64 generaciones:

 Tam. población | Nº genes por cromosoma | Nº generaciones | Tiempo de ejecución
----------------|------------------------|-----------------|--------------------
       128      |           128          |        64       |      0.066901 s
       128      |           512          |        64       |      0.238151 s
       128      |           1024         |        64       |      0.468785 s
       128      |           2048         |        64       |      0.946773 s

En la siguiente gráfica se representa la tabla anterior, con el número de genes en el eje de abscisas y el tiempo de ejecución en el eje de ordenadas. Se puede observar una tendencia completamente lineal.

![Grafica Variacion nGenes]
(https://github.com/pablopaolus/AG_Pablo/raw/master/graphics/Variacion_nGenes.png)

En la siguiente tabla mantenemos fijo el número de genes a 256, el número de generaciones a 128 y variamos el tamaño de la población:

 Tam. población | Nº genes por cromosoma | Nº generaciones | Tiempo de ejecución
----------------|------------------------|-----------------|--------------------
       128      |           256          |       128       |      0.249689 s
       512      |           256          |       128       |      0.971499 s
      1024      |           256          |       128       |      1.964379 s
      2048      |           256          |       128       |      3.963195 s

Si graficamos los resultados, observamos un incremento lineal del tiempo de ejecución con el tamaño de la población:

![Grafica Variacion nPob]
(https://github.com/pablopaolus/AG_Pablo/raw/master/graphics/Variacion_nPob.png)

Por último, vamos a mantener fijo el tamaño de la población y el número de genes a 128 (ambos), y variaremos el número de generaciones:

 Tam. población | Nº genes por cromosoma | Nº generaciones | Tiempo de ejecución
----------------|------------------------|-----------------|--------------------
       128      |           128          |        32       |      0.035760 s
       128      |           128          |       128       |      0.136780 s
       128      |           128          |       512       |      0.523546 s
       128      |           128          |      1024       |      1.054551 s

En este caso, la variación del tiempo con el número de generaciones también es lineal, al igual que en los casos anteriores.

![Grafica Variacion nGeneraciones]
(https://github.com/pablopaolus/AG_Pablo/raw/master/graphics/Variacion_nGeneraciones.png)

Aunque hemos observado que todos los incrementos son lineales, las pendientes de las rectas en cada caso no son iguales, lo que significa que no todos los parámetros afectan por igual al tiempo de ejecución, como es lógico. Para sacar alguna conclusión más, en la siguiente figura se representan las tres anteriores rectas en una sola gráfica.

![Grafica Comparacion]
(https://github.com/pablopaolus/AG_Pablo/raw/master/graphics/Comparacion.png)

Mediante esta comparación podemos apreciar que el parámetro que más afecta al tiempo de ejecución es el tamaño de la población, mientras que el parámetro que hace que se incremente menos el tiempo de ejecución es el número de genes por individuo. El número de generaciones se sitúa en un nivel intermedio entre ambos. En cuanto al fitness (aunque no se recoge en las tablas), su valor aumenta con el número de generaciones, como cabía esperar.

#####Profiling con GProf

Para finalizar, vamos a haer uso del profiler de GNU, con lo que podremos conocer cuáles son las funciones que consumen mayor tiempo de ejecución, así como el número de llamadas qeu se realizan a cada una de ellas, entre otras coasas. Para ello, debemos compilar con la opción `-pg` como explica perfectamente rotty11 en [su documentación](https://github.com/rotty11/Genetico2014/blob/master/Documentacion.md). Para poder medio leer el fichero de salida `gmon.out`, vamos a convertirlo en un fichero de texto mediante la orden:

```bash
gprof gmon.out > timeReport.txt
```

Obteniendo el fichero [timeReport.txt](https://github.com/pablopaolus/AG_Pablo/blob/master/timeReport.txt) con los resultados del profiling. Este fichero puede convertirse a una gráfica utilizando un programa llamado [gprof2dot](http://code.google.com/p/jrfonseca/wiki/Gprof2Dot), para lo cual es necesario instalar [Python](https://www.python.org/download/) y [Graphviz](http://www.graphviz.org/Download.php):

```bash
apt-get install python graphviz
```

Para obtener el diagrama hay que ejecutar:

```bash
gprof path/to/your/executable | gprof2dot.py | dot -Tpng -o output.png
```

En nuestro caso concreto:

```bash
gprof bin/ag_completo | gprof2dot.py | dot -Tpng -o output_gprof.png
```

Con lo que obtenemos el siguiente diagrama (hacer clic sobre la imagen para agrandar):

![Grafica gprof]
(https://github.com/pablopaolus/AG_Pablo/raw/master/output_gprof.png)

Como se puede observar, la mayor parte del tiempo la consumen las funciones de mutación, cálculo del fitness y crossover.
