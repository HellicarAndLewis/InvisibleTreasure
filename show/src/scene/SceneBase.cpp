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
    if (isSlave()) led->update();
    Sequencable::update();
}

void SceneBase::draw() {
    
    if (isSlave()) {
        led->draw();
        // slave projection
        beginSlaveProjectionDraw();
        ofClear(0);
        drawSlaveProjection();
        endSlaveProjectionDraw();
    }
    
    if (isMaster()) {
        beginMasterProjectionDraw();
        drawMasterProjection();
        endMasterProjectionDraw();
    }
    
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
bool SceneBase::isMaster(){
    return (mode == AppModel::MASTER);
}
bool SceneBase::isSlave(){
    return (mode == AppModel::SLAVE);
}
bool SceneBase::isWindow(){
    return (mode == AppModel::WINDOW);
}

// Master screen
bool SceneBase::beginMasterScreenDraw(){
    if (isMaster()) {
        displays->masterScreen.begin();
        return true;
    }
    else return false;
}
bool SceneBase::endMasterScreenDraw(){
    if (isMaster()) {
        displays->masterScreen.end();
        return true;
    }
    else return false;
}

// Master projector
bool SceneBase::beginMasterProjectionDraw(){
    if (isMaster()) {
        displays->masterProjection.begin();
        return true;
    }
    else return false;
}
bool SceneBase::endMasterProjectionDraw(){
    if (isMaster()) {
        displays->masterProjection.end();
        return true;
    }
    else return false;
}


bool SceneBase::beginSlaveScreenDraw(){
    if (isSlave()) {
        displays->slaveScreen.begin();
        return true;
    }
    else return false;
}
bool SceneBase::endSlaveScreenDraw(){
    if (isSlave()) {
        displays->slaveScreen.end();
        return true;
    }
    else return false;
}
bool SceneBase::beginSlaveProjectionDraw(){
    if (isSlave()) {
        displays->slaveProjection.begin();
        return true;
    }
    else return false;
}
bool SceneBase::endSlaveProjectionDraw(){
    if (isSlave()) {
        displays->slaveProjection.end();
        return true;
    }
    else return false;
}

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
