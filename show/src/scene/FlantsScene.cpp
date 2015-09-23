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
    particles.setup();
    SceneBase::setup();
}

void FlantsScene::update() {
    if (isMaster()) {
        particles.update();
    }
    SceneBase::update();
}

void FlantsScene::draw() {
    SceneBase::draw();
}

void FlantsScene::drawMasterProjection() {
    
    // contour tracker
    ContourTracker& tracker = *vision->getTracker();
    ofxCv::ContourFinder& contourFinder = tracker.contourFinder;
    
    float scale = displays->masterProjection.sizeIn->x / tracker.thresholded.width;
    float targetW = displays->masterProjection.sizeIn->x;
    float targetH = tracker.thresholded.height * scale;
    
    tracker.thresholded.draw(0, 0, targetW, targetH);
    for(int i = 0; i < contourFinder.size(); i++) {
        // blob rect and position
        auto rect = toOf(contourFinder.getBoundingRect(i));
        auto center = toOf(contourFinder.getCenter(i));
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
        }
        else if (i == 52) {
            setMode(YELLOW_EXPAND);
            countdown->start(timerTransition);
        }
        else if (i == 53) {
            setMode(YELLOW_BLUE_SHAPES);
        }
        else if (i == 54) {
            setMode(RED_SMALL);
        }
        else if (i == 55 || i == 59) {
            setMode(RED_EXPAND);
            countdown->start(timerTransition);
        }
        else if (i == 56 || i == 60) {
            setMode(RED_BLUE_SHAPES);
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
    
    panel.add(title.set("title1", "Flants"));
    panel.add(boom.set("title2", "Boom"));
    panel.add(gameOver.set("title3", "Game over"));
    panel.add(playAgain.set("title4", "Play again"));
    panel.add(tired.set("title5", "He is tired"));
    panel.add(nextLevel.set("title6", "Next Level"));
    
    timerGroup.setName("Timers");
    timerGroup.add(timerTransition.set("transition", 3, 1, 10));
    timerGroup.add(timerBoom.set("boom", 5, 1, 10));
    timerGroup.add(timerGameOver.set("game over", 3, 1, 10));
    timerGroup.add(timerAgain.set("again", 5, 1, 10));
    timerGroup.add(timerTired.set("tired", 3, 1, 10));
    timerGroup.add(timerNext.set("next", 5, 1, 10));
    
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
}
//////////////////////////////////////////////////////////////////////////////////
// custom event handlers
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
// oF event handlers
//////////////////////////////////////////////////////////////////////////////////
