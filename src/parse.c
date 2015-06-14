#include <stdio.h>
#include <stdlib.h>
#include "../include/parse.h"
#include "../include/Disassembler.h"
#include "../include/similaropcodes.h"

//a short int is 2 bytes and code is only 1  byte
//parse the modr/m here because some difference instructions share the same opcode
errorcode parseopcode(filestruct files, u8 opcode) {

	errorcode returnvalue = success;
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
		shared3partparse(files, opcode, "and", rm32_r32, parsedmodrmm);
		break;
	case 0x23:
		//add r32, r/m32
		shared3partparse(files, opcode, "and", r32_rm32, parsedmodrmm);
		break;
	default:
		fprintf(stderr, "ERROR: Opcode of 0x%x not recognized\n", opcode);
		returnvalue = badopcode;
	}

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

