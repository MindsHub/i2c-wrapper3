#include <stdio.h>
#include <string.h>
#include <stdint.h>

#include "i2c.hpp"

int count=0;

void move(i2c_device dev, int32_t pos){
	printf("\rM->%5d %d\n", pos, ++count);
	fflush(stdout);
	unsigned char buf[5];
	buf[0]='M';
	memcpy(buf+1,&pos, 4);
	dev.delivery(buf, 5);
}

int main(){
	i2c_controller contr;
	i2c_device asseX(contr, 0x10);
	//unsigned char test[3];
	//asseX.writeComand(0, 2);
	//asseX.readComand(0);
	//setup();
	/*for(int a=0;a<10000;a++){
		move((int32_t)rand()%(200*32));
		usleep(1000000);
	}*/
	move(asseX, 1000);
	//i2c_write_comand(0, 'M');
}
