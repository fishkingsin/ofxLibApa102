#pragma once

#include "ofMain.h"

#include "ofxLibApa102.h"
class ofApp : public ofBaseApp, ofThread{
	public:
		void setup();
		void exit();
		void update();
		void draw();
		
		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y);
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void mouseEntered(int x, int y);
		void mouseExited(int x, int y);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
		u_int8_t GAMMA[256];
		ofxAPA102 apa102;
		void threadedFunction();
		ofImage image;
		vector<vector<ofColor>>frames;
		int length;
		vector<unsigned char *> buf;
		unsigned char * toBuf;
		unsigned char * clockStartFrame;
		unsigned char * clockEndFrame;
		int endFrameLen;
		unsigned char* blackFrame;
		
};
