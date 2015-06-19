#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/Disassembler.h"
#include "../include/parse.h"
#include "../include/jump.h"

void jmp_nomodrm(filestruct files, char* opcodename, opcodetype type,
		typeofrun run, Vector* jumplocations) {

	char printbuffer[50];
	u32 jumplocation;

	if (type == opcode_rel32) {
		s32 rel32;
		size_t size = fread(&rel32, 1, 4, files.in);
		readerrorcheck(size, 4, files);
		instructionbytecount += 4;

		//determine if it is a jump forwards or backwards

//		if ((rel32 & 0xF0000000) == 0xF0000000) {
//			//Negative Number
//		}

		jumplocation = totalbytecount + instructionbytecount + rel32;

		snprintf(printbuffer, 40, "%x:\t%s offset_0x%x\n", totalbytecount,
				opcodename, jumplocation);

		if (run == findjumps) {
			insertVector(jumplocations, jumplocation);
		}
	} else if (type == opcode_rel8) {
		s8 rel8;
		size_t size = fread(&rel8, 1, 1, files.in);
		readerrorcheck(size, 1, files);
		instructionbytecount += 1;

		jumplocation = totalbytecount + instructionbytecount + rel8;

		snprintf(printbuffer, 40, "%x:\t%s offset_0x%x\n", totalbytecount,
				opcodename, jumplocation);

		if (run == findjumps) {
			insertVector(jumplocations, jumplocation);
		}
	}

	if (run == disassemble) {
		if (files.outfileused == true) {
			fwrite(printbuffer, 1, strlen(printbuffer), files.out);
		}
		printf("%s", printbuffer);
	}
}

void jump2partparse(filestruct files, char* opcodename, modrmm inputmodrmm,
		typeofrun run, Vector* jumplocations) {

	char printbuffer[50];
	char part2[20];

	getpart2(inputmodrmm, files, part2, sizeof(part2));

	snprintf(printbuffer, 50, "%x:\t%s %s\n", totalbytecount, opcodename,
			part2);

#warning - currently no jump labeling here, do i need one for jumping to a reg or memory location

	if (run == disassemble) {
		if (files.outfileused == true) {
			fwrite(printbuffer, 1, strlen(printbuffer), files.out);
		}
		printf("%s", printbuffer);
	}

}

