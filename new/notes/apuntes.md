## Siguientes pasos a seguir en la práctica global

1. Gestionar cómo leer los datos de la base de datos de Usuarios que será un archivo de texto plano ('UserDatabase.txt').

    > Lo que he hecho (implementado en el .cpp) hasta ahora:
    1. Clase 'User.h' al completo (constructor protected).
    2. Clases 'Admin.h' y 'Employee.h'.
    3. File managment de 'UserDatabase.h'

> CAMBIAR LA LECTURA Y ESCRITURA A MODO BINARIO (MUCHO MÁS EFICIENTE). EL ADMINISTRADOR DEL SISTEMA PODRÁ VER LOS USUARIOS EN EJECUCIÓN.
> EN LA CLASE 'USER.H' MODIFICAR EL CONSTRUCTOR VIRTUAL PARA QUE LO HEREDEN LOS HIJOS, PERO LES OBLIGO A IMPLEMENTAR CADA UNO EL SUYO. NO SE PUEDE CREAR UN USUARIO GENÉRICO.
> OBLIGAR AL ADMIN A CAMBIAR LA CONTRASEÑA EN LA PRIMERA EJECUCIÓN
> QUE NO SE PUEDA BORRAR UN ADMIN A ÉL MISMO
> EN CASO DE QUE NO EXISTA EL FICHERO DE USUARIOS, HACER QUE LA PERSONA SE LOGEE Y SEA ESE USUARIO EL ADMINISTRADOR DEL SISTEMA

> Idea para usar hilos: crear un hilo para cargar el fichero de usuarios y, otro para cargar el hilo de los sensores. Lo mismo para guardar los archivos.
> Usar ICE para gestionar hilos o hacerlo rudimentario.


2. Con toda la información leída, hacer un <set> de Usuarios (no se pueden repetir objetos, se define sobrecargando el operador '<'), ahí es donde se define el administrador por defecto (instarle a cambiar la contraseña por defecto). Este usuario 'root' puede añadir otros administradores o usuarios normales, haciendo uso de la herencia y el polimorfismo.

2.1. Para usar 'set', como he mencionado, hay que sobrecargar el método '<' para que set pueda comparar entre objetos. Hay que tener cuidado con qué atributo se compara porque de este operador es dónde discrimina el hecho que sean, o no, iguales dos objetos.

3. Aplicar este mismo proceso a los sensores, con la sutil diferencia de usar un <multiset>, donde SÍ se pueden repetir objetos. 

5. USAR 'valgrind' SIEMPRE QUE PUEDA!

6. Implementar la clase 'SensorFactory.h' (cuando acabe con el multiset<Sensor*>)

