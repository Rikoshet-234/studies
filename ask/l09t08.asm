.data
   fraczeski: .asciiz "ABCDEFGHI"

.text
j main

newline:
  addi $sp, $sp, -4
  sw $v0, 0($sp)
  
  addi $a0, $zero, 0xA
  addi $v0, $zero, 0xB
  syscall
  
  lw $v0, 0($sp)
  addi $sp, $sp, 4  
  
  jr $ra

main:
  # SPRAWDZANIE NA LICZBACH
  li $t0, 0x4A3B2C1D
  addi $sp, $sp, -4
  sw $t0, 0($sp)
  
  lb $t1, 0($sp)
  lb $t2, 1($sp)
  lb $t3, 2($sp)
  lb $t4, 3($sp)
   
  # wypisz wyniki
  li $v0, 34
   
  addi $a0, $t1, 0
  syscall
  jal newline
  
  addi $a0, $t2, 0
  syscall
  jal newline
   
  addi $a0, $t3, 0
  syscall
  jal newline
   
  addi $a0, $t4, 0
  syscall
  jal newline
  
  jal newline
  jal newline
  
  # WYPISYWANIE ZNAKOW
  li $v0, 0xB
  li $t0, 0
  li $t1, 6
  
  loop:
  lb $a0, fraczeski($t0)
  syscall
  addi $t0, $t0, 1
  bne $t0, $t1, loop
 
   
