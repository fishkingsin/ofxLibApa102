#include "ofMain.h"
extern "C"{
#ifdef TARGET_LINUX_ARM
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <linux/types.h>
#include <linux/spi/spidev.h>
#endif
	class ofxAPA102{
	public:
		~ofxAPA102();
		void setup(int numLEDs);
		void closeSPI();
		void off();
		void setFrameData(int i , ofColor color);
		void update();
		//advance usage to send custom buffer
		void send(unsigned char *data, int len);
		void send(const std::vector<unsigned char>& data);

		void begin();
		void end();
		
	private:

		
		// APA102* apa102;
		vector<ofColor>frames;
		bool connect();

		
		
		
		int spi_init(int filedes);
		string device;
		int spi_device;
		bool connected;
		unsigned char GAMMA[256];
	};
}


