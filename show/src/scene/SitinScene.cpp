//
//  SitinScene.cpp
//  show
//
//  Created by Chris Mullany on 05/09/2015.
//
//

#include "SitinScene.h"
using namespace ofxCv;
using namespace cv;

SitinScene::SitinScene() {
    name = "Sitin";
}

void SitinScene::setup() {
    // subscenes
    subsceneStart = 83;
    subsceneEnd = 86;
    SceneBase::setup();
}

void SitinScene::update() {
    if (isSlave()) {
        imageElement.update();
    }
    SceneBase::update();
}

void SitinScene::draw() {
    SceneBase::draw();
}

void SitinScene::drawSlaveProjection() {
    imageElement.draw();
}

//////////////////////////////////////////////////////////////////////////////////
// public
//////////////////////////////////////////////////////////////////////////////////
void SitinScene::play(int i){
    switch (i) {
        case 83:
            // title, manual cue
            if (isSlave()) led->show(title);
            if (isMaster()) {
                countdown->stop();
                osc->sendLightingCue(lxCueIntro);
                osc->sendSoundCue(soundCueIntro);
            }
            break;
        case 84:
            // the end, 10 seconds
            if (isSlave()) led->show(theEnd);
            if (isMaster()) {
                countdown->start(timerTheEnd);
                osc->sendLightingCue(lxCueTheEnd);
                osc->sendSoundCue(soundCueTheEnd);
            }
            break;
        case 85:
            // final audio track, 10 seconds
            if (isSlave()) led->hide();
            if (isMaster()) {
                countdown->start(timerFadeUp);
                osc->sendLightingCue(lxCueOutro);
                osc->sendSoundCue(soundCueOutro);
            }
            break;
        case 86:
            // image, manual cue
            if (isSlave()) {
                imageElement.setup("images/static.jpg");
                imageElement.setDisplay(&displays->slaveProjection);
                imageElement.show();
                led->hide();
            }
            if (isMaster()) {
                countdown->stop();
            }
            break;
        default:
            break;
    }
    SceneBase::play(i);
}

void SitinScene::stop(){
    SceneBase::stop();
}

void SitinScene::setupGui() {
    guiName = "Sitin";
    panel.setup(guiName, "settings/sitin.xml");
    titleGroup.setName("Titles");
    titleGroup.add(title.set("title", "SIN IT"));
    titleGroup.add(theEnd.set("end", "System power failure"));
    timerGroup.setName("Timers");
    timerGroup.add(timerTheEnd.set("the end", 10, 1, 60));
    timerGroup.add(timerFadeUp.set("fade up", 10, 1, 60));
    panel.add(titleGroup);
    panel.add(timerGroup);
    
    // LX cues
    lxCueGroup.setName("LX Cues");
    lxCueGroup.add(lxCueIntro.set("intro", 36, 0, 100));
    lxCueGroup.add(lxCueTheEnd.set("the end", 38, 0, 100));
    lxCueGroup.add(lxCueOutro.set("outro", 39, 0, 100));
    panel.add(lxCueGroup);
    
    // Sound cues
    soundCueGroup.setName("Sound Cues");
    soundCueGroup.add(soundCueIntro.set("intro", 0, 0, 100));
    soundCueGroup.add(soundCueTheEnd.set("the end", 0, 0, 100));
    soundCueGroup.add(soundCueOutro.set("outro", 0, 0, 100));
    panel.add(soundCueGroup);
    
    panel.loadFromFile("settings/sitin.xml");
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
