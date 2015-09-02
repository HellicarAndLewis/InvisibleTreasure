//
//  VideoInput.h
//  show
//
//  Created by Chris Mullany on 02/09/2015.
//
//

#pragma once
#include "ofMain.h"
#include "IVisionInput.h"

// Video file input for testing/simulating cam input
//
class VideoInput : public IVisionInput {
public:
    VideoInput();
    
	void setup();
	void update();
	void draw();
    void exit();
    ofPixelsRef getPixelsRef();
    bool isFrameNew();
    bool getIsReady();
    void start();
    void stop();

protected:  
private:
    ofVideoPlayer video;
    
};