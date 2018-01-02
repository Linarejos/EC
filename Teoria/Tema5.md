# Tema 5 | Entrada/salida y buses

## Objetivo de los sistemas de E/S
Realizar la conexión del procesador con una gran variedad de dispositivos periféricos, teniendo en cuenta que las características de los dispositivos de E/S suelen diferir notablemente de las del procesador; en especial:
  - Valocidad de transmisión de los perifericos
  - Long. de la palabta
  - Lo cófigos para representar los datos.

Para compativilizar las caracteristicas los dispositivos de E/S ofrecen circuitos de interfaz o controladores de perifericos.

Las funciones que deben incluir el sistema de E/S:
  - **Direccionamiento o selección de perifericos**, existen dos técnicas:
    - **Por selección lineal**: asignar un bit del bus de direcciones a cada puerto.
    - **Por decodificación**: decodificar los bits de dirección para seleccionar un puerto de interfaz. (Centralizada: un único decodificador; En cada puerto de E/S: cada puerto reconoce su propia dirección).
  - **Comunicación fisica entre el periférico y el procesador**:
    - **Usando bues**: Permite conectar en paralelo y es más facil de expandir el sistema.
    - **Usando Multiplexor/demultiplexor**: Multi de entrada y demu para salida. Expansión dificil y mucha circuitería.
  - **Sincronización**: Consiste en el acomodamiento de velocidades, se necesitan mecanismo para saber cuando recibir/enviar datos. Incluyen: buffer y señales de control.
    - **Sincrona**: Deposita el dato y fenera al mismo tiempo señal para escribir estableciendo un tiempo de dato válido y disponible. Para la lectura sumistra una señal de lectura y apartir de ello se dispone un tiempo para ver el dato en el bus de datos.
    - **Asincrona**: O handshaking, se necesita una nueva señal de aceptación ACK con la que el controlador del periferico contesta a la petición de transferencia generada por el procesador. Se usa el acuse y recibo. Las ventajas son: conectar dispositivos con distintos requisitos de tiempo y se tiene una mayor ganancia de que el dato sea válido. Además es necesario establecer un periodo de tiempo máximo.

### Conceptos a diferenciar:
  - **Transferencia elemental de información**: Envio/recepción de una unidad de información, ya sea un datos o una palabra de estado o control.
  - **Operación completa E/S**: Transferencia de un cojunto de datos, seector de un disco y linea de pantalla.
  - **Dispositivos E/S físicos**: Cuando el ordenador carece de SO o drivers, el programador debe tratar directamente con ellos asumiendo detalles del funcionamiento o características.
  - **Dispositivos E/S logicos**: El programador efectua transferencias actuvando rutinas de E/S que proporciona el SO.
  - **E/S aislada o independiente**: El procesador distingue entre el espacio de memoria y el espacio de E/S.
  - **E/S mapeada en memoria**: El procesador no distingue entre accesos a memoria y accesos a los disp. de E/S.

### E/S independiente:
Se emplea la patilla IO/M# del procesador.
  - **Nivel alto**: Indica a la memoria y a los disp. que se va a efectuar una operación de E/S, el ejecutar operaciones como **IN/OUT**.
  - **Nivel bajo**: Operación de intercambio de datos con mem., al ejecutar operaciones como LOAD, STORE o MOVE.

Las intrucciones IN y OUT tienen poca riqueza de direccionamiento. Si el pocesador usa 8bits para puertos puede direccionar 2⁸ puertos.

Ventajas:
  - Diseño más limpio de la decodificación de las direc. de memoria.
  - Facilita la protección de E/S (por ejemplo, haciendo que las instrucciones IN, OUT,... sean privilegiadas).
  - Los programas son relativamente más rápidos por la decodificación más sencilla y el menor tamaño de las instrucciones de E/S

Desventajas:
  - Mayor complejidad en el diseño del procesador.

### E/S mapeada a memoria:
Se usan algunas direcciones de memoria para acceder a los puertos de E/S, tras decodificarlas adecuadamente. El procesador no distingue entre si se accede a memoria o a los disp. No se dispone de instrucciones especiales. Para evitar paricionar el mapa dedicado a memoria se agrupa la E/S en una zona bien definiza.

Ventajas:
  - Menor complejidad en el diseño del procesador.

Desventajas:
  - Cada puerto ocupa una dir de memoria.
  - Las intrucciones de acceso a memoria suelen ser más largas que las especificas de E/S
  - Puede disminuir la velocidad de procesamiento
  - Aumentan los requisitos de memoria.

### Tecnicas de E/S

  ![Tecnicas_e/s](http://)

 - **Programada**: El procesador participa activamente ejecutando instrucciones en todas las fases de una operación de E/S.
 - **Controlada porinterrupciones**: Los dispositivos de E/S se conectan al procesador a través de líneas de petición de interrupción, que se activan cuando los dispositivos requieren los servicios del procesador. El procesador suspende el programa y ejecuta el tratamiento de las interrupciones
 - **DMA**: Requiere la presencia de un controlador DMA, que puede actuar como controlador del bus y supervisar las transferencia de datos entre MP y uno o más dispositivos de E/S, sin intervención directa del procesador salvo en la inicialización.


### E/S programada
Todos los pasos de una operación requieren la ejecución de instrucciones por el procesador. La transderencia de un byte se realiza mediante la ejecución de una instruccion donde un operando es un registro del controlador (puerto) y el otro un registro del procesador o pos de memoria. Al decodificar la instrucción la UC envia una serie de datos (sobre que perfiferico se realiza, tipo de ooperación, temporización).

  - **Como salida**: El procesador ejecuta una instrucción de caga de una palabra de memoria a registro. El procesador ejecuta una instrucción de salida que transfiere el dato desde el procesador a unpuerto de salida.

  - **Como entrada**: El procesador ejecuta una instrucción de entrada y una instrucción de almacenamiento.

Existen dos modelos de programada que son:
  - **Sin consulta de estado**: El procesador decide en que momento se realiza la tranferencia, el disp debe e tar entonces siempre dispuesto para recibir datos o enviarlos.  
  - **Con consulta de estado**: El porcesador pregunta al periférico atraves de una interfaz si está preparado, es parecido al concepto handshaking pero por software.

### Interrupciones
Son bifurcaciones normalmente externas al programa, excepto las software, provocadas por diversas causas. Pueden externas, las señales provienen de fuera del procesador o internas, es el propio procesador. El objetivos es raclamar la atención de este por algún acontecimiento pidiendo que ejecute un tratamiento especifico para ello. De esta manera la ejecución del programa queda suspendida.

**Causas de las interrupciones**:
  1. Fallo hardware.
  2. Errores del programa.
  3. Condiciones de tiempo real.
  4. Entrada / Salida

Clasificación según la procedencia:
  1. **Externas**: Se inicia a petición de disp. externos y pueden ser:
    - **Enmascarable**s: Se pueden habilitar o inhibir (interrupciones E/S)
    - **No enmascarables**: Tienen mayor prioridad que las anteriores por ejemplo, fallos del hardware externo y condiciones tiempo real.
  2. **Internas (excepciones o traps)**: Se activan de forma interna al procesador mediante condiciones excepcionales, como errores del programa o fallo del hardare interno.
  3. **Interrupciones software**: Las instrucciones de interrupción software se emplean para realizar llamadas al SO.

**¿Cómo determinar la ISR?**
  - **Direcciones fijas**: La dirección se fijan y definen en los circuitos del procesador.
  - **Vectorizadas**: término genérico se refiere a todos los esquemas en los cuales el dispositivo que solicita una interrupción suministra de algún modo la dirección de la rutina de servicio

**Sistemas de prioridad de interrupciones**: En un computador con más de un dispositivo con capacidad de interrupción, hay que establecer mecanismos de prioridad que resuelvan los problemas de: interrupciones simultaneas, iterrupciones anidadas e inhibición de interrupciones.
  - **Simultaneas**:
    - **Gestión de prioridades centralizada**: Cuando hay un solo dispositivo en cada línea de interrupción, la CPU acepta la solicitud que llega por la línea de mayor prioridad.
    - **Gestión de prioridades distribuida**: Varios dispositivos comparten una única línea de solicitud de interrupción.
      - **Técnica sonde o polling**: se usa para determinar por software el origen de una interrupción. Encuesta activa.
      - **Técnica daisy-chain**: El dispositivo que esté eléctricamente más cercano a la CPU tendrá la mayor prioridad.
    - **Gestión de prioridades híbrida**: Combinación de esquemas centralizado y distribuido (daisy-chain).
  - **Anidadas**: se produce una interrupción mientras se está atendiendo otra ¿que hacer?
    - Inhabilitar las interrupciones durante el tratamiento de la ISR.
    - Permitir que acepte una segunda solicitud si si prioridad es mayor.
  - **Inhibición de interrupciones**: situaciones en las que conviene evitar temporalmente que se produzcan interrupciones. Existen tres niveles:
    1. Desactivar todas la interrupciones.
    2. Desactivar interrupciones de inferior o igual prioridad.
    3. Desactivar de forma selectiva determinados niveles de interrupción









-
