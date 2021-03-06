#include "ofApp.h"
int brightness = 10;
//--------------------------------------------------------------
void ofApp::setup(){
	ofSetFrameRate(60);
	ofSetLogLevel(OF_LOG_VERBOSE);
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
#ifdef TARGET_LINUX_ARM
	apa102.setup(image.getHeight());
	ofLogVerbose() << "apa102.setup(image.getHeight());";
#endif
	
	
	length = 4+(image.getHeight()*4)+4;
	bufs.resize(image.getWidth());
	for(int i = 0 ; i < image.getWidth() ; i++){
		bufs[i] = (u_int8_t*)malloc(length);
		bufs[i][0] = 0x00;
		bufs[i][1] = 0x00;
		bufs[i][2] = 0x00;
		bufs[i][3] = 0x00;

		bufs[i][length-4] = 0xFF;
		bufs[i][length-3] = 0xFF;
		bufs[i][length-2] = 0xFF;
		bufs[i][length-1] = 0xFF;
	}

	for(int x = 0 ; x < frames.size() ; x++){
		for(int y = 0 ; y < frames[x].size() ; y++){
		// apa102.setFrameData(y,frames[x][y]);
			int index = (y*4)+4;
			bufs[x][index] = 0b11100000 | (0b00011111 & frames[x][y].a);
			bufs[x][index+1] = frames[x][y].b;
			bufs[x][index+2] = frames[x][y].g;
			bufs[x][index+3] = frames[x][y].r;
		}
	}
	
	startThread();
}
void ofApp::exit(){
	stopThread();
	for(int y = 0 ; y < image.getHeight() ; y++){

		int index = (y*4)+4;
		bufs[0][index] = 0b11100000 | (0b00011111 & 0);
		bufs[0][index+1] = 0;
		bufs[0][index+2] = 0;
		bufs[0][index+3] = 0;
	}
#ifdef TARGET_LINUX_ARM

	wiringPiSPIDataRW(0, bufs[0], length);
	ofLogVerbose() << "wiringPiSPIDataRW(0, buf, length);";
#endif
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
				
				// for(int y = 0 ; y < frames[x].size() ; y++){
				// 	// apa102.setFrameData(y,frames[x][y]);
				// 	int index = (y*4)+4;
				// 	buf[index] = 0b11100000 | (0b00011111 & frames[x][y].a);
				// 	buf[index+1] = frames[x][y].b;
				// 	buf[index+2] = frames[x][y].g;
				// 	buf[index+3] = frames[x][y].r;
				// }
#ifdef TARGET_LINUX_ARM
				wiringPiSPIDataRW(0, bufs[x], length);
#endif
				
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