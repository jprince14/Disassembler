#ifndef CALL_H_
#define CALL_H_
#include "parse.h"
#include "Disassembler.h"

extern const char* registerstrings[];

typedef struct {
	u32 *array;
	size_t used;
	size_t size;
} Vector;

void initVector(Vector *vec, size_t initialSize);

void insertVector(Vector *vec, u32 element);

void removeElementVector(Vector *vec, int indextodelete);

void freeVector(Vector *vec);

int searchVector(Vector *vec, u32 findthis);

#endif /* CALL_H_ */
