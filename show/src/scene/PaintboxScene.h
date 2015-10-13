//
//  PaintboxScene.h
//  show
//
//  Created by Chris Mullany on 05/09/2015.
//
//

#pragma once
#include "ofMain.h"
#include "SceneBase.h"
#include "ImageElement.h"

// TODO: blob tracking, draw lines with people
//
class PaintboxScene : public SceneBase {
public:
    
    enum Mode {
        INACTIVE, LINES, ERASER, FADE
    } mode;
    
    PaintboxScene();
    
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
    void clearCanvas();
    ofColor getColour(int blob);
    bool getIsActive();
    
    ofFbo canvas;
    map<int, ofColor> colours;
    ofImage brushImage;
    float totalBlobArea;
    ImageElement imageElement;
    
    ofParameter<float> minAreaEraser;
    ofParameter<float> minAreaBlack;
    
    // gui
    ofParameterGroup titleGroup;
    ofParameter<string> title;
    ofParameter<string> sleeping;
    ofParameter<string> back;
    ofParameter<string> bonusTime;
    ofParameter<string> bonusGame;
    ofParameter<string> goingDark;
    ofParameter<string> pleaseEnjoy;
    
    // timers
    ofParameterGroup timerGroup;
    ofParameter<int> timerIntro;
    ofParameter<int> timerLines;
    ofParameter<int> timerErase;
    ofParameter<int> timerBack;
    ofParameter<int> timerBonus;
    ofParameter<int> timerOutro;
    
    // LX cues
    ofParameter<int> lxCueIntro;
    ofParameter<int> lxCueOutro;
    ofParameterGroup lxCueGroup;
    
    // Sound cues
    ofParameter<int> soundCueSoundScape;
    ofParameter<int> soundCueSoundScape2;
    ofParameter<int> soundCueOutro;
    ofParameterGroup soundCueGroup;
    
};