#include <string.h>

#include "../include/add.h"
#include "../include/parse.h"
#include "../include/Disassembler.h"

errorcode calladd_imm32_to_eax(FILE* IN_fp, FILE* OUT_fp, u8* buffer) {

	u32 imm32;
	size_t size = fread(&imm32, 1, 4, IN_fp);
	if (size != 4) {
		fprintf(stderr, "Either End of File or Error Reading\n");
		return parsingerror;
	}

	char printbuffer[50];
	snprintf(printbuffer, 40, "add, eax, 0x%x\n", imm32);
	fwrite(printbuffer, 1, strlen(printbuffer), OUT_fp);
	return success;
}

errorcode calladd_imm32_to_rm32(FILE* IN_fp, FILE* OUT_fp, u8* buffer) {

	errorcode returnflag;
	char printbuffer[50];
	char part1[] = "add ";
	char part2[20];
	char part3[20];

	size_t size;
	size = fread(buffer + 1, 1, 1, IN_fp);
	if (size != 1) {
		fprintf(stderr, "Either End of File or Error Reading\n");
		return parsingerror;
	}
	modrmm parsedmodrmm = parsemodrmm(buffer[1]);

	// [reg]
	if (parsedmodrmm.modrm_MOD == mod0) {
		snprintf(part2, 20, "[%s], ",
				registerstrings[parsedmodrmm.modrm_RM_Reg]);
	} else if (parsedmodrmm.modrm_MOD == mod1) {
		u8 byte;
		size = fread(&byte, 1, 1, IN_fp);
		if (readerrorcheck(size, 1) == success) {
			snprintf(part2, 20, "[%s + 0x%x], ",
					registerstrings[parsedmodrmm.modrm_RM_Reg], byte);
		} else {
			returnflag = parsingerror;
		}
	} else if (parsedmodrmm.modrm_MOD == mod2) {
		u32 dword;
		size = fread(&dword, 1, 4, IN_fp);
		if (readerrorcheck(size, 4) == success) {
			snprintf(part2, 20, "[%s + 0x%x], ",
					registerstrings[parsedmodrmm.modrm_RM_Reg], dword);
		} else {
			returnflag = parsingerror;
		}
	} else if (parsedmodrmm.modrm_MOD == mod3) {
		snprintf(part2, 20, "%s, ", registerstrings[parsedmodrmm.modrm_RM_Reg]);
	}

	//read the imm32
	u32 imm32;
	size = fread(&imm32, 1, 4, IN_fp);
	if (size != 4) {
		fprintf(stderr, "Either End of File or Error Reading\n");
		return parsingerror;
	}

	snprintf(part3, 20, "0x%x\n", imm32);

	snprintf(printbuffer, 50, "%s%s%s", part1, part2, part3);

	fwrite(printbuffer, 1, strlen(printbuffer), OUT_fp);
	return success;

	return returnflag;
}
