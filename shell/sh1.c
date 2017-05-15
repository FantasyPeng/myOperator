#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>
#define LEN 256
#define WIDTH 256
#define HEIGHT 10
char command1[LEN];
void split(char source[],char dest[HEIGHT][WIDTH]){
	char*p;
	p = strsep(&source," ");
	int i = 0;
	for (i = 0; p[i] != '\0'; i++) {
		dest[0][i] = p[i];
	}
	dest[0][i] = '\0';
	int j = 1;
	while(p) {
		p = strsep(&source," ");
		if (p) {
			for (i = 0; p[i] != '\0'; i++) {
				dest[j][i] = p[i];
			}
			dest[j][i] = '\0';
			j++;
		}
	}
}
int execute(char comm[HEIGHT][WIDTH]) {
	if (strcmp(comm[0],"echo") == 0) {
		int pid = fork();
		if (pid == 0) {
		/*	int i = 0;
			int is = 0;
			for (i = 1; comm[i][0] != '\0'; i++) {
				if (comm[i][0] == '>') {
					is = 1;
					break;
				}	
			}
			if (is == 1) {
				puts(comm[i+1]);
				FILE * fp = fopen(comm[i+1],"w+");
				int j = 0;
				for (j = 1; j < i; j++) {
					fseek(fp,0,SEEK_END);
					fwrite(comm[j],strlen(comm[j]),1,fp);
				}
				fclose(fp);
			} else {
				int j = 0;
				for (j = 1; comm[j][0] != '\0'; j++) {
					printf("%s",comm[j]);
					printf(" ");
				}
				printf("\n"); 
			//	exit(0);
			}	*/	
			execl("/bin/sh","sh","-c",command1,NULL);			
		} else {
			int status;
			wait(&status);
		}

	} else if (strcmp(comm[0],"ls") == 0) {
		int pid = fork();
		if (pid == 0) {
			if(comm[1][0] == '\0') {
				execlp("/bin/ls","ls","./",NULL,NULL,NULL);
			} else {
				execlp("/bin/ls","ls",comm[1],NULL,NULL,NULL);
			}
		
		} else {
			int status;
			wait(&status);
		}
	} else if (strcmp(comm[0],"exit") == 0) {
		return -1;

	} else if (strcmp(comm[0],"cd") == 0) {
		chdir(comm[1]);
	}else if (strcmp(comm[0],"pwd") == 0) {

		system("pwd");

	} else if (strcmp(comm[0],"cat") == 0) {
		int pid = fork();
		if (pid == 0) {
			execl("/bin/cat","cat",comm[1],NULL);
			}	
		else {
			int status;
			wait(&status);
			}
	}	
	return 1;
}
int main() {
	while(1){
		char command[LEN];
		char splitArray[HEIGHT][WIDTH] = {{'\0'}};
		printf("%s",">>");
		gets(command);
		strcpy(command1,command);
		split(command,splitArray);
		if (-1 == execute(splitArray)) {
			return 0;
		}	
	}
}
