#include "ofApp.h"
#include "ofxXmlSettings.h"
//--------------------------------------------------------------
void ofApp::setup(){
    ofSetLogLevel(OF_LOG_VERBOSE);
    ofxXmlSettings settings;
    if( settings.loadFile("settings.xml") ){
        ofLogVerbose() << "loaded settings.xml";
        int numPtTags = settings.getNumTags("SETTINGS:SERVER");

        if(numPtTags > 0){
            string server = settings.getValue("SETTINGS:SERVER","localhost");
            ofLogVerbose() << "loaded settings server "<<server;
            client.connect(server, 9093);
        }


    }else{
        client.connect("localhost", 9093);
        ofLogVerbose() << "client.connect(localhost, 9093)";
    }
    // connect to either the "server blob" or "server blob video" example!
    
    
    client.addListener(this);
    ofSetFrameRate(60);
    
    needToLoad = false;
    locked = false;

    for ( int i = 0 ; i < 256 ; i++){
        GAMMA[i] = int(pow(float(i) / 255.0, 2.7) * 255.0  * 0.1 + 0.5) ;
    }

    fbo.allocate(256,144,GL_RGB);
    apa102.setup(fbo.getHeight());
    

    pixels.allocate(fbo.getWidth(),fbo.getHeight(),OF_IMAGE_COLOR);
    length = 4+(fbo.getHeight()*4)+4;
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
    for(int y = 0 ; y < fbo.getHeight() ; y++){

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
                    buf[index+1] = GAMMA[c.b];
                    buf[index+2] = GAMMA[c.g];
                    buf[index+3] = GAMMA[c.r];
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
    if ( needToLoad ){
        // you can write this directly to a file!
//        ofFile test;
//        test.open("data.jpg", ofFile::WriteOnly);
//        test.writeFromBuffer(buff);
//        test.close();

        turbo.load(buff, incoming);
        needToLoad = false;
        locked = false;
    }
}

//--------------------------------------------------------------
void ofApp::draw(){
    if ( incoming.isAllocated() ){

        fbo.begin();
        incoming.draw(0,0, fbo.getWidth(), fbo.getHeight());
        fbo.end();
        if(lock()){
            fbo.readToPixels(pixels);
            unlock();
        }
    }
}

//--------------------------------------------------------------
void ofApp::onConnect( ofxLibwebsockets::Event& args ){
    ofLogVerbose()<<"on connected";
}

//--------------------------------------------------------------
void ofApp::onOpen( ofxLibwebsockets::Event& args ){
    ofLogVerbose()<<"on open";
}

//--------------------------------------------------------------
void ofApp::onClose( ofxLibwebsockets::Event& args ){
    ofLogVerbose()<<"on close";
}

//--------------------------------------------------------------
void ofApp::onIdle( ofxLibwebsockets::Event& args ){
    ofLogVerbose()<<"on idle";
}

//--------------------------------------------------------------
void ofApp::onMessage( ofxLibwebsockets::Event& args ){
    if ( locked ) return;
    // need to load this next frame!
    if ( args.isBinary ){
        buff.clear();
        buff.set(args.data.getData(), args.data.size());
        locked = true;
        needToLoad = true;
    } else {
        // got a string message
    }
}

//--------------------------------------------------------------
void ofApp::onBroadcast( ofxLibwebsockets::Event& args ){
    cout<<"got broadcast "<<args.message<<endl;    
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

    client.send("Hello");
    cout << "sending hello" <<endl;
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

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
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
