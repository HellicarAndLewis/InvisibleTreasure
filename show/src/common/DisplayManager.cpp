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
    testPattern.loadImage("images/bbc.png");
    font.loadFont("fonts/Andale Mono.ttf", 120);
}

void DisplayManager::update() {
    projectionManager.update();
}

void DisplayManager::draw() {
}


void DisplayManager::setupGui() {
    guiName = "Displays";
    panel.setup(guiName, "settings/displays.xml");
    // add parameters
    scaleToWindow.addListener(this, &DisplayManager::onScaleToWindow);
    panel.add(activeDisplay.set("display mode", 0, 0, 2));
    panel.add(scaleToWindow.set("scale to window", false));
    panel.add(drawOutput.set("master projector output", true));
    panel.add(drawTestPattern.set("master test pattern", false));
    panel.add(isCalibrating.set("master calibrating", false));
    panel.add(identifyProjectors.set("master identify projectors", false));
    
    // sizes
    displaySizes.setName("Display Sizes");
    displaySizes.add(slaveScreen.sizeIn.set("Slave screen", ofVec2f(DISPLAY_W,DISPLAY_H), ofVec2f(0,0), ofVec2f(DISPLAY_W*2,DISPLAY_W*2)));
    displaySizes.add(slaveProjection.sizeIn.set("Slave Projection", ofVec2f(DISPLAY_W,DISPLAY_H), ofVec2f(0,0), ofVec2f(DISPLAY_W*2,DISPLAY_W*2)));
    displaySizes.add(masterScreen.sizeIn.set("Master screen", ofVec2f(DISPLAY_W,DISPLAY_H), ofVec2f(0,0), ofVec2f(DISPLAY_W*2,DISPLAY_W*2)));
    displaySizes.add(masterProjection.sizeIn.set("Master projection", ofVec2f(DISPLAY_W*2,DISPLAY_H*2), ofVec2f(0,0), ofVec2f(DISPLAY_W*4,DISPLAY_H*4)));
    panel.add(displaySizes);
    
    // child panels
    // edge blending
    projectionManager.setupGui();
    panel.add(projectionManager.guiEnabled.set(projectionManager.guiName, false));
    projectionManager.panel.setPosition(270*2, 10);
    
    
    panel.loadFromFile("settings/displays.xml");
    refreshFbos();
    // setup projection manager
    // give it the w/h of each projector
    // this is half the width and half the height of the overall 2x2 projection canvas
    projectionManager.setup(masterProjection.sizeIn.get().x/2, masterProjection.sizeIn.get().y/2);
}

void DisplayManager::drawGui() {
    if (guiEnabled) {
        GuiableBase::drawGui();
        projectionManager.drawGui();
    }
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
    if (projectionManager.width != masterProjection.in.getWidth()*2) {
        allocateProjectorsFbo();
    }
    if (masterScreen.sizeChanged()) masterScreen.refreshFbos();
    if (masterProjection.sizeChanged()) masterProjection.refreshFbos();
    
    // Setup some dimensions for drawing screen, projection or both
    // take into account scaling to fit the window
    // and drawing projection input vs output
    float screenW = masterScreen.in.getWidth();
    float projectionW = (drawOutput) ? projectionManager.width : masterProjection.in.getWidth();
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
    if (scaleToWindow.get() && totalW > ofGetWidth()) {
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
            
            // slice master projection horozontally into the outputs
            // we're turning our single 2048x1536 projection input FBO:
            //   1 2
            //   3 4
            //
            // Into two strips that will be blending together:
            //   top:    [1 2]
            //   bottom: [3 4]
            //
            // So the output will be a long strip like so:
            //   1 2 3 4
            
            // TOP strip
            projectionManager.beginTop();
            {
                // Top Left
                projectionManager.projection[0].rectIn.set(0, 0, inW/2, inH/2);
                // output to the first position of the top blender FBO
                projectionManager.projection[0].rectOut.set(0, 0, inW/2, inH/2);
                masterProjection.in.getTextureReference().bind();
                projectionManager.projection[0].draw();
                masterProjection.in.getTextureReference().unbind();
                if (isCalibrating) projectionManager.projection[0].drawCalibration(ofColor(255,0,0));
                if (identifyProjectors) font.drawString("1", inW/4, inH/4);
                
                // Top Right
                projectionManager.projection[1].rectIn.set(inW/2, 0, inW/2, inH/2);
                // output to the second position of the top blender FBO
                projectionManager.projection[1].rectOut.set(inW/2, 0, inW/2, inH/2);
                masterProjection.in.getTextureReference().bind();
                projectionManager.projection[1].draw();
                masterProjection.in.getTextureReference().unbind();
                if (isCalibrating) projectionManager.projection[1].drawCalibration(ofColor(0,255,0));
                if (identifyProjectors) font.drawString("2", inW/2 + inW/4, inH/4);
            }
            projectionManager.endTop();
            
            
            // BOTTOM strip
            projectionManager.beginBottom();
            {
                // Bottom Left
                // sample from bottom left of the 2x2 input
                projectionManager.projection[2].rectIn.set(0, inH/2, inW/2, inH/2);
                // output to the first position of the bottom blender FBO
                projectionManager.projection[2].rectOut.set(0, 0, inW/2, inH/2);
                masterProjection.in.getTextureReference().bind();
                projectionManager.projection[2].draw();
                masterProjection.in.getTextureReference().unbind();
                if (isCalibrating) projectionManager.projection[2].drawCalibration(ofColor(0,0,255));
                if (identifyProjectors) font.drawString("3", inW/4, inH/4);
                
                // Bottom Right
                // sample from bottom right of the 2x2 input
                projectionManager.projection[3].rectIn.set(inW/2, inH/2, inW/2, inH/2);
                // output to the second position of the bottom blender FBO
                projectionManager.projection[3].rectOut.set(inW/2, 0, inW/2, inH/2);
                masterProjection.in.getTextureReference().bind();
                projectionManager.projection[3].draw();
                masterProjection.in.getTextureReference().unbind();
                if (isCalibrating) projectionManager.projection[3].drawCalibration(ofColor(255,255,0));
                if (identifyProjectors) font.drawString("4", inW/2 + inW/4, inH/4);
                
            }
            projectionManager.endBottom();
            
            // Draw the final output from projection manager
            projectionManager.draw(projectionX, 0, scale);
            
        }
        else {
            // just draw the input FBP
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
    //projectorsOutput.allocate(masterProjection.in.getWidth()*2, masterProjection.in.getHeight()*0.5);
}
//////////////////////////////////////////////////////////////////////////////////
// custom event handlers
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
// oF event handlers
//////////////////////////////////////////////////////////////////////////////////

void DisplayManager::windowResized(int w, int h) {}
