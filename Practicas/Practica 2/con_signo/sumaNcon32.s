.section .data

# Batería de pruebas
.macro linea
    #       .int -1,-1,-1,-1
  #	.int  1,-2,1,-2
  #	.int  1,2,-3,-4
      	.int 0xFFFFFFFF,0xFFFFFFFF,0xFFFFFFFF,0xFFFFFFFF
  #	.int 0x7fffffff
  #	.int 0x80000000,0x80000000,0x80000000,0x80000000
  #	.int 0x04000000,0x04000000,0x04000000,0x04000000
  #	.int 0x08000000,0x08000000,0x08000000,0x08000000
  #	.int 0xfc000000,0xfc000000,0xfc000000,0xfc000000
  #	.int 0xf8000000,0xf8000000,0xf8000000,0xf8000000
  #  .int 0xf0000000,0xe0000000,0xe0000000,0xd0000000
.endm

lista:	.irpc i, 12345678
	linea
.endr

longlista:	.int (.-lista)/4	# Tamañao de la lista

resultado:	.quad 0x0123456789ABCDEF # Lugar donde vamos a guardar resultado

formato: .ascii "suma = %llu = %llx hex\n\0" # formato para 64bits

.section .text
main: .global main

  mov    $lista, %ebx       # 
  mov longlista, %ecx       #
  call suma                 # Llamo a suma

  mov %eax, resultado       # Guardo eax
  mov %edx, resultado+4     # Guardo arriba edx

  push resultado+4
  push resultado
  push resultado+4
  push resultado
  push $formato
  call printf
  add $20, %esp             # Muevo el puntero

  mov $1, %eax              # Como retornamos la finalización
  mov $0, %ebx              # del programa
  int $0x80                 #

suma:
  mov $0, %edi	            # pone a 0 la suma.
  mov $0, %esi	            # pone a 0 segunda palabra edi:esi
  mov $0, %ebp	            # pone a 0 el indice
  #       %ecx              # tam del vector

bucle:
  mov (%ebx,%ebp,4), %eax   # leo no sumo
  cltd                      # Esta instrucción extiende un número con signo de
                            # 32 bits a uno de 64 bits duplicando el signo
  add %eax, %esi            # Sumo la parte menos significativa
  adc %edx, %edi            # Sumo la parte más significativa

  inc %ebp                  # Incremento cnt
  cmp %ebp, %ecx            # Comparo la desigualdad del bucle indice != tam
  jne bucle                 # Vuelvo a ejecutaar el bucle

  mov %edi, %edx            # En caso contrario he terminado salvaguardo los
  mov %esi, %eax            # resultados en este tipo de registro: EDX:EAX

ret
