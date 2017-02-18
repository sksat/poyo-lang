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

FILE *fp;
BF_VM *vm;

void parse(char *buf){
	int i;
	char po[] = "ぽ";
	printf("\nparse> buf=%s\n", buf);
	
	int len = strlen(buf);
	int old_token = 0;
	
	for(i=0;i<=len;i++){
		if(buf[i]==po[0] && len-i > 3){
			if(buf[i+1]==po[1] && buf[i+2]==po[2]){
				i+=3;
				int token = 0;
				uint8_t code = 0;
				if(strncmp(&buf[i+3], "よよ〜ん", 3*4) == 0){
					token = 1;
					i+=3*4;
				}else if(strncmp(&buf[i+3], "よっ", 3*2) == 0){
					token = 2;
					i+=3*2;
				}else if(strncmp(&buf[i+3], "よよっ", 3*3)){
					token = 3;
					i+=3*3;
				}else if(strncmp(&buf[i+3], "や", 3) == 0){
					code = 0;
					token = 4;
					i+=3;
					old_token = 0;
					continue;
				}
				
				if(old_token != 0 && token != 0){
					switch(old_token){
					case 1:
						if(token==3) code = 1;
						if(token==1) code = 3;
						if(token==2) code = 7;
						break;
					case 2:
						if(token==2) code = 4;
						if(token==3) code = 5;
						if(token==1) code = 8;
						break;
					case 3:
						if(token==1) code = 2;
						if(token==2) code = 6;
					}
					
					vm_pushcode(vm, &code, 1);
				}
			}
		}
	}
}

int main(int argc, char **argv){

	vm_init(vm, DEFAULT_CODESIZE, DEFAULT_MEMSIZE);

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


