#include <stdio.h>
#include <stdlib.h>
#include "../include/parse.h"
#include "../include/Disassembler.h"
#include "../include/add.h"
#include "../include/and.h"

//a short int is 2 bytes and code is only 1  byte
errorcode parseopcode(filestruct files, u8 opcode) {

	errorcode returnvalue = success;
	switch (opcode) {
	case 0x05: //add eax, imm32
		shared3partparse(files, opcode, "add", eax_imm32);
		break;
	case 0x81: //add r/m32, imm32
		shared3partparse(files, opcode, "add", rm32_imm32);
		break;
	case 0x83: //add r/m32, imm8
		shared3partparse(files, opcode, "add", rm32_imm8);
		break;
	case 0x01: //add r/m32, r32
		shared3partparse(files, opcode, "add", rm32_r32);
		break;
	case 0x03: //add r32, r/m32
		shared3partparse(files, opcode, "add", r32_rm32);
		break;

	default:
		fprintf(stderr, "ERROR: Opcode of 0x%x not recognized\n", opcode);
		returnvalue = badopcode;
	}

	return returnvalue;
}

char* parsemodrmm(u8 input, filestruct files, char* part2, int arraysize) {

	modrmm result;
	size_t size;

	//Parse the MOD
	//AND with 0b11000000 to get the first 2 bytes
	result.modrm_MOD = ((input >> 6) & 0b00000011);

	result.modrm_Reg = ((input >> 3) & 0b00000111);

	result.modrm_RM_Reg = (input & 0b00000111);

	if (result.modrm_MOD == mod0) {
		snprintf(part2, arraysize, "[%s]",
				registerstrings[result.modrm_RM_Reg]);
	} else if (result.modrm_MOD == mod1) {
		u8 byte;
		size = fread(&byte, 1, 1, files.in);
		readerrorcheck(size, 1, files);
		snprintf(part2, arraysize, "[%s + 0x%x]",
				registerstrings[result.modrm_RM_Reg], byte);

	} else if (result.modrm_MOD == mod2) {
		u32 dword;
		size = fread(&dword, 1, 4, files.in);
		readerrorcheck(size, 4, files);
		snprintf(part2, arraysize, "[%s + 0x%x]",
				registerstrings[result.modrm_RM_Reg], dword);
	} else if (result.modrm_MOD == mod3) {
		snprintf(part2, arraysize, "%s", registerstrings[result.modrm_RM_Reg]);
	}

	return part2;
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
	printf("COMEBACKHERE\n");
	//display to screen adn print to file

}

