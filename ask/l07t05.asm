j main

fact:
	addi $sp, $sp, -8
	sw $ra, 4($sp)
	sw $a0, 0($sp)
	slti $t0, $a0, 1
	beq $t0, $zero, rec
	addi $v0, $zero, 1
	addi $sp, $sp, 8
	jr $ra
	
	rec:
	addi $a0, $a0, -1
	jal fact
	lw $a0, 0($sp)
	lw $ra, 4($sp)
	addi $sp, $sp, 8
	mul $v0, $a0, $v0
	jr $ra

main:
	#addi $a0, $zero, 5
	# fact
	addi $t0, $zero, 5
	addi $t1, $zero, 4
	mul $t0, $t1, $t0
	