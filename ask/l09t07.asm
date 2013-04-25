j main

mul2k:
	# $s1 - wartość w IEEE
	# $s2 - k
	# $s3 - wynik
	
	# wyciągna cechę, ale trzeba ją obrobić
	srl $t0, $s1, 23
	
	# konwersja cechy na ładne 32-bity
	addi $sp, $sp, -4
	sb $t0, 0($sp)
	lb $t0, 0($sp)
	
	# zwiększamy cechę o k
	add $t0, $t0, $s2
	# ... i znowu wyrównujemy na dobre miejsce
	sll $t0, $t0, 23
		
	# maska dla znaku i mantysy:
	# 1000 0000 0111 1111 1111 1111 1111 1111
	# 8    0    7    F    F    F    F    F    
	andi $t1, $s1, 0x807FFFFF
	
	# maska dla cechy:
	# 0111 1111 1000 0000 0000 0000 0000 0000
	# 7    F    8    0    0    0    0    0
	andi $t2, $t0, 0x7F800000
	
	# sklejamy wynik
	or $s3, $t1, $t2	
	
	jr $ra

main:
	#   1.234 to 0x3f9df3b6
	#   1.234 * 2^(-6) = 0.01928125
	li $s1, 0x3f9df3b6
	li $s2, -6
	jal mul2k
	
	mtc1 $s3, $f12
	li $v0, 2
	syscall
	
	# -31.123 to 0xc1f8fbe7
	# -31.123 * 2^4 = −497.968
	li $s1, 0xc1f8fbe7
	li $s2, 4
	jal mul2k
	
	mtc1 $s3, $f12
	li $v0, 2
	syscall
	
