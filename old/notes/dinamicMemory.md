# Memoria dinámica

Esto es un archivo para tener un control sobre la memoría dinámica de mi programa y cómo gestionarla. Voy a enumerar todas las asignaciones que vaya haciendo de forma cronológica para intentar tener el programa bajo control.

1. Se asigna un usuario ```Admin()``` por defecto a la hora de crear una base de datos. Se tiene que limpiar toda la memoria dinámica a la hora de llamar al destructor cuando se acabe el programa. También, si se elimina un usuario, se tiene que hacer ```free``` de ese usuario.

valgrind --leak-check=full --show-leak-kinds=all ./valgrindExample1

# Punteros Inteligentes

Hace, por debajo, lo mismo que los punteros en C. Es decir, cuando quieres pasar al siguiente elemento, miras cuanto ocupa la estructura y avanzas esas posiciones de memoria. Pues eso lo gestionan por debajo los punteros inteligentes, típicamente se sulen demoninar en C++ 'it'.

## Tipos de datos en C++ 
unsigned -> siempre se aprovechan todos los bits (no tiene signo).

Cuando definimos tipos podemos forzar a que almacene el número de bits que quiero utilizar par que, en distintos procesadores, se  obtenga el mismo resultado ('uint32' -> unsigned int de 32 bits. Caben 2^32 números en total). 

Siempre forzar a utilizar el tipo de datos más ajustado a la variable que queramos usar.

En el entorno real, al poner 'int' (por ejemplo), no sabemos realmente cuántos bits utiliza la memoria para almacenar esa variable.Siempre queremos tener el programa bajo control.