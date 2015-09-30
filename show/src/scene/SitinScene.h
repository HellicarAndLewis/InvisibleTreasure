//
//  SitinScene.h
//  show
//
//  Created by Chris Mullany on 05/09/2015.
//
//

#pragma once
#include "ofMain.h"
#include "SceneBase.h"
#include "ImageElement.h"

class SitinScene : public SceneBase {
public:
    
    SitinScene();
    
    void setup();
    void update();
    void draw();
    void drawSlaveProjection();
    void play(int i);
    void stop();
    void setupGui();
    
protected:
private:
    ImageElement imageElement;
    
    // gui
    ofParameterGroup titleGroup;
    ofParameter<string> title;
    ofParameter<string> theEnd;
    // timers
    ofParameterGroup timerGroup;
    ofParameter<int> timerTheEnd;
    ofParameter<int> timerFadeUp;
    
    // LX cues
    ofParameter<int> lxCueIntro;
    ofParameter<int> lxCueTheEnd;
    ofParameter<int> lxCueOutro;
    ofParameterGroup lxCueGroup;
    
    // Sound cues
    ofParameter<int> soundCueIntro;
    ofParameter<int> soundCueTheEnd;
    ofParameter<int> soundCueOutro;
    ofParameterGroup soundCueGroup;
};