#ifndef PARSE_H_
#define PARSE_H_

#include "Disassembler.h"

typedef enum {
	mod0,	// [reg]
	mod1,	// [reg + byte]
	mod2,	// [reg + dword]
	mod3	// reg
} MOD;

typedef enum {
	EAX, ECX, EDX, EBX, ESP, EBP, ESI, EDI
} registers;

typedef struct {
	MOD modrm_MOD;
	registers modrm_Reg;
	registers modrm_RM_Reg;
} modrmm;

errorcode parseopcode(filestruct files, u8 opcode);
modrmm parsemodrmm(u8 input);
void readerrorcheck(size_t sizeread, size_t expectedsize, filestruct files);
void cleanupandclose(filestruct files, errorcode code);
void displayerroroutput(errorcode code);

#endif /* PARSE_H_ */
