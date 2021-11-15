#ifndef i2c_wrapper
#define i2c_wrapper

#include <cstring>
#include <cerrno>
#include <string>

#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/i2c-dev.h>

#ifdef DEBUG
	#define DEBUG_LOGGER(...) printf(__VA_ARGS__)
#else
	#define DEBUG_LOGGER(...)
#endif
#define I2C_BUF_SIZE 20


enum {idle, writing, toStart, executing};

class i2c_controller{
	public:
		int file_i2c;
		i2c_controller(){
			setup();
		}
	private:
		void setup();
};

class i2c_device{
	public:
		i2c_device(i2c_controller inp, unsigned char addr){
			this->addr=addr;
			setup(inp);
		}
		unsigned char readData();
		unsigned char sendData(unsigned char *buf);
	private:
		int file_i2c;
		unsigned char addr;
		unsigned char i2c_inBuffer[10];
		unsigned char i2c_read=0;
		bool isValid();
		void setup(i2c_controller inp);
		
};

//void setup();
void i2c_saveError(char*);
//void i2c_delivery(unsigned char *, unsigned char);

#endif
