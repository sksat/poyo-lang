
#include <stdio.h>
#include <stdint.h>

#define DEFAULT_MEMSIZE		30000

typedef struct {
	uint8_t ip;
	uint8_t dp;
	size_t  memsize;
	uint8_t *memory;
}BF_VM;

void vm_init(BF_VM *vm, size_t memsize);
void vm_throwcode(BF_VM *vm, uint8_t code, uint8_t addr);
void vm_run(BF_VM *vm, uint8_t max_ip);
