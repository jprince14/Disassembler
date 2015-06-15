#include <stdio.h>
#include <stdlib.h>
#include "../include/parse.h"
#include "../include/Disassembler.h"
#include "../include/similaropcodes.h"

//a short int is 2 bytes and code is only 1  byte
//parse the modr/m here because some difference instructions share the same opcode
errorcode parseopcode(filestruct files, u8 opcode) {

	errorcode returnvalue = success;
	bool match = true; //If there is a match in the switch statment it stays true
					   //If it hits the default match = false

	//parsedmodrmm needs to be set here for all opcodes that require a MODR/M byte
	//for ones that done require it pass it in parsedmodrmm as uninitialized, dont
	//know a better way. It is needed at this point for instructions that share the
	//same opcode (such as 0x81 and 0x83)
	modrmm parsedmodrmm;

	switch (opcode) {
	//ADD
	case 0x05: //add eax, imm32
//TODO: passing in parsedmodrmm which is empty, dont really need it but cant think of a better way at the moment
		shared3partparse(files, opcode, "add", eax_imm32, parsedmodrmm);
		break;
	case 0x81:
		parsedmodrmm = getandparsemodrmm(files);
		if (parsedmodrmm.modrm_Reg == 0x0) {
			//add r/m32, imm32
			shared3partparse(files, opcode, "add", rm32_imm32, parsedmodrmm);
		} else if (parsedmodrmm.modrm_Reg == 0x4) {
			//and r/m32, imm32
			shared3partparse(files, opcode, "and", rm32_imm32, parsedmodrmm);
		} else if (parsedmodrmm.modrm_Reg == 0x7) {
			//cmp r/m32, imm32
			shared3partparse(files, opcode, "cmp", rm32_imm32, parsedmodrmm);
		} else {
			returnvalue = badopcode;
		}
		break;
	case 0x83:
		parsedmodrmm = getandparsemodrmm(files);
		if (parsedmodrmm.modrm_Reg == 0x0) {
			//add r/m32, imm8
			shared3partparse(files, opcode, "add", rm32_imm8, parsedmodrmm);
		} else if (parsedmodrmm.modrm_Reg == 0x4) {
			//and r/m32, imm8
			shared3partparse(files, opcode, "and", rm32_imm8, parsedmodrmm);
		} else if (parsedmodrmm.modrm_Reg == 0x7) {
			//cmp r/m32, imm8
			shared3partparse(files, opcode, "cmp", rm32_imm8, parsedmodrmm);
		} else {
			returnvalue = badopcode;
		}
		break;
	case 0xC7:
		parsedmodrmm = getandparsemodrmm(files);
		if (parsedmodrmm.modrm_Reg == 0x0) {
			//mov r/m32, imm32
			shared3partparse(files, opcode, "mov", rm32_imm32, parsedmodrmm);
		} else {
			returnvalue = badopcode;
		}
		break;
	case 0x01:
		//add r/m32, r32
		parsedmodrmm = getandparsemodrmm(files);
		shared3partparse(files, opcode, "add", rm32_r32, parsedmodrmm);
		break;
	case 0x03:
		//add r32, r/m32
		parsedmodrmm = getandparsemodrmm(files);
		shared3partparse(files, opcode, "add", r32_rm32, parsedmodrmm);
		break;
		//AND
	case 0x25:
		//and eax, imm32
		shared3partparse(files, opcode, "and", eax_imm32, parsedmodrmm);
		break;
	case 0x21:
		//and r/m32, r32
		parsedmodrmm = getandparsemodrmm(files);
		shared3partparse(files, opcode, "and", rm32_r32, parsedmodrmm);
		break;
	case 0x23:
		//add r32, r/m32
		parsedmodrmm = getandparsemodrmm(files);
		shared3partparse(files, opcode, "and", r32_rm32, parsedmodrmm);
		break;
	case 0x3d:
		//cmp eax, imm32
		shared3partparse(files, opcode, "cmp", eax_imm32, parsedmodrmm);
		break;
	case 0x39:
		//cmp r/m32, r32
		parsedmodrmm = getandparsemodrmm(files);
		shared3partparse(files, opcode, "cmp", rm32_r32, parsedmodrmm);
		break;
	case 0x3B:
		//cmp r32, r/m32
		shared3partparse(files, opcode, "cmp", r32_rm32, parsedmodrmm);
		break;
	case 0x89:
		//mov r/m32, r32
		parsedmodrmm = getandparsemodrmm(files);
		shared3partparse(files, opcode, "mov", rm32_r32, parsedmodrmm);
		break;
	case 0x8B:
		//mov r32, r/m32
		parsedmodrmm = getandparsemodrmm(files);
		shared3partparse(files, opcode, "mov", r32_rm32, parsedmodrmm);
		break;
	default:
		match = false;
		//try more options below
	}

	//Now try other instructions that dont fit in the switch statement
	if (match == false) {
		if ((opcode & 0xB8) == 0xB8) {
			//mov r32, imm32
			//This matches the mov eax, imm32 for the assignment but adds more functionality
			reg4bytes(files, opcode, "mov");
		} else {
			//There were no matches in the switch or above so its a unknown opcode

			fprintf(stderr, "ERROR: Opcode of 0x%x not recognized\n", opcode);
			returnvalue = badopcode;
		}
	}
//Update the instruction counters
	totalbytecount += instructionbytecount;
	instructionbytecount = 0;

	return returnvalue;
}

modrmm getandparsemodrmm(filestruct files) {

	modrmm result;
	size_t size;
	u8 modrm;

//Read the MODR/M byte
	size = fread(&modrm, 1, 1, files.in);
	readerrorcheck(size, 1, files);
	instructionbytecount += 1;

//Parse the MOD
//AND with 0b11000000 to get the first 2 bytes
	result.modrm_MOD = ((modrm >> 6) & 0b00000011);

	result.modrm_Reg = ((modrm >> 3) & 0b00000111);

	result.modrm_RM_Reg = (modrm & 0b00000111);

	return result;
}

void getpart2(modrmm input, filestruct files, char* part2, int part2size) {
	size_t size;

	if (input.modrm_MOD == mod0) {
		snprintf(part2, part2size, "[%s]", registerstrings[input.modrm_RM_Reg]);
	} else if (input.modrm_MOD == mod1) {
		u8 byte;
		size = fread(&byte, 1, 1, files.in);
		readerrorcheck(size, 1, files);
		instructionbytecount += 1;
		snprintf(part2, part2size, "[%s + 0x%x]",
				registerstrings[input.modrm_RM_Reg], byte);

	} else if (input.modrm_MOD == mod2) {
		u32 dword;
		size = fread(&dword, 1, 4, files.in);
		readerrorcheck(size, 4, files);
		instructionbytecount += 4;
		snprintf(part2, part2size, "[%s + 0x%x]",
				registerstrings[input.modrm_RM_Reg], dword);
	} else if (input.modrm_MOD == mod3) {
		snprintf(part2, part2size, "%s", registerstrings[input.modrm_RM_Reg]);
	}

}

void getpart2fromopcode(u8 opcode, char* part2, int part2size) {
//for when the last 3 bytes of the opcode represent the register
	snprintf(part2, part2size, "%s", registerstrings[(opcode & 0b00000111)]);
}

void readerrorcheck(size_t sizeread, size_t expectedsize, filestruct files) {
	if (sizeread != expectedsize) {
		fprintf(stderr, "Error Size Read in was not size expected\n");
		cleanupandclose(files, parsingerror);
	}
}

void cleanupandclose(filestruct files, errorcode code) {
//	char errorstring[] = "\nERROR with Disassembly\n";

	fclose(files.in);
	fclose(files.out);
	if (code != success) {
		displayerroroutput(code);
	}
	exit(-1);

}

void displayerroroutput(errorcode code) {
//TODO: come back to add errrorcode parsing
	printf("COMEBACKHERE\n");
//display to screen adn print to file

}

