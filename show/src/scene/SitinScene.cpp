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
                // TODO LX and sound cues
            }
            break;
        case 84:
            // the end, 10 seconds
            if (isSlave()) led->show(theEnd);
            if (isMaster()) {
                countdown->start(timerTheEnd);
                // TODO LX and sound cues
            }
            break;
        case 85:
            // final audio track, 10 seconds
            if (isSlave()) led->hide();
            if (isMaster()) {
                countdown->start(timerFadeUp);
                // TODO LX and sound cues
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
                // TODO LX and sound cues
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
    titleGroup.add(title.set("title", "Sit-In"));
    titleGroup.add(theEnd.set("end", "The End"));
    timerGroup.setName("Timers");
    timerGroup.add(timerTheEnd.set("the end", 10, 1, 60));
    timerGroup.add(timerFadeUp.set("fade up", 10, 1, 60));
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
