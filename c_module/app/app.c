#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>      /* open */
#include <unistd.h>     /* exit */
#include <sys/ioctl.h>  /* ioctl */
#include <sys/types.h>
#include <sys/stat.h>


//#include "chardev.h"

#define MAJOR_NUM 245
//#define IOCTL_SET_MSG _IOR(MAJOR_NUM, 0, unsigned int *)
//data는 4-byte stream
#define DEVICE_FILE_NAME "/dev/stopwatch"
//#define SYSCALL_NUMBER 366	//실습과제 포함시 369

int main (int argc, char *argv[]){

    int file_desc;
    unsigned long n = 0;


    if(argc != 1){
        printf("Command form: ./app\n");
        exit(-1);
    }

    file_desc = open(DEVICE_FILE_NAME, O_RDWR);

    if(file_desc < 0) {
        printf("Can't open device file: %s\n", DEVICE_FILE_NAME);
        exit(-1);
    }

    write(file_desc, &n, sizeof(unsigned long));

    close(file_desc);

    return 0;
}


