#ifndef AND_H_
#define AND_H_

#include "Disassembler.h"
#include "parse.h"
#include <stdio.h>
#include <stdlib.h>
#include <netinet/in.h>

void shared3partparse(filestruct files, char* opcodename, opcodetype type,
		modrmm inputmodrmm, typeofrun run);
void registerinopcode4bytes(filestruct files, u8 opcode, char* opcodename,
		typeofrun run);
void sharedonlyimmneeded(filestruct files, char* opcodename, opcodetype type,
		typeofrun run);
void sharedbasicInstruction(filestruct files, char* instruction, typeofrun run);
void shared2partparse(filestruct files, char* opcodename, modrmm inputmodrmm,
		typeofrun run);
void registerinopcode(filestruct files, u8 opcode, char* opcodename,
		typeofrun run);
errorcode shared2plusbyteopcode(filestruct files, u8 opcode, typeofrun run,
		Vector* jumplocations);
void lea(filestruct files, char* opcodename, modrmm inputmodrmm, typeofrun run);

extern const char* registerstrings[];
extern u32 totalbytecount;
extern u32 instructionbytecount;
extern Vector g_jumplocations;
extern char g_opcodes[40];

#endif /* AND_H_ */
