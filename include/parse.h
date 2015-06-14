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

typedef enum {
	eax_imm32, rm32_imm32, rm32_imm8, rm32_r32, r32_rm32

} opcodetype;

errorcode parseopcode(filestruct files, u8 opcode);
char* parsemodrmm(u8 input, filestruct files, char* part2, int arraysize);
void readerrorcheck(size_t sizeread, size_t expectedsize, filestruct files);
void cleanupandclose(filestruct files, errorcode code);
void displayerroroutput(errorcode code);

#endif /* PARSE_H_ */
