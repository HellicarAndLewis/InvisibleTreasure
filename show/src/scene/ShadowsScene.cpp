//
//  ShadowsScene.cpp
//  show
//
//  Created by Chris Mullany on 02/09/2015.
//
//

#include "ShadowsScene.h"

ShadowsScene::ShadowsScene() {
    name = "Shadows";
}

void ShadowsScene::setup() {
    subsceneStart = 1;
    subsceneEnd = 2;
    SceneBase::setup();
}

void ShadowsScene::update() {
    SceneBase::update();
    if (isSlave()) {
        imageElement.update();
    }
}

void ShadowsScene::draw() {
    SceneBase::draw();
}

void ShadowsScene::drawSlaveProjection() {
    imageElement.draw();
}

//////////////////////////////////////////////////////////////////////////////////
// public
//////////////////////////////////////////////////////////////////////////////////
void ShadowsScene::play(int i){
    
    // Subscene 1: Welcome
    if (i==1) {
        if (isSlave()) {
            // LED: Welcome
            led->hide();
            led->queue(LedDisplay::Params(welcomeTitle.get(), 1.5, 4.5, 1, true));
            led->playQueue();
            
            imageElement.setup("images/static.jpg");
            imageElement.setDisplay(&displays->slaveProjection);
            imageElement.show();
        }
        if (isMaster()) {
            osc->sendLightSoundCue(cue1);
        }
    }
    
    // Subscene 2: going dark, ready for Ignite Scene
    else if (i==2) {
        int time = countdownDuration;
        if (isSlave()) {
            // LED: Going Dark and 10 second countdown
            // hide image
            // Cues
            led->hide();
            led->queue(LedDisplay::Params(goingDarkTitle, 0, 2, 0, false));
            led->queue(LedDisplay::Params(goingDarkTitle, 0, 10, 0, false, 10));
            led->playQueue();
            //led->show(goingDarkTitle, time);
            imageElement.hide(time);
        }
        if (isMaster()) {
            countdown->start(time);
            osc->sendLightSoundCue(cue2);
        }
    }
    
    // general/common/base
    SceneBase::play(i);
}

void ShadowsScene::stop(){
    SceneBase::stop();
}

void ShadowsScene::setupGui() {
    guiName = "Shadows";
    panel.setup(guiName, "settings/shadows.xml");
    // titles, times, cues
    panel.add(welcomeTitle.set("title 1", "Welcome"));
    panel.add(goingDarkTitle.set("title 2", "Going Dark"));
    panel.add(countdownDuration.set("countdown", 12, 0, 20));
    cue1.lightCue.set(0.5);
    cue2.lightCue.set(1);
    panel.add(cue1.setup("Cue 1"));
    panel.add(cue2.setup("Cue 2"));
    panel.loadFromFile("settings/shadows.xml");
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
