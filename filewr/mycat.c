#include <stdio.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#define BUFFER_SIZE 1000
int main(int argc,char * arg[]){
	char  buf[BUFFER_SIZE];
	int in_fd,n_chars;
	in_fd = open(arg[1],O_RDONLY);
	if (in_fd == -1){
		printf("cat:can't open '%s' : No such file or directory\n",arg[1]);	
	} else {
		while ((n_chars = read(in_fd,buf,BUFFER_SIZE))>0){
			
		}	
		close(in_fd);
		printf("%s",buf);
	}
	return 0;
}
