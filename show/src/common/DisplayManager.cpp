//
//  DisplayManager.cpp
//  show
//
//  Created by Chris Mullany on 17/09/2015.
//
//

#include "DisplayManager.h"
#define DISPLAY_W 1024
#define DISPLAY_H 768


DisplayManager::DisplayManager() {
}

void DisplayManager::setup() {
    testPattern.loadImage("images/testpattern.png");
    slaveScreen.params.setName("Slave screen");
    slaveScreen.params.add(slaveScreen.pos.set("pos", ofVec2f(0,0), ofVec2f(0,0), ofVec2f(DISPLAY_W,DISPLAY_H)));
    slaveScreen.params.add(slaveScreen.sizeIn.set("size in", ofVec2f(DISPLAY_W,DISPLAY_H), ofVec2f(0,0), ofVec2f(DISPLAY_W,DISPLAY_H)));
    slaveScreen.params.add(slaveScreen.sizeOut.set("size out", ofVec2f(DISPLAY_W,DISPLAY_H), ofVec2f(0,0), ofVec2f(DISPLAY_W,DISPLAY_H)));
    
    slaveProjection.params.setName("Slave projection");
    slaveProjection.params.add(slaveProjection.pos.set("pos", ofVec2f(DISPLAY_W,0), ofVec2f(0,0), ofVec2f(DISPLAY_W,DISPLAY_H)));
    slaveProjection.params.add(slaveProjection.sizeIn.set("size in", ofVec2f(DISPLAY_W,DISPLAY_H), ofVec2f(0,0), ofVec2f(DISPLAY_W,DISPLAY_H)));
    slaveProjection.params.add(slaveProjection.sizeOut.set("size out", ofVec2f(DISPLAY_W,DISPLAY_H), ofVec2f(0,0), ofVec2f(DISPLAY_W,DISPLAY_H)));
    
    masterScreen.params.setName("Master screen");
    masterScreen.params.add(masterScreen.pos.set("pos", ofVec2f(0,0), ofVec2f(0,0), ofVec2f(DISPLAY_W,DISPLAY_H)));
    masterScreen.params.add(masterScreen.sizeIn.set("size in", ofVec2f(DISPLAY_W,DISPLAY_H), ofVec2f(0,0), ofVec2f(DISPLAY_W,DISPLAY_H)));
    masterScreen.params.add(masterScreen.sizeOut.set("size out", ofVec2f(DISPLAY_W,DISPLAY_H), ofVec2f(0,0), ofVec2f(DISPLAY_W,DISPLAY_H)));
    
    masterProjection.params.setName("Master projection");
    masterProjection.params.add(masterProjection.pos.set("pos", ofVec2f(DISPLAY_W,0), ofVec2f(0,0), ofVec2f(DISPLAY_W*2,DISPLAY_H*2)));
    masterProjection.params.add(masterProjection.sizeIn.set("size in", ofVec2f(DISPLAY_W*2,DISPLAY_H*2), ofVec2f(0,0), ofVec2f(DISPLAY_W*4,DISPLAY_H*4)));
    masterProjection.params.add(masterProjection.sizeOut.set("size out", ofVec2f(DISPLAY_W*4,DISPLAY_H), ofVec2f(0,0), ofVec2f(DISPLAY_W*4,DISPLAY_H*4)));
}

void DisplayManager::update() {
}

void DisplayManager::draw() {
}


void DisplayManager::setupGui() {
    guiName = "Displays";
    panel.setup(guiName, "settings/displays.xml");
    // add parameters
    scaleToWindow.addListener(this, &DisplayManager::onScaleToWindow);
    panel.add(scaleToWindow.set("scale to window", true));
    panel.add(drawOutput.set("master projector output", true));
    panel.add(drawTestPattern.set("master test pattern", true));
    panel.add(slaveScreen.params);
    panel.add(slaveProjection.params);
    panel.add(masterScreen.params);
    panel.add(masterProjection.params);
    panel.loadFromFile("settings/displays.xml");
}


void DisplayManager::drawSlave() {
    float scale = 1.0f;
    if (scaleToWindow) {
        scale = ofGetWidth() / (slaveScreen.out.getWidth() + slaveProjection.out.getWidth());
    }
    slaveScreen.draw(scale);
    slaveProjection.draw(scale);
}

void DisplayManager::drawMaster() {
    float w = masterProjection.in.getWidth();
    float h = masterProjection.in.getHeight();
    float scale = 1.0f;
    if (scaleToWindow) {
        if (drawOutput) scale = ofGetWidth() / (masterScreen.out.getWidth() + masterProjection.out.getWidth());
        else scale = ofGetWidth() / (masterScreen.in.getWidth() + masterProjection.in.getWidth());
    }
    
    // master screen first
    masterScreen.draw(scale);
    
    // now master projection
    // this is an output for 4 projectors
    // input is a 2x2 matrix
    // output is 1x4
    if (drawTestPattern) {
        masterProjection.in.begin();
        testPattern.draw(0, 0, w, h);
        masterProjection.in.end();
    }
    // slice master projection horozontally into the output FBO
    masterProjection.out.begin();
    // drawSubsection is where to draw x, y followed by crop w, h and crop start pos x, y
    masterProjection.in.getTextureReference().drawSubsection(0, 0, w, h/2, 0, 0);
    masterProjection.in.getTextureReference().drawSubsection(w, 0, w, h/2, 0, h/2);
    masterProjection.out.end();
    
    if (drawOutput) masterProjection.drawOutput(scale);
    else masterProjection.draw(scale);
}

void DisplayManager::refreshFbos() {
    slaveScreen.refreshFbos();
    slaveProjection.refreshFbos();
    masterScreen.refreshFbos();
    masterProjection.refreshFbos();
}

//////////////////////////////////////////////////////////////////////////////////
// public
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
// protected
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
// private
//////////////////////////////////////////////////////////////////////////////////
void DisplayManager::onScaleToWindow(bool& scale) {
    refreshFbos();
}

//////////////////////////////////////////////////////////////////////////////////
// custom event handlers
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
// oF event handlers
//////////////////////////////////////////////////////////////////////////////////

void DisplayManager::windowResized(int w, int h) {}
