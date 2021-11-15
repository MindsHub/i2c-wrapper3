#include <stdio.h>
#include <string.h>
#include <stdint.h>

#include "i2c.hpp"

int count=0;
/*
void move(int32_t pos){
	printf("\rM->%5d %d\n", pos, ++count);
	fflush(stdout);
	unsigned char buf[5];
	buf[0]='M';
	memcpy(buf+1,&pos, 4);
	i2c_delivery(buf, 5);
}*/

int main(){
	i2c_controller contr;
	i2c_device asseX(contr, 0x10);
	unsigned char test[3];
	test[0]=0;
	test[1]=2;
	test[2]=test[1]+test[0];
	asseX.sendData(test);
	//setup();
	/*for(int a=0;a<10000;a++){
		move((int32_t)rand()%(200*32));
		usleep(1000000);
	}*/
	//move(1000);
	//i2c_write_comand(0, 'M');
}
