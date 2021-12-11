
.text

main:
  move $t0, $a0
  li $a0, 20
  li $v0, 9
  syscall
  sw $fp, 0($v0)
  sw $ra, 4($v0)
  sw $t0, 8($v0)
  move $fp, $v0
  sw $fp, 12($fp)  #instantiate local closure
  la $t0, main1
  sw $t0, 16($fp)
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

  move $t0, $fp
  lw $fp, 0($t0)
  lw $t0 4($t0)
  jr $t0

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
  li $t1, 0
  # store register value into memory address
  move $t0, $fp
  la $t0, 12($t0)
  sw $t1, 0($t0)
loop_1:
  # load memory address into register
  move $t0, $fp
  la $t0, 12($t0)
  lw $t2, 0($t0)
  li $t3, 10
  slt $t4, $t2, $t3
  seq $t5, $t4, 0
  beq $t5, 1, next_1

  li $a0, 12
  li $v0, 9
  syscall
  sw $fp, 0($v0)
  sw $fp, 8($v0)
  move $fp, $v0
  # load memory address into register
  move $t0, $fp
  lw $t0, 8($t0)
  la $t0, 12($t0)
  lw $t6, 0($t0)
  li $t7, 4
  seq $t8, $t6, $t7
  seq $t9, $t8, 0
  beq $t9, 1, else_1
if_1:

  li $a0, 12
  li $v0, 9
  syscall
  sw $fp, 0($v0)
  sw $fp, 8($v0)
  move $fp, $v0

  move $t0, $fp
  lw $t0, 8($t0)
  lw $fp, 0($t0)
  j next_1

  move $t0, $fp
  lw $fp, 0($t0)
  j next_2
else_1:

  li $a0, 12
  li $v0, 9
  syscall
  sw $fp, 0($v0)
  sw $fp, 8($v0)
  move $fp, $v0

  move $t0, $fp
  lw $fp, 0($t0)
  j next_2
next_2:
  # load memory address into register
  move $t0, $fp
  lw $t0, 8($t0)
  la $t0, 12($t0)
  lw $s0, 0($t0)
  li $s1, 1
  add $s2, $s0, $s1
  # store register value into memory address
  move $t0, $fp
  lw $t0, 8($t0)
  la $t0, 12($t0)
  sw $s2, 0($t0)
  # calling builtin function
  move $t0, $fp
  lw $t0, 8($t0)
  la $t0, 12($t0)
  lw $a0, 0($t0)
  li $v0 1
  syscall
  # calling builtin function
  la $a0, data1
  li $v0 4
  syscall

  move $t0, $fp
  lw $fp, 0($t0)
  j loop_1
next_1:
  li $s3, 1
  move $v0, $s3

  move $t0, $fp
  lw $fp, 0($t0)
  lw $t0 4($t0)
  jr $t0

  move $t0, $fp
  lw $fp, 0($t0)
  lw $t0 4($t0)
  jr $t0

.data
data1:  .asciiz ", "
