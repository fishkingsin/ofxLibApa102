#include "ofMain.h"

// #include "ofxWiringPi.h"
#include "ofxAPA102.h"
extern "C"{
	
	class ImagePOV : public ofThread{
	public:
		void setup(ofImage &image);
		void update();
		void threadedFunction();
		void close();
		void start();
		void stop();
	private:
		ofxAPA102 apa102;
		u_int8_t GAMMA[256];
		vector<vector<ofColor>>frames;
		int length;
		vector<unsigned char *> buf;
		unsigned char * toBuf;
		unsigned char * clockStartFrame;
		unsigned char * clockEndFrame;
		int endFrameLen;
		unsigned char* blackFrame;	
		int blackFrameLen;	
		int brightness = 10;
		int step = 0;
	};
}
