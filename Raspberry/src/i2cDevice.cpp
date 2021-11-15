#include<i2c.hpp>

void i2c_device::setup(i2c_controller inp){
	file_i2c=inp.file_i2c;
	if (ioctl(file_i2c, I2C_SLAVE, addr) < 0) {
		i2c_saveError((char *)"Can't find Arduino on i2c");
		exit(-1);
	} 
}
bool i2c_device::isValid(){
	return (i2c_inBuffer[0]=='#')&&((unsigned char)(i2c_inBuffer[1]+'#')==i2c_inBuffer[2]);
}

unsigned char i2c_device::readData(){
	for(int a=0; a<10; a++){
		i2c_read+=read(file_i2c, i2c_inBuffer+i2c_read, 5);
		while(!isValid()&&i2c_read>3){
		  memmove(i2c_inBuffer, i2c_inBuffer+1, --i2c_read);
		}
		if(isValid()){
			
			DEBUG_LOGGER("\tRead %d\n", i2c_inBuffer[1]);
			unsigned char toReturn = i2c_inBuffer[1];
			if(i2c_read>3){
				i2c_read-=3;
				memmove(i2c_inBuffer, i2c_inBuffer+3, i2c_read);
			}else{
				i2c_read=0;
			}
			return toReturn;

		}
		usleep(10);
	}
	i2c_saveError((char *) "can't read from i2c device");
	exit(-1);
}

unsigned char i2c_device::sendData(unsigned char *buf){
	DEBUG_LOGGER("\tSend: %d %d\n", buf[0], buf[1]);
	buf[3]=255;
	for(int a=0; a<10; a++){
		if(4 == write(file_i2c, buf, 4)){
			usleep(100);
			return readData();
		}
		usleep(10);
	}
	i2c_saveError((char *) "can't write to i2c device");
	exit(-1);
}

