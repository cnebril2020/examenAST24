# Paso a paso para generar documentación con Doxygen

## 1. Crear un archivo de configuración de Doxygen

Primero, crea un archivo de configuración básico en el directorio de tu proyecto:

```bash
cd /home/carlosnj/Desktop/AST/teoria/9-GDB-y-Doxygen/9-Doxygen-example/valgrindExample1/
doxygen -g Doxyfile
```

## 2. Configurar el archivo Doxyfile

Puedes editar el archivo manualmente o usar estos comandos para modificar las opciones más importantes:

```bash
# PROJECT_NAME: Define el nombre del proyecto que aparecerá en la documentación generada
# Determina el título principal de la documentación
sed -i 's/PROJECT_NAME           = "My Project"/PROJECT_NAME           = "Valgrind Example 1"/' Doxyfile

# PROJECT_BRIEF: Proporciona una breve descripción del proyecto
# Aparece junto al nombre del proyecto en la documentación
sed -i 's/PROJECT_BRIEF          =/PROJECT_BRIEF          = "This is the first program that demonstrates the differences in memory leaks using Valgrind. In this case, we are referring to "definitely lost" memory."/' Doxyfile

# PROJECT_LOGO: Especifica la ruta a un archivo de imagen para usar como logo del proyecto
# La imagen aparecerá en la esquina superior izquierda de cada página HTML
sed -i 's/PROJECT_LOGO           =/PROJECT_LOGO           = ".\/icono.svg"/' Doxyfile

# OUTPUT_DIRECTORY: Define el directorio donde se generará toda la documentación
# Todos los archivos HTML, LaTeX, etc. se crearán dentro de este directorio
sed -i 's/OUTPUT_DIRECTORY       =/OUTPUT_DIRECTORY       = "doxygen-doc"/' Doxyfile

# DISTRIBUTE_GROUP_DOC: Permite que la documentación de grupo se distribuya a todos los miembros
# Si varios elementos pertenecen a un grupo, la documentación del grupo se replicará para cada miembro
sed -i 's/DISTRIBUTE_GROUP_DOC   = NO/DISTRIBUTE_GROUP_DOC   = YES/' Doxyfile

# EXTRACT_ALL: Extrae todas las entidades, incluso aquellas sin documentación
# Muy útil para generar documentación completa incluso para código no comentado
sed -i 's/EXTRACT_ALL            = NO/EXTRACT_ALL            = YES/' Doxyfile

# EXTRACT_PRIVATE: Incluye en la documentación los miembros privados de clases
# Permite ver detalles internos de implementación que normalmente se ocultarían
sed -i 's/EXTRACT_PRIVATE        = NO/EXTRACT_PRIVATE        = YES/' Doxyfile

# EXTRACT_PACKAGE: Incluye en la documentación los miembros con visibilidad de paquete
# Relevante para lenguajes como Java con el concepto de visibilidad de paquete
sed -i 's/EXTRACT_PACKAGE        = NO/EXTRACT_PACKAGE        = YES/' Doxyfile

# EXTRACT_STATIC: Documenta también los miembros estáticos (static) de clases y archivos
# Útil para documentar variables y funciones que son estáticas en el código
sed -i 's/EXTRACT_STATIC         = NO/EXTRACT_STATIC         = YES/' Doxyfile

# EXTRACT_ANON_NSPACES: Extrae contenido de espacios de nombres anónimos
# Incluye en la documentación elementos declarados dentro de namespaces sin nombre
sed -i 's/EXTRACT_ANON_NSPACES   = NO/EXTRACT_ANON_NSPACES   = YES/' Doxyfile

# WARN_NO_PARAMDOC: Genera advertencias si los parámetros de funciones no están documentados
# Ayuda a asegurar que todos los parámetros tengan su descripción correspondiente
sed -i 's/WARN_NO_PARAMDOC       = NO/WARN_NO_PARAMDOC       = YES/' Doxyfile

# RECURSIVE: Busca archivos de forma recursiva en subdirectorios del proyecto
# Permite documentar proyectos con estructura jerárquica de directorios
sed -i 's/RECURSIVE              = NO/RECURSIVE              = YES/' Doxyfile

# EXCLUDE: Excluye determinados archivos o directorios de la documentación
# En este caso se excluye el README.md para que no forme parte de la documentación
sed -i 's/EXCLUDE                =/EXCLUDE                = README.md/' Doxyfile

# SOURCE_BROWSER: Genera una versión navegable del código fuente
# Permite a los usuarios examinar el código directamente desde la documentación
sed -i 's/SOURCE_BROWSER         = NO/SOURCE_BROWSER         = YES/' Doxyfile

# INLINE_SOURCES: Incluye el código fuente directamente en la documentación
# Muestra el código completo incrustado dentro de la documentación HTML
sed -i 's/INLINE_SOURCES         = NO/INLINE_SOURCES         = YES/' Doxyfile

# REFERENCED_BY_RELATION: Muestra qué entidades hacen referencia a cada elemento
# Genera listas de "Referenciado por" que indican dónde se usa cada función/variable
sed -i 's/REFERENCED_BY_RELATION = NO/REFERENCED_BY_RELATION = YES/' Doxyfile

# REFERENCES_RELATION: Muestra qué entidades son referenciadas por cada elemento
# Genera listas de "Referencias a" que indican qué usa cada función/variable
sed -i 's/REFERENCES_RELATION    = NO/REFERENCES_RELATION    = YES/' Doxyfile

# REFERENCES_LINK_SOURCE: Controla si los enlaces de referencias apuntan al código fuente
# Al establecerlo en NO, los enlaces apuntan a la documentación en lugar del código
sed -i 's/REFERENCES_LINK_SOURCE = YES/REFERENCES_LINK_SOURCE = NO/' Doxyfile

# UML_LOOK: Modifica el estilo de los diagramas de clases para que se parezcan más a UML
# Mejora la presentación visual de las relaciones entre clases
sed -i 's/UML_LOOK               = NO/UML_LOOK               = YES/' Doxyfile

# CALL_GRAPH: Genera gráficos de llamadas para funciones
# Muestra visualmente qué funciones son llamadas por cada función
sed -i 's/CALL_GRAPH             = NO/CALL_GRAPH             = YES/' Doxyfile

# CALLER_GRAPH: Genera gráficos inversos de llamadas para funciones
# Muestra visualmente qué funciones llaman a cada función
sed -i 's/CALLER_GRAPH           = NO/CALLER_GRAPH           = YES/' Doxyfile

# MARKDOWN_SUPPORT: Habilita el uso de sintaxis Markdown en la documentación generada por Doxygen
# Permite utilizar formatos avanzados en los comentarios, como listas, enlaces, y énfasis, para mejorar la presentación del contenido
sed -i 's/^\s*#\?\s*MARKDOWN_SUPPORT\s*=.*/MARKDOWN_SUPPORT = YES/' Doxyfile
```

RAW COMMANDS:
```bash
sed -i 's/PROJECT_NAME           = "My Project"/PROJECT_NAME           = "Valgrind Example 1"/' Doxyfile
sed -i 's/PROJECT_BRIEF          =/PROJECT_BRIEF          = "This is the first program that demonstrates the differences in memory leaks using Valgrind. In this case, we are referring to "definitely lost" memory."/' Doxyfile
sed -i 's/PROJECT_LOGO           =/PROJECT_LOGO           = ".\/icono.svg"/' Doxyfile
sed -i 's/OUTPUT_DIRECTORY       =/OUTPUT_DIRECTORY       = "doxygen-doc"/' Doxyfile
sed -i 's/DISTRIBUTE_GROUP_DOC   = NO/DISTRIBUTE_GROUP_DOC   = YES/' Doxyfile
sed -i 's/EXTRACT_ALL            = NO/EXTRACT_ALL            = YES/' Doxyfile
sed -i 's/EXTRACT_PRIVATE        = NO/EXTRACT_PRIVATE        = YES/' Doxyfile
sed -i 's/EXTRACT_PACKAGE        = NO/EXTRACT_PACKAGE        = YES/' Doxyfile
sed -i 's/EXTRACT_STATIC         = NO/EXTRACT_STATIC         = YES/' Doxyfile
sed -i 's/EXTRACT_ANON_NSPACES   = NO/EXTRACT_ANON_NSPACES   = YES/' Doxyfile
sed -i 's/WARN_NO_PARAMDOC       = NO/WARN_NO_PARAMDOC       = YES/' Doxyfile
sed -i 's/RECURSIVE              = NO/RECURSIVE              = YES/' Doxyfile
sed -i 's/EXCLUDE                =/EXCLUDE                = README.md/' Doxyfile
sed -i 's/SOURCE_BROWSER         = NO/SOURCE_BROWSER         = YES/' Doxyfile
sed -i 's/INLINE_SOURCES         = NO/INLINE_SOURCES         = YES/' Doxyfile
sed -i 's/REFERENCED_BY_RELATION = NO/REFERENCED_BY_RELATION = YES/' Doxyfile
sed -i 's/REFERENCES_RELATION    = NO/REFERENCES_RELATION    = YES/' Doxyfile
sed -i 's/REFERENCES_LINK_SOURCE = YES/REFERENCES_LINK_SOURCE = NO/' Doxyfile
sed -i 's/UML_LOOK               = NO/UML_LOOK               = YES/' Doxyfile
sed -i 's/CALL_GRAPH             = NO/CALL_GRAPH             = YES/' Doxyfile
sed -i 's/CALLER_GRAPH           = NO/CALLER_GRAPH           = YES/' Doxyfile
sed -i 's/^\s*#\?\s*MARKDOWN_SUPPORT\s*=.*/MARKDOWN_SUPPORT = YES/' Doxyfile
```


## 3. Generar la documentación

```bash
doxygen Doxyfile
```

## 4. Ver la documentación generada

La documentación se generará en dos formatos:

HTML: Ideal para navegar en un navegador web
LaTeX: Para generar documentación en PDF (requiere LaTeX instalado)

Para ver la documentación HTML:

```bash
# Para abrir la documentación en el navegador predeterminado
xdg-open html/index.html
```

## 6. Generar PDF (opcional)

Si deseas generar un PDF a partir de la documentación LaTeX:

```bash
cd latex
make
xdg-open refman.pdf
```

## 7. Personalización adicional (opcional)

Si quieres personalizar más la documentación, puedes:

- Añadir un archivo `mainpage.dox` con una página principal personalizada
- Agregar imágenes o diagramas
- Configurar grupos y módulos para organizar mejor el código

Por ejemplo, podrías crear un archivo `mainpage.dox` con:

```c++
/**
 * @mainpage Valgrind Example 1 Documentation
 * 
 * @section intro_sec Introduction
 * This project demonstrates a "definitely lost" memory leak detected by Valgrind.
 * 
 * @section example_sec Code Example
 * The main issue occurs in getRandomInt() function, where memory is allocated but never freed.
 * 
 * @section compile_sec Compilation
 * Compile with: g++ -g -O0 valgrindExample1.cpp -o valgrindExample1
 * 
 * @section run_sec Running with Valgrind
 * valgrind --leak-check=full ./valgrindExample1
 */
```

Y luego volver a generar la documentación con `doxygen Doxyfile`.