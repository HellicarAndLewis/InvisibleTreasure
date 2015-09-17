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
    if (mode == AppModel::SLAVE) {
        led->update();
        imageDisplay.update();
    }
}

void ShadowsScene::draw() {
    if (mode == AppModel::SLAVE) {
        // display 1 is LED
        led->draw();
        // diplay 2 is wall projector
        ofRectangle rect = ofRectangle(ofGetWidth()/2, 0, ofGetWidth()/2, ofGetHeight());
        imageDisplay.draw(rect);
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
            led->show(welcomeTitle);
            // TODO: replace video with fixed-name image
            imageDisplay.setup("images/static.jpg");
            imageDisplay.show();
        }
        else if (i==2) {
            // LED: Going Dark and 10 second countdown
            int time = countdownDuration;
            led->show(goingDarkTitle, time);
            imageDisplay.hide(time);
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
    // add parameters
    panel.add(welcomeTitle.set("title1", "Welcome"));
    panel.add(goingDarkTitle.set("title2", "Going Dark"));
    panel.add(countdownDuration.set("countdown", 10, 0, 20));
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
