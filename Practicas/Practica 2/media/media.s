.section .data
	.macro linea
#		.int 1,-2,1,-2
#		.int 1,2,-3,-4
#		.int 0x7fffffff, 0x7fffffff, 0x7fffffff, 0x7fffffff
		.int 0x80000000, 0x80000000, 0x80000000, 0x80000000
#		.int 0xf0000000,0xe0000000,0xe0000000,0xd0000000
#		.int -1,-1,-1,-1
#		.int 0,-1,-1,-1
#		.int 0,-2,-1,-1
#		.int 1,-2,-1,-1
#		.int -2,-1,-1,-1
#		.int 2,-2,-1,-1
#		.int 3,-2,-1,-1
#		.int 32,-2,-1,-1
#		.int 33,-2,-1,-1
#		.int 34,-2,-1,-1
#		.int 35,-2,-1,-1
#		.int 63,-2,-1,-1
# 	.int 64,-2,-1,-1
# 	.int 95,-2,-1,-1
# 	.int -31,-2,-1,-1
	.endm

lista:
	.irpc i,12345678
			linea
	.endr

longlista:	.int (.-lista)/4
media:		.int 0
resto: 		.int 0

formato:
	.ascii "media = %d = %x hex\n"
	.ascii "resto = %d = %x hex\n\0"

.section .text
main:	.global main

	mov    $lista, %ebx
	mov longlista, %ecx
	call suma
	mov %eax, media
	mov %edx, resto

	push resto #parte mas significativa del primer
	push resto
	push media
	push media
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

	idiv %ecx

ret
