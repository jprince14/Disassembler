#ifndef PARSE_H_
#define PARSE_H_

#include "Disassembler.h"
#include "Vector.h"

#define SAR_CODE 0

typedef enum {
	mod0,	// [reg]
	mod1,	// [reg + byte]
	mod2,	// [reg + dword]
	mod3	// reg
} MOD;

typedef enum {
	EAX, ECX, EDX, EBX, ESP, EBP, ESI, EDI
} registers;

#if SAR_CODE

typedef struct {
	u8 index;
	registers index_reg;
	registers base_reg;
}sar;

#endif

typedef struct {
	MOD modrm_MOD;
	registers modrm_Reg;
	registers modrm_RM_Reg;

#if SAR_CODE
bool sarused;
sar SAR;
#endif
} modrmm;

typedef enum {opcode_eax_imm32,
opcode_imm32,
opcode_imm16,
opcode_imm8,
opcode_rm32_imm32,
opcode_rm32_imm8,
opcode_rm32_r32,
opcode_r32_rm32,
opcode_rm32_1,
opcode_rel32,
opcode_rel8

} opcodetype;

errorcode parseopcode(filestruct files, typeofrun run, Vector* jumplocations);
modrmm getandparsemodrmm(filestruct files);
void readerrorcheck(size_t sizeread, size_t expectedsize, filestruct files);
void cleanupandclose(filestruct files, errorcode code);
void placerm32inarray(modrmm input, filestruct files, char* part2,
	int part2size);
void getpart2fromopcode(u8 opcode, char* part2, int part2size);
void _32bittostring(char* string, size_t strsize, u32 opcode);
void _16bittostring(char* string, size_t strsize, u16 opcode);

extern u32 totalbytecount;
extern u32 instructionbytecount;
extern Vector g_jumplocations;
extern char g_opcodes[40];
extern typeofrun g_runtype;

#endif /* PARSE_H_ */
