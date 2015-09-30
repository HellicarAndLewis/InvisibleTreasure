//
//  FlantsScene.h
//  show
//
//  Created by Chris Mullany on 05/09/2015.
//
//

#pragma once
#include "ofMain.h"
#include "SceneBase.h"
#include "Particles.h"
#define FLANTS_CUE_COUNT 9

// particles, blob tracking, contours, errosion
//
class FlantsScene : public SceneBase {
public:
    
    enum Mode {
        YELLOW_SMALL, YELLOW_EXPAND, YELLOW_BLUE_SHAPES, RED_SMALL, RED_EXPAND, RED_BLUE_SHAPES, EXPLODE
    } mode;
    
    FlantsScene();
    
    void setup();
    void update();
    void draw();
    void drawMasterProjection();
    void play(int i);
    void stop();
    void setupGui();
    
protected:
private:
    void setMode(Mode state);
    Particles particles;
    ofShader alphaShader;
    
    int radius;
    ofParameter<int> eatRate;
    ofParameter<int> radiusSmall;
    ofParameter<int> radiusLarge;
    
    // gui
    ofParameterGroup titleGroup;
    ofParameter<string> title;
    ofParameter<string> boom;
    ofParameter<string> gameOver;
    ofParameter<string> playAgain;
    ofParameter<string> tired;
    ofParameter<string> nextLevel;
    // timers
    ofParameterGroup timerGroup;
    ofParameter<int> timerTransition;
    ofParameter<int> timerBoom;
    ofParameter<int> timerGameOver;
    ofParameter<int> timerAgain;
    ofParameter<int> timerTired;
    ofParameter<int> timerNext;
    
    OscClient::CueParams cues[FLANTS_CUE_COUNT];
};