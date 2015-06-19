#ifndef JUMP_H_
#define JUMP_H_
#include "Disassembler.h"
#include "Vector.h"
#include "parse.h"

extern const char* registerstrings[];
extern Vector g_jumplocations;

void jmp_nomodrm(filestruct files, char* opcodename, opcodetype type,
		typeofrun run, Vector *jumplocations);

void jump2partparse(filestruct files, char* opcodename, modrmm inputmodrmm,
		typeofrun run, Vector* jumplocations);



#endif /* JUMP_H_ */
