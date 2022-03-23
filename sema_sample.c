#include <stdio.h>  
#include <stdlib.h>  
#include <unistd.h>  
#include <string.h>  
  
#include <sys/types.h>  
#include <sys/stat.h>  
#include <fcntl.h>  
  
int  
main(void)  
{  
	int fd;  
	char buf[100];  
	int size;  

	fd = open("/dev/my_Sem_Char_driver", O_RDWR);  
	if(!fd) {  
			perror("open");  
			exit(-1);  
	}  
	printf("open success!\n");  

	close(fd);  
	return 0;  
}