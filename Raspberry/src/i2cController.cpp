#include<i2c.hpp>

char *filename = (char *)"/dev/i2c-1";

void i2c_controller::setup(){
	if ((file_i2c = open(filename, O_RDWR)) < 0) {
		i2c_saveError((char *)"Error opening i2c");
		exit(-1);
	}
	usleep(1000);
}

