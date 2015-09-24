//
//  FlantsScene.cpp
//  show
//
//  Created by Chris Mullany on 05/09/2015.
//
//

#include "FlantsScene.h"
using namespace ofxCv;
using namespace cv;

FlantsScene::FlantsScene() {
    name = "Flants";
}

void FlantsScene::setup() {
    // subscenes
    subsceneStart = 51;
    subsceneEnd = 62;
    ofRectangle bounds = ofRectangle(0, 0, displays->masterProjection.sizeIn.get().x, displays->masterProjection.sizeIn.get().y);
    //bounds.scaleFromCenter(0.9);
    particles.setup(1000, bounds);
    SceneBase::setup();
    radius = 0;
}

void FlantsScene::update() {
    if (isMaster()) {
        
        // contour tracker
        ContourTracker& tracker = *vision->getTracker();
        ofxCv::ContourFinder& contourFinder = tracker.contourFinder;
        float scale = displays->masterProjection.sizeIn->x / tracker.thresholded.width;
        float targetW = displays->masterProjection.sizeIn->x;
        float targetH = tracker.thresholded.height * scale;
        
        if (mode == YELLOW_SMALL || mode == RED_SMALL) {
            radius = ofLerp(radius, radiusSmall, 0.1);
        }
        else {
            radius = ofLerp(radius, radiusLarge, 0.1);
        }
        
        tracker.bgLearningTime = eatRate;
        
        particles.attractPoints.clear();
        for(int i = 0; i < contourFinder.size(); i++) {
            // blob rect and position
            auto rect = toOf(contourFinder.getBoundingRect(i));
            auto center = toOf(contourFinder.getCenter(i));
            particles.attractPoints.push_back(ofPoint(center.x*scale, center.y*scale));
        }
        ofPoint centre = ofPoint(displays->masterProjection.sizeIn.get().x/2, displays->masterProjection.sizeIn.get().y/2);
        particles.bounds.setFromCenter(centre, radius, radius);
        particles.update();
    }
    SceneBase::update();
}

void FlantsScene::draw() {
    SceneBase::draw();
}

void FlantsScene::drawMasterProjection() {
    ofSetColor(255);
    
    // contour tracker
    ContourTracker& tracker = *vision->getTracker();
    ofxCv::ContourFinder& contourFinder = tracker.contourFinder;
    float scale = displays->masterProjection.sizeIn->x / tracker.thresholded.width;
    float targetW = displays->masterProjection.sizeIn->x;
    float targetH = tracker.thresholded.height * scale;
    tracker.thresholded.draw(0, 0, targetW, targetH);

    if (mode == YELLOW_BLUE_SHAPES || mode == RED_BLUE_SHAPES) {
        particles.background.draw(0,0);
    }
    // particles
    particles.draw();
}

//////////////////////////////////////////////////////////////////////////////////
// public
//////////////////////////////////////////////////////////////////////////////////
void FlantsScene::play(int i){
    if (isMaster()) {
        if (i == 51) {
            setMode(YELLOW_SMALL);
            countdown->stop();
        }
        else if (i == 52) {
            setMode(YELLOW_EXPAND);
            countdown->start(timerTransition);
        }
        else if (i == 53) {
            setMode(YELLOW_BLUE_SHAPES);
            countdown->stop();
        }
        else if (i == 54) {
            setMode(RED_SMALL);
            countdown->stop();
        }
        else if (i == 55 || i == 59) {
            setMode(RED_EXPAND);
            countdown->start(timerTransition);
        }
        else if (i == 56 || i == 60) {
            setMode(RED_BLUE_SHAPES);
            countdown->stop();
        }
        else if (i == 57 || i == 61) {
            setMode(EXPLODE);
            countdown->start(timerBoom);
        }
        else if (i == 58) {
            countdown->start(timerGameOver + timerAgain);
        }
        else if (i == 62) {
            countdown->start(timerGameOver + timerTired + timerNext);
        }
    }
    
    // LED title
    if (isSlave()) {
        if ( (i>50 && i<57) || (i>58 && i<61) ) {
            led->show(title.get());
        }
        else if (i == 57 || i == 61) {
            led->show(boom.get());
        }
        else if (i == 58) {
            led->show(gameOver.get() + "\n" + playAgain.get());
        }
        else if (i == 62) {
            led->show(gameOver.get() + "\n" + tired.get() + "\n" + nextLevel.get(), timerNext);
        }
    }
    SceneBase::play(i);
}

void FlantsScene::stop(){
    // stop/unload/clear things
    SceneBase::stop();
}

void FlantsScene::setupGui() {
    guiName = "Flants";
    panel.setup(guiName, "settings/flants.xml");
    
    panel.add(eatRate.set("Eat rate", 5, 1, 30));
    panel.add(radiusSmall.set("Radius small", 800, 100, 768));
    panel.add(radiusLarge.set("Radius large", 1536, 100, 1536));
    
    titleGroup.setName("Titles");
    titleGroup.add(title.set("title1", "Flants"));
    titleGroup.add(boom.set("title2", "Boom"));
    titleGroup.add(gameOver.set("title3", "Game over"));
    titleGroup.add(playAgain.set("title4", "Play again"));
    titleGroup.add(tired.set("title5", "He is tired"));
    titleGroup.add(nextLevel.set("title6", "Next Level"));
    panel.add(titleGroup);
    
    timerGroup.setName("Timers");
    timerGroup.add(timerTransition.set("transition", 3, 1, 10));
    timerGroup.add(timerBoom.set("boom", 5, 1, 10));
    timerGroup.add(timerGameOver.set("game over", 3, 1, 10));
    timerGroup.add(timerAgain.set("again", 5, 1, 10));
    timerGroup.add(timerTired.set("tired", 3, 1, 10));
    timerGroup.add(timerNext.set("next", 5, 1, 10));
    panel.add(timerGroup);
    
    panel.loadFromFile("settings/flants.xml");
}
//////////////////////////////////////////////////////////////////////////////////
// protected
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
// private
//////////////////////////////////////////////////////////////////////////////////
void FlantsScene::setMode(Mode mode) {
    this->mode = mode;
    switch (mode) {
        case YELLOW_SMALL:
        case YELLOW_BLUE_SHAPES:
            particles.currentMode = Particle::EAT_GREEN;
            break;
        case YELLOW_EXPAND:
        case RED_EXPAND:
            particles.currentMode = Particle::EAT_NOTHING;
            particles.resetEating();
            break;
        case RED_SMALL:
        case RED_BLUE_SHAPES:
            particles.currentMode = Particle::EAT_GROW;
            break;
        default:
            break;
    }
    
    if (mode == RED_EXPAND || mode == RED_SMALL || mode == RED_BLUE_SHAPES) {
        particles.setColour(ofColor(255,0,0));
    }
    else {
        particles.setColour(ofColor(255,234,119));
    }
    
    if (mode == YELLOW_BLUE_SHAPES || mode == RED_BLUE_SHAPES) {
        particles.eatBackground = true;
        // draw into the particles FBO
        particles.background.begin();
        ofClear(0, 0, 0, 0);
        particles.background.end();
        particles.backgroundShapes.clear();
    }
    else {
        particles.eatBackground = false;
    }
}
//////////////////////////////////////////////////////////////////////////////////
// custom event handlers
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
// oF event handlers
//////////////////////////////////////////////////////////////////////////////////
