#include <stdio.h>
#include <stdlib.h>
#include "../include/Disassembler.h"
#include "../include/parse.h"

errorcode readopcode(FILE* IN_fp, FILE* OUT_fp, u8* buffer) {
	//Read one byte
	errorcode returnflag;

	size_t size = fread(buffer, 1, 1, IN_fp);

//	printf("size = %d\n", size);
	if (size == 0) {
		printf("END of File Reached\n");
		return endoffile;
	}
	printf("Buffer = %02x\n", buffer[0]);
	returnflag = parseopcode(IN_fp, OUT_fp, buffer);

	return returnflag;
}

int main(int argc, char *argv[]) {
	if (argc != 3) {
		fprintf(stderr, "usage %s input_BinaryFile output_x86_AssemblyFile",
				argv[0]);
		return (missing_arg_2);
	}

	FILE *inputfp;

	u8 buffer[10];
	inputfp = fopen(argv[1], "rb");

	if (inputfp == NULL) {
		fprintf(stderr, "File %s does not exist", argv[1]);
		return (filedoesnotexist);
	}

	FILE *outputfp;
	outputfp = fopen("outputfile.txt", "w");

	errorcode runningflag;

	do {
		runningflag = readopcode(inputfp, outputfp, buffer);

	} while (runningflag == success);

//	int x = buffer[0];
//	printf("int = %x\n", x);

	fclose(inputfp);
	return EXIT_SUCCESS;
}

