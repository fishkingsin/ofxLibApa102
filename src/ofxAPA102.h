#include "ofMain.h"
extern "C"{
	#include "apa102.h"
	class ofxAPA102{
	public:
		void setup(int numLEDs);
		void off();
		void setFrameData(int i , ofColor color);
		void update();

		
	private:
		APA102* apa102;
		vector<ofColor>frames;
	};
}


