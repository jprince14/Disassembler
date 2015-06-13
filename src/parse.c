#include <stdio.h>
#include <stdlib.h>
#include "../include/parse.h"
#include "../include/Disassembler.h"



//a short int is 2 bytes and code is only 1  byte
void parseopcode(FILE* IN_fp, FILE* OUT_fp, u8* buffer) {
	size_t size;

	switch (buffer[0]) {
	case 0x05:
		calladd_imm32_to_eax(IN_fp, OUT_fp, buffer);




		//do something
		size = fread(buffer, 1, 1, IN_fp);
		if (size != 1) {
			fprintf(stderr, "Either End of File or Error Reading\n");
			exit(-1);
		}
		char str[] = "Testing";
		fwrite(str,1, sizeof(str), OUT_fp);
		break;

	default:
		fprintf(stderr, "ERROR: Opcode of 0x%x not recognized\n", buffer[0]);
	}
}
