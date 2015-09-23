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
    slaveScreen.params.add(slaveScreen.sizeIn.set("size", ofVec2f(DISPLAY_W,DISPLAY_H), ofVec2f(0,0), ofVec2f(DISPLAY_W*2,DISPLAY_W*2)));
    slaveProjection.params.setName("Slave projection");
    slaveProjection.params.add(slaveProjection.sizeIn.set("size", ofVec2f(DISPLAY_W,DISPLAY_H), ofVec2f(0,0), ofVec2f(DISPLAY_W*2,DISPLAY_W*2)));
    masterScreen.params.setName("Master screen");
    masterScreen.params.add(masterScreen.sizeIn.set("size", ofVec2f(DISPLAY_W,DISPLAY_H), ofVec2f(0,0), ofVec2f(DISPLAY_W*2,DISPLAY_W*2)));
    masterProjection.params.setName("Master projection");
    masterProjection.params.add(masterProjection.sizeIn.set("size", ofVec2f(DISPLAY_W*2,DISPLAY_H*2), ofVec2f(0,0), ofVec2f(DISPLAY_W*4,DISPLAY_H*4)));
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
    panel.add(activeDisplay.set("display mode", 0, 0, 2));
    panel.add(scaleToWindow.set("scale to window", true));
    panel.add(drawOutput.set("master projector output", true));
    panel.add(drawTestPattern.set("master test pattern", true));
    panel.add(slaveScreen.params);
    panel.add(slaveProjection.params);
    panel.add(masterScreen.params);
    panel.add(masterProjection.params);
    panel.loadFromFile("settings/displays.xml");
    refreshFbos();
}


void DisplayManager::drawSlave() {
    if (slaveScreen.sizeChanged()) slaveScreen.refreshFbos();
    if (slaveProjection.sizeChanged()) slaveProjection.refreshFbos();
    // Setup some dimensions for drawing screen, projection or both
    // take into account scaling to fit the window
    // and drawing projection input vs output
    float screenW = slaveScreen.in.getWidth();
    float projectionW = slaveProjection.in.getWidth();
    float scale = 1.0f;
    float totalW = 0.0f;
    if (activeDisplay == 0) {
        // Draw both screen and projections
        totalW = projectionW + screenW;
    } else if (activeDisplay == 1) {
        // Draw screen
        totalW = screenW;
    } else if (activeDisplay == 2) {
        // Draw projection
        totalW = projectionW;
    }
    if (scaleToWindow && totalW > ofGetWidth()) {
        scale = ofGetWidth() / totalW;
    }
    
    //
    // Now we can draw stuff
    float projectionX = 0.0f;
    // Screen
    if (activeDisplay == 0 || activeDisplay == 1) {
        projectionX = slaveScreen.in.getWidth() * scale;
        slaveScreen.draw(0, 0, scale);
    }
    // projection
    if (activeDisplay == 0 || activeDisplay == 2) {
        slaveProjection.draw(projectionX, 0, scale);
    }
}

void DisplayManager::drawMaster() {
    // make sure the projectors FBO is at the right size
    // it will contain 2 horzizontal slices from masterProjection.in side by side
    // so it needs to be double the width and half the height
    if (projectorsOutput.getWidth() != masterProjection.in.getWidth()*2) {
        allocateProjectorsFbo();
    }
    if (masterScreen.sizeChanged()) masterScreen.refreshFbos();
    if (masterProjection.sizeChanged()) masterProjection.refreshFbos();
    
    // Setup some dimensions for drawing screen, projection or both
    // take into account scaling to fit the window
    // and drawing projection input vs output
    float screenW = masterScreen.in.getWidth();
    float projectionW = (drawOutput) ? projectorsOutput.getWidth() : masterProjection.in.getWidth();
    float scale = 1.0f;
    float totalW = 0.0f;
    // set the total target width
    if (activeDisplay == 0) {
        // Draw both screen and projections
        totalW = projectionW + screenW;
    } else if (activeDisplay == 1) {
        // Draw screen
        totalW = screenW;
    } else if (activeDisplay == 2) {
        // Draw projection
        totalW = projectionW;
    }
    // work out scaling
    if (scaleToWindow && totalW > ofGetWidth()) {
        scale = ofGetWidth() / totalW;
    }
    
    //
    // Now we can draw stuff
    float projectionX = 0.0f;
    // Screen
    if (activeDisplay == 0 || activeDisplay == 1) {
        // master screen first
        masterScreen.draw(0, 0, scale);
        projectionX = screenW * scale;
    }
    if (activeDisplay == 0 || activeDisplay == 2) {
        // now master projection
        // this is an output for 4 projectors
        // input is a 2x2 matrix
        // output is 1x4
        float inW = masterProjection.in.getWidth();
        float inH = masterProjection.in.getHeight();
        if (drawTestPattern) {
            masterProjection.in.begin();
            testPattern.draw(0, 0, inW, inH);
            masterProjection.in.end();
        }
        if (drawOutput) {
            // slice master projection horozontally into the output FBO
            projectorsOutput.begin();
            ofClear(0);
            // drawSubsection is where to draw x, y followed by crop w, h and crop start pos x, y
            masterProjection.in.getTextureReference().drawSubsection(0, 0, inW, inH/2, 0, 0);
            masterProjection.in.getTextureReference().drawSubsection(inW, 0, inW, inH/2, 0, inH/2);
            projectorsOutput.end();
            projectorsOutput.draw(projectionX, 0, projectorsOutput.getWidth()*scale, projectorsOutput.getHeight()*scale);
        }
        else {
            masterProjection.draw(projectionX, 0, scale);
        }
    }
}

void DisplayManager::refreshFbos() {
    slaveScreen.refreshFbos();
    slaveProjection.refreshFbos();
    masterScreen.refreshFbos();
    masterProjection.refreshFbos();
    allocateProjectorsFbo();
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

void DisplayManager::allocateProjectorsFbo(){
    projectorsOutput.allocate(masterProjection.in.getWidth()*2, masterProjection.in.getHeight()*0.5);
}
//////////////////////////////////////////////////////////////////////////////////
// custom event handlers
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
// oF event handlers
//////////////////////////////////////////////////////////////////////////////////

void DisplayManager::windowResized(int w, int h) {}
