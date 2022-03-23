#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<unistd.h>

#define MAX_SIZE 1024
int8_t write_buf[MAX_SIZE];
int8_t read_buf[MAX_SIZE];
int main()
{
	int fd;
	char option;
	int number = 0, value = 0;
	fd = open("/proc/my_proc_dir/my_proc",O_RDWR);
	if(fd <0)
	{
		printf("can not open proc file \n");
		return 0;
	}
	while(1)
	{
		printf("****Please Enter the Option******\n"); 
		printf("        1. Write               \n");
		printf("        2. Read                 \n");		
		printf("        3. Exit                 \n");
		printf("*********************************\n");
		scanf(" %c", &option);
		printf("Your Option = %c\n", option);
		
		switch(option)
		{
			case '1':
				printf("Enter the string to write into proc:"); 
				scanf("  %[^\t\n]s", write_buf);
				printf("Data Writing ...");
				write(fd, write_buf, strlen(write_buf)+1);
				printf("Done!\n");
				break;
			case '2':
			   	printf("Data Reading ...");
			   	read(fd, read_buf, MAX_SIZE);
			   	printf("Done!\n\n");
			   	printf("Data = %s\n\n", read_buf);
			   	break;
			
		  	 case '3':
				   close(fd);
				   exit(1);
				   break;
		   	default:
				   printf("Enter Valid option = %c\n",option);
				   break;
		}
	
	}
	close(fd);

}