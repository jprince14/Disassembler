#ifndef ADD_H_
#define ADD_H_

#include <stdio.h>
#include <stdlib.h>
#include "Disassembler.h"

errorcode calladd_imm32_to_eax(FILE* IN_fp, FILE* OUT_fp, u8* buffer);

errorcode calladd_imm32_to_rm32(FILE* IN_fp, FILE* OUT_fp, u8* buffer);


#endif /* ADD_H_ */
