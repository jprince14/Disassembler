#x86 Disassembler#
This Project contains a x86 disassembler that uses a linear sweep.

The assembler only works on a portion of x86. The following mnuematics are supported:

  * add
  * eax, imm32
  * add r/m32, imm32
  * add r/m32, imm8
  * add r/m32, r32
  * add r32, r/m32
  * and eax, imm32
  * and r/m32, imm32
  * and r/m32, imm8
  * and r/m32, r32
  * and r32, r/m32
  * call rel32
  * call r/m32
  * cmp eax, imm32
  * cmp r/m32, imm32
  * cmp r/m32, imm8
  * cmp r/m32, r32
  * cmp r32, r/m32
  * dec r/m32
  * dec r32
  * inc r/m32
  * inc r32
  * int 3
  * int imm8
  * jmp rel8
  * jmp rel32
  * jmp r/m32
  * jCC rel8
  * jCC rel32
  * lea r32,m
  * mov eax, imm32
  * mov r/m32, r32
  * mov r32, r/m32
  * mov r32, imm32
  * mov r/m32, imm32
  * nop
  * nop r/m32
  * or eax, imm32
  * or r/m32, imm32
  * or r/m32, imm8
  * or r/m32, r32
  * or r32, r/m32
  * pop r/m32
  * pop r32
  * popcnt r32, r/m32
  * push r/m32
  * push r32
  * push imm8
  * push imm32
  * retn
  * retn imm16
  * sal r/m32, 1
  * sal r/m32, imm8
  * sar r/m32, 1
  * sar r/m32, imm8
  * shl r/m32, 1
  * shl r/m32, imm8
  * shr r/m32, 1
  * shr r/m32, imm8
  * test eax, imm32
  * test r/m32, imm32
  * test r/m32, r32
  * xor eax, imm32
  * xor r/m32, imm32
  * xor r/m32, imm8
  * xor r/m32, r32
  * xor r32, r/m32
