//  según la versión de gcc y opciones de optimización usadas, tal vez haga falta
//  usar gcc –fno-omit-frame-pointer si gcc quitara el marco pila (%ebp)

#include <stdio.h>	// para printf()
#include <stdlib.h>	// para exit()
#include <sys/time.h>	// para gettimeofday(), struct timeval

#define TEST              0
#define COPY_PASTE_CALC   1

#if ! TEST
  #define NBITS 20
  #define SIZE (1<<NBITS)
  unsigned lista[SIZE];
  #define RESULT  (1<<NBITS-1)
#else

/* ----------------------------------------------------------------- * /

#define SIZE 4
unsigned lista[SIZE]={0x80000000, 0x00100000, 0x00000800, 0x00000001};
#define RESULT 4

/* ----------------------------------------------------------------- * /

#define SIZE 8
unsigned lista[SIZE]={0x7fffffff, 0xffefffff, 0xfffff7ff, 0xfffffffe,
                      0x01000024, 0x00356700, 0x8900ac00, 0x00bd00ef};
#define RESULT 8

/* ----------------------------------------------------------------- */

#define SIZE 8
unsigned lista[SIZE]={0x0       , 0x10204080, 0x3590ac06, 0x70b0d0e0,
                      0xffffffff, 0x12345678, 0x9abcdef0, 0xcafebeef};
#define RESULT 2

/* ----------------------------------------------------------------- */

#endif

int resultado=0;

//////////////////////////////////////////////////////////////////////
// Parity1:
// ⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻
// Por cada elemento leido en un segundo for hago un xor y
// desplazo. Es la misma idea que el popcpunt1.
//
int parity1(unsigned* array, int len){
  int  i,  j, val, res=0;
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

//////////////////////////////////////////////////////////////////////
// Parity2:
// ⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻
// Exactamente la misma idea que antes pero grcias al while(x)
// si el elemento es 0 paro de contar, o si ya lo he contando
// entero.
//
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
}

//////////////////////////////////////////////////////////////////////
// Parity3:
// ⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻
// La misma idea pero realizo la máscara tras realizar los
// desplazamientos y el xor.
//
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

//////////////////////////////////////////////////////////////////////
// Parity4:
// ⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻
// Aparentemenete es una traducción literal del parity anterior.
// Gracias a jne lo que hacemos es el xor y el deesplazamiento
// siempre y cuando ZF no está activado.
//
int parity4(unsigned* array, int len){
  int  i,     res=0, val;
  unsigned x;

  for (i = 0; i < len; i++){
    x = array[i];
    val = 0;
    asm("\n"
    "init4:                 \n \t"
    " xor %[x],%[v]	        \n \t"
    " shr $1,%[x]           \n \t"
    " jne   init4           \n \t"
    : [v] "+r" (val)                // e/s:		inicialmente 0, salilda valor
    : [x] "r" (x)                   // entrada:	valor elemento
    );

    res += val & 0x1;
  }
  return res;
}

//////////////////////////////////////////////////////////////////////
// Parity5:
// ⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻⁻
// La idea es someter al elemento del array a XOR y desplazamientos
// sucesivos cada vez a mitad de distancia (16 6, 8, 4, 2, 1)
// hasta que finalmente se hace sencillamente x^=x>>1
//
int parity5(unsigned* array, int len){
  int  i,j;
  unsigned x;
  int result=0, val;
  for (i=0; i < len; i++){
    x = array[i];
    for(j = 16; j >= 1; j /= 2)
      x ^= x >> j;
    x &= 0x01010101;
    x &= 0xFF ;
    result += x;
  }
  return result;
}

void crono(int (*func)(), char* msg){
  struct timeval tv1,tv2;	// gettimeofday() secs-usecs
  long           tv_usecs;	// y sus cuentas

  gettimeofday(&tv1,NULL);
  resultado = func(lista, SIZE);
  gettimeofday(&tv2,NULL);

  tv_usecs=(tv2.tv_sec -tv1.tv_sec )*1E6+
           (tv2.tv_usec-tv1.tv_usec);

#if COPY_PASTE_CALC
  printf(    "%ld" "\n",      tv_usecs);
#else
  printf("resultado = %d\t", resultado);
  printf("%s:%9ld us\n", msg, tv_usecs);
#endif

}

int main(){

#if ! TEST
  int i;			// inicializar array
  for (i=0; i<SIZE; i++)	// se queda en cache
    lista[i]=i;
#endif

  crono(parity1, "parity1 (lenguaje C       -        for)");
  crono(parity2, "parity2 (lenguaje C       -      while)");
  crono(parity3, "parity3 (1.33:CS:APP 3.22 - mask final)");
  crono(parity4, "parity4 (ASM              -        for)");
  crono(parity5, "parity5 (ASM              -      SSSE3)");

#if ! COPY_PASTE_CALC
  printf("Calculado = %d\n", RESULT);
#endif
  exit(0);
}
