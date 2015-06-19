#ifndef DISASSEMBLER_H_
#define DISASSEMBLER_H_

typedef enum {
	FILE_OPEN_SUCCESS, FILE_OPEN_FAIL
} file_open_codes;

typedef enum {
	success,
	missing_arg,
	filedoesnotexist,
	readerror,
	parsingerror,
	badopcode,
	endoffile

} errorcode;

typedef enum {
	false, true
} bool;

typedef enum {
	findjumps, disassemble
} typeofrun;

typedef struct {
	FILE* in;
	FILE* out;
	bool outfileused;
} filestruct;

typedef unsigned char u8;
typedef signed char s8;
typedef unsigned short int u16;
typedef unsigned int u32;
typedef signed int s32;

#endif /* DISASSEMBLER_H_ */

