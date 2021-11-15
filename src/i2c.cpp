#include <cstring>
#include <cerrno>
#include <string>

#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/i2c-dev.h>

#include<i2c.hpp>

char *loggerFile = (char *) "./log.txt";
char *filename = (char *)"/dev/i2c-1";
int addr = 0x10;
int file_i2c = -1;

char * trim(char * message) {
	message[strlen(message)-1] = 0;
	return message;
}

void saveError(char *message) {
	FILE *output = fopen(loggerFile, "a");
	time_t currentTime = time(NULL);
	printf("%s\n",message);
	fprintf(output,"%s %s\n", trim(ctime(&currentTime)), message);
	fclose(output);

#ifdef DEBUG
	printf("\n%s\n", message);
#endif
}

void i2c_controller::setup(){
	if ((file_i2c = open(filename, O_RDWR)) < 0) {
		saveError((char *)"Error opening i2c");
		exit(-1);
	}
}

void setup() {
	// try for ten seconds
	if ((file_i2c = open(filename, O_RDWR)) < 0) {
		saveError((char *)"Error opening i2c");
		exit(-1);
	} else if (ioctl(file_i2c, I2C_SLAVE, addr) < 0) {
		saveError((char *)"Can't find Arduino on i2c");
		exit(-1);
	} 
	usleep(100000);
		
}

unsigned char i2c_read(){
	unsigned char out;
	for(int a=0; a<10; a++){
		if(1 == read(file_i2c, &out, 1)){
			return out;
		}
		usleep(10);
	}
	saveError((char *) "can't read from i2c device");
	exit(-1);
}

unsigned char i2c_send(unsigned char *buf){
	buf[3]=255;
	for(int a=0; a<10; a++){
		if(4 == write(file_i2c, buf, 4)){
			usleep(10);
			return i2c_read();
		}
		usleep(10);
	}
	saveError((char *) "can't write to i2c device");
	exit(-1);
}

bool i2c_write_comand(unsigned char reg, unsigned char val){
	printf("%d %d\n",reg, val);
	if(reg<I2C_BUF_SIZE){
		unsigned char buf[4];
		buf[0]=reg;
		buf[1]=val;
		buf[2]=buf[0]+buf[1];
		for(int a=0; a<10; a++){
			if(i2c_send(buf)==buf[1])
				return true;
		}
	}
	return false;
}

unsigned char i2c_read_comand(unsigned char reg){
	if(reg<I2C_BUF_SIZE){
		unsigned char buf[4];
		buf[0]=I2C_BUF_SIZE;
		buf[1]=reg;
		buf[2]=buf[0]+buf[1];
		return i2c_send(buf);
	}
	return 0;
}

unsigned char i2c_read_output(unsigned char reg){
	if(reg<I2C_BUF_SIZE){
		unsigned char buf[4];
		buf[0]=I2C_BUF_SIZE+1;
		buf[1]=reg;
		buf[2]=buf[0]+buf[1];
		return i2c_send(buf);
	}
	return 0;
}

bool i2c_write_status(unsigned char val){
	unsigned char buf[4];
	buf[0]=I2C_BUF_SIZE+2;
	buf[1]=val;
	buf[2]=buf[0]+buf[1];
	return i2c_send(buf)==val;
}

unsigned char i2c_read_status(){
	unsigned char buf[4];
	buf[0]=I2C_BUF_SIZE+3;
	buf[1]=0;
	buf[2]=buf[0]+buf[1];
	return i2c_send(buf);
}

void i2c_delivery(unsigned char* data, unsigned char size){
	unsigned char parity=0;
	for(int a=0; a<size; a++){
		i2c_write_comand(a+2, data[a]);
		parity+=data[a];
	}
	i2c_write_comand(0, parity);
	i2c_write_comand(1, size);
	i2c_write_status(toStart);
	while(i2c_read_status()!=idle);
	if(i2c_read_output(1)!=0){
		exit(-1);
	}
}
