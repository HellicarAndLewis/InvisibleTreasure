//
//  ContourTracker.cpp
//  show
//
//  Created by Chris Mullany on 03/09/2015.
//
//

#include "FlowFinder.h"

using namespace ofxCv;
using namespace cv;

FlowFinder::FlowFinder() {
    active = false;
}

void FlowFinder::setup() {
    active = false;
    imageScale = 0.5;
}

void FlowFinder::update() {
    if(active) {
        image.update();
        image.resize(imageScale*image.width, imageScale*image.height);
        //image.mirror(false, true);
        //cv::resize(image.getPixelsRef(), smallImage, cv::Size(round(imageScale*image.width), round(imageScale*image.height)));
        flow.setNumIterations(numIterations);
        flow.setNumLevels(numLevels);
        flow.setPolyN(polyN);
        flow.setPolySigma(polySigma);
        flow.setPyramidScale(pyramidScale);
        flow.setUseGaussian(useGaussian);
        flow.setWindowSize(windowSize);
        //cv::resize(toCv(image), toCv(smallImage), cv::Size(round(imageScale*image.width), round(imageScale*image.height)));
        flow.calcOpticalFlow(image);
    }
}

void FlowFinder::draw() {
    ofPushStyle();
    ofSetColor(255);
    image.draw(0, 0);
    flow.draw(0, 0);
    ofPopStyle();
}

void FlowFinder::setActive(bool active) {
    this->active = active;
}


//////////////////////////////////////////////////////////////////////////////////
// public
//////////////////////////////////////////////////////////////////////////////////

void FlowFinder::setupGui() {
    guiName = "Flow Finder";
    parameters.setName(guiName);
    parameters.add(numIterations.set("Num Iterations", 2, 0, 10));
    parameters.add(numLevels.set("Num Levels", 4, 0, 10));
    parameters.add(pyramidScale.set("Pyramid Scale", 0.5, 0, 1));
    parameters.add(polyN.set("Poly N", 7, 0, 20));
    parameters.add(polySigma.set("Ply Sigma", 1.5, 0, 5));
    parameters.add(windowSize.set("windowSize", 8, 1, 20));
    parameters.add(useGaussian.set("Use Gaussian", false));
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
