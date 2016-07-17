#include "ofApp.h"
int brightness = 10;
int step = 0;
//--------------------------------------------------------------
void ofApp::setup(){
	ofSetLogLevel(OF_LOG_VERBOSE);
	ofSetFrameRate(60);
	//spi
	//spi
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
	
	for ( int i = 0 ; i < 256 ; i++){
		GAMMA[i] = int(pow(float(i) / 255.0, 2.7) * 255.0  *0.2+ 0.5) ;
	}
	apa102.setup(image.getHeight());
	ofLogVerbose() << "apa102.setup(image.getHeight());";
	ofLogVerbose() << " image.getHeight() : " << image.getHeight() ;
	ofLogVerbose() << " image.getWidth() : " << image.getWidth();
	
	length = ((image.getHeight()+4)*4);
	buf.resize(image.getWidth());
	toBuf = ( unsigned char*)malloc(length);

	clockStartFrame = ( unsigned char*)malloc(4);
	memset(clockStartFrame, 0, 4);
	endFrameLen = round((image.getHeight() + 16) / 16.0f);
	ofLogVerbose() << "endFrameLen : " << endFrameLen;
	clockEndFrame = ( unsigned char*)malloc(endFrameLen);
	memset(clockEndFrame, 0xFF, endFrameLen);

	for(int i = 0 ; i < image.getWidth() ; i++){
		buf[i] = ( unsigned char*)malloc(length);
		for(int y = 0 ; y < image.getHeight() ; y++){
			int index = (y*4);
			buf[i][index] = 0b11100000 | (0b00011111 & frames[i][y].a);
			buf[i][index+1] = GAMMA[frames[i][y].b];
			buf[i][index+2] = GAMMA[frames[i][y].g];
			buf[i][index+3] = GAMMA[frames[i][y].r];
		}
	}
	
	startThread();

	blackFrame = ( unsigned char*)malloc((image.getHeight()+4)*4);
	for(int y = 0 ; y < image.getHeight()+4 ; y++){

		int index = (y*4);
		blackFrame[index] = 0b11100000 | (0b00011111 & 0);
		blackFrame[index+1] = 0;
		blackFrame[index+2] = 0;
		blackFrame[index+3] = 0;
	}
}
void ofApp::exit(){
	ofLogVerbose() << "exit";
	stopThread();
	while(!lock()){
		sleep(1000);
	}
	
	
	wiringPiSPIDataRW(0, clockStartFrame, 4);
	wiringPiSPIDataRW(0, blackFrame, (image.getHeight()+4)*4);
	wiringPiSPIDataRW(0, clockEndFrame, endFrameLen);
	
	int spi_device = wiringPiSPIGetFd(0);
	close(spi_device);
}
void ofApp::threadedFunction(){
	while(isThreadRunning())
	{
            // Attempt to lock the mutex.  If blocking is turned on,
		if(lock())
		{
			for(int x = 0 ; x < frames.size() ; x++){
				memcpy ( toBuf, buf[x], length );
				wiringPiSPIDataRW(0, clockStartFrame, 4);
				wiringPiSPIDataRW(0, toBuf, length);
				wiringPiSPIDataRW(0, clockEndFrame, endFrameLen);
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