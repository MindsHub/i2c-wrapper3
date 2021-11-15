#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

#include <fcntl.h>
#include <sys/ioctl.h>
#include <cstring>

#include <linux/i2c-dev.h>

#define I2C_PRINT_MESSAGE
#define BYTES std::string
#include <i2c.hpp>

int count=0;

void move(int32_t pos){
	//printf("\rM->%5d %d", pos, ++count);
	//fflush(stdout);
	unsigned char buf[5];
	buf[0]='M';
	memcpy(buf+1,&pos, 4);
	i2c_delivery(buf, 5);
}

int main(){
	setup();
	for(int a=0;a<1000;a++){
		move((int32_t)rand()%20000-10000);
		usleep(100000);
	}
}
