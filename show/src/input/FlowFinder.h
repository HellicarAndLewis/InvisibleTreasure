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

class FlowFinder : public GuiableBase {
public:
    FlowFinder();
    
    void setup();
    void update();
    void draw();
    
    void setupGui();
    
    void setActive(bool);
    
    ofImage image;
    ofImage smallImage;
    
    float imageScale;
    
    // tracking
    ofxCv::FlowFarneback flow;
    bool active;
    
    ofParameter<int> numIterations;
    ofParameter<int> numLevels;
    ofParameter<int> polyN;
    ofParameter<float> polySigma;
    ofParameter<float> pyramidScale;
    ofParameter<int> windowSize;
    ofParameter<bool> useGaussian;
    
    
protected:
private:
    
};