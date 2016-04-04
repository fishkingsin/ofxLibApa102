#include "ofApp.h"
int brightness = 10;
//--------------------------------------------------------------
void ofApp::setup(){
	ofSetFrameRate(60);
	image.load("earth.png");
	image.setImageType(OF_IMAGE_COLOR);
	frames.resize(image.getWidth());
	for(int x = 0 ; x < image.getWidth() ; x++){
		frames[x].resize(image.getHeight());
		for(int y = 0 ; y < image.getHeight() ; y++){
			ofColor color  = image.getColor(x,y);
			frames[x][y].set(color.r, color.g, color.b, brightness);
		}
	}
	apa102.setup(image.getHeight());
	
	
	length = 4+(image.getHeight()*4)+4;
	buf = (u_int8_t*)malloc(length);
	buf[0] = 0x00;
	buf[1] = 0x00;
	buf[2] = 0x00;
	buf[3] = 0x00;

	buf[length-4] = 0xFF;
	buf[length-3] = 0xFF;
	buf[length-2] = 0xFF;
	buf[length-1] = 0xFF;
	
	startThread();
}
void ofApp::exit(){
	stopThread();
	for(int y = 0 ; y < image.getHeight() ; y++){
	
		int index = (y*4)+4;
		buf[index] = 0b11100000 | (0b00011111 & 0);
		buf[index+1] = 0;
		buf[index+2] = 0;
		buf[index+3] = 0;
	}

	wiringPiSPIDataRW(0, buf, length);
}
void ofApp::threadedFunction(){
	while(isThreadRunning())
	{
            // Attempt to lock the mutex.  If blocking is turned on,
		if(lock())
		{

                // Unlock the mutex.  This is only
                // called if lock() returned true above.
			//scan each line x
			

			for(int x = 0 ; x < frames.size() ; x++){
				
				//scan each pixels of the line
				
				for(int y = 0 ; y < frames[x].size() ; y++){
					// apa102.setFrameData(y,frames[x][y]);
					int index = (y*4)+4;
					buf[index] = 0b11100000 | (0b00011111 & frames[x][y].a);
					buf[index+1] = frames[x][y].b;
					buf[index+2] = frames[x][y].g;
					buf[index+3] = frames[x][y].r;
				}
				
				wiringPiSPIDataRW(0, buf, length);
				
			}

			unlock();

                // Sleep for 1 second.
                // sleep(1000);

		}
		else
		{
                // If we reach this else statement, it means that we could not
                // lock our mutex, and so we do not need to call unlock().
                // Calling unlock without locking will lead to problems.
			ofLogWarning("threadedFunction()") << "Unable to lock mutex.";
		}
	}

}
//--------------------------------------------------------------
void ofApp::update(){

}

//--------------------------------------------------------------
void ofApp::draw(){

}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}