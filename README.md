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
- Detalle los aspectos importantes de la implementación de dichas técnicas.
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