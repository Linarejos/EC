# Tema 1 | Introducción

## Organización en bytes memoria

Distinguimos entre:
  - Palabra: cada una de las posiciones de memoria
  - Long. Palabra: Cuanto cabe en cada posición de Palabra

De esta dorma una memoria de 256 palabras X 16bits es lo mismo que 512palabras de 8 bits. Nacen dos formas de ordenar en memoria, pj con ab75 2356:
  - Little-endian: Primero se almacena el bit menos significativo (LSB)
    75 ab - 56 25
  - Big-endian: Primero el más significativo (MSB)
    ab 75 - 25 56


## Alineación en memoria de bytes
Las palabras de n butes alineada comienda si o si en una dirección múltiplo de n.
  - Algunas CPU rwquieren alineación en los accesos a memoria sino se produce un error en el bus.
  - Otras acceden más rápido si el acceso es alienado.

Esto quiere decir que si nuestra memoria direcciona en palabras de 1 byte si el datos requiere 4 bytes, esta palabra se guardará en las posiciones multiplo de 4, es decir 0, 4, 8, 12, 16, ...

## Clasificaciones m/n y pila-acumulador-RPG
La clasificacion m/n significa que la ALU admite n operandos y m de ellos son de memoria. Las combinaciones típicas son:
  - 0/0 Máquinas de pila: Repertorio Push M, Pop M, Add, And..
  - 1/1 Máquinas de acumulador: Con un registro más rápido que M, Load M, Store M, Add M, And M ...
  - x/2,x/3 Máquinas de RPG: Multiples registros, Repertorio Move R/M R/M, Add R/M R/M R/M

### Clasidicación R/M
Solo para máquinas de registros de proposito general.
  - R/R: Registro a registro.
    - 0/2, 0/3. **Típico RISC**
  - R/M: Registro a memoria
    - 1/2, 1/3. **Típico CISC**
  - M/M: Memoria a Memoria
    - 2/2, 3/3: Permite operar directamente a memoria pero demasiados accesos.

## Sobre los Repertorios
  - RISC: Reduced Instruction Set Computer
    - Pocas intrucciones, pocos modos y formato sencillo.
    - UC es decilla y muchos registros.
  -CISC: Complex Instruction Set Computer
    - Muchas instrucciones, complejas y pocos registros, proximas a lenguaje alto nivel.
    - Debate RISC/CISC agotado -> diseños misxtos.


## Fases del ciclo de ejecución de instrucciones
  - Captación: Leer posición de la siguiente instrucción máquina IR <- M[PC]
    - Se usa MAR/MBR, IR
    - Se interpreta como codop
    - Incrementa PC
  - Decodificación: Deglosa el codop en operandos (registros)
    - Posible etapa, tiene que acceder a memoria: capta dato e incrementa PC
  - Ejecución: llevar datos a ALU y operar
  - Almacenamientos: salvar resultados en los registros y/o memoria.

## Modos de Direccionamiento
  - Inmediato: $0, $variable
  - Registro: $eax, $edx
  - Memoria: disp(%base,%index,%scale)
    - Directo: solo dirección (disp) op=M[dis]
    - Indirecto: sólo registro (reg) op=M[reg]
    - Relativo a base: registro y desplazamiento op=M[reg+disp]
    - Indexado: índice (x escala) y dirección op=M[disp + index*scale]
    - Combinado: todo op=M[disp+base+idx*sc]

## Estructura del bus.
  - Buses paralelos: Varios buses conectan E/S, M, CPU.
    - Bus direcciones m bits: palabras n bits M/ALU
    - Bus datos de n bits: direcciones m bits M
    - Bus control: para líneas UC (R/W, etc)
  - Bus único: Un único bus que lo conecta todo.
    - CPU escribe bus dirección y control R/W.
    - E/S y M comprueban si es su dirección
    - Ventaja: Sencillez, bajo coste y flexibilidad.
  - Buses múltiples: Tipicamente dos buses, del sistema (CPU/M) y bus E/S
    - Puede haber multiples E/S o dos buses del sistema (arq. Harvard)
    - Ventajas: uno más rápido, ambos funcionan en paralelo
    - Desventaja: coste y complejidad.

## Medida del rendimiento
  - SPEC:  System Performance Evaluation Corporation
    - Tests: CPU92, CPU95, CPU2000, CPU2006
    - CPU2006:
      - vel_gzip = Trefgzip/Tgzip **uSPARC II**
      - vel_SPEC = n^√(∏i vel_prgi) **media geo**

## Perspectiva Historica
  - 1ª Generación (45-55): tubos de vacío
    - von Neumann: concepto prog. almacenado
    - M: líneas retardo mercurio, núcleos magneticos
    - E/S: tarjetas, cintas magnéticas
  - 2ª Generación (55-65): transistores,
    - procesadores E/S (cintas) en paralelo
    - FORTRAN como compilador
  - 3ª Generación (65-75): Circuito Integrado
    - arquitectura: μProgr, segm.cauce, M cache
    - software: SO multiusuario, memoria virtual
  - 4ª Generación (75-…): VLSI, μProcesador: procesador completo en 1 chip
    - arquitectura: mejoras segm. cauce, cache, M virtual
    - hardware: portátiles, PCs, WS, redes
    - todos los 8008..
