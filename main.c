#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "bf-vm.h"

/*
1 ぽよよ〜ん	Ook.
2 ぽよっ	Ook!
3 ぽよよっ	Ook?
4 ぽや
*/

#define DEFAULT_STACKSIZE	10

typedef struct {
	size_t  size;
	uint8_t *base;
	uint8_t sp;
}STACK;

STACK* init_stack(size_t size){
	STACK *s = (STACK*)malloc(sizeof(STACK));
	s->base  = (uint8_t*)malloc(sizeof(uint8_t) * size);
	s->sp	 = 0;
	return s;
}

int push_stack(STACK *s, uint8_t data){
	if(s == NULL) return -1;
	if(s->sp > s->size) return -1;
	*(s->base + s->sp) = data;
	s->sp++;
}

uint8_t pop_stack(STACK *s){
	uint8_t ret;
	ret = *(s->base + s->sp - 1);
	return ret;
}

FILE *fp;
BF_VM *vm;
STACK *addr_stack;


void parse(char *buf){
	int i;
	char po[] = "ぽ";
	printf("\nparse> buf=%s\n", buf-3);
	
//	printf("%d\n", strlen("よよっ"));
	
	int len = strlen(buf);
	static int old_token = 0;
	
	for(i=0;i<=len;i++){
//		printf("%d:%s\n", i, buf+i);
		if(buf[i]==po[0] && len-i > 3){
	//		printf("po[0]を検出：%s\n", buf+i);
			if(buf[i+1]==po[1] && buf[i+2]==po[2]){
				i+=3;
				int token = 0;
				uint8_t code = 0;
				
				printf("ぽを検出:%s\n", buf+i-3);
				
				if(strncmp(&buf[i], "よよ〜ん", 12) == 0){
					token = 1;
					i+=(12-1);
				}else if(strncmp(&buf[i], "よっ", 6) == 0){
					token = 2;
					i+=(6-1);
				}else if(strncmp(&buf[i], "よよっ", 9) == 0){
					token = 3;
					i+=(9-1);
				}else if(strncmp(&buf[i+3], "や", 3) == 0){
					code = 0;
					token = 4;
					i+=(3-1);
					old_token = 0;
					continue;
				}
				
				printf("token=%d\n", token);
				
				if(old_token != 0 && token != 0){
					switch(old_token){
					case 1:
						if(token==3) code = 1;
						if(token==1) code = 3;
						if(token==2) code = 7;
						break;
					case 2:
						if(token==2) code = 4;
						if(token==3){
							code = 5;
							// push addr
							uint8_t addr;
							addr = vm_getpushmax() + 1;
							push_stack(addr_stack, addr);
						}
						if(token==1) code = 8;
						break;
					case 3:
						if(token==1) code = 2;
						if(token==2){
							code = 6;
							vm_pushcode(vm, 6);
							// pop addr
							uint8_t addr;
							addr = pop_stack(addr_stack);
							vm_pushcode(vm, addr);
							old_token = 0;
							continue;
						}
						break;
					}
					
					printf("code=%d\n", code);
					vm_pushcode(vm, code);
					old_token = 0;
				}
				old_token = token;
			}
		}
	}
}

int main(int argc, char **argv){

	vm = vm_init(DEFAULT_CODESIZE, DEFAULT_MEMSIZE);
	addr_stack = init_stack(DEFAULT_STACKSIZE);

	if(argc == 1){
		fp = stdin;
	}else{
		fp = fopen(argv[1], "r");
		if(fp == NULL){
			printf("can't open.\n");
			return -1;
		}
	}
	
	printf("> ");
	
	char buf[256];
	int p=0;
	memset(buf, '\0', 256);
	
	for(;;){
		char c = getc(fp);
		buf[p] = c;
		p++;
		if(p==256){
			printf("\nerror> too long.\n");
			goto next_line;
		}
		
		if(c == '\n'){
			buf[p-1] = '\0';
			parse(buf);
			vm_run(vm, vm_getpushmax());
next_line:
			memset(buf, '\0', 256);
			p = 0;
			if(fp == stdin) printf("> ");
		}
		if(c == EOF) return 0;
	}
	
	return 0;
}


