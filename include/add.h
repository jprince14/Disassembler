#ifndef ADD_H_
#define ADD_H_

#include <stdio.h>
#include <stdlib.h>
#include "Disassembler.h"


void calladd_rm32(filestruct files, u8 opcode);

extern const char* registerstrings[];

#endif /* ADD_H_ */
