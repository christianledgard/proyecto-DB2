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

- Búsqueda: ```O(log(n) + m)```, donde ```n``` es la cantidad de registros ordenados y ```m``` es la cantidad de registros no ordenados.
- Inserción: ```O(log(n) + m)```, donde ```n``` es la cantidad de registros ordenados y ```m``` es la cantidad de registros no ordenados.
- Eliminación:
    - De registros ordenados: ```O(n)```, donde ```n``` es la cantidad de registros totales.
    - De registros no ordenados: ```O(log(n) + m)```, donde ```n``` es la cantidad de registros ordenados y ```m``` es la cantidad de registros no ordenados.

### Extendible Hashing

- Búsqueda ```O(1)```
- Inserción ```O(n)```   Split bucket y Reescalar (agrandar) IndexFile
- Eliminación ```O(n)``` Merge buckets y Reescalar (reducir) IndexFile


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

```
RecordType search(KeyType ID) {
    RecordType baseRecord = this->searchInOrderedRecords(ID);

    if (baseRecord.ID == ID) {
        return baseRecord;
    }

    if ((baseRecord.prev == -1 && ID < baseRecord.ID) || (baseRecord.next == -2 && ID > baseRecord.ID)) {
        throw std::out_of_range("Search out of range. ID: " + std::to_string(ID));
    }

    if (baseRecord.ID > ID) {
        baseRecord = this->getPrevRecord(baseRecord);
    }

    RecordType current = baseRecord;

    current = this->read(this->sequentialFileName, current.next);
    while (current.ID <= ID) {
        if (current.ID == ID) {
            return current;
        } else {
            current = this->read(this->sequentialFileName, current.next);
        }
    }

    throw std::out_of_range("Search out of range. ID: " + std::to_string(ID));
}
```

### Búsqueda por rangos

El método de búsqueda por rangos es ```std::vector<RecordType> searchByRanges(KeyType begin, KeyType end)```.

Primero se realiza una búsqueda binaria sobre la parte ordenada del archivo para encontrar el registro base para realizar la búsqueda por rangos. Luego, se va iterando a través de los punteros ```next``` de los registros al mismo tiempo que se agregan los registros que están dentro del rango a un vector. Cuando el registro actual ya se haya pasado del rango o se haya llegado al último registro, se retorna el vector con los registros encontrados.

```
std::vector<RecordType> searchByRanges(KeyType begin, KeyType end) {
    if (begin > end) {
        std::swap(begin, end);
    }

    RecordType current = this->searchInOrderedRecords(begin);

    if (current.prev != -1) {
        if (current.ID > begin) {
            current = this->getPrevRecord(current);
        }
    }

    std::vector<RecordType> searchResult;

    while (true) {
        if (current.ID >= begin && current.ID <= end) {
            searchResult.push_back(current);
        }
        if (current.ID > end || current.next == -2) {
            return searchResult;
        }
        current = this->read(this->sequentialFileName, current.next);
    }
}
```

### Inserción

El método de inserción es ```void insert(RecordType toInsert)```.

Primero se realiza una búsqueda binaria sobre los registros ordenados para hallar el registro base. Una vez encontrado, se verifica que el ID del registro a insertar no sea igual al registro base para evitar repetidos. Luego, se verifican los distintos casos de inserción:

- Insertar antes del primer registro ```void insertAtLastPosition(RecordType record)```: Se escribe el primer registro actual en la parte de registros no ordenados. Luego, se escribe en el archivo el registro a insertar en la primera posición lógica del archivo.
- Insertar después del último registro de la sección ordenada ```void insertAtLastPosition(RecordType record)```: El registro a insertar se inserta en la sección de registros no ordenados con el puntero a ```next``` apuntando a -2.
- Insertar cuando el registro base apunta a otro registro de la sección ordenada ```void simpleInsert(RecordType record)```: Dado que el registro base no apunta a la sección de los registros no ordenados, basta con insertar el registro a insertar en la sección no ordenada.
- Insertar entre registros no ordenados ```void insertBetweenUnorderedRecords```: En caso el registro base apunte a un registro de la sección no ordenada, se debe encontrar la posición adecuada en la cual insertar para poder actualizar correctamente los punteros. Finalmente, se añade el registro en la sección no ordenada.

```
void insert(RecordType toInsert) {
    RecordType baseRecord = this->searchInOrderedRecords(toInsert.ID);

    if (baseRecord.ID == toInsert.ID) {
        throw std::out_of_range("User attempted to insert an already existing ID");
    }

    if (baseRecord.prev == -1 && toInsert.ID < baseRecord.ID) { // insert at the beginning
        this->insertAtFirstPosition(toInsert);
    } else if (baseRecord.next == -2) { // insert at last position
        this->insertAtLastPosition(toInsert);
    } else { 
        if (baseRecord.ID > toInsert.ID) {
            baseRecord = this->getPrevRecord(baseRecord);
        }
        if (baseRecord.next < totalOrderedRecords) {
            this->simpleInsert(baseRecord, toInsert); // when it's not necessary to insert "between" unordered registers
        } else { // when baseRecord points to unordered records
            this->insertBetweenUnorderedRecords(baseRecord, toInsert);
        }
    }
    if (++totalUnorderedRecords == 5) {
        this->rebuildAfterInsert();
    }
}
```

**NOTAS**: 
- Al insertar se actualizan los punteros de los registros en O(1) de modo que para cualquier registro R, el registro de la posición ```R.next``` es estrictamente mayor que R, y el registro de la posición ```R.prev``` es estrictamente menor que R.
- Una vez hay 5 registros en la sección no ordenada, se reconstruye el archivo.

### Eliminación

El método de eliminación es ```void deleteRecord(KeyType ID)```.

Primero se busca en todo el archivo el registro a eliminar. Una vez se encuentra, se obtiene su posición lógica. Luego, se llama al método ```void updatePointersDelete(RecordType toDelete, KeyType toDeleteLogPos)``` para actualizar los punteros de ```toDelete.prev``` y ```toDelete.next```. Luego se evalúa si el registro se encuentra en la sección ordenada o no ordenada. En caso el registro a eliminar se encuentre en la sección ordenada, se borra el registro y se reconstruye el archivo, debido a que aquella eliminación arruina la búsqueda binaria en la sección ordenada usando el método privado ```void deleteOrderedRecord(KeyType toDeleteLogPos)```. En caso el registro a eliminar se encuentre en la sección no ordenada, se utiliza un free list LIFO para manejar la eliminación ```void deleteUnorederedRecord(toDeleteLogPos)```.

```
void deleteRecord(KeyType ID) {
    RecordType toDelete = this->search(ID);

    if (toDelete.ID != ID) {
        throw std::out_of_range("Record with ID " + std::to_string(ID) + " not found.");
    }

    long toDeleteLogPos = this->getLogicalPosition(toDelete);

    this->updatePointersDelete(toDelete, toDeleteLogPos);

    if (toDeleteLogPos < totalOrderedRecords) {
        this->deleteOrderedRecord(toDeleteLogPos);
        --totalOrderedRecords;
    } else {
        this->deleteUnorderedRecord(toDeleteLogPos);
        --totalUnorderedRecords;
    }
}
```

## Extendible Hashing

### Lineamientos

- Esta estructura asociada a una función Hash la cual determina a que bucket está asociado cada registro para cualquier operación.
- Existe un IndexFile que se carga o se genera dada una profundidad global, contiene 2^(profundidad global entradas) representadas de forma binaria.
- Cada entrada en el IndexFile tiene la forma <Bucket,LocalDepth>, lo cual permite saber que archivo apunta cada entrada.
- Cada bucket del Hash es un archivo independiente con el nombre "bucket.dat". e.g: "010.dat"
- LocalDepth no puede exceder Global Depth.
- La cantidad de registros por Bucket no puede exceder el factor de bloque.

### Búsqueda

Dada por la función ```RecordType* searchRecord(KeyType ID)```.

- Dada un ID de tipo KeyType el algoritmo de búsqueda sigue la siguiente estructura


![Alt text](https://i.ibb.co/5rTTrRM/Untitled-Diagram-Basic.png)

1. Se ingresa el ID
2. Se ingresa a la función HASH, esta retorna un valor binario.
3. EL valor del paso anterior es usado como offset de lectura en el IndexFile
4. El localDepth leido anteriormente es usado para leer el bucket respectivo
5. Se recorre el bucket registro tras registro, si se encuentra se retorna un puntero a record, caso contrario se retorna nullptr.

La búsqueda tiene un costo de O(1) ya que su costo no depende del número de registros almacenados, siempre se calculará el offset para realizar una única lectura en el IndexFile O(1) y luego se recorre el bucket, sin embargo, el tamaño máximo del Bucket depende del factor de bloque, es decir oscila entre 0 y el Factor Bloque, pero sigue siendo una constante, por ende, O(1).

### Inserción

Dada por la función ```bool insertRecord(RecordType record)``` que retorna True en caso se logró insertar, caso contrario, retorna False, lo cual ocurre si el registro ya existía.

- El primer paso es buscar si ya existe el registro, si no se encontró, se intenta insertar.
- Se lee el tamaño del Bucket, si no generaría overflow la insercióneste caso, el costo depende si ocurre o no OverFlow, cuando no ocurre, la inserción se realiza en O(1), por otra parte el overFlow se maneja de la siguiente manera:


![Alt text](https://i.ibb.co/w0vCXMx/Untitled-Diagram-Over-Flow.png)


El algoritmo de SplitBucket es: 

```
    void splitBucket(long localDepth, const string &bucketName) {
        createNewFiles(bucketName);  
        updatePointersOfIndixes(bucketName, localDepth);
        insertRecordsFromOldBucket(bucketName, localDepth + 1);
        removeOldBucket(bucketName);
    }
```

Lo que determina el costo es la actualización de los índices O(n) e insertar los Records que se encontraban el Bucket original.

El algoritmo de DirectoryRebuild es:

```
    void rebuildIndexFile(){
        fstream file,temp;
        
        Se genera un archivo temp
        ++globalDepth; 
        Escribir en Temp  globalDepth y blockFactor

        file.seekg(2*sizeof(long),ios::beg);

        for(int i=0;i<pow(2,globalDepth-1);++i){
            file.read((char*)&index,sizeof(char)*(globalDepth-1));
            file.read((char*)&localDepth,sizeof(long));
            char extended[globalDepth];
            extended[0]='0';
            for(int item=1;item<globalDepth;++item)
                extended[item]=index[item-1];
            temp.write((char*)&extended,sizeof(char)*globalDepth);
            temp.write((char*)&localDepth,sizeof(long));
        }

        file.seekg(2*sizeof(long),ios::beg);

        //Same for loop as before to duplicate size

        temp.close(); file.close();
        string oldname= "temp.dat";
        rename(oldname.c_str(),hashFile.c_str());
    }

```

Se recorre el Hash original dos veces, ya que se duplica el tamaño de las entradas. Esto ocurre en O(n), esta operación sí depende de la cantidad registros.

Finalmente, se concluye  que la inserción tiene O(n).


### Eliminación

Para realizar el algorítmo de eliminación seguimos el siguiente Pseudocódigo adaptado de Folk, Zoellick and Riccardi (1998). 

La función es ``` bool removeRecord(KeyType key) ``` retorna True si se pudo eliminar, caso contrario, False.

El algoritmo funciona de la siguiente manera:


Dado un bucket, este tiene un buddy si:

```
buddy= swap the first bit of i
if(bucket[i]!=bucket[buddy]) then
    bucket de buddy es el buddyBucket
else 
    no existe un buddyBucket
```


Ahora que ya sabemos determinar si un bucket tiene un buddyBucket, para poder afirmar que se puede comprimir el IndexFile, ningún bucket debe tener buddyBucket

```
bool collapse(){
        bool canColapse=false;
        if(globalDepth>1){
            canColapse=true;
            long halfSize= pow(2,globalDepth)/2;
            fstream file;
            file.open(hashFile,ios::binary |ios::in);

            char hashValue[globalDepth], buddyHashValue[globalDepth];
            for(long i=0;i<halfSize;++i){
                file.seekg(sizeof(long)*2+(sizeof(char)*globalDepth+sizeof(long))*i,ios::beg);
                long localDepth,localDepthBuddy;
                file.read((char*)&hashValue,+ sizeof(char)*globalDepth);
                file.read((char*)&localDepth,sizeof(long));
                file.seekg(sizeof(long)*2+(sizeof(char)*globalDepth+sizeof(long))*(i+pow(2,globalDepth-1)),ios::beg);
                file.read((char*)&buddyHashValue,+ sizeof(char)*globalDepth);
                file.read((char*)&localDepthBuddy,sizeof(long));


                string bucketName= hashString.substr(globalDepth-localDepth,globalDepth);
                string bucketBuddyName= buddyHashString.substr(globalDepth-localDepthBuddy,globalDepth);

                if(bucketBuddyName!=bucketName){
                    canColapse= false;
                    return canColapse;
                }
            }
            rebuildColapse();
        }
        return canColapse;
    }
```

En caso, se cumpla la condición mencionada, se hace el rebuild, que funciona de forma similar que en el Insert, pero en vez de depublicar las entradas, se parten por la mitad. Esto ocurre en O(n)

La eliminación como tal, busca el registro, en caso lo encuentre, lo marca como eliminado en su Bucket (mecanísmo FreeList). Luego ejecuta TryCombine
```
eliminar(key):
    key = search(key);
    if key.notFound(): return;
    else
    {
        remove key from bucket;
        tryCombine(bucket);
    }
```
En C++ el código es: 

```
   bool removeRecord(KeyType key){
        pair<RecordType*,string> recordAndBuffer=getRecordAndBuffer(key);
        if(recordAndBuffer.first!=nullptr){
            deleteRecordFromFile(key,recordAndBuffer.second);
            tryCombine(key,recordAndBuffer.second);
            return true;
        }
        else{
            return false;
        }
    }
```

El pseudocódigo de TryCombine es:

```
tryCombine(bucket):
    if(bucket a tiene bucket hermano b)
    {
        if(a.numkeys + b.numkeys <= globalDepth)
        {
            Merge buckets a y b en a; 
            try to collapse the directory;
            if(directory was collapsed)
            {
                tryCombine(a);
            }
        }
    }

```
Se llama recursivamente para hacer merge de los buckets tanto como sea posible, luego de cada merge se comprueba si es posible comprimir el el IndexFile, de tal forma se hace merge y compresión del IndexFile tantas veces como sea posible, con el fin de reducir el Hash.

De tal forma, lo que determina el costo de la eliminación es el Merge de los buckets y la compresión del IndexFile O(n)

Por ende, se concluye que la eliminación ocurre en O(n).

# Transacciones

Realizamos una simulación concurrente implementada en C++. Utilizamos la librería pthread (POSIX) y pthread_mutex para controlar los accesos a recursos compartidos. 

Iniciamos dicha implementación creando 2 threads para insertar 2 registros en nuestra base de datos. En dicha operación, debemos usar mutex locks dado que estamos modificando punteros y posiciones de memoria.

```
void* fthread_write(void *t)
{
  pthread_mutex_lock(&lockMutex);
  teamsSequentialFile.insert(*((struct Team<long>*)t));
  pthread_mutex_unlock(&lockMutex);
  return NULL;
}

void* fthread_read(void *t)
{
  pthread_mutex_lock(&lockMutex);
  Team<long> temp = teamsSequentialFile.search(*((long*)t));
  pthread_mutex_unlock(&lockMutex);
  responses.push_back(temp);
  return NULL;
}
```

Es necesario usar un mutex en read y write, debido a que hay errores si una thread intenta leer un registro mientras que otra thread mueve los punteros cuando desea escribir un registro. Es por ello, que nuestra transacción es concurrente, no paralela.

Se puede observar la implementación de las dos thread en el código abajo.

```
  pthread_t thread_write,thread_write2, thread_read[N_TO_READ];

  Team<long>* team1 = new Team<long>(99, "Jesus", 1, 1, 1, 1, 1, 1, 1, 1, 1);
  Team<long>* team2 = new Team<long>(100, "Pedro", 1, 1, 1, 1, 1, 1, 1, 1, 1);


  pthread_create(&thread_write, NULL, fthread_write, (void *)team1);
  pthread_create(&thread_write2, NULL, fthread_write, (void *)team2);

  pthread_join(thread_write,  NULL);
  pthread_join(thread_write2,  NULL);
```

Luego, realizamos una transacción concurrente para leer distintos registros e imprimirlos en pantalla.

```
for (size_t i = 0; i < N_TO_READ; i++)
{
pthread_create(&thread_read[i], NULL, fthread_read, (void *)&idToSearch[i]);
}

for (size_t i = 0; i < N_TO_READ; i++)
{
pthread_join(thread_read[i],NULL);
}

for(auto& response : responses) {
    response.shortPrint();
    cout << "\n";
}
```

Ambas transacciones finalizan con éxito y realizan su labor correctamente.

# Resultados Experimentales

Los tiempo de ejecución estan medidos en microsegundos (ms) y los accesos a disco hacen referencia a las operaciones read y write sobre el disco duro. Las líneas naranja hacen referencia a la cantidad de registros almacenados en la estructura, mientras que los puntos azules hacen referencia al tiempo y accesos a disco respectivamente.

## Sequential File

### Search

**Time search**

![Alt_text](https://i.ibb.co/3p0fMkn/time-search.jpg)

En teoría, el tiempo de búsqueda es ```O(log(n) + m)```. Sin embargo, se puede observar que los tiempos en todos los casos no pasan de 1ms (con excepción de algunos outliers que demoran más de 1ms cuando se busca sobre 500 registros). Se puede concluir que la búsqueda es sumamente eficiente.

**Accesos a disco search**

![Alt_text](https://i.ibb.co/gtxyp5S/disk-access-search.jpg)

Se puede observar que los accesos a disco aparentan ser ```O(1)``` sin importar la cantidad de registros. Sin embargo, en teoría deberían ser ```O(log(n) + m)```.

### Insert

**Time insert**

![Alt_text](https://i.ibb.co/whfc8mj/time-insert.jpg)

En la gráfica se pueden observar los tiempos de inserción sin reconstrucción y de inserción con reconstrucción. La inserción con reconstrucción toma ```O(n)``` debido a que la reconstrucción del archivo toma un tiempo lineal. La inserción sin reconstrucción toma ```O(log(n) + m)```, sin embargo si nos fijamos únicamente la gráfica, podemos observar que como la ```m``` es muy pequeña (como máximo 5), la complejidad aparenta ser ```O(log(n))```.

**Accesos a disco insert**

![Alt_text](https://i.ibb.co/vQKzYJb/disk-access-insert.jpg)

En la gráfica se pueden observar los accesos a disco de inserción sin reconstrucción y de inserción con reconstrucción. Los accesos a disco en la inserción con reconstrucción tienen una complejidad de ```O(n)```, debido a que se tienen que leer y volver a escribir todos los registros del archivo. Los accesos a disco en la inserción sin reconstrucción tiene una complejidad de ```O(log(n) + m)```, sin embargo en la gráfica aparente ser ```O(1)```.

### Remove

**Time remove**

![Alt_text](https://i.ibb.co/VqbjXnm/time-remove.jpg)

En la gráfica se puede observar la complejidad de tiempo para los dos casos de borrado. 

El primero, es cuando se borra un registro de la zona ordenada del archivo. En este caso, se tiene que reconstruir el archivo, por lo que esta operación toma ```O(n)```. Se puede observar cómo va disminuyendo la complejidad de tiempo a medida que disminuye la cantidad de registros en el Sequential File.

El segundo caso, es cuando se borra un registro de la zona no ordenada. En este caso, se usa el free list implementado. Este caso de borrado toma ```O(log(n) + m)```. Se puede observar que sin importar la cantidad de registros en el archivo, el tiempo que toma este caso de eliminado se mantiene bajo los 10ms.

**Accesos a disco remove**

![Alt_text](https://i.ibb.co/pWcxQGT/disk-access-remove.jpg)

En la gráfica se pueden observar los accesos a disco para los dos casos de borrado.

El primero, es cuando se borra un registro de la zona ordenada del archivo. En este caso, se tiene que reconstruir el archivo, y dado que se tiene que leer y escribir cada registro del archivo, los accesos a disco toman ```O(n)```. Se puede observar cómo van disminuyendo los accesos a disco a medida que disminuye la cantidad de registros en el Sequential File.

El segundo caso, es cuando se borra un registro de la zona no ordenada. En este caso, se usa el free list implementado. Este caso de borrado los accesos a disco toman ```O(log(n) + m)```. Sin embargo, en la gráfica se puede observar que los accesos a disco aparentan ser constantes.

## Hashing

### Search 
**Time search**

![Alt_text](https://i.ibb.co/r46mCgq/time-search.png)

Se puede observar que el tiempo de búsqueda es constante en todos los casos, sin importar la cantidad de registros **```O(1)```**

**Accesos a disco search**

![Alt_text](https://i.ibb.co/HFtGwXd/memory-busqueda.png)

Se puede observar que la cantidad de accesos a disco de la búsqueda es constante en todos los casos, sin importar la cantidad de registros **```O(1)```**

### Insert

**Time insert**

![Alt_text](https://i.ibb.co/GskrKpb/Time-insert.png)

Se puede observar que en el mejor y caso promedio, el tiempo de ejecución es aproximadamente **```O(1)```**, ya que no existe alguna operación que haga más prolongada la ejecución, por otra parte, cada cierta cantidad de inserciones existen picos pequeños y grandes, los picos pequeños se deben al ```splitBucket``` y los picos grandes se deben a la reestructuración del IndexFile, lo cual tiene un comportamiento **```O(n)```**.

**Accesos a disco insert**

![Alt_text](https://i.ibb.co/M10HnD4/memory-insert.png)

Se puede observar que en el mejor y caso promedio, la cantidad de lecturas a disco es aproximadamente **```O(1)```**, ya que no se efectua ```splitBucket``` o ```rebuildIndexFile```, por otra parte, cada cierta cantidad de inserciones existen picos pequeños y grandes, los picos pequeños se deben al ```splitBucket``` y los picos grandes se deben a la reestructuración del IndexFile, lo cual tiene un comportamiento **```O(n)```**. La línea azul abajo es debido a las inserciones fallidas de registros ya existentes, dichas inserciones fallidas ocurren en **```O(1)```**

### Remove

**Time remove**

![Alt_text](https://i.ibb.co/znyW3Fc/time-remove.png)

El tiempo de ejecución del remove tiene picos altos, que ocurren cuando se ejecuta un ```collapseIndexFile```, lo cual es relativamente pesado y ocurre en **```O(n)```**, dichas operaciones ocurren inmediatamente después de un ```mergeBuckets```, por lo que previo a cada pico alto, ocurre un incremento progresivo en el tiempo de ejecución. Cuando el remover realiza el mejor caso, se trata de **```O(1)```**

**Accesos a disco remove**

![Alt_text](https://i.ibb.co/5Wx58fz/memory-remove.png)

La cantidad de lecturas a disco del remove tiene picos altos, que ocurren cuando se ejecuta un ```collapseIndexFile```, lo cual accede a discos muchas veces y ocurre en **```O(n)```**, dichas operaciones ocurren inmediatamente después de un ```mergeBuckets```, por lo que previo a cada pico alto, ocurre un incremento progresivo en las lecturas a disco. Cuando el remover realiza el mejor caso, se trata de **```O(1)```**, lo cual se observa en las zonas planas, esto ocurre entre 2000 y 4000 registros aproximadamente.

# Pruebas de uso y presentación

Link del vídeo: 
