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
    slaveScreen.params.setName("Slave screen");
    slaveScreen.params.add(slaveScreen.pos.set("pos", ofVec2f(0,0), ofVec2f(0,0), ofVec2f(DISPLAY_W,DISPLAY_H)));
    slaveScreen.params.add(slaveScreen.sizeIn.set("size in", ofVec2f(DISPLAY_W,DISPLAY_H), ofVec2f(0,0), ofVec2f(DISPLAY_W,DISPLAY_H)));
    slaveScreen.params.add(slaveScreen.sizeOut.set("size out", ofVec2f(DISPLAY_W,DISPLAY_H), ofVec2f(0,0), ofVec2f(DISPLAY_W,DISPLAY_H)));
    slaveScreen.params.add(slaveScreen.useWindowSize.set("use window size", true));
    
    slaveProjection.params.setName("Slave projection");
    slaveProjection.params.add(slaveProjection.pos.set("pos", ofVec2f(DISPLAY_W,0), ofVec2f(0,0), ofVec2f(DISPLAY_W,DISPLAY_H)));
    slaveProjection.params.add(slaveProjection.sizeIn.set("size in", ofVec2f(DISPLAY_W,DISPLAY_H), ofVec2f(0,0), ofVec2f(DISPLAY_W,DISPLAY_H)));
    slaveProjection.params.add(slaveProjection.sizeOut.set("size out", ofVec2f(DISPLAY_W,DISPLAY_H), ofVec2f(0,0), ofVec2f(DISPLAY_W,DISPLAY_H)));
    slaveProjection.params.add(slaveScreen.useWindowSize.set("use window size", true));
    
    masterScreen.params.setName("Master screen");
    masterScreen.params.add(masterScreen.pos.set("pos", ofVec2f(0,0), ofVec2f(0,0), ofVec2f(DISPLAY_W,DISPLAY_H)));
    masterScreen.params.add(masterScreen.sizeIn.set("size in", ofVec2f(DISPLAY_W,DISPLAY_H), ofVec2f(0,0), ofVec2f(DISPLAY_W,DISPLAY_H)));
    masterScreen.params.add(masterScreen.sizeOut.set("size out", ofVec2f(DISPLAY_W,DISPLAY_H), ofVec2f(0,0), ofVec2f(DISPLAY_W,DISPLAY_H)));
    masterScreen.params.add(slaveScreen.useWindowSize.set("use window size", true));
    
    masterProjection.params.setName("Master projection");
    masterProjection.params.add(masterProjection.pos.set("pos", ofVec2f(DISPLAY_W,0), ofVec2f(0,0), ofVec2f(DISPLAY_W*2,DISPLAY_H*2)));
    masterProjection.params.add(masterProjection.sizeIn.set("size in", ofVec2f(DISPLAY_W*4,DISPLAY_H), ofVec2f(0,0), ofVec2f(DISPLAY_W*4,DISPLAY_H*4)));
    masterProjection.params.add(masterProjection.sizeOut.set("size out", ofVec2f(DISPLAY_W*2,DISPLAY_H*2), ofVec2f(0,0), ofVec2f(DISPLAY_W*4,DISPLAY_H*4)));
    masterProjection.params.add(slaveScreen.useWindowSize.set("use window size", true));
}

void DisplayManager::update() {
}

void DisplayManager::draw() {
}


void DisplayManager::setupGui() {
    guiName = "Displays";
    panel.setup(guiName, "settings/displays.xml");
    // add parameters
    panel.add(slaveScreen.params);
    panel.add(slaveProjection.params);
    panel.add(masterScreen.params);
    panel.add(masterProjection.params);
    panel.loadFromFile("settings/displays.xml");
}


void DisplayManager::drawSlave() {
    slaveScreen.draw();
    slaveProjection.draw();
}

void DisplayManager::drawMaster() {
    masterScreen.draw();
    // TODO: slice master projection in FBO horozontally, draw slices next to each other
    float w = masterProjection.sizeIn.get().x;
    float h = masterProjection.sizeIn.get().y;
    // slice master projection in FBO horozontally
    masterProjection.out.begin();
    // subsection x, y, w, h, then x, y where you want to draw it
    masterProjection.in.getTextureReference().drawSubsection(0, 0, w, h/2, 0, 0);
    masterProjection.in.getTextureReference().drawSubsection(0, h/2, w, h/2, w, 0);
    masterProjection.out.end();
    
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

//////////////////////////////////////////////////////////////////////////////////
// custom event handlers
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
// oF event handlers
//////////////////////////////////////////////////////////////////////////////////

void DisplayManager::windowResized(int w, int h) {}
