#include <stdio.h>	// para printf()
#include <stdlib.h>	// para exit()
#include <string.h>	// para strncmp()/strlen()
#include <sys/time.h>	// para gettimeofday(), struct timeval

#define SIZE 5
#define TLIM 60

void boom(){
  printf("**************************\n");
  printf("******** BOOM!!! *********\n");
  printf("**************************\n");
  exit(-1);
}

void defused(){
  printf("·······························\n");
  printf("······ Bomba desactivada ······\n");
  printf("·······························\n");
  exit(0);
}

int contrasenia_caracteres(char *c){
  int n = strlen(c);
  int cnt = 0; // mkxv
  char aux;

  if (n != 4)
    cnt++;

  // swap para despistar y enciptar
  if (c[1] == 'k') {
    aux  = c[1];
    c[1] = c[2];
    c[2] = aux;
  }else
    cnt++;

  if (c[3] == 'v') {
    c[1] = 'i';
    c[3] = 'e';
  }else
    cnt++;

  // Falso if
  if (n == 57 || n == 76)
    return -1;

  if (c[0] != 'm' || c[1] != 'i' || c[2] != 'k' || c[3] != 'e')
    cnt++;

  return cnt;
}

unsigned contrasenia_numerica(unsigned num){ // 110
  unsigned res;
  unsigned mask = 1101001;
  res = num ^ mask;

  return res;
}

int main(){
  struct timeval tv1,
                 tv2;	// gettimeofday() secs-usecs
  char           pass[SIZE];
  unsigned            pasv;

  gettimeofday(&tv1,NULL);

  printf("Introduce la contraseña: ");
  fgets(pass,SIZE,stdin);
  if (contrasenia_caracteres(pass))
    boom();

  gettimeofday(&tv2,NULL);
  if (tv2.tv_sec - tv1.tv_sec > TLIM)
    boom();

  printf("Introduce el código BINARIO: ");
  scanf("%u",&pasv);
  if (contrasenia_numerica(pasv))
    boom();

  gettimeofday(&tv2,NULL);
  if (tv2.tv_sec - tv1.tv_sec > TLIM)
    boom();

  defused();

  return 0;
}
