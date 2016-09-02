#include "ofxAPA102.h"

extern "C"{
	ofxAPA102::~ofxAPA102(){
		closeSPI();
	}
	void ofxAPA102::setup(int numLEDs){
		frames.resize(numLEDs);
		device = "/dev/spidev0.0";
		connected = false;
		spi_device = 0;
		connect();
		for (int i = 0 ; i < 256; i++)
		{
			GAMMA[i] = 0x80 | int(pow(float(i) / 255.0, 2.5) * 127.0 + 0.5);
		}
	}
	
	void ofxAPA102::closeSPI()
	{
	#ifdef TARGET_LINUX_ARM
		close(spi_device);
	#endif
	}
	void ofxAPA102::off(){
		// APA102_Frame* frame = APA102_CreateFrame(0,0,0,0);
		// APA102_Begin();
		// for(int i = 0 ; i < frames.size() ; i++){
		// 	APA102_WriteLED(frame);
		// }
		// APA102_End();
		uint8_t led_frame[4];
		led_frame[0] = 0b11100000 | (0b00011111 & 0);
		led_frame[1] = 0;
		led_frame[2] = 0;
		led_frame[3] = 0;

		begin();
		for(int i = 0 ; i < frames.size() ; i++){
		
			send(led_frame,4);
		}
		end();
	}
	void ofxAPA102::setFrameData(int i, ofColor color){
		if(i < frames.size()){
			frames[i].set(color);
		}
		else{
			ofLogError("ofxAPA102") << "index exist numLEDs";
		}
	}
	void ofxAPA102::update(){
		// APA102_Frame* frame = APA102_CreateFrame(0,0,0,0);
		// APA102_Begin();
		begin();
		for(int i = 0 ; i < frames.size() ; i++){
			uint8_t led_frame[4];
			// if(frames[i].a > 31) {
		 //    	frames[i].a = 31;
		 //  	}

		  led_frame[0] = 0b11100000 | (0b00011111 & frames[i].a);
		  led_frame[1] = frames[i].b;
		  led_frame[2] = frames[i].g;
		  led_frame[3] = frames[i].r;
		// 	frame->r = frames[i].r;
		// 	frame->g = frames[i].g;
		// 	frame->b = frames[i].b;
		// 	frame->brightness = frames[i].a;
		// 	if(frame->brightness > 31) {
		// 		frame->brightness = 31;
		// 	}
		// 	APA102_WriteLED(frame);
			send(led_frame,4);
		}
		// APA102_End();
		end();
	}

	void ofxAPA102::begin() {
		uint8_t buf[4];
		int i;

		for(i = 0; i < 4; i++) {
			buf[i] = 0x00;

		}
		send(buf,4);
	}
	void ofxAPA102::end() {
		uint8_t buf[4];
		int i;

		for(i = 0; i < 4; i++) {
			buf[i] = 0xFF;

		}
		send(buf,4);
	}


	bool ofxAPA102::connect()
	{
#ifdef TARGET_LINUX_ARM
		spi_device = open(device.c_str(),O_WRONLY);
		if(spi_device<0) {
			fprintf(stderr, "Can't open device.\n");
			connected = false;
		}else connected = true;
		int ret=spi_init(spi_device);
		if(ret==-1) {
			fprintf(stderr,"error=%d, %s\n", errno, strerror(errno));
			connected = false;
		}else connected = true;
		return connected;
#else
		return false;
#endif

	}
	void ofxAPA102::send(unsigned char *data, int len){
#ifdef TARGET_LINUX_ARM
		if (connected)
			write(spi_device, (unsigned char*)data, len);
#endif
	}
//--------------------------------------------------------------
	void ofxAPA102::send(const std::vector<unsigned char>& data)
	{
#ifdef TARGET_LINUX_ARM
		if (connected)
			write(spi_device, (unsigned char*)data.data(), data.size());
#endif
	}

	int ofxAPA102::spi_init(int filedes) {
#ifdef TARGET_LINUX_ARM
		int ret;
		const uint8_t mode = SPI_MODE_0;
		const uint8_t bits = 8;
		const uint32_t speed = 16000000;

		ret = ioctl(filedes,SPI_IOC_WR_MODE, &mode);
		if(ret==-1) {
			return -1;
		}

		ret = ioctl(filedes,SPI_IOC_WR_BITS_PER_WORD, &bits);
		if(ret==-1) {
			return -1;
		}

		ret = ioctl(filedes,SPI_IOC_WR_MAX_SPEED_HZ,&speed);
		if(ret==-1) {
			return -1;
		}

		return 0;
#endif
	}
}