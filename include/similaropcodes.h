#ifndef AND_H_
#define AND_H_

#include "Disassembler.h"
#include "parse.h"
#include <stdio.h>
#include <stdlib.h>

void shared3partparse(filestruct files, char* opcodename,
		opcodetype type, modrmm inputmodrmm);
void reg4bytes(filestruct files, u8 opcode, char* opcodename);
void sharednomodrmneeded(filestruct files, char* opcodename,
		opcodetype type);
void sharedbasicInstruction(filestruct files, char* instruction);
void shared2partparse(filestruct files, char* opcodename, modrmm inputmodrmm);
void registerinopcode(filestruct files, u8 opcode, char* opcodename);

errorcode shared2plusbyteopcode(filestruct files, u8 opcode);

extern const char* registerstrings[];
extern u32 totalbytecount;
extern u32 instructionbytecount;

#endif /* AND_H_ */
