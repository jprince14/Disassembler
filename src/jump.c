#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/Disassembler.h"
#include "../include/parse.h"
#include "../include/jump.h"

void jumprel(filestruct files, char* opcodename, opcodetype type, typeofrun run,
		Vector* jumplocations) {

	char printbuffer[120];
	u32 jumplocation;

	if (type == opcode_rel32) {
		s32 rel32;
		size_t size = fread(&rel32, 1, 4, files.in);
		readerrorcheck(size, 4, files);
		instructionbytecount += 4;

		char opcodestring[20];
		_32bittostring(opcodestring, sizeof(opcodestring), rel32);
		snprintf(g_opcodes + strlen(g_opcodes), sizeof(g_opcodes), "%s",
				opcodestring);

		//determine if it is a jump forwards or backwards byt he jump location being read
		//into a signed 4 byte int (s32)

		jumplocation = totalbytecount + instructionbytecount + rel32;

		snprintf(printbuffer, sizeof(printbuffer),
				"%x:\t%-25s\t%-20s offset_0x%x\n", totalbytecount, g_opcodes,
				opcodename, jumplocation);

		if (run == findjumps) {
			insertVector(jumplocations, jumplocation);
		}
	} else if (type == opcode_rel8) {
		s8 rel8;
		size_t size = fread(&rel8, 1, 1, files.in);
		readerrorcheck(size, 1, files);
		instructionbytecount += 1;
		snprintf(g_opcodes + strlen(g_opcodes), sizeof(g_opcodes), "%02x ",
				rel8);

		jumplocation = totalbytecount + instructionbytecount + rel8;

		snprintf(printbuffer, sizeof(printbuffer),
				"%x:\t%-25s\t%-20s offset_0x%x\n", totalbytecount, g_opcodes,
				opcodename, jumplocation);

		if (run == findjumps) {
			insertVector(jumplocations, jumplocation);
		}
	}

	if (run == disassemble) {
		if (files.outfileused == true) {
			fwrite(printbuffer, 1, strlen(printbuffer), files.out);
		}
		printf("%s", printbuffer);
	}
}

void jumprm32(filestruct files, char* opcodename, modrmm inputmodrmm,
		typeofrun run, Vector* jumplocations) {

	char printbuffer[120];
	char part2[20];

	placerm32inarray(inputmodrmm, files, part2, sizeof(part2));

	snprintf(printbuffer, sizeof(printbuffer), "%x:\t%-25s\t%-20s %s\n",
			totalbytecount, opcodename, g_opcodes, part2);

	if (run == disassemble) {
		if (files.outfileused == true) {
			fwrite(printbuffer, 1, strlen(printbuffer), files.out);
		}
		printf("%s", printbuffer);
	}

}

errorcode parsejCC(filestruct files, u8 opcode, typeofrun run,
		Vector* jumplocations) {
	errorcode returnFlag = success;
	switch (opcode) {
	case 0x77:
		//JA rel8
		jumprel(files, "ja", opcode_rel8, run, jumplocations);
		break;

		//Second command with same opcode
		//JNBE rel8
		//jumprel(files, "jnbe", opcode_rel8, run, jumplocations);

	case 0x73:
		//JAE rel8
		jumprel(files, "jae", opcode_rel8, run, jumplocations);
		break;

		//Second command with same opcode
		//JNC rel8
		//jumprel(files, "jnc", opcode_rel8, run, jumplocations);

		//Third Command with same opcode
		//JNB rel8
		//jumprel(files, "jnb", opcode_rel8, run, jumplocations);
	case 0x72:
		//JB rel8
		jumprel(files, "jb", opcode_rel8, run, jumplocations);
		break;

		//Second command with same opcode
		//JC rel8
		//jumprel(files, "jc", opcode_rel8, run, jumplocations);

		//Third Command with same opcode
		//JNAE rel8
		//jumprel(files, "jnae", opcode_rel8, run, jumplocations);
	case 0x76:
		//JBE rel8
		jumprel(files, "jbe", opcode_rel8, run, jumplocations);
		break;

		//Second command with same opcode
		//JNA rel8
		//jumprel(files, "jna", opcode_rel8, run, jumplocations);
	case 0xE3:
		//JCEXZ rel8
		jumprel(files, "jcexz", opcode_rel8, run, jumplocations);
		break;
	case 0x74:
		//JE rel8
		jumprel(files, "je", opcode_rel8, run, jumplocations);
		break;

		//Second command with same opcode
		//JZ rel8
		//jumprel(files, "jz", opcode_rel8, run, jumplocations);
	case 0x7F:
		//JG rel8
		jumprel(files, "jg", opcode_rel8, run, jumplocations);
		break;

		//Second Command with same opcode
		//JNLE rel8
		//jumprel(files, "jnle", opcode_rel8, run, jumplocations);
	case 0x7D:
		//JGE rel8
		jumprel(files, "jge", opcode_rel8, run, jumplocations);
		break;

		//Second command with same opcode
		//JNL rel8
		//jumprel(files, "jnl", opcode_rel8, run, jumplocations);
	case 0x7C:
		//JL rel8
		jumprel(files, "jl", opcode_rel8, run, jumplocations);
		break;

		//Second command with same opcode
		//JNGE rel8
		//jumprel(files, "jnge", opcode_rel8, run, jumplocations);
	case 0x7E:
		//JLE rel8
		jumprel(files, "jle", opcode_rel8, run, jumplocations);
		break;

		//Second command with same opcode
		//JGE rel8
		//jumprel(files, "jng", opcode_rel8, run, jumplocations);

	case 0x75:
		//JNE rel8
		jumprel(files, "jne", opcode_rel8, run, jumplocations);
		break;

		//Second command with same opcode
		//JNZ rel8
		//jumprel(files, "jnz", opcode_rel8, run, jumplocations);
	case 0x71:
		//JNO rel8
		jumprel(files, "jno", opcode_rel8, run, jumplocations);
		break;
	case 0x7B:
		//JNP rel8
		jumprel(files, "jnp", opcode_rel8, run, jumplocations);
		break;

		//Second command with same opcode
		//JPO rel8
		//jumprel(files, "jpo", opcode_rel8, run, jumplocations);
	case 0x79:
		//JNS rel8
		jumprel(files, "jns", opcode_rel8, run, jumplocations);
		break;

	case 0x70:
		//JO rel8
		jumprel(files, "jo", opcode_rel8, run, jumplocations);
		break;
	case 0x7A:
		//JP rel8
		jumprel(files, "jp", opcode_rel8, run, jumplocations);
		break;

		//Second command with same opcode
		//JPE rel8
		//jumprel(files, "jpe", opcode_rel8, run, jumplocations);
	case 0x78:
		//JS rel8
		jumprel(files, "js", opcode_rel8, run, jumplocations);
		break;

	default:
		returnFlag = badopcode;
		break;
	}

	return returnFlag;
}

