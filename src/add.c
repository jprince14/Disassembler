#include "../include/add.h"
#include "../include/Disassembler.h"

errorcode calladd_imm32_to_eax(FILE* IN_fp, FILE* OUT_fp, u8* buffer){
			size_t size = fread(buffer + 1, 1, 4, IN_fp);
			if (size != 4) {
				fprintf(stderr, "Either End of File or Error Reading\n");
				return parsingerror;
			}

			char printbuffer[50];
			snprintf(printbuffer,40, "add, eax, 0x%x%x%x%x\n", buffer[4], buffer[3], buffer[2], buffer[1]);
			fwrite(printbuffer,1, strlen(printbuffer), OUT_fp);
			return success;
}
