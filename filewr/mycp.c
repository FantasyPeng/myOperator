#include <stdio.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#define BUFFER_SIZE 4096
#define COPY_MODE 0644
int main(int argc,char * arg[]){

	char buf[BUFFER_SIZE];
	int in_fd,out_fd,n_chars;
	in_fd = open(arg[1],O_RDONLY);
	out_fd = creat(arg[2],COPY_MODE);
	while ((n_chars = read(in_fd,buf,BUFFER_SIZE))>0){
		write(out_fd,buf,n_chars);
	}
	close(in_fd);
	close(out_fd);
	return 0;
}

