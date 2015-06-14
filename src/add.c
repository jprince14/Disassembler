#include <string.h>

#include "../include/add.h"
#include "../include/parse.h"
#include "../include/Disassembler.h"

void calladd_imm32_to_eax(filestruct files, u8 buffer) {

	u32 imm32;
	size_t size = fread(&imm32, 1, 4, files.in);
	readerrorcheck(size, 4, files);

	char printbuffer[50];
	snprintf(printbuffer, 40, "add, eax, 0x%x\n", imm32);
	fwrite(printbuffer, 1, strlen(printbuffer), files.out);
}

void calladd_rm32(filestruct files, u8 opcode) {

	//return starts with success and becomes an error if there is one
	char printbuffer[50];
	char part1[] = "add";
	char part2[20];
	char part3[20];
	modrmm parsedmodrmm;
	size_t size;
	u8 modrm;

	size = fread(&modrm, 1, 1, files.in);
	readerrorcheck(size, 1, files);

	parsedmodrmm = parsemodrmm(modrm);

	printf("location1\n");

	if (parsedmodrmm.modrm_MOD == mod0) {
		snprintf(part2, 20, "[%s]", registerstrings[parsedmodrmm.modrm_RM_Reg]);
	} else if (parsedmodrmm.modrm_MOD == mod1) {
		u8 byte;
		size = fread(&byte, 1, 1, files.in);
		readerrorcheck(size, 1, files);
		snprintf(part2, 20, "[%s + 0x%x]",
				registerstrings[parsedmodrmm.modrm_RM_Reg], byte);

	} else if (parsedmodrmm.modrm_MOD == mod2) {
		u32 dword;
		size = fread(&dword, 1, 4, files.in);
		readerrorcheck(size, 4, files);
		snprintf(part2, 20, "[%s + 0x%x]",
				registerstrings[parsedmodrmm.modrm_RM_Reg], dword);
	} else if (parsedmodrmm.modrm_MOD == mod3) {
		snprintf(part2, 20, "%s", registerstrings[parsedmodrmm.modrm_RM_Reg]);
	}
	printf("location2\n");

	if (opcode == 0x81) {
		u32 imm32;
		size = fread(&imm32, 1, 4, files.in);
		readerrorcheck(size, 4, files);
		snprintf(part3, 20, "0x%x", imm32);

	} else if (opcode == 0x83) {

		u8 imm8;
		size = fread(&imm8, 1, 1, files.in);
		readerrorcheck(size, 4, files);
		snprintf(part3, 20, "0x%x", imm8);

	} else if (opcode == 0x01) {

		snprintf(part3, 20, "%s", registerstrings[parsedmodrmm.modrm_Reg]);

	} else if (opcode == 0x03) {

		snprintf(part3, 20, "%s", registerstrings[parsedmodrmm.modrm_Reg]);

	}

	else {

		cleanupandclose(files, badopcode);

	}

	printf("location4\n");

	if (opcode == 0x81 || opcode == 0x83 || opcode == 0x01) {

		snprintf(printbuffer, 50, "%s %s, %s", part1, part2, part3);

	} else if (opcode == 0x03) {

		//for opcode 03 part 2 and 3 are swapped
		snprintf(printbuffer, 50, "%s %s, %s", part1, part3, part2);

	}

	fwrite(printbuffer, 1, strlen(printbuffer), files.out);

}

errorcode calladd_imm8_to_rm32(filestruct files, u8 buffer) {
	errorcode returnflag = success;

	return returnflag;
}

