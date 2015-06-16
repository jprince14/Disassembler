//This will work for ADD, AND, CMP, MOV, OR and XOR

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "../include/similaropcodes.h"
#include "../include/parse.h"
#include "../include/Disassembler.h"

void shared3partparse(filestruct files, u8 opcode, char* opcodename,
		opcodetype type, modrmm inputmodrmm) {

	//return starts with success and becomes an error if there is one
	char printbuffer[50];
	char part2[20];
	char part3[20];
	size_t size;

	if (type == eax_imm32) {

		u32 imm32;
		size_t size = fread(&imm32, 1, 4, files.in);
		readerrorcheck(size, 4, files);
		instructionbytecount += 4;

		char printbuffer[50];
		snprintf(printbuffer, 40, "%x:\t%s, eax, 0x%x\n", totalbytecount,
				opcodename, imm32);
		fwrite(printbuffer, 1, strlen(printbuffer), files.out);
#if printtoscreen
		printf("%s", printbuffer);
#endif
	} else {

		getpart2(inputmodrmm, files, part2, sizeof(part2));

		if (type == rm32_imm32) {

			u32 imm32;
			size = fread(&imm32, 1, 4, files.in);
			readerrorcheck(size, 4, files);
			instructionbytecount += 4;
			snprintf(part3, 20, "0x%x", imm32);

			snprintf(printbuffer, 50, "%x:\t%s %s, %s\n", totalbytecount,
					opcodename, part2, part3);

		} else if (type == rm32_imm8) {

			u8 imm8;
			size = fread(&imm8, 1, 1, files.in);
			readerrorcheck(size, 1, files);
			instructionbytecount += 1;
			snprintf(part3, 20, "0x%x", imm8);

			snprintf(printbuffer, 50, "%x:\t%s %s, %s\n", totalbytecount,
					opcodename, part2, part3);

		} else if (type == rm32_r32) {

			snprintf(part3, 20, "%s", registerstrings[inputmodrmm.modrm_Reg]);
			snprintf(printbuffer, 50, "%x:\t%s %s, %s\n", totalbytecount,
					opcodename, part2, part3);

		} else if (type == r32_rm32) {
			snprintf(part3, 20, "%s", registerstrings[inputmodrmm.modrm_Reg]);

			//parts 2 and 3 are flipped for this one
			snprintf(printbuffer, 50, "%x:\t%s %s, %s\n", totalbytecount,
					opcodename, part3, part2);
		} else {
			//Invalid type received
			cleanupandclose(files, badopcode);
		}

		fwrite(printbuffer, 1, strlen(printbuffer), files.out);
#if printtoscreen
		printf("%s", printbuffer);
#endif
	}
}

void reg4bytes(filestruct files, u8 opcode, char* opcodename) {
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

	fwrite(printbuffer, 1, strlen(printbuffer), files.out);
#if printtoscreen
	printf("%s", printbuffer);
#endif
}

