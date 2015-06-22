#include <stdio.h>
#include <stdlib.h>
#include <netinet/in.h>
#include "../include/parse.h"

#include "../include/Disassembler.h"

const char* registerstrings[] = { "eax", "ecx", "edx", "ebx", "esp", "ebp",
		"esi", "edi" };

u32 totalbytecount;
u32 instructionbytecount;
Vector g_jumplocations;
char g_opcodes[40];

int main(int argc, char *argv[]) {

	if (argc != 2 && argc != 3) {
		fprintf(stderr,
				"usage %s [REQUIRED input_BinaryFile] [OPTIONAL output_x86_AssemblyFile]",
				argv[0]);
		return (missing_arg);
	}

	filestruct files;

	files.in = fopen(argv[1], "rb");

	if (files.in == NULL) {
		fprintf(stderr, "File %s does not exist", argv[1]);
		return (filedoesnotexist);
	}

	if (argc == 3) {
		files.out = fopen(argv[2], "w");
		files.outfileused = true;
	} else {
		files.outfileused = false;
	}

	initVector(&g_jumplocations, 20);

	totalbytecount = 0;
	instructionbytecount = 0;
	memset(g_opcodes, 0x00, sizeof(g_opcodes));

	errorcode runningflag;

	do {
		//Run the first pass to figure out the jumps and place them in a vector
		//If success then run again this time labeling the jumps and printing the assembly
		//I thought that parsing through twice one to get the jump locations and the second
		//time to get the assembly (which I also get in the first run b/c of function reuse)
		//would be the best way to print the backwards jumps
		runningflag = parseopcode(files, findjumps, &g_jumplocations);

	} while (runningflag == success);

	int returnvalue;

	if (runningflag == endoffile) {
//Successfully build the vector of jumps, now time to print the assembly with the jumps labeled

		//set the input file stream to the beginning of the file
		rewind(files.in);

		//reset the byte counts
		//resetting instructionbytecount to be safe
		totalbytecount = 0;
		instructionbytecount = 0;

		do {
			runningflag = parseopcode(files, disassemble, &g_jumplocations);
		} while (runningflag == success);

		printf("Successfully disassembled file\n");
		cleanupandclose(files, success);
		returnvalue = EXIT_SUCCESS;
	} else {
		cleanupandclose(files, runningflag);
		returnvalue = -1;
	}

	return returnvalue;
}

