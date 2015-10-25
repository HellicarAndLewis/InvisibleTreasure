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
    radius = 0;
    isFirstPlay = true;
    alphaShader.load("shaders/greyscaleAlpha.vert", "shaders/greyscaleAlpha.frag");
    SceneBase::setup();
}

void FlantsScene::update() {
    if (isMaster() && state == INTERACTIVE) {
        
        // contour tracker
        ContourTracker& tracker = *vision->getTracker();
        ofxCv::ContourFinder& contourFinder = tracker.contourFinder;
        float scale = displays->masterProjection.sizeIn->x / tracker.thresholded.width;
        float targetW = displays->masterProjection.sizeIn->x;
        float targetH = tracker.thresholded.height * scale;
        
        if (mode == YELLOW_SMALL || mode == RED_SMALL) {
            radius = ofLerp(radius, radiusSmall, 0.1);
        } else if (mode == EXPLODE) {
            radius = radiusLarge * 2;
        } else {
            radius = ofLerp(radius, radiusLarge, 0.1);
        }
        
        if (isFirstPlay) {
            revertBgLearn = tracker.bgLearningTime;
            isFirstPlay = false;
        }
        tracker.bgLearningTime = eatRate;
        
        particles.attractPoints.clear();
        for(int i = 0; i < contourFinder.size(); i++) {
            // blob rect and position
            auto rect = toOf(contourFinder.getBoundingRect(i));
            auto center = toOf(contourFinder.getCenter(i));
            particles.attractPoints.push_back(ofRectangle(rect.x*scale, rect.y*scale, rect.width*scale, rect.height*scale));
        }
        ofPoint centre = ofPoint(displays->masterProjection.sizeIn.get().x/2, displays->masterProjection.sizeIn.get().y/2);
        particles.bounds.setFromCenter(centre, radius, radius);
        particles.update();
        
        if (mode == YELLOW_SMALL || mode == YELLOW_BLUE_SHAPES || mode == RED_SMALL || mode == RED_BLUE_SHAPES) {
            if (particles.allFull) {
                particles.resetEating();
                nextSubscene();
                cout<<"entering Next Scene"<<endl;
            }
        }
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
        // Draw the background shapes FBO into a shader
        // Anything with r == 0 is made transparent
        alphaShader.begin();
        alphaShader.setUniformTexture("tex0", particles.background.getTextureReference(), 0);
        particles.background.draw(0,0);
        alphaShader.end();
    }
    // particles
    particles.draw();
}

//////////////////////////////////////////////////////////////////////////////////
// public
//////////////////////////////////////////////////////////////////////////////////
void FlantsScene::play(int i){
    if (isMaster()) {
        vision->isEnabled = true;
        if (i == 51) {
            setMode(YELLOW_SMALL);
            countdown->stop();
            osc->sendLightSoundCue(cues[0]);
        }
        else if (i == 52) {
            setMode(YELLOW_EXPAND);
            countdown->start(timerTransition);
            osc->sendLightSoundCue(cues[1]);
        }
        else if (i == 53) {
            setMode(YELLOW_BLUE_SHAPES);
            countdown->stop();
            osc->sendLightSoundCue(cues[2]);
        }
        else if (i == 54) {
            setMode(RED_SMALL);
            countdown->stop();
            osc->sendLightSoundCue(cues[3]);
        }
        else if (i == 55 || i == 59) {
            setMode(RED_EXPAND);
            countdown->start(timerTransition);
            osc->sendLightSoundCue(cues[4]);
        }
        else if (i == 56 || i == 60) {
            setMode(RED_BLUE_SHAPES);
            countdown->stop();
            osc->sendLightSoundCue(cues[5]);
        }
        else if (i == 57 || i == 61) {
            setMode(EXPLODE);
            countdown->start(timerBoom);
            osc->sendLightSoundCue(cues[6]);
        }
        else if (i == 58) {
            setMode(EXPLODE);
            countdown->start(timerGameOver + timerAgain);
            osc->sendLightSoundCue(cues[7]);
        }
        else if (i == 62) {
            countdown->start(17 /*timerGameOver + timerTired + timerNext*/);
            osc->sendLightSoundCue(cues[8]);
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
            led->hide();
            led->queue(LedDisplay::Params(gameOver.get(), 0, timerGameOver, 0, false, 0));
            led->queue(LedDisplay::Params(playAgain.get(), 0, timerAgain, 0, false, 0));
            led->playQueue();
        }
        else if (i == 62) {
            led->hide();
            led->queue(LedDisplay::Params(gameOver.get(), 0, 4, 0, false, 0));
            led->queue(LedDisplay::Params(tired.get(), 0, 8, 0, false, 0));
            led->queue(LedDisplay::Params(nextLevel.get(), 0, 5, 0, false, 5));
            led->playQueue();
        }
    }
    SceneBase::play(i);
}

void FlantsScene::stop(){;
    vision->getTracker()->bgLearningTime = revertBgLearn;
    isFirstPlay = true;
    SceneBase::stop();
}

void FlantsScene::setupGui() {
    guiName = "Flants";
    panel.setup(guiName, "settings/flants.xml");
    
    panel.add(eatRate.set("Eat rate", 5, 1, 30));
    panel.add(radiusSmall.set("Radius small", 800, 100, 768));
    panel.add(radiusLarge.set("Radius large", 1536, 100, 1536));
    
    titleGroup.setName("Titles");
    titleGroup.add(title.set("title1", "APE DELETET"));
    titleGroup.add(boom.set("title2", "Boom"));
    titleGroup.add(gameOver.set("title3", "Game over"));
    titleGroup.add(playAgain.set("title4", "Play again"));
    titleGroup.add(tired.set("title5", "He is satisfied"));
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
    
    // cues
    panel.add(cues[0].setup("cue intro", 23, 0));
    // pool expand, lx24, change track
    panel.add(cues[1].setup("cue expand", 24, 0));
    // add blue shapes
    panel.add(cues[2].setup("cue add blue", 23, 0));
    // red
    panel.add(cues[3].setup("cue red", 25, 0));
    // expand
    panel.add(cues[4].setup("cue red expand", 25, 0));
    // add blue
    panel.add(cues[5].setup("cue red blue", 25, 0));
    // boom
    panel.add(cues[6].setup("cue boom", 26, 0));
    // play again
    panel.add(cues[7].setup("cue play again", 0, 0));
    // game over
    panel.add(cues[8].setup("cue game over", 27, 0));
    
    panel.loadFromFile("settings/flants.xml");
    
    // setup bounds after GUI
    ofRectangle bounds = ofRectangle(0, 0, displays->masterProjection.sizeIn.get().x, displays->masterProjection.sizeIn.get().y);
    particles.setup(1000, bounds);
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
            particles.eatBackground = false;
            break;
        case EXPLODE:
            particles.currentMode = Particle::EXPLODE;
            particles.resetEating();
            particles.eatBackground = false;
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
        ofClear(0, 0, 0);
        particles.background.end();
        particles.backgroundShapes.clear();
        particles.addShapesToBg();
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
