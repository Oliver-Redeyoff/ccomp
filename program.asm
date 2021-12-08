
.text

main:
  move $t0, $a0
  li $a0, 28
  li $v0, 9
  syscall
  sw $fp, 0($v0)
  sw $ra, 4($v0)
  sw $t0, 8($v0)
  move $fp, $v0
  sw $fp, 12($fp)  #instantiate local closure
  la $t0, main1
  sw $t0, 16($fp)
  sw $fp, 20($fp)  #instantiate local closure
  la $t0, fact
  sw $t0, 24($fp)
  # calling function, loading it's lexical scope as first argument
  move $t0, $fp
  la $t0, 12($t0)
  lw $a0, 0($t0)
  lw $t0, 4($t0)
  jal getPC1
  getPC1:
  la $ra, 8($ra)
  jr $t0
  li $v0, 10
  syscall


main1:
  move $t0, $a0
  li $a0, 16
  li $v0, 9
  syscall
  sw $fp, 0($v0)
  sw $ra, 4($v0)
  sw $t0, 8($v0)
  move $fp, $v0
  sw $zero, 12($fp)  #instantiate local int
  li $t1, 4
  move $a1, $t1
  # calling function, loading it's lexical scope as first argument
  move $t0, $fp
  lw $t0, 8($t0)
  la $t0, 20($t0)
  lw $a0, 0($t0)
  lw $t0, 4($t0)
  jal getPC2
  getPC2:
  la $ra, 8($ra)
  jr $t0
  move $t2, $v0
  # store register value into memory address
  move $t0, $fp
  la $t0, 12($t0)
  sw $t2, 0($t0)
  # calling builtin function
  la $a0, data1
  li $v0 4
  syscall
  # calling builtin function
  move $t0, $fp
  la $t0, 12($t0)
  lw $a0, 0($t0)
  li $v0 1
  syscall


fact:
  move $t0, $a0
  li $a0, 24
  li $v0, 9
  syscall
  sw $fp, 0($v0)
  sw $ra, 4($v0)
  sw $t0, 8($v0)
  move $fp, $v0
  sw $zero, 12($fp)  #instantiate local int
  sw $fp, 16($fp)  #instantiate local closure
  la $t0, inner_fact
  sw $t0, 20($fp)
  # store register value into memory address
  move $t0, $fp
  la $t0, 12($t0)
  sw $a1, 0($t0)
  # load memory address into register
  move $t0, $fp
  la $t0, 12($t0)
  lw $t?, 0($t0)
  move $a2, $t?
  li $t@, 1
  move $a3, $t@
  # calling function, loading it's lexical scope as first argument
  move $t0, $fp
  la $t0, 16($t0)
  lw $a0, 0($t0)
  lw $t0, 4($t0)
  jal getPC3
  getPC3:
  la $ra, 8($ra)
  jr $t0
  move $tA, $v0
  move $v0, $tA

  move $t0, $fp
  lw $fp, 0($t0)
  lw $t0 4($t0)
  jr $t0


inner_fact:
  move $t0, $a0
  li $a0, 20
  li $v0, 9
  syscall
  sw $fp, 0($v0)
  sw $ra, 4($v0)
  sw $t0, 8($v0)
  move $fp, $v0
  sw $zero, 12($fp)  #instantiate local int
  sw $zero, 16($fp)  #instantiate local int
  # store register value into memory address
  move $t0, $fp
  la $t0, 12($t0)
  sw $a2, 0($t0)
  # store register value into memory address
  move $t0, $fp
  la $t0, 16($t0)
  sw $a3, 0($t0)
  # load memory address into register
  move $t0, $fp
  la $t0, 12($t0)
  lw $t3, 0($t0)
  li $t4, 0
  seq $t5, $t3, $t4
  seq $t6, $t5, 0
  beq $t6, 1, else_1
if_1:

  li $a0, 12
  li $v0, 9
  syscall
  sw $fp, 0($v0)
  lw $fp, 8($v0)
  move $fp, $v0
  # load memory address into register
  move $t0, $fp
  lw $t0, 8($t0)
  la $t0, 16($t0)
  lw $t7, 0($t0)
  move $v0, $t7

  move $t0, $fp
  lw $t0, 8($t0)
  lw $fp, 0($t0)
  lw $t0 4($t0)
  jr $t0

  j next_1
else_1:

  li $a0, 12
  li $v0, 9
  syscall
  sw $fp, 0($v0)
  lw $fp, 8($v0)
  move $fp, $v0

  j next_1
next_1:
  # load memory address into register
  move $t0, $fp
  la $t0, 12($t0)
  lw $t8, 0($t0)
  li $t9, 1
  sub $t:, $t8, $t9
  move $a2, $t:
  # load memory address into register
  move $t0, $fp
  la $t0, 16($t0)
  lw $t;, 0($t0)
  # load memory address into register
  move $t0, $fp
  la $t0, 12($t0)
  lw $t<, 0($t0)
  mul $t=, $t;, $t<
  move $a3, $t=
  # calling function, loading it's lexical scope as first argument
  move $t0, $fp
  lw $t0, 8($t0)
  la $t0, 16($t0)
  lw $a0, 0($t0)
  lw $t0, 4($t0)
  jal getPC4
  getPC4:
  la $ra, 8($ra)
  jr $t0
  move $t>, $v0
  move $v0, $t>

  move $t0, $fp
  lw $fp, 0($t0)
  lw $t0 4($t0)
  jr $t0


.data
data1:  .asciiz "value of x : "
