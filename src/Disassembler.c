#include <stdio.h>
#include <stdlib.h>
#include "../include/Disassembler.h"
#include "../include/parse.h"


const char* registerstrings[] = { "EAX", "ECX", "EDX", "EBX", "ESP",
		"EBP", "ESI", "EDI" };
u32 totalbytecount;
u32	instructionbytecount;

errorcode readopcode(filestruct files) {
	//Read one byte
	errorcode returnflag;
	u8 opcode;

	instructionbytecount += 1;
	size_t size = fread(&opcode, 1, 1, files.in);

	if (size == 0) {
		printf("END of File Reached\n");
		return endoffile;
	}
	printf("Buffer = %02x\n", opcode);
	returnflag = parseopcode(files, opcode);

	return returnflag;
}

int main(int argc, char *argv[]) {
	if (argc != 3) {
		fprintf(stderr, "usage %s input_BinaryFile output_x86_AssemblyFile",
				argv[0]);
		return (missing_arg_2);
	}

	filestruct files;

	files.in = fopen(argv[1], "rb");

	if (files.in == NULL) {
		fprintf(stderr, "File %s does not exist", argv[1]);
		return (filedoesnotexist);
	}

	files.out = fopen(argv[2], "w");

	totalbytecount = 0;
	instructionbytecount = 0;

	errorcode runningflag;

	do {
		runningflag = readopcode(files);

	} while (runningflag == success);

//	int x = buffer[0];
//	printf("int = %x\n", x);

	int returnvalue;

	if (runningflag == endoffile) {
		printf("Successfully disassembled file\n");
		cleanupandclose(files, success);
		returnvalue = EXIT_SUCCESS;
	} else {
		cleanupandclose(files, runningflag);
		return -1;
	}

	return returnvalue;
}

