
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
  la $t0, cplus
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

  move $t0, $fp
  lw $fp, 0($t0)
  lw $t0 4($t0)
  jr $t0

main1:
  move $t0, $a0
  li $a0, 24
  li $v0, 9
  syscall
  sw $fp, 0($v0)
  sw $ra, 4($v0)
  sw $t0, 8($v0)
  move $fp, $v0
  sw $fp, 12($fp)  #instantiate local closure
  sw $zero, 20($fp)  #instantiate local int
  li $t1, 2
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
  lw $v0, 0($t2)
  sw $v0, 0($t0)
  lw $v0, 4($t2)
  sw $v0, 4($t0)
  li $t3, 3
  move $a1, $t3
  # calling function, loading it's lexical scope as first argument
  move $t0, $fp
  la $t0, 12($t0)
  lw $a0, 0($t0)
  lw $t0, 4($t0)
  jal getPC3
  getPC3:
  la $ra, 8($ra)
  jr $t0
  move $t4, $v0
  # store register value into memory address
  move $t0, $fp
  la $t0, 20($t0)
  sw $t4, 0($t0)
  # calling builtin function
  move $t0, $fp
  la $t0, 20($t0)
  lw $a0, 0($t0)
  li $v0 1
  syscall

  move $t0, $fp
  lw $fp, 0($t0)
  lw $t0 4($t0)
  jr $t0

cplus:
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
  la $t0, cplusa
  sw $t0, 20($fp)
  # store register value into memory address
  move $t0, $fp
  la $t0, 12($t0)
  sw $a1, 0($t0)
  # load memory address into register
  move $t0, $fp
  la $t0, 16($t0)
  la $t8, 0($t0)
  move $v0, $t8

  move $t0, $fp
  lw $fp, 0($t0)
  lw $t0 4($t0)
  jr $t0

  move $t0, $fp
  lw $fp, 0($t0)
  lw $t0 4($t0)
  jr $t0

cplusa:
  move $t0, $a0
  li $a0, 16
  li $v0, 9
  syscall
  sw $fp, 0($v0)
  sw $ra, 4($v0)
  sw $t0, 8($v0)
  move $fp, $v0
  sw $zero, 12($fp)  #instantiate local int
  # store register value into memory address
  move $t0, $fp
  la $t0, 12($t0)
  sw $a1, 0($t0)
  # load memory address into register
  move $t0, $fp
  lw $t0, 8($t0)
  la $t0, 12($t0)
  lw $t5, 0($t0)
  # load memory address into register
  move $t0, $fp
  la $t0, 12($t0)
  lw $t6, 0($t0)
  add $t7, $t5, $t6
  move $v0, $t7

  move $t0, $fp
  lw $fp, 0($t0)
  lw $t0 4($t0)
  jr $t0

  move $t0, $fp
  lw $fp, 0($t0)
  lw $t0 4($t0)
  jr $t0

.data
