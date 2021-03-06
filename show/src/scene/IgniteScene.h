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
#define CUE_COUNT 5
#define WINDOW_COUNT 4

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
    ofParameter<string> title1;
    ofParameter<string> title2;
    OscClient::CueWithListParams inCues[CUE_COUNT];
    OscClient::CueWithListParams outCues[CUE_COUNT];
    OscClient::CueWithListParams intro;
    OscClient::CueWithListParams FourToFive;
    OscClient::CueWithListParams FiveToSix;
    OscClient::CueWithListParams SixToSeven;
    OscClient::CueWithListParams SevenToEight;
    OscClient::CueParams nextCue;
    ofParameter<int> timeIntro;
    ofParameter<int> minMicTime;
    
    ofParameter<float> targetVolume;
    ofParameter<int> targetFrames;
    
    ofParameter<int> radiusMin;
    ofParameter<int> radiusMax;
    ofParameter<int> noiseScale;
    ofParameter<float> threshold;
    ofParameter<bool> debugDraw;

protected:  
private:
    
    Countdown minTime;
    
    vector<float> levels;
    vector<float> previousLevels;
    bool audioMirror;
    float averageVolume;
    int targetHitCount;
    
    // TODO: tidy this into array of structs
    float windowVolumes[WINDOW_COUNT];
    bool windowTriggers[WINDOW_COUNT];
    
    bool getWindowActive();
    void onWindowVolume(OscClient::VolumeEventArgs& args);
    void onWindowVolumeTrigger(OscClient::VolumeEventArgs& args);
    
};