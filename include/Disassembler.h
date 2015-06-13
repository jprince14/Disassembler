/*
 * Disassembler.h
 *
 *  Created on: Jun 12, 2015
 *      Author: jprin
 */

#ifndef DISASSEMBLER_H_
#define DISASSEMBLER_H_

typedef enum {
	FILE_OPEN_SUCCESS, FILE_OPEN_FAIL
} file_open_codes;

typedef enum {
	success, missing_arg_2, filedoesnotexist, readerror

} errorcode;


typedef unsigned char u8;
typedef unsigned short int u16;
typedef unsigned int u32;

#endif /* DISASSEMBLER_H_ */

