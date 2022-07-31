# Simulador de Memoria Cache

## Como compilar
Navegar por terminar hasta la carpeta `C++/src`, una vez allí correr el comando:
```
g++ main.cpp -o cacheSim.out
```

## Parámetros a usar (obligatorios)
* `-l` : Numero de conjuntos de la Cache.
* `-s` : Numero de bloques de cada conjunto.
* `-b` : Numero de bytes en cada bloque.
* `-t1`: Tiempo de acceso a la memoria 1.
* `-t2`: Tiempo de acceso a la memoria 2.
* `-y` : Bandera para *write-allocate* o *no-write-allocate*.
* `-w` : Bandera para *write-back* o *write-throught*.
* `-r` : Algoritmos de reemplazo; opciones disponibles: *lru*, *random*, o *fifo* (seteado por defecto).
* `-f` : Nombre del archivo donde se encuentra el trazado.

Ejemplo de envio de parámetros:
`./cacheSim.out -l 256 -s 4 -b 16 -t1 1 -t2 5 -y write-allocate -w write-back -r lru -f archivoTraza.txt`

## Ejemplo de un archivo .txt de trazo
```
// En esta traza solo vamos a la posición 0, por lo que la cache puede tener un tamaño pequeño
// Acceso a la cache de este nivel: 13 ciclos, acceso al siguiente nivel: 230 ciclos
// Política: Write-Through
// La memoria cache está inicialmente vacía
//
// Primer "load": genera un miss en este nivel (13 ciclos) y un acceso al siguiente nivel (230 ciclos)
//   En total 243 ciclos para este "load"
//
// Primer "store": genera un "hit" en este nivel (13 ciclos)
//   La política dice "Write-Through", por lo que requerimos de otros 230 ciclos para escribir en el siguiente nivel,
//   si el bloque donde vamos a escribir tiene que ser desalojado, incurriríamos en otro acceso al siguiente nivel
//
// Por simplicidad en la traza pueden suponer una operación 'M' de modificar que haría un "load" y un "store" a la vez
// Cambiando las dos primeras líneas por: M 0,8
//
// ./cacheSim.out -c 4 -s 1 -b 16 -t1 13 -t2 230 -y no-write-allocate -w write-through -r lru < Traza-1.txt

L 0
S 0 8
L 0
S 0 8
L 0
S 0 6
L 0
S 0 8
L 0
S 0 5
L 0
S 0 2
L 0
S 0 8
L 0
S 0 6
```
