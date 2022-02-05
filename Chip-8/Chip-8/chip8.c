#include "chip8.h"
#include "chip8_file.h"
#include "array_functions.h"
#include <stdint.h>
#include <stdlib.h>

#define MEM_SIZE 4096
#define STACK_SIZE 16
#define INSTRUCTION_SIZE 2

typedef struct _chip8 {
	uint8_t V[16];
	uint16_t I;
	uint16_t PC;
	uint16_t SP;

	uint8_t memory[MEM_SIZE];
} chip8;

chip8* init_chip8() {
	chip8* c8 = (chip8*)malloc(sizeof(chip8));
	if (c8 == NULL)
		return NULL;
	for (int i = 0; i < 16; i++) {
		c8->V[i] = 0;
	}
	c8->I = 0;
	c8->PC = 0;
	c8->SP = MEM_SIZE - 256 - 1;
	return c8;
}

static uint16_t pop_stack(chip8* chip)
{
	return ((uint16_t)chip->memory[chip->SP++]) << 8 | (uint16_t)(chip->memory[chip->SP++]);
}

static void push_stack(chip8* chip)
{
	chip->memory[chip->SP--] = (uint8_t)(chip->PC);
	chip->memory[chip->SP--] = (uint8_t)(chip->PC >> 8);
}

static void register_operations(chip8* chip, uint16_t opcode) {
	uint8_t registerX = (opcode & 0x0F00) >> 0x8;
	uint8_t registerY = (opcode & 0x00F0) >> 0x4;

	switch (opcode & 0x000F)
	{
		case 0x0000:
			chip->V[registerX] = chip->V[registerY];
			break;
		case 0x0001:
			chip->V[registerX] |= chip->V[registerY];
			break;
		case 0x0002:
			chip->V[registerX] &= chip->V[registerY];
			break;
		case 0x0003:
			chip->V[registerX] ^= chip->V[registerY];
			break;
		case 0x0004:
			chip->V[registerX] += chip->V[registerY];
			break;
		case 0x0005:
			chip->V[registerX] -= chip->V[registerY];
			break;
		case 0x0006:
			chip->V[0xF] = chip->V[registerX] & 0x1;
			chip->V[registerX] >>= 1;
			break;
		case 0x0007:
			chip->V[registerX] = chip->V[registerY] - chip->V[registerX];
			break;
		case 0x000E:
			chip->V[0xF] = chip->V[registerX] & 0x80;
			chip->V[registerX] <<= 1;
			break;
		default:
			break;
	}
}

static void run_opcode_0XXX(chip8* chip, uint16_t opcode) {
	switch (opcode & 0x0FFF)
	{
		case 0x00E0:
			printf("Display clear\n");
			break;
		case 0x00EE:
			chip->PC = pop_stack(chip);
			return;
		default:
			break;
	}
	chip->PC += INSTRUCTION_SIZE;
}

static void run_opcode(chip8* chip) {

	uint16_t opcode = chip->memory[chip->PC] << 8 | chip->memory[chip->PC+1];

	switch (opcode & 0xF000)
	{
		case 0x0000:
			run_opcode_0XXX(chip, opcode);
			return; // This function handles incrementing the instruction pointer
		case 0x1000:
			chip->PC = opcode & 0x0FFF;
			return; // We dont want to incremnet the instruction pointer any more
		case 0x2000:
			push_stack(chip);
			chip->PC = chip->memory[opcode & 0x0FFF];
			return; // We dont want to incremnet the instruction pointer any more
		case 0x3000:
			if(chip->V[(opcode & 0x0F00) >> 0x8] == (opcode & 0x00FF))
				chip->PC += INSTRUCTION_SIZE;
			break;
		case 0x4000:
			if (chip->V[(opcode & 0x0F00) >> 0x8] != (opcode & 0x00FF))
				chip->PC += INSTRUCTION_SIZE;
			break;
		case 0x5000:
			if (chip->V[(opcode & 0x0F00) >> 0x8] == chip->V[(opcode & 0x00F0) >> 4])
				chip->PC += INSTRUCTION_SIZE;
			break;
		case 0x8000:
			register_operations(chip, opcode);
			break;
		case 0x9000:
			if (chip->V[(opcode & 0x0F00) >> 0x8] != chip->V[(opcode & 0x00F0) >> 0x4])
				chip->PC += INSTRUCTION_SIZE;
			break;
		case 0xA000:
			chip->I = opcode & 0x0FFF;
			break;
		case 0xB000:
			chip->PC = chip->V[0] + (opcode & 0x0FFF);
			break;
		default:
			break;

	}
	chip->PC += INSTRUCTION_SIZE;
}

static void load_program_to_memory(chip8* c8, File_out file) {
	copy_to(file.buffer, &c8->memory, 0, file.size);
}

void run(chip8* c8, FILE* file_to_code) {
	File_out program = read_all_binary(file_to_code);
	load_program_to_memory(c8, program);
}