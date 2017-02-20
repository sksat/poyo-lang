#include "bf-vm.h"

uint8_t pushmax;

BF_VM* vm_init(size_t codesize, size_t memsize){
	BF_VM *vm = (BF_VM*)malloc(sizeof(BF_VM));
	vm->pc = 0x00;
	vm->p  = 0x00;
	vm->codes  = (uint8_t*)malloc(sizeof(uint8_t) * codesize);
	vm->memory = (uint8_t*)malloc(sizeof(uint8_t) * memsize);
	pushmax = 0;
	return vm;
}

uint8_t vm_getpushmax(){
	return pushmax-1;
}
/*
void vm_editcode(BF_VM *vm, uint8_t addr, uint8_t code){
	vm->codes
}
*/
void vm_pushcode(BF_VM *vm, uint8_t code){
	vm->codes[pushmax] = code;
	pushmax++;
}

void vm_run(BF_VM *vm, uint8_t max_pc){
	if(vm == NULL){
		printf("vmrun> error.\n");
		return;
	}
	if(max_pc == 0x00) return;
	while(vm->pc <= max_pc){
//		printf("pc = %x code = %d\n", vm->pc, vm->codes[vm->pc]);
		switch(vm->codes[vm->pc]){
		case 0:
			printf("exit.\n");
			exit(0);
		case 1:
			vm->p++;
			break;
		case 2:
			vm->p--;
			break;
		case 3:
			vm->memory[vm->p]++;
			break;
		case 4:
			vm->memory[vm->p]--;
			break;
		case 5:
			if(vm->memory[vm->p] == 0){
				vm->pc = vm->codes[vm->pc + 1];
				continue;
			}
		case 6:
			if(vm->memory[vm->p] != 0){
				vm->pc = vm->codes[vm->pc + 1];
				continue;
			}
		case 7:
			printf("input> ");
			vm->memory[vm->p] = getchar();
			break;
		case 8:
			putchar(vm->memory[vm->p]);
			break;
		default:
			break;
		}
		
		vm->pc++;
	}
}
