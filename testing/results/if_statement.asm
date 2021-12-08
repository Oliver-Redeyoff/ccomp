
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
  li $t1, 1
  # store register value into memory address
  move $t0, $fp
  la $t0, 12($t0)
  sw $t1, 0($t0)
  # load memory address into register
  move $t0, $fp
  la $t0, 12($t0)
  lw $t2, 0($t0)
  li $t3, 2
  slt $t4, $t2, $t3
  seq $t5, $t4, 0
  beq $t5, 1, else_1
if_1:

  li $a0, 12
  li $v0, 9
  syscall
  sw $fp, 0($v0)
  sw $fp, 8($v0)
  move $fp, $v0
  # calling builtin function
  la $a0, data1
  li $v0 4
  syscall

  lw $fp, 0($fp)
  j next_1
else_1:

  li $a0, 12
  li $v0, 9
  syscall
  sw $fp, 0($v0)
  sw $fp, 8($v0)
  move $fp, $v0
  # calling builtin function
  la $a0, data2
  li $v0 4
  syscall

  lw $fp, 0($fp)
  j next_1
next_1:
  li $t6, 1
  move $v0, $t6

  move $t0, $fp
  lw $fp, 0($t0)
  lw $t0 4($t0)
  jr $t0

  move $t0, $fp
  lw $fp, 0($t0)
  lw $t0 4($t0)
  jr $t0

.data
data1:  .asciiz "x is less than 2"
data2:  .asciiz "x is more than 2"
