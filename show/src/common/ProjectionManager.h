//
//  ProjectionManager.h
//  show
//
//  Created by Chris Mullany on 29/09/2015.
//
//

#pragma once
#include "ofMain.h"
#include "ofxProjectorBlend.h"

class ProjectionManager {
public:
    ProjectionManager();
    
	void setup(int w=1024, int h=768);
	void update();
	void draw(int x, int y, float scale);
    
    void beginTop();
    void endTop();
    void beginBottom();
    void endBottom();
    void setupGui();
    
    int width;
    int height;
    
    ofParameter<float> gamma;
    ofParameter<float> luminance;
    ofParameter<float> power;
    ofParameter<bool> showBlend;
    ofParameterGroup group;
    
    ofxProjectorBlend blender[3];

protected:
private:
    
};