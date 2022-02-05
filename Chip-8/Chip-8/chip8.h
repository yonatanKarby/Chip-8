#pragma once
#include <stdio.h>

typedef struct _chip8 chip8;

chip8* init_chip8();

void run(chip8* c8 ,FILE* file_to_code);
