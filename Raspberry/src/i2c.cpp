#include<i2c.hpp>


char *loggerFile = (char *) "./log.txt";

int addr = 0x10;
int file_i2c = -1;

char * trim(char * message) {
	message[strlen(message)-1] = 0;
	return message;
}

void i2c_saveError(char *message) {
	FILE *output = fopen(loggerFile, "a");
	time_t currentTime = time(NULL);
	printf("%s\n",message);
	fprintf(output,"%s %s\n", trim(ctime(&currentTime)), message);
	fclose(output);

	#ifdef DEBUG
		printf("\n%s\n", message);
	#endif
}
/*

void setup() {
	// try for ten seconds
	if ((file_i2c = open(filename, O_RDWR)) < 0) {
		i2c_saveError((char *)"Error opening i2c");
		exit(-1);
	} else if (ioctl(file_i2c, I2C_SLAVE, addr) < 0) {
		i2c_saveError((char *)"Can't find Arduino on i2c");
		exit(-1);
	} 
	usleep(100000);
		
}*/




/*




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


void i2c_delivery(unsigned char* data, unsigned char size){
	int i=0;
	bool correct=false;
	while(!correct&&(i)<1){
		unsigned char parity=0;
		for(int a=0; a<size; a++){
			i2c_write_comand(a+2, data[a]);
			parity+=data[a];
		}
		i2c_write_comand(0, parity);
		i2c_write_comand(1, size);
		/*printf("\n%d %d| ", parity, size);
		for(int a=0;a<size;a++){
			printf("%d ", data[a]);
		}
		correct=true;
		i++;
	} 
	if(i!=1){
		printf("Send after %d errors\n", i);
	}
	printf("\n");
	i2c_write_status(toStart);
	while(i2c_read_status()!=idle);
}*/
