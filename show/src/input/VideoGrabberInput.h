//
//  VideoGrabberInput.h
//  show
//
//  Created by Chris Mullany on 21/09/2015.
//
//

#pragma once
#include "ofMain.h"
#include "IVisionInput.h"

class VideoGrabberInput : public IVisionInput {
public:
    VideoGrabberInput();
    
	void setup();
	void update();
    void draw();
    ofPixelsRef getPixelsRef();
    bool isFrameNew();
    bool getIsReady();
    void start();
    void stop();

protected:  
private:
    ofVideoGrabber grabber;
    
};