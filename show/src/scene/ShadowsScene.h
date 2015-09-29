//
//  ShadowsScene.h
//  show
//
//  Created by Chris Mullany on 02/09/2015.
//
//

#pragma once
#include "ofMain.h"
#include "SceneBase.h"
#include "ImageElement.h"

class ShadowsScene : public SceneBase {
public:
    ShadowsScene();
	void setup();
	void update();
	void draw();
    void drawSlaveProjection();
    void play(int i);
    void stop();
    void setupGui();
    
    ofParameter<string> welcomeTitle;
    ofParameter<string> goingDarkTitle;
    OscClient::CueWithFloatParams cue1;
    OscClient::CueWithFloatParams cue2;

protected:
private:
    ImageElement imageElement;
};