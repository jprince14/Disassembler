#ifndef AND_H_
#define AND_H_

#include "Disassembler.h"
#include "parse.h"
#include <stdio.h>
#include <stdlib.h>

void shared3partparse(filestruct files, u8 opcode, char* opcodename,
		opcodetype type) ;

extern const char* registerstrings[];

extern u32 totalbytecount;
extern u32	instructionbytecount;

#endif /* AND_H_ */
