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
        if (isSlave()) led->show(tunes[tuneI].ledTitle);
        if (isMaster()) {
            countdown->start(tunes[tuneI].duration);
            osc->sendSoundCue(tunes[tuneI].soundCueNum);
        }
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
