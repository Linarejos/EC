# Tema 2 | Nivel de máquina

## Nivel de máquina 1 (Basico)

### Procesadores Intel x86
    Arquitecturas     | Procesadores
    ---------------------------------
    x86-16            | 8086, 286
    x86 / IA32        | 386, 486, Pentium
    MMX               | Pentium MMX
    SSE               | Pentium III
    SSE2              | Pentium 4
    SSE3              | Pentium 4E
    x86-64 / EM64t    | Pentium 4F, Core 2 Duo
    SSE4              | Core i7

### Nueva especie Itanium
  - Itanium
    - Fue el primer intento de 64b, inicialmente denominada IA64.
    - Repertorio inst. nuevo radicalmente, diseñado para altas prestaciones.
    - Puede ejecutar programas de IA32.
  - Itanium 2
    - Aumento de las prestaciones.
  - Itanium 2 Dual-Core
  - Itanium no ha cuajado en el mercado
    - Sin compatibilidad ascendente, sin apoyo del compilador.
    - Pentium 4 salió demasiado bueno.

### Definiciones
  - Arquitectura: (también arquitectura del repositorio de instrucciones): Las partes del diseño de in procesador que se necesitan entender para describir codigo ensamblador.
  - Microarquitectura: Implementación de la arquitectura.
    - Ejemplos: Tam de la cachés y frecuencia de cores.

### Registros Enteros (IA32)

    [%eax[ %ax[ %ah | %al ] ]] acumulador
    [%ecx[ %cx[ %ch | %cl ] ]] contador
    [%edx[ %dx[ %dh | %dl ] ]] datos
    [%ebx[ %bx[ %bh | %bl ] ]] base
    [%esi[ %si[           ] ]] indice fuente
    [%edi[ %di[           ] ]] indice destino
    [%esp[ %sp[           ] ]] puntero pila
    [%ebp[ %bp[           ] ]] puntero base.

### Combinaciones con mov
    Imm -> Reg --- movl $0x4  , %eax   ---  tem = 0x4
    Imm -> Mem --- movl $-147 , (%eax) ---  \*p = -147
    Reg -> Reg --- movl %eax  , %edx   ---  temp2 = temp1
    Reg -> Men --- movl %eax  , (%eax) ---  \*p = temp
    Mem -> Reg --- movl (%eax), %edx   ---  temp = \*p

### Modos Direccionamiento a Memoria completos
La forma general para uno de los operandos anteriores, cuando se trabaja con memoria es: D(Rb, Ri, S) -> Mem[Reg[Rb]+S*Reg[Ri]+D] donde:
  - D: Desplazamiento, const 1,2 ó 4bytes
  - Rb: Registro base, cualquiera de los 8 registros enteros
  - Ri: Registro indice, cualquiera excepto %esp, no se suele usar %ebp
  - S: Factor de escala, 1,2,4 ú 8

Hay casos especiales como:
  - (Rb,Ri)   - Mem[Reg[Rb]+Reg[Ri]]
  - D(Rb,Ri)  - Mem[Reg[Rb]+Reg[Ri]+D]
  - (Rb,Ri,S) - Mem[Reg[Rb]+S*Reg[Ri]]

### Tamaños de los objetos en C
    Tipo de datos   | Normal 32b | Intel IA32 | x86-64
    unsigned        |     4      |      4     |   4
    int             |     4      |      4     |   4
    long int        |     4      |      4     |   8
    long long int   |     8      |      8     |   8
    char            |     1      |      1     |   1
    short           |     2      |      2     |   2
    float           |     4      |      4     |   4
    double          |     8      |      8     |   8
    long doble      |     8      |    10/12   |   16
    punteros *      |     4      |      4     |   8

### Registros de x86-64

    [%raw [%eax]]   [%r8  [%r8d]]
    [%rbw [%ebx]]   [%r9  [%r9d]]
    [%rcw [%ecx]]   [%r10 [%10d]]
    [%rdw [%edx]]   [%r11 [%11d]]
    [%rsi [%esi]]   [%r12 [%12d]]
    [%rdi [%edi]]   [%r13 [%13d]]
    [%rsp [%esp]]   [%r14 [%14d]]
    [%rbd [%ebp]]   [%r15 [%15d]]

Existen los registros anteriores y añade 8 nuevos. Hace %ebp / %rbp de proposito general.

### Codigos en 32 y 64 bits
Hay diferentes diferencias entre las que podemos encontrar:
  - En 64 bits no hay que ajustar la pila, en 32 sí: pushl %ebp, movl %esp, %ebp, pushl %ebx
  - En 64 bits no hay que eliminar lo que se ha hecho con %ebx y %ebp. Con 32 directamente ret.
  - En 64 bits los parametros van en orden a: %rdi y %rsi

### Uso de los sufijos con las intrucciones

* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *

## Nivel de máquina 2 (Aritmetica Control)

### Algunas operaciones aritméticas
        Formato       |     Operación *
    addl    Src,Dest  |  Dest = Dest + Src
    subl    Src,Dest  |  Dest = Dest - Src
    imull   Src,Dest  |  Dest = Dest * Src
    sall    Src,Dest  |  Dest = Dest << Src
    sarl    Src,Dest  |  Dest = Dest >>A
    shrl    Src,Dest  |  Dest = Dest >> Src
    xorl    Src,Dest  |  Dest = Dest ^ Src
    andl    Src,Dest  |  Dest = Dest & Src
    orl     Src,Dest  |  Dest = Dest | Src
    incl    Dest      |  Dest = Dest + 1
    decl    Dest      |  Dest = Dest - 1
    negl    Dest      |  Dest = - Dest
    notl    Dest      |  Dest = ~Dest

### Códigos de condición (Implicito)
 - CF Flag Acarreo (p/ sin signo)
 - SF Flag de Signo (para ops. con signo)
 - ZF Flag de Cero
 - OF Flag Overflow** (ops. con signo)

Ajustados implícitamente por las operaciones aritméticas, por ejemplo: addl/addq Src,Dest. No son afectados por lea.

### Códigos de condición (Explicito)
Para el ajusta explicito se usa la instrucción compare y test.
  - cmpl/cmpq Src2, Src1
  - cmpl b, a que equivalea **restar** a-b
  - testl/testq Src2, Src1
  - testl b,a equivale a hacer a&b pero sin ajustar el destino

### Consultado los códigos de condición
Ajusta un byte suelto según el código de condición (combinación deseada).

    SetX    |   Condición     |   Descripción
    -------------------------------------------
    sete    |   ZF            |   Equal / Zero
    setne   |   ~ZF           |   Not Equal / Not Zero
    sets    |   SF            |   Sign (negativo)
    setns   |   ~SF           |   Not Sign
    setl    |   (SF^OF)       |   Less (signo)
    setge   |   ~(SF^OF)      |   Greater or Equal (signo)
    setg    |   ~(SF^OF)&~ZF  |   Greater          (signo)
    setle   |   (SF^OF)|ZF    |   Less or Equal    (signo)
    setb    |   CF            |   Below            (sin signo)
    seta    |   ~CF&~ZF       |   Above            (sin signo)


Lo que haces guardar en el registro deseado el recultado del test o cmp para poder trabajar con el resultado, por ejemplo una función que devuelva si un número es menor o igual a otro.

    cmpl %esi, %edi
    setg %al
    movzbl %al, %eax

### Intrucciones de saltos
Saltar a otro lugar del código si se cumple el código de condición.

    jX    |     Condición     |    Descripción
    -------------------------------------------
    jmp   |     1             |   Incondicional
    je    |     ZF            |   Equal / Zero
    jne   |     ~ZF           |   Not Equal / Not Zero
    js    |     SF            |   Sign (negativo)
    jns   |     ~SF           |   Not Sign
    jg    |     ~(SF^OF)&~ZF  |   Greater (signo)
    jge   |     ~(SF^OF)      |   Greater or Equal (signo)
    jl    |     (SF^OF)       |   Less (signo)
    jle   |     (SF^OF)|ZF    |   Less or Equal (signo)
    ja    |     ~CF&~ZF       |   Above (sin signo)
    jb    |     CF            |   Below (sin signo)

### Bucles de C a ASM

  - Do-while

      do{               loop:
        body              body
      }while(test)        if (test) goto loop

  - While, la expresión en C es while (test) { body } que lo pasamos a do while

      if(!test) goto done        if (!test) goto done
      do{                        loop:
        body                      body
      }while(test);               if (test) goto loop
                                   done:
      done:

  - For, se puede transdorma en while facilmente:

      for (Init: Test; sdUpdate)          Init;
        body                            while (test){ body; update; }

      **El init iría arriba del if(!test) y el update debajo del body**
      **Aunque la comparación anterior puede quitarse, la de if(!test)***

## Nivel de máquina 3 (Procedimientos)

### Switch
Lo que hace la sentencia switch es basicamente generar una especie de tabla de saltos de 1xN, donde N es el numero de etiquetas del switch. El nombre de casa target se guarda en una especie de vector:

    target = JTab[x];
    goto \*target

Pueden haber dos tipos de saltos:
  - Directo: jmp .L2
  - Indirecto: jmp \*.L7(,%eax,4)

Características notables
  - La tabla de saltos ahorra secuenciar a través de los casos
  - Tiempo constante, en vez de lineal
  - Usar la tabla de saltos para gestionar huecos y etiquetas múltiples*
  - Usar secuenciación del programa para gestionar caída en cascada
  - No inicializar w = 1 a menos de que realmente se necesite

### Funcionamiento de la pila
La pila es una región de memoria gestionada con disciplina de pila que crece hacia posiciones inferiores de la memoria. El registro %esp contiene la dirección más baja de la pila, es decir, la dir del elemento tope.

Hay dos intrucciones que hacen que la pila crezca o decrezca que son:

  - Push Src:
    - Capta el operando en Src
    - Decrementa %esp en 4
    - Escribe el operando en dirección indicada por %esp
    - En definitiva, push hace que la pila crezca en uno introduciendo src en ella.

  - Pop Src:
    - Capta operando de dir. indicada por %esp
    - Incrementa %esp en 4
    - Escribe el operando en Dest

### Llamada a procedimientos
Se usa pila para soportar llamadas y retornos de procedimientos, es decir, llamadas a funciones. Las llamadas se hacen con call label que produce dos acontecimientos:
  - Recordar la dirección de retorno en la pila
  - Salta a la etiqueta label.

La dirección de retorno es la dirección de la siguiente instrucción justo después de la llamada call. Por ejemplo según el siguiente ejemplo:

    804854e: e8 3d 06 00 00   call 8048b90 <main>
    8048553: 50               pushl %eax    

La dirección de retorno = 0x8048553

Con el uso de ret al final de la llamada al procedimiento lo que hacemos es recuperar la dirección de retorno de la pila y salta a esa dirección.

### Lenguajes bajados en pila
Algunos lenguajes que soportan recursividad son:
  - Pascal, Java y C pj.
  - El código es reentrante, multiples instanciaciones de un mismo procedimiento.
  - Se necesita un lugar para guardar el estado de cada instancia, datos, argumentos...

Se usa por tanto la disciplina de la pila, el procedimiento invocado termina siempre antes del que invoca. La pila se reserva en marcos de pila que son estados para una sola instanciación del procedimiento.

### Convenciones de preservación de registros
Cuando el procedimiento yoo llama a who, yoo es el invocante y who es el llamado, no se puede usar un registro de almacenamiento ya que el procedimiento nuevo puede afectar a registros anteriores.

Se usa pues una convección que diferencia entre salva invocados y salvainvocantes:
  - Salva Invocante: El que llama salva valores temporales en su marco antes de la llamada
  - Salva Invocado: El llamado salva valores temporales en su marco antes de reusar regs

Los registros concretamente son:
  - %eax, %edx, %ecx: Invocante salva antes de llamada si los valores se usan después
  - %eax: tb. usado para devolver v. entero
  - %ebx, %esi, %edi: Invocado salva si quiere usarlos
  - %esp, %ebp forma especial de invocado-salva. Restaurados a su valor original al salir del procedimiento

### Procedimientos (x86-64)

## Nivel de máquina 4 (Array/Structs)

### Arrays

### Estructuras

## Nivel de máquina 5 (Alineamiento)
