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
    countdown->update();
    Sequencable::update();
}

void SceneBase::draw() {
    
    //
    // Common slave drawing
    //
    if (isSlave()) {
        // slave screen
        led->draw();
        // slave projection
        beginSlaveProjectionDraw();
        {
            ofClear(0, 255);
            drawSlaveProjection();
        }
        endSlaveProjectionDraw();
    }
    
    //
    // Common Master drawing
    //
    if (isMaster()) {
        // mater creen
        beginMasterScreenDraw();
        {
            ofClear(50, 255);
            drawMasterScreen();
        }
        endMasterScreenDraw();
        // master projection
        beginMasterProjectionDraw();
        {
            ofClear(0, 255);
            drawMasterProjection();
        }
        endMasterProjectionDraw();
    }
    
    
    //
    // Debug drawing
    //
    if (isDebugMode || isMaster()) {
        
        // draw common UI for master screen
        float screenH = displays->masterScreen.sizeIn.get().y;
        float x = 10;
        float y = MIN(ofGetHeight(), screenH);
        ofRectangle rect;
        string s;
        
        if (led->paramsQueue.size() > 0) {
            s = "LED queue " + ofToString(led->paramsQueue.size());
            y -= 20;
            ofDrawBitmapStringHighlight(s, x, y, ofColor(200,100,0));
        }
        if (countdown->progress > 0.001) {
            s = "Next subscene in " + ofToString(countdown->progress);
            y -= 20;
            ofDrawBitmapStringHighlight(s, x, y, ofColor(0,0,200));
        }
        s = name + " subscene " + ofToString(subsceneI);
        y -= 20;
        ofDrawBitmapStringHighlight(s, x, y, ofColor(0,0,100));
        // mode
        s = modeLabel;
        if (isWindow()) s += " " + ofToString(appModel->windowId);
        s += "  (" + ofToString(ofGetFrameRate()) + " FPS)";
        y -= 20;
        ofDrawBitmapStringHighlight(s, x, y, ofColor(0,0,100));
    }
    
    // Generic intro/outro fade to black
    if (state == INTRO) {
        ofSetColor(0,0,0, (1-progress)*255);
        ofRect(0, 0, ofGetWidth(), ofGetHeight());
    }
    else if (state == OUTRO) {
        ofSetColor(0,0,0, progress*255);
        //ofRect(0, 0, ofGetWidth(), ofGetHeight());
    }
    else if (state == INACTIVE) {
        ofSetColor(0,0,0, 255);
        //ofRect(0, 0, ofGetWidth(), ofGetHeight());
    }
    ofSetColor(255);
}

void SceneBase::drawMasterScreen() {
    vision->isEnabled = true;
    drawVision();
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
    ofAddListener(this->countdown->countdownCompleteEvent, this, &SceneBase::onCountdownComplete);
    //Sequencable::play();
}

void SceneBase::stop(){
    ofRemoveListener(this->countdown->countdownCompleteEvent, this, &SceneBase::onCountdownComplete);
    Sequencable::stop();
    led->hide();
    countdown->stop();
    vision->isEnabled = false;
}

//////////////////////////////////////////////////////////////////////////////////
// protected
//////////////////////////////////////////////////////////////////////////////////

void SceneBase::drawVision() {
    ofImage * image = &vision->outputImage;
    
    if (!image->isAllocated()) {
        ofLogError() << "in SceneBase::drawVision vision image is not allocated";
        return;
    }
    
    float targetWidth = MIN(ofGetWidth(), displays->masterScreen.sizeIn->x) * 0.5;
    float scale = targetWidth / image->width;
    ofPushStyle();
    ofPushMatrix();
    {
        ofTranslate(10, 10);
        ofPushMatrix();
        {
            ofScale(scale, scale);
            ofSetColor(255);
            image->draw(0, 0);
        }
        ofPopMatrix();
    }
    ofPopMatrix();
    ofPopStyle();
}

void SceneBase::nextSubscene(int i) {
    if (state == INTERACTIVE) {
        ofNotifyEvent(nextSubsceneEvent, i, this);
    }
}

void SceneBase::playSubscene(int i) {
    if (state == INTERACTIVE) {
        ofNotifyEvent(nextSubsceneEvent, i, this);
    }
}

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
void SceneBase::onModeChange(AppModel::Mode& mode) {
    if (state == INTERACTIVE) {
        play(subsceneI);
    }
}
void SceneBase::onCountdownComplete(int& i) {
    if (isMaster()) {
        ofLogVerbose() << "SceneBase::onCountdownComlete for " << name;
        nextSubscene();
    }
}
//////////////////////////////////////////////////////////////////////////////////
// oF event handlers
//////////////////////////////////////////////////////////////////////////////////
