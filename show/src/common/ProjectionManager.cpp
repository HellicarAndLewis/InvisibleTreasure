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
    // Top row is 2 projectors, an overlap of 20 pixels, horizontally blended
    blender[0].setup(w, h, 2, 20, ofxProjectorBlend_Horizontal);
    // Bottom row is 2 projectors, an overlap of 20 pixels, horizontally blended
    blender[1].setup(w, h, 2, 20, ofxProjectorBlend_Horizontal);
    
    // Final blend is 2 composite textures, an overlap of 20 pixels, vertically blended
    blender[2].setup(w*2, h, 2, 20, ofxProjectorBlend_Vertical);
    
    // final output width is x4
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
    // Draw the final output
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
}

void ProjectionManager::endTop() {
    blender[0].end();
}

void ProjectionManager::beginBottom() {
    blender[1].begin();
}

void ProjectionManager::endBottom() {
    blender[1].end();
}

void ProjectionManager::setupGui() {
    guiName = "Projectors";
    panel.setup(guiName, "settings/projectors.xml");
    
    // edge blending
    group.setName("Edge Blending");
    group.add(gamma.set("gamma", 0.5, 0, 1));
    group.add(luminance.set("luminance", 0, 0, 1));
    group.add(power.set("power", 1, 0, 1));
    group.add(showBlend.set("show blend", true));
    panel.add(group);
    
    // projectors
    panel.add(projection[0].set("Top left 1"));
    panel.add(projection[1].set("Top right 2"));
    panel.add(projection[2].set("Bottom left 3"));
    panel.add(projection[3].set("Bottom right 4"));
    
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

