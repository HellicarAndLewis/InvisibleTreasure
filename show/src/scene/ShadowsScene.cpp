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
    video.update();
    SceneBase::update();
    if (mode == AppModel::SLAVE) {
        led->update();
    }
}

void ShadowsScene::draw() {
    if (mode == AppModel::SLAVE) {
        // display 1 is LED
        led->draw();
        // diplay 2 is wall projector
        video.draw(ofGetWidth()/2, 0, ofGetWidth()/2, ofGetHeight());
    }
    SceneBase::draw();
}

//////////////////////////////////////////////////////////////////////////////////
// public
//////////////////////////////////////////////////////////////////////////////////
void ShadowsScene::play(int i){
    // Slave
    if (mode == AppModel::SLAVE) {
        if (i==1) {
            // LED: Welcome
            led->show("Welcome");
            // TODO: replace video with fixed-name image
            if (!video.isPlaying()) {
                video.loadMovie("videos/15peopletest_sm2.mov");
                video.setLoopState(OF_LOOP_NORMAL);
                video.play();
            }
        }
        else if (i==2) {
            // LED: Going Dark and 10 second countdown
            led->show("Going Dark", 10);
        }
    }
    // general/common/base
    SceneBase::play(i);
}

void ShadowsScene::stop(){
    video.stop();
    video.close();
    SceneBase::stop();
}

void ShadowsScene::setupGui() {
    guiName = "Shadows";
    panel.setup(guiName, "settings/shadows.xml");
    // add parameters
    panel.loadFromFile("settings/shadows.xml");
}

//////////////////////////////////////////////////////////////////////////////////
// protected
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
// private
//////////////////////////////////////////////////////////////////////////////////
void ShadowsScene::onModeChange(AppModel::Mode& mode) {
    play(subsceneI);
}
//////////////////////////////////////////////////////////////////////////////////
// custom event handlers
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
// oF event handlers
//////////////////////////////////////////////////////////////////////////////////
