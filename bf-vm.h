#ifndef BF_VM_H_
#define BF_VM_H_

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#define DEFAULT_CODESIZE	30000
#define DEFAULT_MEMSIZE		30000

typedef struct {
	uint8_t pc;
	uint8_t p;
	size_t  codesize;
	size_t  memsize;
	uint8_t *codes;
	uint8_t *memory;
}BF_VM;

BF_VM* vm_init(size_t codesize, size_t memsize);
void vm_pushcode(BF_VM *vm, uint8_t code);
//uint8_t vm_now_codeaddr();
uint8_t vm_getpushmax();
void vm_run(BF_VM *vm, uint8_t max_pc);

#endif
