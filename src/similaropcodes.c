//This will work for ADD, AND, CMP, MOV, OR and XOR

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "../include/similaropcodes.h"
#include "../include/parse.h"
#include "../include/Disassembler.h"

void sharednomodrmneeded(filestruct files, char* opcodename, opcodetype type) {
	char printbuffer[50];

	if (type == opcode_eax_imm32) {

		u32 imm32;
		size_t size = fread(&imm32, 1, 4, files.in);
		readerrorcheck(size, 4, files);
		instructionbytecount += 4;

		snprintf(printbuffer, 40, "%x:\t%s, eax, 0x%x\n", totalbytecount,
				opcodename, imm32);

	} else if (type == opcode_imm32) {
		u32 imm32;
		size_t size = fread(&imm32, 1, 4, files.in);
		readerrorcheck(size, 4, files);
		instructionbytecount += 4;

		snprintf(printbuffer, 40, "%x:\t%s 0x%x\n", totalbytecount, opcodename,
				imm32);
	} else if (type == opcode_imm8) {
		u8 imm8;
		size_t size = fread(&imm8, 1, 1, files.in);
		readerrorcheck(size, 1, files);
		instructionbytecount += 1;

		snprintf(printbuffer, 40, "%x:\t%s 0x%x\n", totalbytecount, opcodename,
				imm8);
	} else {
		//Invalid opcode type
		cleanupandclose(files, badopcode);
	}

	fwrite(printbuffer, 1, strlen(printbuffer), files.out);
#if printtoscreen
	printf("%s", printbuffer);
#endif
}

//modrm needs to be read before this function using getandparsemodrmm()
void shared3partparse(filestruct files, char* opcodename, opcodetype type,
		modrmm inputmodrmm) {

	//return starts with success and becomes an error if there is one
	char printbuffer[50];
	char part2[20];
	char part3[20];
	size_t size;

	getpart2(inputmodrmm, files, part2, sizeof(part2));

	if (type == opcode_rm32_imm32) {

		u32 imm32;
		size = fread(&imm32, 1, 4, files.in);
		readerrorcheck(size, 4, files);
		instructionbytecount += 4;
		snprintf(part3, 20, "0x%x", imm32);

		snprintf(printbuffer, 50, "%x:\t%s %s, %s\n", totalbytecount,
				opcodename, part2, part3);

	} else if (type == opcode_rm32_imm8) {

		u8 imm8;
		size = fread(&imm8, 1, 1, files.in);
		readerrorcheck(size, 1, files);
		instructionbytecount += 1;
		snprintf(part3, 20, "0x%x", imm8);

		snprintf(printbuffer, 50, "%x:\t%s %s, %s\n", totalbytecount,
				opcodename, part2, part3);

	} else if (type == opcode_rm32_r32) {

		snprintf(part3, 20, "%s", registerstrings[inputmodrmm.modrm_Reg]);
		snprintf(printbuffer, 50, "%x:\t%s %s, %s\n", totalbytecount,
				opcodename, part2, part3);

	} else if (type == opcode_r32_rm32) {
		snprintf(part3, 20, "%s", registerstrings[inputmodrmm.modrm_Reg]);

		//parts 2 and 3 are swapped for this one
		snprintf(printbuffer, 50, "%x:\t%s %s, %s\n", totalbytecount,
				opcodename, part3, part2);
	} else if (type == opcode_rm32_1) {
		snprintf(printbuffer, 50, "%x:\t%s %s, %s\n", totalbytecount,
				opcodename, part2, "1");
	} else {
		//Invalid type received
		cleanupandclose(files, badopcode);
	}

	fwrite(printbuffer, 1, strlen(printbuffer), files.out);
#if printtoscreen
	printf("%s", printbuffer);
#endif

}

void reg4bytes(filestruct files, u8 opcode, char* opcodename) {
	char part2[20];
	char part3[20];
	char printbuffer[50];
	size_t size;

	//get the register for part2 in the array
	getpart2fromopcode(opcode, part2, sizeof(part2));

	u32 imm32;
	size = fread(&imm32, 1, 4, files.in);
	readerrorcheck(size, 4, files);
	instructionbytecount += 4;
	snprintf(part3, 20, "0x%x", imm32);

	snprintf(printbuffer, 50, "%x:\t%s %s, %s\n", totalbytecount, opcodename,
			part2, part3);

	fwrite(printbuffer, 1, strlen(printbuffer), files.out);
#if printtoscreen
	printf("%s", printbuffer);
#endif
}

void registerinopcode(filestruct files, u8 opcode, char* opcodename) {
	char part2[20];
	char printbuffer[50];

	//get the register for part2 in the array
	getpart2fromopcode(opcode, part2, sizeof(part2));

	snprintf(printbuffer, 50, "%x:\t%s %s\n", totalbytecount, opcodename,
			part2);

	fwrite(printbuffer, 1, strlen(printbuffer), files.out);
#if printtoscreen
	printf("%s", printbuffer);
#endif
}

void sharedbasicInstruction(filestruct files, char* instruction) {
	char printbuffer[50];

	snprintf(printbuffer, 50, "%x:\t%s\n", totalbytecount, instruction);

	fwrite(printbuffer, 1, strlen(printbuffer), files.out);
#if printtoscreen
	printf("%s", printbuffer);
#endif
}

void shared2partparse(filestruct files, char* opcodename, modrmm inputmodrmm) {

	char printbuffer[50];
	char part2[20];

	getpart2(inputmodrmm, files, part2, sizeof(part2));

	snprintf(printbuffer, 50, "%x:\t%s %s\n", totalbytecount, opcodename,
			part2);

	fwrite(printbuffer, 1, strlen(printbuffer), files.out);
#if printtoscreen
	printf("%s", printbuffer);
#endif
}

errorcode shared2plusbyteopcode(filestruct files, u8 opcode) {
//read in the 2nd byte of the opcode
	size_t size;
	u8 secondopcode;
	modrmm parsedmodrmm;

	errorcode returnvalue = success;

	size = fread(&secondopcode, 1, 1, files.in);
	readerrorcheck(size, 1, files);
	instructionbytecount += 1;

	switch (opcode) {
	case 0x0F:
		switch (secondopcode) {
		case 0x1F:
			parsedmodrmm = getandparsemodrmm(files);
			if (parsedmodrmm.modrm_Reg == 0x0) {
				//opcode 0F 1F /0
				//nop r/m32
				shared2partparse(files, "nop", parsedmodrmm);
			} else {
				returnvalue = badopcode;
			}
			break;
		default:
			returnvalue = badopcode;
			break;
		}
		break;

	case 0xF3:
		switch (secondopcode) {

		u8 thirdopcode;

	case 0x0F:
		//These are 3 byte opcodes
		size = fread(&thirdopcode, 1, 1, files.in);
		readerrorcheck(size, 1, files);
		instructionbytecount += 1;

		switch (thirdopcode) {
		case 0xB8:
			//opcode F3 0F B8
			//popcnt r32, r/m32
			parsedmodrmm = getandparsemodrmm(files);
			shared3partparse(files, "cmp", opcode_r32_rm32, parsedmodrmm);

			break;
		default:
			returnvalue = badopcode;
			break;
		}
		break;
	default:
		returnvalue = badopcode;
		break;
		}
		break;
	}
	return returnvalue;
}

errorcode shared3byteopcode(filestruct files, u8 firstopcode, u8 secondopcode) {
//read in the 3rd byte of the opcode

	errorcode returnvalue = success;
	size_t size;
	modrmm parsedmodrmm;

	u8 thirdopcode;

	size = fread(&thirdopcode, 1, 4, files.in);
	readerrorcheck(size, 1, files);
	instructionbytecount += 1;

	switch (thirdopcode) {
	case 0xB8:
		parsedmodrmm = getandparsemodrmm(files);
		shared3partparse(files, "popcnt", opcode_r32_rm32, parsedmodrmm);

		break;
	default:
		returnvalue = badopcode;
		break;
	}
	return returnvalue;
}
