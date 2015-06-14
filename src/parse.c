#include <stdio.h>
#include <stdlib.h>
#include "../include/parse.h"
#include "../include/Disassembler.h"
#include "../include/add.h"

//a short int is 2 bytes and code is only 1  byte
errorcode parseopcode(FILE* IN_fp, FILE* OUT_fp, u8* buffer) {

	errorcode returnvalue;

	switch (buffer[0]) {
	case 0x05:
		returnvalue = calladd_imm32_to_eax(IN_fp, OUT_fp, buffer);
		break;
	case 0x81:
		returnvalue = calladd_imm32_to_rm32(IN_fp, OUT_fp, buffer);
		break;

	default:
		fprintf(stderr, "ERROR: Opcode of 0x%x not recognized\n", buffer[0]);
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

errorcode readerrorcheck(size_t sizeread, size_t expectedsize) {
	errorcode returnflag;
	if (sizeread != expectedsize) {
		fprintf(stderr, "Either End of File or Error Reading\n");
		returnflag = parsingerror;
	}
	returnflag = success;
	return returnflag;
}

