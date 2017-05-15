#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <errno.h>
#include <stdlib.h>
int mysys(const char * cmdstring) {
    pid_t pid;
    int status;
    if (cmdstring == NULL) {
	printf("null cmd\n");
        return 1;	
    }
    if ((pid = fork()) < 0) {
        status = -1;	
    }
    else if(pid == 0){
        execl("/bin/sh","sh","-c",cmdstring,NULL);
	_exit(127);
    }
    else {
        while(waitpid(pid,&status,0) < 0) {
	    if (errno != EINTR)  {
	        status = -1;
		break;
	    }
	}
    }
    return status;
}
int main(){
    char *str = NULL;
    printf("-----------------------------\n");
    mysys("echo HELLO WORLD");
    mysys(str);
    printf("-----------------------------\n");
    system("ls /");	
    printf("------------------------------\n");
    return 0;
}
