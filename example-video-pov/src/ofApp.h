#pragma once

#include "ofMain.h"
#include "ofxLibApa102.h"
#include "ofxOMXPlayer.h"

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
		ofxAPA102 apa102;

		void threadedFunction();
		
		
		
		u_int8_t* buf;

		ofxOMXPlayer omxPlayer;
		ofFbo fbo;
    
        ofPixels pixels;
    
        int length;
		unsigned char GAMMA [256];
		
};
