# Proyecto DB2
Franco, Ledgard & Reátegui - CS UTEC

# Introducción
## Objetivo del proyecto.

Hoy en día las bases de datos son indispensables en nuestra sociedad. Sin una forma estructurada de manejar grandes volúmenes de información, seguramente que grandes avances científicos no se hubieran podido haber llevado acabo. Sumado a esto, la importancia de conocer algoritmos eficientes de inserción, búsqueda y eliminación no es menos importante. En este proyecto nosotros nos enfocaremos en la elaboración de 2 estructuras (**Sequential File** y **Extendible Hashing**) para guardar información. Luego, desarollaremos algunos experimentos y validaremos la importancia de las mismas. 

## Descripción del dominio de datos a usar.

En este proyecto utilizaremos dos sets de datos: "Players.csv" y "Teams.csv". Dichos sets de datos, altamente conocidos en el mundo del Machine Learning, cuentan con 595 y 32 tuplas respectivamente. Consideramos que es una cantidad relevante de tuplas para realizar todas nuestras pruebas, validaciones, test y experimentos. 

## Resultados que se esperan obtener.

En primer lugar esperamos obtener un tiempo de búsqueda menor en la estructura indexada (**Extendible Hashing**) a comparación con el **Sequential File**. Esto se debe a que las complejidades de búsqueda de ambas estructuras son diferentes. Al tener un hash, la búsqueda se vuelve O(1) y en el caso del Sequential File la complejidad es O(n).

# Fundamente y describa las técnicas.

- Describa brevemente las técnicas de indexación de archivos que ha elegido. Explique procedimentalmente el proceso de inserción, eliminación y búsqueda.

## Sequencial File

### Inserción

![Inserción Sequencial File](https://i.ibb.co/9chv09W/Captura-de-Pantalla-2020-10-12-a-la-s-14-05-39.png)

![](https://i.ibb.co/swjb0cd/Captura-de-Pantalla-2020-10-12-a-la-s-14-10-37.png)

### Eliminación
![](https://i.ibb.co/7Vyv89y/Captura-de-Pantalla-2020-10-12-a-la-s-14-17-16.png)

![](https://i.ibb.co/z6q7bpr/Captura-de-Pantalla-2020-10-12-a-la-s-14-18-20.png)

### Búsqueda

![](https://i.ibb.co/Gv46N7p/Captura-de-Pantalla-2020-10-12-a-la-s-14-15-31.png)

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