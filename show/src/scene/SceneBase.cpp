//
//  SceneBase.cpp
//  show
//
//  Created by Chris Mullany on 02/09/2015.
//
//

#include "SceneBase.h"

SceneBase::SceneBase() {
    state = INACTIVE;
    isDebugMode = true;
    subsceneStart = 0;
    subsceneEnd = 0;
}

void SceneBase::setup() {
    ofAddListener(this->appModel->modeChangeEvent, this, &SceneBase::onModeChange);
}

void SceneBase::update() {
    Sequencable::update();
}

void SceneBase::draw() {
    
    if (isDebugMode) {
        //ofSetColor(10, 10, 200);
        //ofRect(0, 0, ofGetWidth(), ofGetHeight());
        ofSetColor(150);
        string s = name + " scene " + ofToString(subsceneI) + " (" + modeLabel + ")";
        ofRectangle rect = font->getStringBoundingBox(s, 0, 0);
        font->drawString(s, (ofGetWidth()/2) - (rect.width/2), ofGetHeight()*.6);
    }
    
    // Generic intro/outro fade to black
    if (state == INTRO) {
        ofSetColor(0,0,0, (1-progress)*255);
        ofRect(0, 0, ofGetWidth(), ofGetHeight());
    }
    else if (state == OUTRO) {
        ofSetColor(0,0,0, progress*255);
        ofRect(0, 0, ofGetWidth(), ofGetHeight());
    }
    ofSetColor(255);
}

//////////////////////////////////////////////////////////////////////////////////
// public
//////////////////////////////////////////////////////////////////////////////////
void SceneBase::play(){
    Sequencable::play();
}

void SceneBase::play(int i){
    // playing a sub scene
    // close existing one if it exists
    // open new open after
    subsceneI = i;
    setState(INTERACTIVE);
    //Sequencable::play();
}

void SceneBase::stop(){
    Sequencable::stop();
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

void SceneBase::keyPressed (int key) {}

void SceneBase::keyReleased (int key) {}

void SceneBase::mouseMoved(int x, int y) {}

void SceneBase::mouseDragged(int x, int y, int button) {}

void SceneBase::mousePressed(int x, int y, int button) {}

void SceneBase::mouseReleased(int x, int y, int button) {}

void SceneBase::windowResized(int w, int h) {}

void SceneBase::dragEvent(ofDragInfo dragInfo) {}

void SceneBase::gotMessage(ofMessage msg) {}
