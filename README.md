# Proyecto DB2
Franco, Ledgard & Reátegui - CS UTEC

# Introducción
## Objetivo del proyecto.

Hoy en día las bases de datos son indispensables en nuestra sociedad. Sin una forma estructurada de manejar grandes volúmenes de información, seguramente grandes avances científicos no se hubieran podido haber llevado acabo. Sumado a esto, la importancia de conocer algoritmos eficientes de inserción, búsqueda y eliminación no es menos importante. En este proyecto nosotros nos enfocaremos en la elaboración de 2 estructuras (**Sequential File** y **Extendible Hashing**) para guardar información. Luego, desarollaremos algunos experimentos y validaremos la importancia de las mismas. 

## Descripción del dominio de datos a usar.

En este proyecto utilizaremos dos sets de datos: "Players.csv" y "Teams.csv". Dichos sets de datos, conocidos en el mundo del Machine Learning, cuentan con 595 y 32 tuplas respectivamente. Consideramos que es una cantidad adecuada de tuplas para realizar todas nuestras pruebas, validaciones, test y experimentos. 

## Resultados que se esperan obtener.

En primer lugar esperamos obtener un tiempo de búsqueda menor en la estructura indexada (**Extendible Hashing**) a comparación con el **Sequential File**. Esto se debe a que las complejidades de búsqueda de ambas estructuras son diferentes. Al tener un hash, la búsqueda se vuelve O(1) y en el caso del Sequential File la complejidad es O(n).

# Fundamente y describa las técnicas.

- Describa brevemente las técnicas de indexación de archivos que ha elegido. Explique procedimentalmente el proceso de inserción, eliminación y búsqueda.

## Sequencial File

### Lineamientos

- Cada registro tiene un puntero al registro anterior y otro puntero al registro siguiente.
- El puntero null previo es -1 (es decir, el previo al primer registro).
- El puntero null siguiente es -2 (es decir, el siguiente al último registro).
- Los registros se encuentran ordenados (en orden ascendente) bajo su ID.
- Sea R cualquier registro del sequential file. El registro de la posición ```R.next``` es estrictamente mayor que R, y el registro de la posición ```R.prev``` es estrictamente menor que R.
- Los registros ordenados y no ordenados se encuentran todos en un mismo archivo.
- Una vez hay 

### Búsqueda

El método de búsqueda de un registro es ```RecordType search(KeyType ID)```.

Primero se realiza una búsqueda binaria sobre la parte ordenada del archivo. En caso el registro a buscar no se encuentre en esta sección del archivo, se busca sobre la parte no ordenada del archivo siguiendo los punteros. En caso no se encuentre el archivo, se lanza una excepción.

### Búsqueda por rangos

El método de búsqueda por rangos es ```std::vector<RecordType> searchByRanges(KeyType begin, KeyType end)```.

Primero se realiza una búsqueda binaria sobre la parte ordenada del archivo para encontrar el registro base para realizar la búsqueda por rangos. Luego, se va iterando a través de los punteros ```next``` de los registros al mismo tiempo que se agregan los registros que están dentro del rango a un vector. Cuando el registro actual ya se haya pasado del rango o se haya llegado al último registro, se retorna el vector con los registros encontrados.

### Inserción

El método de inserción es ```void insert(RecordType toInsert)```.

Primero se realiza una búsqueda binaria sobre los registros ordenados para hallar el registro base. Una vez encontrado, se verifica que el ID del registro a insertar no sea igual al registro base para evitar repetidos. Luego, se verifican los distintos casos de inserción:

- Insertar antes del primer registro ```void insertAtLastPosition(RecordType record)```: Se escribe el primer registro actual en la parte de registros no ordenados. Luego, se escribe en el archivo el registro a insertar en la primera posición lógica del archivo.
- Insertar después del último registro de la sección ordenada ```void insertAtLastPosition(RecordType record)```: El registro a insertar se inserta en la sección de registros no ordenados con el puntero a ```next``` apuntando a -2.
- Insertar cuando el registro base apunta a otro registro de la sección ordenada ```void simpleInsert(RecordType record)```: Dado que el registro base no apunta a la sección de los registros no ordenados, basta con insertar el registro a insertar en la sección no ordenada.

### Eliminación

## Extendible Hashing

### Inserción

Para los casos de inserrción, un punto muy importante a tener en cuenta son los casos de **overflow**. Estos ocurren cuando queremos insertar un elemento en un bucket que esta lleno (el factor de bloque es igual a la cantidad de elementos). En este caso, cuando ocurre overflow, tenemos que realizar un split y expansión como observamos a continuación:

![](https://i.ibb.co/JdQbHmx/Captura-de-Pantalla-2020-10-12-a-la-s-15-41-20.png)

![](https://i.ibb.co/92VQD20/Captura-de-Pantalla-2020-10-12-a-la-s-15-41-25.png)

### Eliminación

Para realizar el algorítmo de eliminación seguimos el siguiente Pseudocódigo adaptado de Folk, Zoellick and Riccardi (1998). 

```
eliminar(key):
    key = search(key);
    if key.notFound(): return;
    else
    {
        remove key from bucket;
        tryCombine(bucket);
    }
    
tryCombine(bucket):
    if(bucket a tiene bucket hermano b)
    {
        if(a.numkeys + b.numkeys)
        {
            combine buckets a y b en a;
            try to collapse the directory;
            if(directory was collapsed)
            {
                tryCombine(a);
            }
        }
    }

```


### Búsqueda

- Debe notarse con claridad el manejo de la memoria secundaria.
- Describa como realizó la simulación de transacciones. Explique el uso de hilos para la ejecución en paralelo de transacciones.

# Resultados Experimentales
- Cuadro y/o gráfico comparativo de desempeño de las técnicas de indexación de archivos sobre el dominio de datos. Tanto para inserción como para búsqueda.
- Considerar dos métricas: total de accesos a disco duro (read & write) y tiempo de ejecución en milisegundos.
- Discusión y Análisis de los resultados experimentales.

# Pruebas de uso y presentación

- Presentar las pruebas de uso de la aplicación.
- Muestre la funcionalidad del aplicativo mediante un video (deben participar todos los
miembros del grupo).