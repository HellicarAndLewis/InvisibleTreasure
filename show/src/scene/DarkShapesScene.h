//
//  DarkShapesScene.h
//  show
//
//  Created by Chris Mullany on 02/09/2015.
//
//

#pragma once
#include "ofMain.h"
#include "SceneBase.h"

class DarkShapesScene : public SceneBase {
public:
    DarkShapesScene();
    
	void setup();
	void update();
	void draw();
    
    void play();
    void stop();
    void setupGui();
    void drawGui();
    
    // gui
    ofParameter<int> radiusMin;
    ofParameter<int> radiusMax;
    ofParameter<int> noiseScale;
    ofParameter<float> threshold;
    ofParameter<bool> debugDraw;

protected:  
private:
    vector<float> levels;
    vector<float> previousLevels;
    bool audioMirror;
    float averageVolume;
    
};