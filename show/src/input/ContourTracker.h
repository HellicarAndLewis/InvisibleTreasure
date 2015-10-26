//
//  ContourTracker.h
//  show
//
//  Created by Chris Mullany on 03/09/2015.
//
//

#pragma once
#include "ofMain.h"
#include "ofxCv.h"
#include "GuiableBase.h"

class ContourTracker : public GuiableBase {
public:
    ContourTracker();
    
	void setup();
	void update();
	void draw();
    
    void resetBg();
    void setupGui();
    
    ofImage image;
    ofImage thresholded;
    
    ofParameter<float> maxRadius;
    ofParameter<int> threshold;
    ofParameter<int> perstistence;
    ofParameter<float> maxDistance;
    ofParameter<float> blurAmount;
    ofParameter<int> bgLearningTime;
    ofParameter<int> bgThreshold;
    
    ofParameter<float> minAreaNorm;
    ofParameter<float> maxAreaNorm;
    ofParameter<bool> doTrackLight;
    
    // tracking
    ofxCv::ContourFinder contourFinder;
    bool showLabels;

protected:  
private:
    
    // bg
    ofxCv::RunningBackground background;
    
    
};