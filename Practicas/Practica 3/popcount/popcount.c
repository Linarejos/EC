/******************************************************************************/
/*
/*   El popcount lo que hace es básicamente sumar los unos de una lista,
/*   de un array de elementos que serán enteros sin signo
/*
*/

#include <stdio.h>	 // para printf()
#include <stdlib.h>	 // para exit()
#include <sys/time.h>	 // para gettimeofday(), struct timeval

#define TEST              0
#define COPY_PASTE_CALC   1

#if ! TEST  // Si test es 1 pasa en caso contrario liberar un else.
  #define NBITS 20
  #define SIZE (1<<NBITS)
  unsigned lista[SIZE];
  #define RESULT  (NBITS*(1<<NBITS-1))
#else

/* ----------------------------------------------------------------- * /

#define SIZE 4
unsigned lista[SIZE]={0x80000000, 0x00100000, 0x00000800, 0x00000001};
#define RESULT 4

/* ----------------------------------------------------------------- */

#define SIZE 8
unsigned lista[SIZE]={0x7fffffff, 0xffefffff, 0xfffff7ff, 0xfffffffe,
                      0x01000024, 0x00356700, 0x8900ac00, 0x00bd00ef};
#define RESULT 156

/* ----------------------------------------------------------------- * /

#define SIZE 8
unsigned lista[SIZE]={0x0       , 0x10204080, 0x3590ac06, 0x70b0d0e0,
                      0xffffffff, 0x12345678, 0x9abcdef0, 0xcafebeef};
#define RESULT 116

/* ----------------------------------------------------------------- */

#endif

int resultado=0;

//////////////////////////////////////////////////////////////////////
// Popcount1:
// ⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻
// Cuenta los bits de cada elemento del array, siempre itera hasta
// el tamaño total del array. La forma de funcionar es cojer un
// elemento, hacele una máscara al último bit, si es un 1 se suma
// sino añado un cero por la izquierda para desechar la operación
// anterior y seguir contando unos.
//
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

//////////////////////////////////////////////////////////////////////
// Popcount2:
// ⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻
// Sigue la misma filosofía que popcoun1 solo que en cada
// desplazamiento comprubo con el while que ya no quedan
// unos. Es mucho más eficiente si existen ceros a la izquierda
// del primer uno (parte más significativa)
//
int popcount2 (unsigned* array, int len){
  int  i,     res=0;
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

//////////////////////////////////////////////////////////////////////
// Popcount3:
// ⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻
// El funcionamiento es practicamente igual que la versión que
// implementa el while.
//
int popcount3 (unsigned* array, int len){
  int i;
  unsigned x;
  int result = 0;
  for(i=0; i<len; i++){
      x=array[i];
      asm("           \n  "
      "ini3:          \n\t" // seguir mientras que x!=0
      "shr %[x]       \n\t" // Desplaza todos los bits a la derecha
      "adc $0x0, %[r] \n\t" // Si se activa el bit de signo sumo carry+result
      "test %[x], %[x]\n\t" // Se hace para activar o no el ZF
      "jnz ini3"            // Salta si no es cero
      :[r] "+r" (result)
      :[x] "r" (x));

  }
  return result;
}

//////////////////////////////////////////////////////////////////////
// Popcount4:
// ⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻
//  El código se basa en aplicar sucesivam mente (8 vec ces) la
//  másca ara 0x0101... a a cada eleme nto, para ir ac cumulando
//  lo os bits de cada byte en una nueva variable val
//
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

//////////////////////////////////////////////////////////////////////
// Popcount5:
// ⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻
//  Voy leyendo de 4 en 4 ya que dentro del bucle lo que hago
//  es dividirlo por la mitad, los bits más significativos
//  y los menos (inferioresy superiores)
//
//
//
//
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
        "movdqa  %%xmm2, %%xmm3      \n\t" // ccopio en 2 y en 3
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

//////////////////////////////////////////////////////////////////////
// Popcount6:
// ⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻
// Aplica la instrucción popcnt del repertorio SSSE3 cuyo
// funcionamiento interno desconozco pero buscando he
// encontrado algo de información pero nada que explique
// que hace popcnt internamente.
//
// http://danluu.com/assembly-intrinsics/
//

int popcount6 (unsigned* array, int len){
  int i, val, result = 0;
  unsigned x;

  for(i=0; i<len; i++){
    x = array[i];
    asm("popcnt %[x], %[val]"
        : [val] "=r" (val)
        :   [x] "r" (x)
    );
    result += val;
  }
  return result;
}

//////////////////////////////////////////////////////////////////////
// Popcount7:
// ⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻
//  Es la misma idea que antes pero leyendo del buff de dos
//  en dos.
//
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

//////////////////////////////////////////////////////////////////////
// Función crono
// ⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻
// Lo que se busca es usando un un puntero a la función original
// pasado por puntero, ejecutarla dentro de esta para medir el
// tiempo. Así solo tendremos que poner un gettimeofday antes
// y despues de haber ejecutado la función apuntanda por func.
//
void crono(int (*func)(), char* msg){
  struct timeval tv1,tv2;	// gettimeofday() secs-usecs
  long           tv_usecs;	// y sus cuentas

  gettimeofday(&tv1,NULL);
  resultado = func(lista, SIZE);
  gettimeofday(&tv2,NULL);

  tv_usecs=(tv2.tv_sec -tv1.tv_sec )*1E6+
           (tv2.tv_usec-tv1.tv_usec);

// En caso que sea para copiar solo los segundos
// o si nos interesa el resultado
#if COPY_PASTE_CALC
  printf(    "%ld" "\n",      tv_usecs);
#else
  printf("resultado = %d\t", resultado);
  printf("%s:%9ld us\n", msg, tv_usecs);
#endif

}

int main(){

#if ! TEST
  for (int i = 0; i < SIZE; i++)	// se queda en cache
    lista[i]=i;
#endif

  crono(popcount1, "popcount1 (lenguaje C     -          for)");
  crono(popcount2, "popcount2 (lenguaje C     -        while)");
  crono(popcount3, "popcount3 (lenguaje ASM   -        while)");
  crono(popcount4, "popcount4 (l.CD:APP 3.49  -     group 8b)");
  crono(popcount5, "popcount5 (ASM SSE3       -  pshufb 128b)");
  crono(popcount6, "popcount6 (ASM SSE4       - popcount 32b)");
  crono(popcount7, "popcount7 (ASM SSE4       -  opcnt 2x32b)");

#if ! COPY_PASTE_CALC
  printf("calculado = %d\n", RESULT);
#endif

  exit(0);
}
