#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "../include/Disassembler.h"
#include "../include/similaropcodes.h"
#include "../include/parse.h"
#include "../include/Vector.h"

//COMMENT: For this vector code I reused portions of code the following website
//While also adding my own additional functionality to the vectors
//Source Site: http://stackoverflow.com/questions/3536153/c-dynamically-growing-array

void initVector(Vector *vec, size_t initialSize) {
	vec->array = (u32 *) malloc(initialSize * sizeof(u32));
	vec->used = 0;
	vec->size = initialSize;
}

void insertVector(Vector *vec, u32 element) {
	if (vec->used == vec->size) {
		//Realloc by size*2 so realloc's aren't as frequent
		vec->size *= 2;
		vec->array = (u32 *) realloc(vec->array, vec->size * sizeof(u32));
	}
	vec->array[vec->used++] = element;
}

void removeElementVector(Vector *vec, int indextodelete) {

	//Make sure that the element is in the bounds so vec->used
	//is only decremented if a valid element is deleted
	if (indextodelete < vec->used) {
		u32* newarray = (u32 *) malloc(vec->size * sizeof(u32));

		int x;
		for (x = 0; x < vec->used; x++) {
			if (x < indextodelete) {
				newarray[x] = vec->array[x];
			} else if (x == indextodelete) {
				//Do nothing, this element is getting deleted
				continue;
			} else if (x > indextodelete) {
				newarray[x - 1] = vec->array[x];
			}
		}
		free(vec->array);
		vec->used -= 1;
		vec->array = newarray;
	}
}

void freeVector(Vector *vec) {
	free(vec->array);
	vec->array = NULL;
	vec->used = vec->size = 0;
}

int searchVector(Vector *vec, u32 findthis) {
	int index;

	for (index = 0; index < vec->used; index++) {
		if (vec->array[index] == findthis) {
			break;
		} else {
			continue;
		}
	}
	//Index will equal the index of the match or vec->used if no match
	return index;
}

