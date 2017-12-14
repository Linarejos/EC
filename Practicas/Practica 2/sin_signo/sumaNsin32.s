.section .data

# Batería de pruebas
.macro linea
    #.int 1,1,1,1
    #.int 2,2,2,2
    #.int 1,2,3,4
    #.int -1,-1,-1,-1
    #.int 0xffffffff,0xffffffff,0xffffffff,0xffffffff
    #.int 0x08000000,0x08000000,0x08000000,0x08000000
    #.int 0x10000000,0x20000000,0x40000000,0x80000000
      	.int 0xFFFFFFFF,0xFFFFFFFF,0xFFFFFFFF,0xFFFFFFFF
.endm

lista:	.irpc i, 12345678
	linea
.endr

longlista:	.int (.-lista)/4	# Tamañao de la lista

resultado:	.quad 0x0123456789ABCDEF # Lugar donde vamos a guardar resultado

formato: .ascii "suma = %llu = %llx hex\n\0" # formato para 64bits

.section .text
main: .global main

	mov    $lista, %ebx        # Ajuste del marco de pila
	mov longlista, %ecx        #
	call suma                  # Llamo a suma 
	
	mov %eax, resultado        # Guardo eax
	mov %edx, resultado+4      # Guardo arriba edx

	push resultado+4           
	push resultado
	push resultado+4
	push resultado
	push $formato
	call printf
	add $20, %esp              # Muevo el puntero 

	mov $1, %eax               # Como retornamos la finalización 
	mov $0, %ebx               # del programa
	int $0x80                  # 

suma:
	mov $0, %eax	          # Pone a 0 la suma
	mov $0, %edx	          # Pone a 0 indice
	mov $0, %esi	          # Pone a 0 el acumulador (carry)
                #ecx              # tam del vector

bucle:
	add (%ebx,%esi,4), %eax   # sumo
	jnc no_carry              # miro el flag y salto si no hay accareo 
	inc %edx                  # si hay cary incremento en edx

no_carry:
        inc %esi		  # Incremento el contador
        cmp %esi   ,%ecx	  # Comparo indice con el tam del vector
        jne bucle	          # en ca afirmativo salto al bucle de nuevo (otra vuelta)

	ret
