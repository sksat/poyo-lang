#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "bf-vm.h"

FILE *fp;

void parse(char *buf){
	printf("\nparse> buf=%s\n", buf);
	if(strcmp(buf, "ぽや") == 0){
		printf("poyo> ぽやしみ〜\n");
		exit(0);
	}
}

int main(int argc, char **argv){
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
next_line:
			memset(buf, '\0', 256);
			p = 0;
			if(fp == stdin) printf("> ");
		}
		if(c == EOF) return 0;
	}
	
	return 0;
}


