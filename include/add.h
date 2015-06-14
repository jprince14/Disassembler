#ifndef ADD_H_
#define ADD_H_

#include <stdio.h>
#include <stdlib.h>
#include "Disassembler.h"

void calladd_imm32_to_eax(filestruct files, u8 opcode);

void calladd_rm32(filestruct files, u8 opcode);

#endif /* ADD_H_ */
