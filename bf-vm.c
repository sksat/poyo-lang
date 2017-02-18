#include "bf-vm.h"

void vm_init(BF_VM *vm, size_t codesize, size_t memsize){
	vm = (BF_VM*)malloc(sizeof(BF_VM));
	vm->pc = 0x00;
	vm->p  = 0x00;
	vm->codes  = (uint8_t*)malloc(sizeof(uint8_t) * codesize);
	vm->memory = (uint8_t*)malloc(sizeof(uint8_t) * memsize);
}

uint8_t pushmax = 0;

uint8_t vm_getpushmax(){
	return pushmax;
}

void vm_pushcode(BF_VM *vm, uint8_t *code, size_t size){
	memcpy(&vm->codes[pushmax], code, size);
	pushmax+=size;
}

void vm_run(BF_VM *vm, uint8_t max_pc){
	while(vm->pc <= max_pc){
		uint8_t *p = vm->p;
		switch(vm->codes[vm->pc]){
		case 0:
			exit(0);
		case 1:
			vm->p++;
			break;
		case 2:
			vm->p--;
			break;
		case 3:
			*p++;
			break;
		case 4:
			*p--;
			break;
		case 5:
		case 6:
			break;
		case 7:
			*p = getchar();
			break;
		case 8:
			putchar(*p);
			break;
		default:
			break;
		}
		
		vm->pc++;
	}
}
