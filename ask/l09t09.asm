.data
  prompt: .ascizz "Podaj jakis lancuch znakowy:"
  input: .space 100

.text
j main

invert: # invert(char[] str)
  subi $sp, $sp, 4
  sw $s0, 0($sp)
  
  # i := 0
  li $s0, 0
  
  while: # str[i] != NULL
    add $t0, $a0, $s0
    lb $t1, 0($t0)
    beq $t1, $zero, endwhile
    
    # jeżeli str[i] == ' ' to zlewka
    seq $t2, $t1, 0x20
    bne $t2, $zero, cont
    
    # str[i] := str[i] ^ ' ' (MAGIC)
    xori $t1, $t1, 0x20
    sb $t1, 0($t0)
    
    cont:
    # i := i + 1
    addi $s0, $s0, 1
    j while
  
  endwhile:
  lw $s0, 0($sp)
  addi $sp, $sp, 4
  
  jr $ra

main:
  # Wczytaj wejście.
  li $v0, 54
  la $a0, prompt
  la $a1, input
  li $a2, 400
  syscall
  
  # Zamień znaki.
  la $a0, input
  jal invert
  
  # Wyświetl wyjście.
  li $v0, 55
  la $a0, input
  li $a1, 1
  syscall




