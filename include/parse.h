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

errorcode parseopcode(FILE* IN_fp, FILE* OUT_fp, u8* buffer);
modrmm parsemodrmm(u8 input);
errorcode readerrorcheck(size_t sizeread, size_t expectedsize);

#endif /* PARSE_H_ */
