#include <stdio.h>
#include <stdlib.h>
#include "../include/Disassembler.h"
#include "../include/parse.h"


size_t readopcode(FILE* IN_fp, FILE* OUT_fp, u8* buffer) {
	//Read one byte
	size_t size = fread(buffer, 1, 1, IN_fp);

//	printf("size = %d\n", size);
	if (size == 0) {
		printf("Error reading file\n");
		exit(-2);
	}
	parseopcode(IN_fp, OUT_fp, buffer);

	return size;
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

	size_t size;

	do {
		size = readopcode(inputfp, outputfp, buffer);
		printf("Buffer = %02x\n", buffer[0]);
		if (size != 1) {
			fprintf(stderr, "Error reading opcode\n");
			return (readerror);
		}

	} while (buffer[0] != EOF);

//	int x = buffer[0];
//	printf("int = %x\n", x);

	fclose(inputfp);
	puts("!!!Hello World!!!"); /* prints !!!Hello World!!! */
	return EXIT_SUCCESS;
}


