#pragma once
#include <stdint.h>
#include <stdio.h>

typedef struct _file_out {
	uint8_t* buffer;
	long size;
} File_out;

File_out read_all_binary(FILE* file);
