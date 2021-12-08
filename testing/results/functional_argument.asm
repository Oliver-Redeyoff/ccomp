
.text

main:
  move $t0, $a0
  li $a0, 36
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
  la $t0, plus_one
  sw $t0, 24($fp)
  sw $fp, 28($fp)  #instantiate local closure
  la $t0, twice
  sw $t0, 32($fp)
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
  li $a0, 32
  li $v0, 9
  syscall
  sw $fp, 0($v0)
  sw $ra, 4($v0)
  sw $t0, 8($v0)
  move $fp, $v0
  sw $fp, 12($fp)  #instantiate local closure
  sw $fp, 20($fp)  #instantiate local closure
  sw $zero, 28($fp)  #instantiate local int
  # load memory address into register
  move $t0, $fp
  lw $t0, 8($t0)
  la $t0, 20($t0)
  la $t1, 0($t0)
  # store register value into memory address
  move $t0, $fp
  la $t0, 12($t0)
  lw $v0, 0($t1)
  sw $v0, 0($t0)
  lw $v0, 4($t1)
  sw $v0, 4($t0)
  # load memory address into register
  move $t0, $fp
  lw $t0, 8($t0)
  la $t0, 20($t0)
  la $t2, 0($t0)
  move $a1, $t2
  # calling function, loading it's lexical scope as first argument
  move $t0, $fp
  lw $t0, 8($t0)
  la $t0, 28($t0)
  lw $a0, 0($t0)
  lw $t0, 4($t0)
  jal getPC2
  getPC2:
  la $ra, 8($ra)
  jr $t0
  move $t3, $v0
  # store register value into memory address
  move $t0, $fp
  la $t0, 20($t0)
  lw $v0, 0($t3)
  sw $v0, 0($t0)
  lw $v0, 4($t3)
  sw $v0, 4($t0)
  li $t4, 2
  move $a1, $t4
  # calling function, loading it's lexical scope as first argument
  move $t0, $fp
  la $t0, 20($t0)
  lw $a0, 0($t0)
  lw $t0, 4($t0)
  jal getPC3
  getPC3:
  la $ra, 8($ra)
  jr $t0
  move $t5, $v0
  # store register value into memory address
  move $t0, $fp
  la $t0, 28($t0)
  sw $t5, 0($t0)
  # calling builtin function
  move $t0, $fp
  la $t0, 28($t0)
  lw $a0, 0($t0)
  li $v0 1
  syscall
  li $t6, 0
  move $v0, $t6

  move $t0, $fp
  lw $fp, 0($t0)
  lw $t0 4($t0)
  jr $t0

  move $t0, $fp
  lw $fp, 0($t0)
  lw $t0 4($t0)
  jr $t0

plus_one:
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
  la $t0, 12($t0)
  lw $t7, 0($t0)
  li $t8, 1
  add $t9, $t7, $t8
  move $v0, $t9

  move $t0, $fp
  lw $fp, 0($t0)
  lw $t0 4($t0)
  jr $t0

  move $t0, $fp
  lw $fp, 0($t0)
  lw $t0 4($t0)
  jr $t0

twice:
  move $t0, $a0
  li $a0, 28
  li $v0, 9
  syscall
  sw $fp, 0($v0)
  sw $ra, 4($v0)
  sw $t0, 8($v0)
  move $fp, $v0
  sw $fp, 12($fp)  #instantiate local closure
  sw $fp, 20($fp)  #instantiate local closure
  la $t0, g
  sw $t0, 24($fp)
  # store register value into memory address
  move $t0, $fp
  la $t0, 12($t0)
  lw $v0, 0($a1)
  sw $v0, 0($t0)
  lw $v0, 4($a1)
  sw $v0, 4($t0)
  # load memory address into register
  move $t0, $fp
  la $t0, 20($t0)
  la $s3, 0($t0)
  move $v0, $s3

  move $t0, $fp
  lw $fp, 0($t0)
  lw $t0 4($t0)
  jr $t0

  move $t0, $fp
  lw $fp, 0($t0)
  lw $t0 4($t0)
  jr $t0

g:
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
  la $t0, 12($t0)
  lw $s0, 0($t0)
  move $a1, $s0
  # calling function, loading it's lexical scope as first argument
  move $t0, $fp
  lw $t0, 8($t0)
  la $t0, 12($t0)
  lw $a0, 0($t0)
  lw $t0, 4($t0)
  jal getPC4
  getPC4:
  la $ra, 8($ra)
  jr $t0
  move $s1, $v0
  move $a1, $s1
  # calling function, loading it's lexical scope as first argument
  move $t0, $fp
  lw $t0, 8($t0)
  la $t0, 12($t0)
  lw $a0, 0($t0)
  lw $t0, 4($t0)
  jal getPC5
  getPC5:
  la $ra, 8($ra)
  jr $t0
  move $s2, $v0
  move $v0, $s2

  move $t0, $fp
  lw $fp, 0($t0)
  lw $t0 4($t0)
  jr $t0

  move $t0, $fp
  lw $fp, 0($t0)
  lw $t0 4($t0)
  jr $t0

.data
