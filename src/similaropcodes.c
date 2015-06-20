//This will work for ADD, AND, CMP, MOV, OR and XOR

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "../include/similaropcodes.h"
#include "../include/parse.h"
#include "../include/Disassembler.h"
#include "../include/jump.h"

void sharedonlyimmneeded(filestruct files, char* opcodename, opcodetype type,
		typeofrun run) {
	//This covers instructions where no MODR/M is needed and only the immm needs to be scanned

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
	//This covers the majority of instructions that follow the form
	//add r/m32, imm32
	//add r/m32, imm8
	//add r/m32, r32
	//add r32, r/m32
	//sar r/m32, 1

	//return starts with success and becomes an error if there is one
	char printbuffer[50];
	char part2[20];
	char part3[20];
	size_t size;

	placerm32inpart2(inputmodrmm, files, part2, sizeof(part2));

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

void registerinopcode4bytes(filestruct files, u8 opcode, char* opcodename,
		typeofrun run) {
	//Register in the first byte of the opcode, 4 more bytes needed for imm32
	//Example instruction = mov r32, imm32 = B8 + rd

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
	//Register in the first byte of the opcode, no more bytes needed
	//Example instruction = pop r32 = 58 + rd

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
	//Nothing else to parse.
	//Example instruction = "nop"

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
//This is used in instructions where there is the opcode then modr/m where the
//instructions are only 2 parts
///Example instruction = pop r/m32

	char printbuffer[50];
	char part2[20];

	placerm32inpart2(inputmodrmm, files, part2, sizeof(part2));

	snprintf(printbuffer, 50, "%x:\t%s %s\n", totalbytecount, opcodename,
			part2);

	if (run == disassemble) {
		if (files.outfileused == true) {
			fwrite(printbuffer, 1, strlen(printbuffer), files.out);
		}
		printf("%s", printbuffer);
	}
}

errorcode shared2plusbyteopcode(filestruct files, u8 opcode, typeofrun run,
		Vector* jumplocations) {
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
		case 0x87:
			//opcode 0F 87
			//JA rel32
			jumprel(files, "jz", opcode_rel32, run, jumplocations);
			break;

			//Second opcode for OF 87
			//JNBE rel32
			//jumprel(files, "jnbe", opcode_rel32, run, jumplocations);
		case 0x83:
			//opcode 0F 83
			//JAE rel32
			jumprel(files, "jae", opcode_rel32, run, jumplocations);
			break;

			//Second opcode for OF 83
			//JNB rel32
			//jumprel(files, "jnb", opcode_rel32, run, jumplocations);

			//Third opcode for OF 83
			//JNC rel32
			//jumprel(files, "jnc", opcode_rel32, run, jumplocations);
		case 0x82:
			//opcode 0F 82
			//JB rel32
			jumprel(files, "jb", opcode_rel32, run, jumplocations);
			break;

			//Second opcode for OF 82
			//JC rel32
			//jumprel(files, "jc", opcode_rel32, run, jumplocations);

			//Third opcode for 0F 82
			//JNAE rel32
			//jumprel(files, "jnae", opcode_rel32, run, jumplocations);
		case 0x86:
			//opcode 0F 86
			//JBE rel32
			jumprel(files, "jbe", opcode_rel32, run, jumplocations);
			break;

			//Second opcode for OF 86
			//JBE rel32
			//jumprel(files, "jbe", opcode_rel32, run, jumplocations);

			//Third opcode for 0F 86
			//JNA rel32
			//jumprel(files, "jna", opcode_rel32, run, jumplocations);
		case 0x84:
			//opcode 0F 84
			//JE rel32
			jumprel(files, "je", opcode_rel32, run, jumplocations);
			break;

			//Second opcode for OF 84
			//JZ rel32
			//jumprel(files, "jz", opcode_rel32, run, jumplocations);
		case 0x8F:
			//opcode 0F 8F
			//JG rel32
			jumprel(files, "jg", opcode_rel32, run, jumplocations);
			break;

			//Second opcode for OF 8F
			//JNLE rel32
			//jumprel(files, "jnle", opcode_rel32, run, jumplocations);
		case 0x8D:
			//opcode 0F 8D
			//JGE rel32
			jumprel(files, "jge", opcode_rel32, run, jumplocations);
			break;

			//Second opcode for OF 8D
			//JNL rel32
			//jumprel(files, "jnl", opcode_rel32, run, jumplocations);
		case 0x8C:
			//opcode 0F 8C
			//JL rel32
			jumprel(files, "jl", opcode_rel32, run, jumplocations);
			break;

			//Second opcode for OF 8C
			//JNGE rel32
			//jumprel(files, "jnge", opcode_rel32, run, jumplocations);
		case 0x8E:
			//opcode 0F 8E
			//JLE rel32
			jumprel(files, "jle", opcode_rel32, run, jumplocations);
			break;

			//Second opcode for OF 85
			//JNZ rel32
			//jumprel(files, "jnz", opcode_rel32, run, jumplocations);
		case 0x85:
			//opcode 0F 85
			//JNE rel32
			jumprel(files, "jne", opcode_rel32, run, jumplocations);
			break;

			//Second opcode for OF 8E
			//JNG rel32
			//jumprel(files, "jng", opcode_rel32, run, jumplocations);
		case 0x81:
			//opcode 0F 81
			//JNO rel32
			jumprel(files, "jno", opcode_rel32, run, jumplocations);
			break;
		case 0x8B:
			//opcode 0F 8B
			//JNP rel32
			jumprel(files, "jnp", opcode_rel32, run, jumplocations);
			break;

			//Second opcode for 0F 8B
			//JPO rel32
			//jumprel(files, "jpo", opcode_rel32, run, jumplocations);
		case 0x89:
			//opcode 0F 89
			//JNS rel32
			jumprel(files, "jns", opcode_rel32, run, jumplocations);
			break;
		case 0x80:
			//opcode 0F 80
			//JNO rel32
			jumprel(files, "jno", opcode_rel32, run, jumplocations);
			break;
		case 0x8A:
			//opcode 0F 8A
			//JP rel32
			jumprel(files, "jp", opcode_rel32, run, jumplocations);
			break;

			//Second opcode for OF 8A
			//JPE rel32
			//jumprel(files, "jpe", opcode_rel32, run, jumplocations);
		case 0x88:
			//opcode 0F 88
			//JS rel32
			jumprel(files, "js", opcode_rel32, run, jumplocations);
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
	default:
		returnvalue = badopcode;
		break;
	}
	return returnvalue;
}

