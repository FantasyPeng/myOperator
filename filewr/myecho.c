#include <stdio.h>
int main(int argc,char ** arg){
	for(int i = 1; i < argc; i++){
		printf("%s ",arg[i]);
	}
	printf("\n"); 
	return 0;
}

