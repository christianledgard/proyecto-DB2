# Proyecto DB2
Franco, Ledgard & Reátegui - CS UTEC

# Introducción
## Objetivo del proyecto.

Hoy en día las bases de datos son indispensables en nuestra sociedad. Sin una forma estructurada de manejar grandes volúmenes de información, seguramente grandes avances científicos no se hubieran podido haber llevado acabo. Sumado a esto, la importancia de conocer algoritmos eficientes de inserción, búsqueda y eliminación no es menos importante. En este proyecto nosotros nos enfocaremos en la elaboración de 2 estructuras (**Sequential File** y **Extendible Hashing**) para guardar información. Luego, desarollaremos algunos experimentos y validaremos la importancia de las mismas. 

## Descripción del dominio de datos a usar.

En este proyecto utilizaremos dos sets de datos: "Players.csv" y "Teams.csv". Dichos sets de datos, conocidos en el mundo del Machine Learning, cuentan con 595 y 32 tuplas respectivamente. Consideramos que es una cantidad adecuada de tuplas para realizar todas nuestras pruebas, validaciones, test y experimentos. 

## Resultados que se esperan obtener.

En primer lugar esperamos obtener un tiempo de búsqueda menor en la estructura indexada (**Extendible Hashing**) a comparación con el **Sequential File**. Esto se debe a que las complejidades de búsqueda de ambas estructuras son diferentes. Al tener un hash, la búsqueda se vuelve O(1) y en el caso del Sequential File la complejidad es O(n).

## Costos de operaciones (peor caso) en términos de acceso a memoria secundaria

### Sequential File

- Búsqueda O(n)
- Inserción O(n)
- Eliminación O(n)

### Extendible Hashing

- Búsqueda O(1)
- Inserción O(n)   // Split bucket y Reescalar (agrandar) IndexFile
- Eliminación O(n) // Merge buckets y Reescalar (reducir) IndexFile


# Descripción de las operaciones

## Sequential File

### Lineamientos

- Cada registro tiene un puntero al registro anterior y otro puntero al registro siguiente.
- El puntero null previo es -1 (es decir, el previo al primer registro).
- El puntero null siguiente es -2 (es decir, el siguiente al último registro).
- Los registros se encuentran ordenados (en orden ascendente) bajo su ID.
- Sea R cualquier registro del sequential file. El registro de la posición ```R.next``` es estrictamente mayor que R, y el registro de la posición ```R.prev``` es estrictamente menor que R.
- Los registros ordenados y no ordenados se encuentran todos en un mismo archivo.
- Cuando se elimina un registro de la sección ordenada, se reconstruye el archivo debido a que la eliminación estropea la búsqueda binaria.
- Se maneja un free list LIFO para manejar la eliminación de los registros no ordenados.
- Cuando se inserta, primero se revisa la free list. En caso no hayan registros eliminados, se inserta al final de la sección no ordenada.

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
- Insertar entre registros no ordenados ```void insertBetweenUnorderedRecords```: En caso el registro base apunte a un registro de la sección no ordenada, se debe encontrar la posición adecuada en la cual insertar para poder actualizar correctamente los punteros. Finalmente, se añade el registro en la sección no ordenada.

**NOTAS**: 
- Al insertar se actualizan los punteros de los registros en O(1) de modo que para cualquier registro R, el registro de la posición ```R.next``` es estrictamente mayor que R, y el registro de la posición ```R.prev``` es estrictamente menor que R.
- Una vez hay 5 registros en la sección no ordenada, se reconstruye el archivo.

### Eliminación

El método de eliminación es ```void deleteRecord(KeyType ID)```.

Primero se busca en todo el archivo el registro a eliminar. Una vez se encuentra, se obtiene su posición lógica. Luego, se llama al método ```void updatePointersDelete(RecordType toDelete, KeyType toDeleteLogPos)``` para actualizar los punteros de ```toDelete.prev``` y ```toDelete.next```. Luego se evalúa si el registro se encuentra en la sección ordenada o no ordenada. En caso el registro a eliminar se encuentre en la sección ordenada, se borra el registro y se reconstruye el archivo, debido a que aquella eliminación arruina la búsqueda binaria en la sección ordenada usando el método privado ```void deleteOrderedRecord(KeyType toDeleteLogPos)```. En caso el registro a eliminar se encuentre en la sección no ordenada, se utiliza un free list LIFO para manejar la eliminación ```void deleteUnorederedRecord(toDeleteLogPos)```.

## Extendible Hashing

### Inserción

Para los casos de inserrción, un punto muy importante a tener en cuenta son los casos de **overflow**. Estos ocurren cuando queremos insertar un elemento en un bucket que esta lleno (el factor de bloque es igual a la cantidad de elementos). En este caso, cuando ocurre overflow, tenemos que realizar un split y expansión como observamos a continuación:


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
