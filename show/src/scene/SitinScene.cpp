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
void SitinScene::play(){
    // start/load/kick things off
    SceneBase::play();
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
