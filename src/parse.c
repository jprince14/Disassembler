#include <stdio.h>
#include <stdlib.h>
#include "../include/parse.h"
#include "../include/Disassembler.h"
#include "../include/add.h"

//a short int is 2 bytes and code is only 1  byte
errorcode parseopcode(filestruct files, u8 opcode) {

	errorcode returnvalue = success;
	switch (opcode) {
	case 0x05: //add eax, imm32
		calladd_imm32_to_eax(files, opcode);
		break;
	case 0x81: //add r/m32, imm32
			   //fall through
	case 0x83: //add r/m32, imm8
			   //fall through
	case 0x01: //add r/m32, r32
		//fall through
	case 0x03: //add r32, r/m32
		calladd_rm32(files, opcode);
		break;

	default:
		fprintf(stderr, "ERROR: Opcode of 0x%x not recognized\n", opcode);
		returnvalue = badopcode;
	}

	return returnvalue;
}

modrmm parsemodrmm(u8 input) {

	modrmm result;

	//Parse the MOD
	//AND with 0b11000000 to get the first 2 bytes
	result.modrm_MOD = ((input >> 6) & 0b00000011);

	result.modrm_Reg = ((input >> 3) & 0b00000111);

	result.modrm_RM_Reg = (input & 0b00000111);

	return result;
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

