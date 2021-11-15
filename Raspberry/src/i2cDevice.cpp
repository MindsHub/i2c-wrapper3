#include<i2c.hpp>

void i2c_device::setup(i2c_controller inp){
	file_i2c=inp.file_i2c;
	outBuffer[3]=255;
	if (ioctl(file_i2c, I2C_SLAVE, addr) < 0) {
		i2c_saveError((char *)"Can't find Arduino on i2c");
		exit(-1);
	} 
}
bool i2c_device::isValid(){
	return (inBuffer[0]=='#')&&((unsigned char)(inBuffer[1]+'#')==inBuffer[2]);
}

unsigned char i2c_device::readData(){
	for(int a=0; a<10; a++){
		readed+=read(file_i2c, inBuffer+readed, 5);
		while(!isValid()&&readed>3){
		  memmove(inBuffer, inBuffer+1, --readed);
		}
		if(isValid()){
			
			DEBUG_LOGGER("\tRead %d\n", inBuffer[1]);
			unsigned char toReturn = inBuffer[1];
			if(readed>3){
				readed-=3;
				memmove(inBuffer, inBuffer+3, readed);
			}else{
				readed=0;
			}
			return toReturn;

		}
		usleep(10);
	}
	i2c_saveError((char *) "can't read from i2c device");
	exit(-1);
}

unsigned char i2c_device::sendData(){
	DEBUG_LOGGER("\tSend: %d %d\n", outBuffer[0], outBuffer[1]);
	outBuffer[2]=outBuffer[0]+ outBuffer[1];
	
	for(int a=0; a<10; a++){
		if(4 == write(file_i2c, outBuffer, 4)){
			usleep(100);
			return readData();
		}
		usleep(10);
	}
	i2c_saveError((char *) "can't write to i2c device");
	exit(-1);
}

bool i2c_device::writeComand(unsigned char reg, unsigned char val){
	//printf("%d %d\n",reg, val);
	if(reg<I2C_BUF_SIZE){
		outBuffer[0]=reg;
		outBuffer[1]=val;
		for(int a=0; a<10; a++){
			if(sendData()==outBuffer[1])
				return true;
		}
	}
	i2c_saveError((char *)"failed write\n");
	return false;
}

unsigned char i2c_device::readComand(unsigned char reg){
	if(reg<I2C_BUF_SIZE){
		outBuffer[0]=I2C_BUF_SIZE;
		outBuffer[1]=reg;
		return sendData();
	}
	return 0;
}

bool i2c_device::writeStatus(unsigned char val){
	outBuffer[0]=I2C_BUF_SIZE+2;
	outBuffer[1]=val;
	for(int a=0; a<10; a++){
		if(sendData()==val)
			return true;
	}
	return false;
}

unsigned char i2c_device::readStatus(){
	outBuffer[0]=I2C_BUF_SIZE+3;
	outBuffer[1]=0;
	return sendData();
}

void i2c_device::delivery(unsigned char* data, unsigned char size){

	unsigned char parity=0;
	for(int a=0; a<size; a++){
		writeComand(a+2, data[a]);
		parity+=data[a];
	}
	writeComand(0, parity);
	writeComand(1, size);
	
	writeStatus(toStart);
	while(readStatus()!=idle);
}
