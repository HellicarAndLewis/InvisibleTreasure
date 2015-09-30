//
//  DancingDarkScene.cpp
//  show
//
//  Created by Chris Mullany on 05/09/2015.
//
//

#include "DancingDarkScene.h"
using namespace ofxCv;
using namespace cv;

DancingDarkScene::DancingDarkScene() {
    name = "Dancing in the Dark";
}

void DancingDarkScene::setup() {
    // subscenes
    subsceneStart = 67;
    subsceneEnd = 76;
    SceneBase::setup();
}

void DancingDarkScene::update() {
    SceneBase::update();
}

void DancingDarkScene::draw() {
    SceneBase::draw();
}

//////////////////////////////////////////////////////////////////////////////////
// public
//////////////////////////////////////////////////////////////////////////////////
void DancingDarkScene::play(int i){
    int tuneI = i - 67;
    if (tuneI >= 0 && tuneI < DANCING_TUNE_COUNT) {
        Tune & tune = tunes[tuneI];
        if (isSlave()) led->show(tune.ledTitle);
        if (isMaster()) {
            if (tune.duration > 0) countdown->start(tune.duration);
            if (tune.soundCueNum > -1) osc->sendSoundCue(tune.soundCueNum);
        }
    }
    if (isMaster()) {
        if (i == 67) osc->sendLightingCue(lxCueBlackout);
        else if (i == 74) osc->sendLightingCue(lxCueLightsUp);
        else if (i == 75) osc->sendLightingCue(lxCueOutro);
    }
    SceneBase::play(i);
}

void DancingDarkScene::stop(){
    SceneBase::stop();
}

void DancingDarkScene::setupGui() {
    guiName = "Dancing Dark";
    panel.setup(guiName, "settings/dancingdark.xml");
    
    panel.add(tunes[0].set("1", "The Twist", 30, 67));
    panel.add(tunes[1].set("2", "Line Dance", 30, 68));
    panel.add(tunes[2].set("3", "Flamenco", 30, 69));
    panel.add(tunes[3].set("4", "Shake it", 30, 70));
    panel.add(tunes[4].set("5", "Salsa", 30, 71));
    panel.add(tunes[5].set("6", "Can Can", 30, 72));
    panel.add(tunes[6].set("7", "Conga", 30, 73));
    panel.add(tunes[7].set("8", "Go go go", 150, 74));
    panel.add(tunes[8].set("exit", "", 5, 75));
    panel.add(tunes[9].set("fade", "", 10, 76));
    
    
    // LX cues
    lxCueGroup.setName("LX Cues");
    lxCueGroup.add(lxCueBlackout.set("blackout", 30, 0, 100));
    lxCueGroup.add(lxCueLightsUp.set("light up", 31, 0, 100));
    lxCueGroup.add(lxCueOutro.set("outro", 33, 0, 100));
    panel.add(lxCueGroup);
    
    panel.loadFromFile("settings/dancingdark.xml");
}
//////////////////////////////////////////////////////////////////////////////////
// protected
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
// private
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
// custom event handlers
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
// oF event handlers
//////////////////////////////////////////////////////////////////////////////////
