#ifndef JUMP_H_
#define JUMP_H_
#include "Disassembler.h"
#include "Vector.h"
#include "parse.h"

#include <netinet/in.h>

extern const char* registerstrings[];
extern Vector g_jumplocations;
extern char g_opcodes[40];

void jumprel(filestruct files, char* opcodename, opcodetype type, typeofrun run,
		Vector *jumplocations);

void jumprm32(filestruct files, char* opcodename, modrmm inputmodrmm,
		typeofrun run, Vector* jumplocations);

errorcode parsejCC(filestruct files, u8 opcode, typeofrun run,
		Vector* jumplocations);

#endif /* JUMP_H_ */
