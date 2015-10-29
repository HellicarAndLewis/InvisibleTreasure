//
//  Paintbrush.cpp
//  test_paintboxFlow
//
//  Created by James Bentley on 10/29/15.
//
//

#include "Paintbrush.h"

void paintBrush::setup(int width, int height) {
    col = ofColor(255, 0, 0);
    loc = ofVec2f(ofRandom(width, height), ofRandom(width, height));
    vel = ofVec2f(0, 0);
    acc = ofVec2f(0, 0);
}

void paintBrush::update(ofxCv::FlowFarneback* flow) {
    flow->getAverageFlowInRegion(ofRectangle(loc.x, loc.y, 10, 10));
    vel += acc;
    loc += vel;
}

void paintBrush::draw() {
    ofSetColor(col.r, col.g, col.b);
    ofCircle(loc.x, loc.y, 10);
}