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
    if (mode==AppModel::SLAVE) {}
    else if (mode==AppModel::WINDOW) {}
    else if (mode==AppModel::MASTER) {}
    SceneBase::update();
}

void SitinScene::draw() {
    
    if (mode==AppModel::SLAVE) {}
    else if (mode==AppModel::WINDOW) {}
    else if (mode==AppModel::MASTER) {}
    SceneBase::draw();
}


//////////////////////////////////////////////////////////////////////////////////
// public
//////////////////////////////////////////////////////////////////////////////////
void SitinScene::play(int i){
    switch (i) {
        case 83:
            // title, manual cue
            break;
        case 84:
            // the end, 10 seconds
            break;
        case 85:
            // final audio track, 10 seconds
            break;
        case 86:
            // image, manual cue
            break;
        default:
            break;
    }
    SceneBase::play(i);
}

void SitinScene::stop(){
    // stop/unload/clear things
    SceneBase::stop();
}

void SitinScene::setupGui() {
    guiName = "Sitin";
    panel.setup(guiName, "settings/sitin.xml");
    // add parameters
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
