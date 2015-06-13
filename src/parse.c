#include <stdio.h>
#include <stdlib.h>
#include "../include/parse.h"
#include "../include/add.h"



//a short int is 2 bytes and code is only 1  byte
void parseopcode(FILE* IN_fp, FILE* OUT_fp, u8* buffer) {

	switch (buffer[0]) {
	case 0x05:
		calladd_imm32_to_eax(IN_fp, OUT_fp, buffer);
		break;

	default:
		fprintf(stderr, "ERROR: Opcode of 0x%x not recognized\n", buffer[0]);
	}
}
