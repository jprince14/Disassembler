//This will work for ADD, AND, CMP, MOV, OR and XOR

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "../include/similaropcodes.h"
#include "../include/parse.h"
#include "../include/Disassembler.h"

void sharednomodrmneeded(filestruct files, char* opcodename, opcodetype type,
		typeofrun run) {
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
	} else if (type == opcode_imm16) {

		u16 imm16;
		size_t size = fread(&imm16, 1, 2, files.in);
		readerrorcheck(size, 2, files);
		instructionbytecount += 2;

		snprintf(printbuffer, 40, "%x:\t%s 0x%x\n", totalbytecount, opcodename,
				imm16);

	} else {
		//Invalid opcode type
		cleanupandclose(files, badopcode);
	}

	if (run == disassemble) {
		if (files.outfileused == true) {
			fwrite(printbuffer, 1, strlen(printbuffer), files.out);
		}
		printf("%s", printbuffer);
	}

}

//modrm needs to be read before this function using getandparsemodrmm()
void shared3partparse(filestruct files, char* opcodename, opcodetype type,
		modrmm inputmodrmm, typeofrun run) {

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

	if (run == disassemble) {
		if (files.outfileused == true) {
			fwrite(printbuffer, 1, strlen(printbuffer), files.out);
		}
		printf("%s", printbuffer);
	}

}

void reg4bytes(filestruct files, u8 opcode, char* opcodename, typeofrun run) {
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

	if (run == disassemble) {
		if (files.outfileused == true) {
			fwrite(printbuffer, 1, strlen(printbuffer), files.out);
		}
		printf("%s", printbuffer);
	}
}

void registerinopcode(filestruct files, u8 opcode, char* opcodename,
		typeofrun run) {
	char part2[20];
	char printbuffer[50];

	//get the register for part2 in the array
	getpart2fromopcode(opcode, part2, sizeof(part2));

	snprintf(printbuffer, 50, "%x:\t%s %s\n", totalbytecount, opcodename,
			part2);

	if (run == disassemble) {
		if (files.outfileused == true) {
			fwrite(printbuffer, 1, strlen(printbuffer), files.out);
		}
		printf("%s", printbuffer);
	}
}

void sharedbasicInstruction(filestruct files, char* instruction, typeofrun run) {
	char printbuffer[50];

	snprintf(printbuffer, 50, "%x:\t%s\n", totalbytecount, instruction);

	if (run == disassemble) {
		if (files.outfileused == true) {
			fwrite(printbuffer, 1, strlen(printbuffer), files.out);
		}
		printf("%s", printbuffer);
	}

}

void shared2partparse(filestruct files, char* opcodename, modrmm inputmodrmm,
		typeofrun run) {

	char printbuffer[50];
	char part2[20];

	getpart2(inputmodrmm, files, part2, sizeof(part2));

	snprintf(printbuffer, 50, "%x:\t%s %s\n", totalbytecount, opcodename,
			part2);

	if (run == disassemble) {
		if (files.outfileused == true) {
			fwrite(printbuffer, 1, strlen(printbuffer), files.out);
		}
		printf("%s", printbuffer);
	}
}

errorcode shared2plusbyteopcode(filestruct files, u8 opcode, typeofrun run) {
//read in the 2nd byte of the opcode and determine if a 3rd byte needs to be read in
//then call the proper function to parse the opcode
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
				shared2partparse(files, "nop", parsedmodrmm, run);
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
			shared3partparse(files, "cmp", opcode_r32_rm32, parsedmodrmm, run);

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
