//
//  ContourTracker.cpp
//  show
//
//  Created by Chris Mullany on 03/09/2015.
//
//

#include "ContourTracker.h"

using namespace ofxCv;
using namespace cv;

ContourTracker::ContourTracker() {
}

void ContourTracker::setup() {
    
    contourFinder.setMinAreaRadius(10);
    contourFinder.setMaxAreaRadius(200);
    contourFinder.setThreshold(threshold);
    // wait for half a frame before forgetting something
    contourFinder.getTracker().setPersistence(15);
    // an object can move up to 32 pixels per frame
    contourFinder.getTracker().setMaximumDistance(32);
    
    showLabels = true;
    image = NULL;
}

void ContourTracker::update() {
    // bg
    background.setLearningTime(bgLearningTime);
    background.setThresholdValue(bgThreshold);
    background.update(*image, thresholded);
    thresholded.update();
    // contours
    blur(thresholded, 10);
    contourFinder.findContours(thresholded);
    contourFinder.setThreshold(threshold);
}

void ContourTracker::draw() {
    RectTracker& tracker = contourFinder.getTracker();
    
    if(showLabels) {
        ofSetColor(255);
        thresholded.draw(0, 0);
        ofSetColor(255, 0, 0);
        contourFinder.draw();
        for(int i = 0; i < contourFinder.size(); i++) {
            ofPoint center = toOf(contourFinder.getCenter(i));
            ofPushMatrix();
            ofTranslate(center.x, center.y);
            int label = contourFinder.getLabel(i);
            string msg = ofToString(label) + ":" + ofToString(tracker.getAge(label));
            ofDrawBitmapString(msg, 0, 0);
            ofVec2f velocity = toOf(contourFinder.getVelocity(i));
            ofScale(5, 5);
            ofLine(0, 0, velocity.x, velocity.y);
            ofPopMatrix();
        }
    } else {
        for(int i = 0; i < contourFinder.size(); i++) {
            unsigned int label = contourFinder.getLabel(i);
            // only draw a line if this is not a new label
            if(tracker.existsPrevious(label)) {
                // use the label to pick a random color
                ofSeedRandom(label << 24);
                ofSetColor(ofColor::fromHsb(ofRandom(255), 255, 255));
                // get the tracked object (cv::Rect) at current and previous position
                const cv::Rect& previous = tracker.getPrevious(label);
                const cv::Rect& current = tracker.getCurrent(label);
                // get the centers of the rectangles
                ofVec2f previousPosition(previous.x + previous.width / 2, previous.y + previous.height / 2);
                ofVec2f currentPosition(current.x + current.width / 2, current.y + current.height / 2);
                ofLine(previousPosition, currentPosition);
            }
        }
    }
    
    // this chunk of code visualizes the creation and destruction of labels
    const vector<unsigned int>& currentLabels = tracker.getCurrentLabels();
    const vector<unsigned int>& previousLabels = tracker.getPreviousLabels();
    const vector<unsigned int>& newLabels = tracker.getNewLabels();
    const vector<unsigned int>& deadLabels = tracker.getDeadLabels();
    ofSetColor(cyanPrint);
    for(int i = 0; i < currentLabels.size(); i++) {
        int j = currentLabels[i];
        ofLine(j, 0, j, 4);
    }
    ofSetColor(magentaPrint);
    for(int i = 0; i < previousLabels.size(); i++) {
        int j = previousLabels[i];
        ofLine(j, 4, j, 8);
    }
    ofSetColor(yellowPrint);
    for(int i = 0; i < newLabels.size(); i++) {
        int j = newLabels[i];
        ofLine(j, 8, j, 12);
    }
    ofSetColor(ofColor::white);
    for(int i = 0; i < deadLabels.size(); i++) {
        int j = deadLabels[i];
        ofLine(j, 12, j, 16);
    }
}


//////////////////////////////////////////////////////////////////////////////////
// public
//////////////////////////////////////////////////////////////////////////////////
void ContourTracker::resetBg() {
    background.reset();
}

void ContourTracker::setupGui() {
    
    //ofParameter<float> maxRadius;
    //ofParameter<float> threhshold;
    //ofParameter<int> perstistence;
    //ofParameter<float> maxDistance;
    //ofParameter<float> blurAmount;
    //ofParameter<float> bgLearningTime;
    //ofParameter<float> bgThreshold;
    
    guiName = "Contour Tracking";
    parameters.setName(guiName);
    parameters.add(threshold.set("threshold", 15, 0, 255));
    parameters.add(bgLearningTime.set("bg learn time", 900, 100, 2000));
    parameters.add(bgThreshold.set("bg threshold", 10, 0, 255));
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
