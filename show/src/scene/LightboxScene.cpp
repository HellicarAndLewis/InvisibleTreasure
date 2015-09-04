//
//  LightboxScene.cpp
//  show
//
//  Created by Chris Mullany on 02/09/2015.
//
//

#include "LightboxScene.h"
using namespace ofxCv;
using namespace cv;

LightboxScene::LightboxScene() {
    name = "Lightbox";
}

void LightboxScene::setup() {
    // add hit areas
    // positions and sizes are normalized
    // so they can later be mapped to the tracking rect
    hitAreas.push_back(HitArea("centre"));
    hitAreas.push_back(HitArea("window1"));
    hitAreas.push_back(HitArea("window2"));
    hitAreas.push_back(HitArea("window3"));
    hitAreas.push_back(HitArea("window4"));
}

void LightboxScene::update() {
    SceneBase::update();
}

void LightboxScene::draw() {
    if (mode == AppModel::MASTER) {
        
        // get vision image
        // do cv tracking
        // fire off OSC when blobs enter certain areas
        ContourTracker& tracker = *vision->getTracker();
        ofxCv::ContourFinder& contourFinder = tracker.contourFinder;
        
        ofPushMatrix();
        ofTranslate(10, 10);
        ofSetColor(120);
        tracker.thresholded.draw(0, 0);
        ofSetColor(255);
        float width = tracker.thresholded.width;
        float height = tracker.thresholded.height;
        int blobCount;
        bool isHit;
        ofNoFill();
        ofSetLineWidth(4);
        
        // iterate over tracked blobs
        for(int i = 0; i < contourFinder.size(); i++) {
            // blob rect and position
            auto rect = toOf(contourFinder.getBoundingRect(i));
            auto center = toOf(contourFinder.getCenter(i));
            isHit = false;
            
            // iterate over all hit areas
            for (auto & hitArea: hitAreas) {
                // set the hit area's rect, scaled to the tracked area
                float scaledX = hitArea.position.get().x * width;
                float scaledY = hitArea.position.get().y * height;
                float scaledW = hitArea.size * width;
                float scaledH = hitArea.size * height;
                hitArea.rect.setFromCenter(scaledX, scaledY, scaledW, scaledH);
                // hit test, increase hit area blob count to track the total
                if (rect.intersects(hitArea.rect)) {
                    isHit = true;
                    hitArea.blobCount++;
                }
            }
            
            // draw the blob rect
            if (isHit) ofSetHexColor(0x43E06D);
            else ofSetHexColor(0x385BE0);
            ofRect(rect);
        }
        
        // now check each hit area to see how many people are in it
        // reset the count to 0 ready for the next loop
        ofSetColor(255, 255, 255, 100);
        for (auto & hitArea: hitAreas) {
            if (hitArea.blobCount > 0) {
                // someone is in this area, send OSC
                osc->sendPresence(hitArea.name, blobCount);
                ofSetHexColor(0xF7D63E);
            }
            else ofSetHexColor(0xA8912A);
            ofRect(hitArea.rect);
            ofDrawBitmapString(hitArea.name, hitArea.rect.getCenter()-ofPoint(30,0));
            hitArea.blobCount = 0;
        }
        ofSetLineWidth(1);
        ofFill();
        ofPopMatrix();
        
    }
    SceneBase::draw();
}

//////////////////////////////////////////////////////////////////////////////////
// public
//////////////////////////////////////////////////////////////////////////////////
void LightboxScene::play(){
    setState(INTRO);
}

void LightboxScene::stop(){
    setState(OUTRO);
}

void LightboxScene::setupGui() {
    guiName = "Lightbox";
    panel.setup(guiName, "settings/lightbox.xml");
    for (auto hit: hitAreas) {
        panel.add(hit.position);
        panel.add(hit.size);
    }
    panel.loadFromFile("settings/lightbox.xml");
}

void LightboxScene::drawGui() {
    GuiableBase::drawGui();
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
