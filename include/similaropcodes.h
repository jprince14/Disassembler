#ifndef AND_H_
#define AND_H_

#include "Disassembler.h"
#include "parse.h"
#include <stdio.h>
#include <stdlib.h>

void shared3partparse(filestruct files, char* opcodename, opcodetype type,
		modrmm inputmodrmm, typeofrun run);
void reg4bytes(filestruct files, u8 opcode, char* opcodename, typeofrun run);
void sharednomodrmneeded(filestruct files, char* opcodename, opcodetype type,
		typeofrun run);
void sharedbasicInstruction(filestruct files, char* instruction, typeofrun run);
void shared2partparse(filestruct files, char* opcodename, modrmm inputmodrmm,
		typeofrun run);
void registerinopcode(filestruct files, u8 opcode, char* opcodename,
		typeofrun run);
errorcode shared2plusbyteopcode(filestruct files, u8 opcode, typeofrun run,
		Vector* jumplocations);

extern const char* registerstrings[];
extern u32 totalbytecount;
extern u32 instructionbytecount;
extern Vector g_jumplocations;

#endif /* AND_H_ */
