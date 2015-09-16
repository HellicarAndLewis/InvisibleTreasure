//
//  IgniteScene.h
//  show
//
//  Created by Chris Mullany on 02/09/2015.
//
//

#pragma once
#include "ofMain.h"
#include "SceneBase.h"

class IgniteScene : public SceneBase {
public:
    IgniteScene();
    
	void setup();
	void update();
	void draw();
    void play(int i);
    void stop();
	
	void keyPressed(int key);
	void keyReleased(int key);
	void mouseMoved(int x, int y );
	void mouseDragged(int x, int y, int button);
	void mousePressed(int x, int y, int button);
	void mouseReleased(int x, int y, int button);
	void windowResized(int w, int h);
	void dragEvent(ofDragInfo dragInfo);
	void gotMessage(ofMessage msg);
    
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
    
    bool getWindowActive();
    
};