#include "ofApp.h"
extern "C"{
//--------------------------------------------------------------
	void ofApp::setup(){
		ofSetFrameRate(60);
		struct APA102* strip = APA102_Init(60);
		struct APA102_Animation* anim = APA102_StripesAnimation(strip, APA102_CreateFrame(31, 0xFF, 0xFF, 0xFF), 50, 4, 2, -1);
		APA102_KillAnimation(anim);
		anim = APA102_StripesAnimation(strip, APA102_CreateFrame(31, 0x00, 0x00, 0x00), 50, 4, 2, -1);
		APA102_KillAnimation(anim);

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
}