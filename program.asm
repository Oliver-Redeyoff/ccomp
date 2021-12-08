
.text

premain:
  move $t0, $a0
  li $a0, 20
  li $v0, 9
  syscall
  lw $fp, 0($v0)
  lw $ra, 4($v0)
  lw $t0, 8($v0)
  move $fp, $v0
  sw $fp, 12($fp)  #instantiate local closure
  la $t0, main
  sw $t0, 16($fp)
  # calling function, loading it's lexical scope as first argument
  move $t0, $fp
  la $t0, 12($t0)
  lw $a0, 0($t0)
  lw $t0, 4($t0)
  jr $t0
  li $v0, 10
  syscall


main:
  move $t0, $a0
  li $a0, 12
  li $v0, 9
  syscall
  lw $fp, 0($v0)
  lw $ra, 4($v0)
  lw $t0, 8($v0)
  move $fp, $v0
  li $t1, 2
  move $v0, $t1

  move $t0, $fp
  lw $fp, 0($t0)
  la $t0 4($t0)
  jr $t0

