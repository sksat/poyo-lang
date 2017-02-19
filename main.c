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
						}
						if(token==1) code = 8;
						break;
					case 3:
						if(token==1) code = 2;
						if(token==2) code = 6;
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


