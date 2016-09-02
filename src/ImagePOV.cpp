#include "ImagePOV.h"



extern "C"{
	using namespace std;
	void ImagePOV::setup(ofImage &image){

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

		length = ((image.getHeight())*4);
		buf.resize(image.getWidth());
		toBuf = ( unsigned char*)malloc(length);

		clockStartFrame = ( unsigned char*)malloc(4);
		memset(clockStartFrame, 0, 4);
		endFrameLen = round((image.getHeight() + 15) / 16.0f);
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


		blackFrameLen = (image.getHeight()+4)*4;
		blackFrame = ( unsigned char*)malloc(blackFrameLen);
		for(int y = 0 ; y < image.getHeight()+4 ; y++){

			int index = (y*4);
			blackFrame[index] = 0b11100000 | (0b00011111 & 0);
			blackFrame[index+1] = 0;
			blackFrame[index+2] = 0;
			blackFrame[index+3] = 0;
		}
		ofLogVerbose() << "ImagePOV ready";
	}

	void ImagePOV::update(){
		
		
		
		for(int x = 0 ; x < frames.size() ; x++){
			memcpy ( toBuf, buf[x], length );
			apa102.send(clockStartFrame, 4);
			apa102.send(toBuf,length);
			apa102.send(clockEndFrame, endFrameLen);
		}

	}
	void ImagePOV::threadedFunction(){
		while(isThreadRunning())
		{
            // Attempt to lock the mutex.  If blocking is turned on,
			if(lock())
			{
				update();

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

	void ImagePOV::close(){
		stopThread();
		while(!lock()){
			sleep(1000);
		}


		apa102.send(clockStartFrame, 4);
		apa102.send(blackFrame, blackFrameLen);
		apa102.send(clockEndFrame, endFrameLen);
		apa102.closeSPI();
		// int spi_device = wiringPiSPIGetFd(0);
		// std::close(spi_device);
	}

	void ImagePOV::start(){
		startThread();
	}
	void ImagePOV::stop(){
		stopThread();
		while(!lock()){
			sleep(1000);
		}
		apa102.send(clockStartFrame, 4);
		apa102.send(blackFrame,  blackFrameLen);
		apa102.send(clockEndFrame, endFrameLen);
	}
}
