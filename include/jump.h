#ifndef JUMP_H_
#define JUMP_H_
#include "Disassembler.h"
#include "Vector.h"
#include "parse.h"

extern const char* registerstrings[];

void jmp_nomodrm(filestruct files, char* opcodename, opcodetype type,
		typeofrun run, Vector *jumplocations);

void jmp_modrm(filestruct files, char* opcodename, opcodetype type,
		modrmm inputmodrmm, typeofrun run, Vector *jumplocations);

#endif /* JUMP_H_ */
