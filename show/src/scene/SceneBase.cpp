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
}

void SceneBase::setup() {
}

void SceneBase::update() {
    progress = tween.update();
    if (tween.isCompleted()) {
        if (state == INTRO) setState(INTERACTIVE);
        if (state == OUTRO) setState(INACTIVE);
    }
}

void SceneBase::draw() {
    
    if (isDebugMode) {
        //ofSetColor(10, 10, 200);
        //ofRect(0, 0, ofGetWidth(), ofGetHeight());
        ofSetColor(255);
        string s = name  + " (" + modeLabel + ")";
        ofRectangle rect = font->getStringBoundingBox(s, 0, 0);
        font->drawString(s, (ofGetWidth()/2) - (rect.width/2), ofGetHeight()/2);
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
}

void SceneBase::stop(){
}

void SceneBase::setState(State state) {
    this->state = state;
    ofNotifyEvent(stateChangeEvent, state, this);
    if (state == INTRO || state == OUTRO) {
        progress = 0;
        unsigned delay = 0;
        unsigned duration = 1000;
        tween.setParameters(easinglinear,ofxTween::easeOut,0,1,duration,delay);
        tween.start();
    }
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
