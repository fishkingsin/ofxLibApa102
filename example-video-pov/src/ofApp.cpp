#include "ofApp.h"
int brightness = 10;

//--------------------------------------------------------------
void ofApp::setup(){
	ofSetFrameRate(60);
	

	
	//Somewhat like ofFboSettings we may have a lot of options so this is the current model
	ofxOMXPlayerSettings settings;
	settings.videoPath = ofToDataPath("Timecoded_Big_bunny.mov", true);
	settings.useHDMIForAudio = true;	//default true
	settings.enableTexture = true;		//default true
	settings.enableLooping = true;		//default true
	settings.enableAudio = true;		//default true, save resources by disabling
	//settings.doFlipTexture = true;		//default false
	
	//so either pass in the settings
	omxPlayer.setup(settings);
	if(omxPlayer.getHeight()<1){
		std::exit(0);
	}
	ofLogVerbose() << "omxPlayer.getWidth() " << omxPlayer.getWidth() << "omxPlayer.getHeight() " << omxPlayer.getHeight();
	apa102.setup(omxPlayer.getHeight());
	
    fbo.allocate(omxPlayer.getWidth(),omxPlayer.getHeight(),GL_RGB);
    pixels.allocate(omxPlayer.getWidth(),omxPlayer.getHeight(),OF_IMAGE_COLOR);
	length = 4+(omxPlayer.getHeight()*4)+4;
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
    omxPlayer.close();
	stopThread();
	for(int y = 0 ; y < omxPlayer.getHeight() ; y++){
	
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
			

			for(int x = 0 ; x < fbo.getWidth() ; x++){
				
				//scan each pixels of the line
				
				for(int y = 0 ; y <fbo.getHeight() ; y++){
					// apa102.setFrameData(y,frames[x][y]);
                    ofColor c = pixels.getColor(x,y);
					int index = (y*4)+4;
					buf[index] = 0b11100000 | (0b00011111 & c.a);
					buf[index+1] = c.b;
					buf[index+2] = c.g;
					buf[index+3] = c.r;
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
    lock();
	if(!omxPlayer.isTextureEnabled())
	{
		return;
	}
	fbo.begin();
	omxPlayer.draw(0, 0, ofGetWidth(), ofGetHeight());
	fbo.end();
    fbo.readToPixels(pixels);
    unlock();
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