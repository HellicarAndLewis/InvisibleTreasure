//
//  ProjectionManager.cpp
//  show
//
//  Created by Chris Mullany on 29/09/2015.
//
//

#include "ProjectionManager.h"

ProjectionManager::ProjectionManager() {
}

void ProjectionManager::setup(int w, int h) {
    blender[0].setup(w, h, 2, 20, ofxProjectorBlend_Horizontal);
    blender[1].setup(w, h, 2, 20, ofxProjectorBlend_Horizontal);
    blender[2].setup(w*2, h, 2, 20, ofxProjectorBlend_Vertical);
    width = w*4;
    height = h;
}

void ProjectionManager::update() {
    for (int i=0; i<3; i++) {
        blender[i].gamma[0] = gamma;
        blender[i].gamma[1] = gamma;
        blender[i].luminance[0] = luminance;
        blender[i].luminance[1] = luminance;
        blender[i].blendPower[0] = power;
        blender[i].blendPower[1] = power;
        blender[i].showBlend = showBlend;
    }
}

void ProjectionManager::draw(int x, int y, float scale) {
    // draw top and bottom into the main output
    blender[2].begin();
    blender[0].draw();
    ofPushMatrix();
    ofTranslate(0, blender[0].getCanvasHeight());
    blender[1].draw();
    ofPopMatrix();
    blender[2].end();
    
    // TODO: scale
    ofPushMatrix();
    ofScale(scale, scale);
    blender[2].draw(x, y);
    ofPopMatrix();
}

//////////////////////////////////////////////////////////////////////////////////
// public
//////////////////////////////////////////////////////////////////////////////////
void ProjectionManager::beginTop() {
    blender[0].begin();
    //light gray backaground
    ofSetColor(100, 100, 100);
    ofRect(0, 0, blender[0].getCanvasWidth(), blender[0].getCanvasHeight());
    
    //thick grid lines for blending
    ofSetColor(255, 255, 255);
    ofSetLineWidth(3);
    
    //vertical line
    for(int i = 0; i <= blender[0].getCanvasWidth(); i+=40){
        ofLine(i, 0, i, blender[0].getCanvasHeight());
    }
    
    //horizontal lines
    for(int j = 0; j <= blender[0].getCanvasHeight(); j+=40){
        ofLine(0, j, blender[0].getCanvasWidth(), j);
    }
}

void ProjectionManager::endTop() {
    blender[0].end();
}

void ProjectionManager::beginBottom() {
    blender[1].begin();
    //light gray backaground
    ofSetColor(100, 100, 100);
    ofRect(0, 0, blender[1].getCanvasWidth(), blender[1].getCanvasHeight());
    
    //thick grid lines for blending
    ofSetColor(255, 255, 255);
    ofSetLineWidth(3);
    
    //vertical line
    for(int i = 0; i <= blender[1].getCanvasWidth(); i+=40){
        ofLine(i, 0, i, blender[1].getCanvasHeight());
    }
    
    //horizontal lines
    for(int j = 0; j <= blender[1].getCanvasHeight(); j+=40){
        ofLine(0, j, blender[1].getCanvasWidth(), j);
    }
}

void ProjectionManager::endBottom() {
    blender[1].end();
}

void ProjectionManager::setupGui() {
    guiName = "Projectors";
    panel.setup(guiName, "settings/projectors.xml");
    
    group.setName("Edge Blending");
    group.add(gamma.set("gamma", 0.5, 0, 1));
    group.add(luminance.set("luminance", 0, 0, 1));
    group.add(power.set("power", 1, 0, 1));
    group.add(showBlend.set("show blend", true));
    panel.add(group);
    
    //projectors
    panel.add(projection[0].set("1 Top left"));
    panel.add(projection[1].set("2 Top right"));
    panel.add(projection[2].set("3 Bottom left"));
    panel.add(projection[3].set("4 Bottom right"));
    
    panel.loadFromFile("settings/projectors.xml");
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

