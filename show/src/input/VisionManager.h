//
//  VisionManager.h
//  show
//
//  Created by Chris Mullany on 02/09/2015.
//
//

#pragma once
#include "ofMain.h"
#include "ofxCv.h"
#include "IPCamInput.h"
#include "VideoInput.h"
#include "VideoGrabberInput.h"
#include "GuiableBase.h"
#include "ContourTracker.h"

// manages vision-based inputs: IP camera, video file, image file, etc
// applies distortion coorection to active input
// exposes the output image to the rest of the app
//
class VisionManager : public GuiableBase {
public:
    VisionManager();
    
	void setup();
	void update();
	void draw();
	void exit();
	void keyPressed(int key);
    
    void setupGui();
    void drawGui();
    ContourTracker* getTracker();
    
    void setToIPCamMain();
    void setToIPCamCassandra();
    
    ofParameter<bool> debugDraw;
    
protected:  
private:
    
    // inputs
    IPCamInput ipcam;
    IPCamInput ipcamCassandra;
    VideoInput video;
    VideoGrabberInput grabber;
    // input management
    vector<IVisionInput*> inputs;
    int inputIndex;
    IVisionInput* input;
    
    // tracking and calibration
    ContourTracker contourTracker;
    ofxCv::Calibration calibration;
    ofImage inputImage, outputImage;
    ofPixels previous;
    ofPixels diff;
    float diffMean;
    float lastTime;
    bool isFirstImage;
    
    // gui
    ofParameter<int> inputSelector;
    ofParameter<bool> isCalibrating;
    ofParameter<string> ipCamURLMain;
    ofParameter<string> ipCamURLCassandra;
    void onInputChange(int & i);
    
};