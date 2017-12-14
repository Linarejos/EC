# Practica 3
En esta práctica incrsustaremos código ASM en código C. Realizaremos tres
programas distintos que serán:

### Popcount
Este programa cuenta el número de unos de una lista. De este mismo
realizaremos seis versiones que serán:

  - For   (C): En cada iteracción analizamos un elemento del array. Una vez
  lo tengamos en la variable X usamos otro bucle para acumular en una variable
  local "res" si existe o no un 1 en la poss final del numero. Tras esto
  introducimos ceros por la izquierda desplazando todos sus unos, así, para
  01101 pasará a 00110 -> 00011 -> 00001 -> 00000.

    ``` [C]
    int popcount1 (unsigned* array, int len){
      int  i,  j,   res=0;
      unsigned x;

      for (i = 0; i < len; i++){
         x = array[i];
         for (j = 0; j < 8 * sizeof(unsigned); j++){
           res += x & 0x1;
           x >>= 1;
         }
      }
       return res;
    }
    ```

  - While (C): Misma idea que antes pero ahora la tener un do ·· while nos
  da la oportunidad de parar el buble interno un poco antes, una vez ya se
  hayan contado todos los unos. Si tuviesemos que vontar: 000100, solo
  desplazaría tres veces y acaba con ese número, en la versión anterior
  continuaría con el for.

    ``` [C]
      int popcount2 (unsigned* array, int len){
        int  i, res=0;
        unsigned x;

        for (i=0; i<len; i++){
          x = array[i];
          do{
            res += x & 0x1;
            x >>= 1;
          }while(x);
        }
          return res;
      }
    ```

  - Do While (ASM): El funcionamiento es basicamente un do ·· while pero como
  la versión anterior. Inicialmente se desplaza x a la derecha, y se hace adc.
  Esta instrucción lo que proboca es sumar en r (result) 0 + CF, tras esto
  se comprueba si x no es 0, o sea, si no he acabado de contar unos.
  SHR activa el carry flag si lo que ha salido del vector es un 1.

    ```[asm]
      int popcount3 (unsigned *array, int len){
        int i;
        unsigned x;
        int result = 0;
        for(i=0; i<len; i++){
          x=array[i];
          asm("           \n  "
          "ini3:          \n\t" // seguir mientras que x!=0
          "shr %[x]       \n\t" // Desplaza todos los bits a la derecha
          "adc $0x0,  %[r]\n\t" // Si se activa el bit de signo sumo carry+result
          "test %[x], %[x]\n\t" // Se hace para activar o no el ZF
          "jnz ini3"            // Salta si no es cero
          :[r] "+r" (result)
          :[x] "r" (x));
        }
        return result;
      }
    ```

  - Mask 8bits (C): El procedimiento es parecido al del pcount2 pero la
  diferencia principal es por cada iteracción lo que hago es como hacer una
  mask = 1 pero 4 veces, 4x8 = 32, por lo que nos ahorramos muchas iteraciones.
  Por ejemplo, si tenemos:     
        01011011|01011100|01100001|11101010
      & 00000001|00000001|00000001|00000001
  Con x>>1 lo que hago es meterle un 0 a 0 para seguir contando en la siguiente
  iteracción.

    ```[c]
      int popcount4 (unsigned* array, int len){
        int  i, j, result=0;
        unsigned x, val;

        for (i=0; i<len; i++){
          val = 0;
          x=array[i];
          for (j=0; j<8; j++) {
            val += x & 0x01010101;
            x>>=1;
          }
          val+=(val>>16);
          val+=(val>>8);
          val&=0xFF;
          result += val;
        }
        return result;
      }
    ```

  - SSSE3: En esta versión se utilizan los registros de 64 para paralelizar
  la suma. La explicación completa la aporta Antonio Cañas como un archivo
  plano de texto que se ve graficamente.

    ```[c]
      int popcount5 (unsigned* array, int len){
        int i, val, result=0;
        int SSE_mask[] = { 0x0f0f0f0f, 0x0f0f0f0f, 0x0f0f0f0f, 0x0f0f0f0f};
        int SSE_LUTb[] = { 0x02010100, 0x03020201, 0x03020201, 0x04030302};
        //                    3 2 1 0,    7 6 5 4,   1110 9 8,  15141312
        if(len &0x3)
          printf("Leyendo 128b pero len no multiplo de 4?\n");

        for(i=0; i<len; i+=4){
          asm("movdqu  %[x],   %%xmm0      \n\t" // Copio el array dos veces
              "movdqa  %%xmm0, %%xmm1      \n\t" // Tanto en 0 como en 1
              "movdqu    %[m], %%xmm6      \n\t" // muevo la maskara al 6
              "psrlw       $4, %%xmm1      \n\t" // Cambiamos a doubleword
              "pand    %%xmm6, %%xmm0      \n\t" // Aplico AND con la mascara
              "pand    %%xmm6, %%xmm1      \n\t" // Igual

              "movdqu    %[l], %%xmm2      \n\t" // Como queríamos dos copias
              "movdqa  %%xmm2, %%xmm3      \n\t" // copio en 2 y en 3
              "pshufb  %%xmm0, %%xmm2      \n\t" // Intercambio los registros
              "pshufb  %%xmm1, %%xmm3      \n\t" // Igual

              "paddb   %%xmm2, %%xmm3      \n\t" // vector de bytes
              "pxor    %%xmm0, %%xmm0      \n\t" // pongo todo a 0 xor
              "psadbw  %%xmm0, %%xmm3      \n\t"
              "movhlps %%xmm3, %%xmm0      \n\t"
              "paddd   %%xmm3, %%xmm0      \n\t"
              "movd    %%xmm0, %[val]      \n\t"
              : [val]"=r" (val)
              : [x]  "m"  (array[i]),
              [m]  "m"  (SSE_mask[0]),
              [l]  "m"  (SSE_LUTb[0])
          );
          result += val;
        }
        return result;
      }
    ```

  - SSSE4: La última versión es usando las funciones propias de popcnt de
  asm.

    ```[asm]
      int popcount7 (unsigned* array, int len){
        int i, val, result = 0;
        unsigned x1,x2;
        if( len & 0x1)
          printf("leer 64b y len impar?\n");
        for(i=0; i<len; i+=2){
          x1 = array[i];
          x2 = array[i+1];
          asm("popcnt %[x1], %[val] \n\t"
              "popcnt %[x2], %%edi  \n\t"
              "add    %%edi, %[val] \n\t"
              : [val] "=&r" (val)
              :  [x1]  "r"  (x1),
                 [x2]  "r"  (x2)
              : "edi");
          result += val;
        }
        return result;
      }
    ```


### Parity
Calcula la paridad impar de todos los elementos del vector. La pridad se
define en que existe un 1 en la posiones impares del tam del vector.

  - For   (C): En cada iteracción analizamos un elemento del array. Una vez
  lo tengamos en la variable X usamos otro bucle para acumular en una variable
  val la cantidad de unos que hay. Se usa XOR para porque solo con 0 ^ 1 pone
  a 1 la pos de val, así acumulamos.
    ```[c]
      int parity1(unsigned* array, int len){
        int  i,  j, var res=0;
        unsigned x;

        for (i = 0; i < len; i++){
          x = array[i];
          val = 0;
          for (j = 0; j < 8 * sizeof(unsigned); j++){
            val ^=x & 0x1;
            x >>= 1;
          }
          res += val;
        }
        return res;
      }
    ```

  - While (C): Seguimos la misma temática, que en el popcnt, ahora
  usamos un do ·· while, para saber cuando no quedan más 1 en el número.
  ```[c]
    int parity2(unsigned* array, int len){
    int  i,     res=0, val;
    unsigned x;

      for (i=0; i < len; i++){
        x = array[i];
        val = 0;
        do
        {
          val ^= x & 0x1;
          x >>= 1;
        }while(x);
        res += val;
      }
      return res;
    ```

  - Mask y shr (C):
  ```[c]
    int parity3(unsigned* array, int len){
      int  i,     res=0, val;
      unsigned x;

      for (i=0; i<len; i++){
        x = array[i];
        val = 0;
        while(x) { val ^= x ; x >>= 1; }
        res += val & 0x1;
      }
      return res;
    }
  ```
  - Mask y shr (ASM):

  - SSSE3 (C y ASM):

  - SSSE3 (ASM):
