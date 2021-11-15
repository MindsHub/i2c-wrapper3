#ifndef i2c_wrapper
#define i2c_wrapper

#define I2C_BUF_SIZE 20
enum {idle, writing, toStart, executing};

class i2c_controller{
	public:
		i2c_controller(){
			setup();
		}
	private:
		void setup();
};

void setup();
void i2c_delivery(unsigned char *, unsigned char);

#endif
