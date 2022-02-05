#include "array_functions.h"

void copy_to(uint8_t* source, 
	uint8_t* dest, 
	int start_index, 
	int length) {
	
	for (int i = 0; i < length; i++) {
		dest[start_index + i] = source[i];
	}
}