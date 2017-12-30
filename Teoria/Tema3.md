# Tema 3 | Unidad de Control

## Camino de datos

### Arquitectura von Nemann
Consta de tre bloques fundamentales:
  - CPU
  - Memoria principal
  - Unidad de E/S

Todos ellos unidos mediante buses.

### Unidad de procesamiento
Comprende el hardware requerido para la ejecución de las instrucciones de un programa. La unidad de control es la encargada de gestionar todo ello. La Unidad de procesamiento está compuesta por:
  - Unicades funcionales: ALU, desplazad., multiplic...
  - Registros: de uso general, de estado, PC, SP, IR, MDR/MBR y MAR.
  - Multiplexores
  - Buses internos

### Unidad de control
La unidad de control interpreta y controla la ejecución de las intrucciones leidas de la memoria principal en dos dases:
  - Captación:
    - La UC lee de MP las intrucciones apuntadas por PC, IR <- M[PC]
    - Determina la dirección de la signte instrucción también.
  - Ejecución:
    - La UC reconoce el tipo de instrucción
    - Manda las señales necesarias para tomar los operandos y dirigirlos a las unidades funcionales.
    - Manda las señales necesarias para realizar la Operación
    - MAnda las señales necesarias para enciar los resultados a su destino.

### UP con 1 bus vs multiples
La diferencia principal es que con múltiples buses los registros pueden depositar sus datos "paralelamente" en los buses ya que lo hacen en buses distintos. En cambio con un único bus esto sería imposible ya que un registro solo podrá depositarlo si el bus no está ocupado.

Por otro lado se elimina la necesidad de registro Y y Z para la ALU ya que los tomará del bus sin que tenga q existir buffer como en los de 1 solo bus.

## Diseño de la UC

### Señales de la UC
Por un lado tenemos señales de entrada que son:
  - Señal de reloj
  - Instrucción acutal
  - Estado de la UC
  - Señales externas

Las de salida serían:
  - Señales que van a gobernar la UP: carga registros, incrementos registros, desplazamientos, selección de entrada multiplexores.
  - Selañes externas como las de  lectura/escritura en memoria.

### Tipos de diseño:
  - Control fijo o cableado:
    - Se emplean métdos de diseño de circuitos digitales secuenciales a partir de diagramas de estado.
    - El circuito final se obtiene conectando componentes básicos
  - Control microprogramado:
    - Todas las señales que pueden activarse simultáneamente se agrupan para formae palabras de control, que se almacenan en una memoria.
    - Una instrucción de lenguaje máquina se transforma sintacticamente en un programa (micro) almacenado en la memoria de control, esto implica: mayor facilidad de diseño para instrucciones complejas.


### Diseño de UC cableada.
Se diseña mediante puertas lógicas y biestables siguiendo uno de los métodos clásicos de diseño de sistemas digitales secuenciales. El diseño es laborioso y dificil de modificar debido a la complegidad de los cicuitos. Suele ser más rápida que la micropogramad. Se usa una PLA para llevar a cabo la implementación.

Una de las técnicas es el diseño por computador (CAD) para circuitos VLSI (compiladores de silidio), que resuelven automaticamente la mayor parte de las dificultades de diseño de lógica cableada. Genera directamente las máscaras de fabricación a partir de descriptores del comportamiento funcional en un lenguaje de alto nivel.

Los biestables contienen información relativa al estado en el que se encuentra el sistema. La PLA usa esta información de estado junto a las entradas externas para generar el sig estado.

Ventajas:
  - Minimización del esfuerzo del diseño
  - Mayor flexibilidad y fiabilidad
  - Ahorrro de espacio y potencia

Pasos:
  1. Definir una máquina de estados finitos. (Modeo Mealy)
  2. Describir dicha máquina en un lenguaje de alto nivel
  3. Generar la tabla de verdad para la PLA. Dos tipos de modelos: Mealy o Moore, salidas dependen de entradas y estado actual o salidas dependen solo del estado actual
  4. Minimizar la tabla de verdad. Con algoritmos.
  5. Diseñar físicamente la PLA partiendo de la tabla de verdad


### UC microprogramada

Definiciones:
  - Microinstrucción: cada palabra de la memoria de control
  - Microprograma: conjunto ordenado de microinstrucciones cuyas señales de control constituyen el cronograma de una (macro)instrucción del lenguaje máquina.
  - Ejecución de un microprograma: lectura en cada pulso de reloj de una de las microinstrucciones que lo forman, enviando las señales leídas a la unidad de proceso como señales de control.
  - Microcódigo: conjunto de los microprogramas de una máquina.

**Ventajas**:
  - Simplicidad conceptual. Información de control reside en una memoria.
  - Se pueden introducir nuevas intrucciones sin que sea algo complejo.
  - Correcciones, modificaciones y apliaciones más faciles.
  - Permite contruir computadores con varios juegos de instrucciones.

 **Desventaja**: Lentitud frente a la cableada debido a la menor capacidad para expresar el paralelismo de las intruscciones.


## Control microprogramado

### Formato de las microinstrucciones
Las señales que gobiernan la gestión de la unidad de procesamiento se suelen agrupar en campos. El formato es una especie de vector con un bit por señal.

Para acortar el tamaño de las microinstrucciones, estas se codifican, todos o algunos de sus campos usando un decodificador. El inconveniente es precimanete el uso de hardware adicional que produce euna menor capacidad para expresar el paralelismo. Una solución sería añadir dos decodificadores.

Econtramos dos formas de realizar las microinstrucciones:
  - **Microprogramación horizontal**:
    - Ninguna o escasa codificación
    - Capacidad para expresar un talto grado de paralelismo.
    - Microinstrucicones largas
  - **Microprogramación vertical**:
    - Mucha codificación
    - Microinstrucciones cortas
    - Escasa capacidad para expresar paralelismo

### Nanoprogramación
Surge con el objetivo de reducir el mamaño de la memoria de control e implica crear dos niveles nuevo de memoria: de control y nanometria.

El tamaño del programa por microprogramación suele ser n*m que se remplaza por una primera memoria: n*log_2(m) y otra donde están las m instrucciones diferentes que solo se incluyen una vez. La primera memoria es una especie de tabla de punteros hacia las instrucciones únicas de la segunda.

### Secuenciamiento explicito e Implicito
El secuenciamiento explicito:
  - El encadenamiento con una nueva instrucción se resuelve con la señal de control del multiplexor de direcciones.
  - Las primeras palabras en la memoria de control pueden albergar la primera μinstrucción de cada instrucción, justo en la dirección que expresa el valor de su código de operación
  - Cada μinstrucción contiene la dirección de la μinstrucción siguiente
  - **Inconveniente**: gran cantidad de memoria empleada en el secuenciamiento (dirección de la siguiente microinstrucción)

El secuenciamiento implicito:
  - El encadenamiento con una nueva instrucción se resuelve con la señal de control del multiplexor de direcciones.
  - Las μinstrucciones de cada microprograma están ordenadas en posiciones consecutivas de la memoria de control
  - Es necesaria una etapa traductora entre el código de operación y el multiplexor de direcciones
  - Es necesario un registro de direcciones y un incrementador
  - **Inconveniente**: esta estructura sólo permite ejecutar programas lineales

### Camino de datos (MAR y MBR)
  - MAR: Memory Adress Register
    - Contiene direcciones
    - Conectado al Bus de direcciones
  - MBR: Memory Buffer Register
    - Se usa para leer/escribir datos en memoria
    - Conectado al bus de datos














  -
