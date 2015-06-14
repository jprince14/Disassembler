#ifndef DISASSEMBLER_H_
#define DISASSEMBLER_H_

typedef enum {
	FILE_OPEN_SUCCESS, FILE_OPEN_FAIL
} file_open_codes;

typedef enum {
	success,
	missing_arg_2,
	filedoesnotexist,
	readerror,
	parsingerror,
	badopcode,
	endoffile

} errorcode;

typedef enum {
	false, true
} bool;

typedef struct {
	FILE* in;
	FILE* out;
} filestruct;

typedef unsigned char u8;
typedef unsigned short int u16;
typedef unsigned int u32;

static const char* registerstrings[] = { "EAX", "ECX", "EDX", "EBX", "ESP",
		"EBP", "ESI", "EDI" };

errorcode readopcode(filestruct files);

#endif /* DISASSEMBLER_H_ */

