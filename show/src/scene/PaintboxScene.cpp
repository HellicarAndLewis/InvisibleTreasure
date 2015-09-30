//
//  PaintboxScene.cpp
//  show
//
//  Created by Chris Mullany on 05/09/2015.
//
//

#include "PaintboxScene.h"
using namespace ofxCv;
using namespace cv;

PaintboxScene::PaintboxScene() {
    name = "Paintbox";
}

void PaintboxScene::setup() {
    // subscenes
    subsceneStart = 63;
    subsceneEnd = 66;
    brushImage.loadImage("images/paintbrush/brush1.png");
    SceneBase::setup();
}

void PaintboxScene::update() {
    SceneBase::update();
    if (isMaster() && getIsActive()) {
        totalBlobArea = 0;
        ContourTracker& tracker = *vision->getTracker();
        ofxCv::ContourFinder& contourFinder = tracker.contourFinder;
        for(int i = 0; i < contourFinder.size(); i++) {
            auto rect = toOf(contourFinder.getBoundingRect(i));
            totalBlobArea += rect.getArea();
        }
    }
    if (isWindow()) {
        imageElement.update();
    }
}

void PaintboxScene::draw() {
    if (isWindow()) {
        imageElement.draw(ofRectangle(0, 0, ofGetWidth(), ofGetHeight()));
    }
    SceneBase::draw();
}

void PaintboxScene::drawMasterProjection() {
    ofSetColor(255);
    
    if (getIsActive()) {
        
        // contour tracker
        ContourTracker& tracker = *vision->getTracker();
        ofxCv::ContourFinder& contourFinder = tracker.contourFinder;
        float scale = displays->masterProjection.sizeIn->x / tracker.thresholded.width;
        float targetW = displays->masterProjection.sizeIn->x;
        float targetH = tracker.thresholded.height * scale;
        
        // start canvas FBO
        canvas.begin();
        for(int i = 0; i < contourFinder.size(); i++) {
            auto rect = toOf(contourFinder.getBoundingRect(i));
            auto center = toOf(contourFinder.getCenter(i));
            // label identifies specific blobs
            // use the label as a key for assigning colours
            int label = contourFinder.getLabel(i);
            ofSetColor(getColour(label));
            // in eraser mode, blobs above a certain size have specific colours
            // white acts as an eraser
            // black is, black.
            if (mode == ERASER) {
                float areaPercent = rect.getArea() / totalBlobArea;
                if (areaPercent >= minAreaEraser) {
                    ofSetColor(255);
                }
                if (areaPercent >= minAreaBlack) {
                    ofSetColor(0);
                }
            }
            // rotate the brush image to vary the painted lines
            ofPushMatrix();
            ofTranslate(center*scale);
            ofRotateZ(ofRandom(360));
            float scale = ofClamp((rect.width)/brushImage.width, 0.5, 4);
            int w = brushImage.width*scale;
            int h = brushImage.height*scale;
            brushImage.draw(-w/2, -h/2, w, h);
            ofPopMatrix();
        }
        ofSetColor(255);
        canvas.end();
    }
    
    canvas.draw(0,0);
}

//////////////////////////////////////////////////////////////////////////////////
// public
//////////////////////////////////////////////////////////////////////////////////
void PaintboxScene::play(int i){
    if (isMaster()) {
        vision->isEnabled = true;
    }
    switch (i) {
        case 63:
            if (isSlave()) {
                led->show(sleeping.get());
            }
            if (isMaster()) {
                countdown->start(timerIntro);
                setMode(INACTIVE);
            }
            break;
        case 64:
            if (isSlave()) led->show(title.get());
            if (isMaster()) {
                countdown->start(timerLines);
                setMode(LINES);
            }
            break;
        case 65:
            if (isSlave()) led->show(title.get());
            if (isMaster()) {
                countdown->start(timerErase);
                setMode(ERASER);
            }
            if (isWindow()) imageElement.hide();
            break;
        case 66:
            if (isSlave()) {
                led->hide();
                led->queue(LedDisplay::Params(back.get(), 0, 5, 0));
                led->queue(LedDisplay::Params(bonusTime.get(), 0, 5, 0));
                led->queue(LedDisplay::Params(bonusGame.get(), 0, 1, 0, false, 5));
                led->queue(LedDisplay::Params(goingDark.get(), 0, 1, 0));
                led->queue(LedDisplay::Params(bonusGame.get(), 0, 1, 0));
                led->queue(LedDisplay::Params(goingDark.get(), 0, 1, 0));
                led->queue(LedDisplay::Params(bonusGame.get(), 0, 1, 0));
                led->playQueue();
            }
            if (isMaster()) {
                countdown->start(timerOutro);
                setMode(FADE);
            }
            if (isWindow()) {
                // pink/yellow image
                imageElement.setup("images/bonusGame.jpg");
                imageElement.show();
            }
            break;
        default:
            break;
    }
    SceneBase::play(i);
}

void PaintboxScene::stop(){
    if (isWindow()) imageElement.hide();
    SceneBase::stop();
}

void PaintboxScene::setupGui() {
    guiName = "Paintbox";
    panel.setup(guiName, "settings/paintbox.xml");
    
    panel.add(minAreaEraser.set("min area eraser", 0.1667, 0, 1));
    panel.add(minAreaBlack.set("min area black", 0.3333, 0, 1));
    
    titleGroup.setName("Titles");
    titleGroup.add(title.set("title", "Paintbox"));
    titleGroup.add(sleeping.set("sleeping", "He is sleeping"));
    titleGroup.add(back.set("back", "He's back"));
    titleGroup.add(bonusTime.set("game time", "Bonus game time"));
    titleGroup.add(bonusGame.set("bonus game", "Bonus game"));
    titleGroup.add(goingDark.set("going dark", "Going dark"));
    panel.add(titleGroup);
    
    timerGroup.setName("Timers");
    timerGroup.add(timerIntro.set("intro", 5, 1, 10));
    timerGroup.add(timerLines.set("draw lines", 4*60, 1, 7*60));
    timerGroup.add(timerErase.set("draw eraser", 5*60, 1, 7*60));
    timerGroup.add(timerBack.set("hes back", 5, 1, 10));
    timerGroup.add(timerBonus.set("bonus", 5, 1, 10));
    timerGroup.add(timerOutro.set("outro", 15, 1, 20));
    panel.add(timerGroup);
    
    panel.loadFromFile("settings/paintbox.xml");
    
    
    // canvas FBO setup
    ofRectangle bounds = ofRectangle(0, 0, displays->masterProjection.sizeIn.get().x, displays->masterProjection.sizeIn.get().y);
    canvas.allocate(bounds.width, bounds.height, GL_RGB);
}
//////////////////////////////////////////////////////////////////////////////////
// protected
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
// private
//////////////////////////////////////////////////////////////////////////////////
void PaintboxScene::setMode(Mode mode) {
    this->mode = mode;
    clearCanvas();
}

void PaintboxScene::clearCanvas(){
    canvas.begin();
    ofClear(255, 255, 255, 255);
    canvas.end();
}

ofColor PaintboxScene::getColour(int blob) {
    auto it = colours.find(blob);
    if (it != colours.end()) {
        return it->second;
    } else {
        int min = 100;
        int max = 255;
        colours[blob] = ofColor(ofRandom(min, max), ofRandom(min, max), ofRandom(min, max));
    }
}

bool PaintboxScene::getIsActive(){
    return (mode == LINES || mode == ERASER);
}
//////////////////////////////////////////////////////////////////////////////////
// custom event handlers
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
// oF event handlers
//////////////////////////////////////////////////////////////////////////////////
