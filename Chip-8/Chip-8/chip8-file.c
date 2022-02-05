#include "chip8_file.h"
#include <stdlib.h>


File_out read_all_binary(FILE* file) {
	//Get buffer size
	fseek(file, SEEK_SET, SEEK_END);
	long size = ftell(file);
	fseek(file, SEEK_END, SEEK_SET);
	//Read to buffer
	uint8_t* buffer = (uint8_t*)malloc(size * sizeof(uint8_t));
	fread(buffer, sizeof(uint8_t), size, file);
	fclose(file);

	File_out out;
	out.buffer = buffer;
	out.size = size;
	return out;
}