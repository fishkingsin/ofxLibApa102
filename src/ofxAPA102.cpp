#include "ofxAPA102.h"

extern "C"{
	void ofxAPA102::setup(int numLEDs){
		frames.resize(numLEDs);
		apa102 = APA102_Init(numLEDs);
	}
	void ofxAPA102::off(){
		APA102_Frame* frame = APA102_CreateFrame(0,0,0,0);
		APA102_Begin();
		for(int i = 0 ; i < frames.size() ; i++){
			APA102_WriteLED(frame);
		}
		APA102_End();
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
		APA102_Frame* frame = APA102_CreateFrame(0,0,0,0);
		APA102_Begin();
		for(int i = 0 ; i < frames.size() ; i++){
			frame->r = frames[i].r;
			frame->g = frames[i].g;
			frame->b = frames[i].b;
			frame->brightness = frames[i].a;
			if(frame->brightness > 31) {
				frame->brightness = 31;
			}
			APA102_WriteLED(frame);
		}
		APA102_End();
	}
}