//
//  DarkShapesScene.cpp
//  show
//
//  Created by Chris Mullany on 02/09/2015.
//
//

#include "DarkShapesScene.h"
using namespace ofxCv;
using namespace cv;

DarkShapesScene::DarkShapesScene() {
    name = "Dark Shapes";
}

void DarkShapesScene::setup() {
    ofSetCircleResolution(100);
    shapeRenderer.setup();
    modeSelector.addListener(this, &DarkShapesScene::onShapeModeSelect);
    modeSelector.set("Shape Mode", 0, 0, shapeRenderer.shapes.size()-1);
    
    shapeGames.push_back(ShapeGame(ShapeRenderer::CIRCLE, 18, 21));
    shapeGames.push_back(ShapeGame(ShapeRenderer::RECTANGLE, 22, 25));
    shapeGames.push_back(ShapeGame(ShapeRenderer::TRIANGLE, 26, 29));
    shapeGames.push_back(ShapeGame(ShapeRenderer::STAR, 30, 33));
    shapeGames.push_back(ShapeGame(ShapeRenderer::CIRCLE, 34, 37));
    shapeGames.push_back(ShapeGame(ShapeRenderer::RECTANGLE, 38, 41));
    shapeGames.push_back(ShapeGame(ShapeRenderer::TRIANGLE, 42, 45));
    shapeGames.push_back(ShapeGame(ShapeRenderer::STAR, 46, 49));
    currentShapeGame = NULL;
    
    // subscenes
    subsceneStart = 17;
    subsceneEnd = 50;
    SceneBase::setup();
}

void DarkShapesScene::update() {
    shapeRenderer.update();
    SceneBase::update();
}

void DarkShapesScene::draw() {
    if (isWindow()) {
        shapeRenderer.draw();
    }
    if (isMaster()) {
    }
    SceneBase::draw();
}

void DarkShapesScene::drawMasterScreen() {
    ContourTracker& tracker = *vision->getTracker();
    ofxCv::ContourFinder& contourFinder = tracker.contourFinder;
    
    if (!tracker.thresholded.isAllocated()) {
        ofLogError() << "in DarkShapesScene::drawMasterScreen tracker image is not allocated";
        return;
    }
    
    float targetWidth = MIN(ofGetWidth(), displays->masterScreen.sizeIn->x) * 0.5;
    float scale = targetWidth / tracker.thresholded.width;
    float col1Width = 0;
    
    ofPushStyle();
    ofPushMatrix();
    {
        ofTranslate(10, 10);
        ofPushMatrix();
        {
            //ofScale(scale, scale);
            ofSetColor(255);
            drawVision();
            //tracker.thresholded.draw(0, 0);
        }
        ofPopMatrix();
        
        ofTranslate(tracker.thresholded.width*scale, 10);
        col1Width = 10 + tracker.thresholded.width*scale;
        if (currentShapeGame != NULL) currentShapeGame->draw();
        
        
        ofTranslate(0, 20);
        targetWidth = MIN(ofGetWidth(), displays->masterScreen.sizeIn->x) * 0.3;
        if (shapeRenderer.getBgRect().width > 1) {
            scale = targetWidth / shapeRenderer.getBgRect().width;
            ofPushMatrix();
            {
                ofScale(scale, scale);
                shapeRenderer.draw();
            }
            ofPopMatrix();
        }
    }
    ofPopMatrix();
    ofPopStyle();
    
    shapesPanel.setPosition(col1Width, 40 + shapeRenderer.getBgRect().height*scale);
    shapesPanel.draw();
}

//////////////////////////////////////////////////////////////////////////////////
// public
//////////////////////////////////////////////////////////////////////////////////
void DarkShapesScene::play(int i){
//    if(isSlave()) {
//        if (i == 35) osc->sendLightingCue(lxCues[1]);
//        else if (i == 36 || i == 37) osc->sendLightingCue(lxCues[2]);
//        else if (i == 39) osc->sendLightingCue(lxCues[3]);
//        else if (i == 40 || i == 41) osc->sendLightingCue(lxCues[4]);
//        else if (i == 43) osc->sendLightingCue(lxCues[5]);
//        else if (i == 44 || i == 45) osc->sendLightingCue(lxCues[6]);
//        else if (i == 47) osc->sendLightingCue(lxCues[7]);
//        else if (i == 48 || i == 49) osc->sendLightingCue(lxCues[8]);
//        else if (i == 50) osc->sendLightingCue(lxCues[9]);
//    }
    if (isMaster()) {
        vision->isEnabled = true;
        if (i == 35) osc->sendLightingCue(lxCues[1]);
        else if (i == 36 || i == 37) osc->sendLightingCue(lxCues[2]);
        else if (i == 39) osc->sendLightingCue(lxCues[3]);
        else if (i == 40 || i == 41) osc->sendLightingCue(lxCues[4]);
        else if (i == 43) osc->sendLightingCue(lxCues[5]);
        else if (i == 44 || i == 45) osc->sendLightingCue(lxCues[6]);
        else if (i == 47) osc->sendLightingCue(lxCues[7]);
        else if (i == 48 || i == 49) osc->sendLightingCue(lxCues[8]);
        else if (i == 50) {
            osc->sendLightingCue(lxCues[9]);
            osc->sendSoundCue(48);
        }
    }
    if (i == 17) {
        if (isSlave()) led->hide();
        if (isMaster()) {
            countdown->start(0);
            //osc->sendSoundCue(soundCueBonus);
            //osc->sendLightingCue(lxCues[0]);
        }
    }
    if (i > 17 && i < 50) {
        shapeRenderer.invert = (i >= 34);
        for (auto & game : shapeGames) {
            if (i >= game.startScene && i <= game.endScene) {
                game.setScene(i);
                currentShapeGame = &game;
                switch (game.state) {
                    case ShapeGame::INTRO:
                        shapeRenderer.showShape(game.shapeMode);
                        if (isMaster()) {
                            if(game.label == "Circle") osc->sendSoundCue(CircleStarting);
                            else if(game.label == "Rectangle") osc->sendSoundCue(SquareStarting);
                            else if(game.label == "Triangle") osc->sendSoundCue(TriangleStarting);
                            else if(game.label == "Star") osc->sendSoundCue(StarStarting);
                            countdown->start(timers[1]);
                        }
                        if (isSlave()){
                            if (i < 34) led->show(game.label);
                            else led->show("Going Dark");
                        }
                        break;
                    case ShapeGame::PLAY:
                        shapeRenderer.showShape(game.shapeMode);
                        if (isSlave()) {
                            led->hide();
                            led->queue(LedDisplay::Params("", 0, 10, 0, false, 10));
                            led->queue(LedDisplay::Params("", 0, 2, 0, false));
                            led->playQueue();
                            //led->show("", timers[2]);
                        }
                        if (isMaster()) {
                            //osc->sendSoundCue(soundCueCount);
                            countdown->start(timers[2]);
                        }
                        break;
                    case ShapeGame::FAIL:
                        if (isSlave()) {
                            led->hide();
                            led->queue(LedDisplay::Params(game.label, 0, 1, 0, false, 0));
                            led->queue(LedDisplay::Params("Fail", 0, 1, 0));
                            led->queue(LedDisplay::Params(game.label, 0, 1, 0));
                            led->queue(LedDisplay::Params("Fail", 0, 1, 0));
                            led->playQueue();
                        }
                        shapeRenderer.hide();
                        if (isMaster()) {
                            osc->sendSoundCue(soundCueBad);
                            countdown->start(timers[3]);
                        }
                        break;
                    case ShapeGame::PASS:
                        shapeRenderer.hide(true);
                        if (isSlave()) led->hide();
                        if (isMaster()) {
                            osc->sendSoundCue(soundCueGood);
                            countdown->start(timers[4]);
                        }
                        break;
                    default:
                        break;
                }
            }
        }
    }
//    if(i == 18) {
//        if(isSlave()) {
//            led->hide();
//            led->queue(LedDisplay::Params("Circle", 0, 2, 0, false));
//            led->playQueue();
//        }
//    }
//    if(i == 18 || i == 34) {
//        if(isMaster()) {
//            osc->sendSoundCue(CircleStarting);
//            osc->sendLightingCue(lxCues[0]);
//        }
//    }
//    if(i == 22 || i == 38 ) {
//        if(isMaster())
//            osc->sendSoundCue(SquareStarting);
//    }
//    if(i == 26 || i == 42) {
//        if(isMaster())
//            osc->sefffndSoundCue(TriangleStarting);
//    }
//    if(i == 30 || i == 48) {
//        if(isMaster())
//            osc->sendSoundCue(StarStarting);
//    }
    // outro
    if (i == 50) {
        if (isSlave()) {
            led->hide();
            // text to display message, time in, time out, hold loop, countdown
            led->queue(LedDisplay::Params(bonusComplete.get(), 0, 6, 0, false, 0));
            led->queue(LedDisplay::Params(heIs.get(), 0, 8, 0, false, 0));
            led->queue(LedDisplay::Params(nextLevel.get(), 0, 5, 0, false, 5));
            led->queue(LedDisplay::Params("", 0, 0, 0, false));
            led->playQueue();
        }
        if (isMaster()) {
            countdown->start(19);
        }
    }
    SceneBase::play(i);
}

void DarkShapesScene::stop(){
    for (auto & game : shapeGames) game.attemptNum = 1;
    SceneBase::stop();
}

void DarkShapesScene::setupGui() {
    guiName = "Dark Shapes";
    panel.setup(guiName, "settings/darkshapes.xml");
    panel.add(title.set("title", "Bonus game"));
    panel.add(playAgain.set("play again", "Play again"));
    panel.add(goingDark.set("going dark", "Going dark"));
    panel.add(bonusComplete.set("bonus complete", "Bonus game completed"));
    panel.add(heIs.set("he is", "He is pleased"));
    panel.add(nextLevel.set("next level", "Next level"));
    
    timerGroup.setName("Timers");
    timerGroup.add(timers[0].set("intro", 5, 1, 20));
    timerGroup.add(timers[1].set("shape intro", 2, 1, 20));
    timerGroup.add(timers[2].set("shape play", 10, 1, 30));
    timerGroup.add(timers[3].set("shape fail", 4, 1, 20));
    timerGroup.add(timers[4].set("shape pass", 3, 1, 20));
    timerGroup.add(timers[5].set("bonus complete", 3, 1, 20));
    timerGroup.add(timers[6].set("he is", 3, 1, 20));
    timerGroup.add(timers[7].set("next level", 5, 1, 20));
    panel.add(timerGroup);
    
    
    // LX cues:
    // 11 bright, 12 blackout, 13 lights up, 14 black, 15 lights,
    // 16 black, 17 lights, 18 black, 19 lights, 22 countdown
    lxCueGroup.setName("LX Cues");
    lxCueGroup.add(lxCues[0].set("bright", 11, 0, 100));
    lxCueGroup.add(lxCues[1].set("blackout 1", 12, 0, 100));
    lxCueGroup.add(lxCues[2].set("lights 1", 13, 0, 100));
    lxCueGroup.add(lxCues[3].set("blackout 2", 14, 0, 100));
    lxCueGroup.add(lxCues[4].set("lights 2", 15, 0, 100));
    lxCueGroup.add(lxCues[5].set("blackout 3", 16, 0, 100));
    lxCueGroup.add(lxCues[6].set("lights 3", 17, 0, 100));
    lxCueGroup.add(lxCues[7].set("blackout 4", 18, 0, 100));
    lxCueGroup.add(lxCues[8].set("lights 4", 19, 0, 100));
    lxCueGroup.add(lxCues[9].set("countdown", 22, 0, 100));
    ofParameter<int> outroCueLighting;
    panel.add(lxCueGroup);
    
    // Sound cues
    // bonus, countdown, bad, good
    soundCueGroup.setName("Sound Cues");
    soundCueGroup.add(soundCueBonus.set("bonus", 11, 0, 100));
    soundCueGroup.add(soundCueCount.set("countdown", 12, 0, 100));
    soundCueGroup.add(soundCueBad.set("bad", 13, 0, 100));
    soundCueGroup.add(soundCueGood.set("good", 14, 0, 100));
    soundCueGroup.add(CircleStarting.set("Circle Starting", 15, 0, 100));
    soundCueGroup.add(SquareStarting.set("Square Starting", 16, 0, 100));
    soundCueGroup.add(TriangleStarting.set("Triangle Starting", 17, 0, 100));
    soundCueGroup.add(StarStarting.set("Star Starting", 18, 0, 100));
    soundCueGroup.add(outroCueSound.set("OutroCueSound", 48, 0, 100));
    
    panel.add(soundCueGroup);
    
    ofParameterGroup timerGroup;
    ofParameter<int> timers[DARK_SHAPES_TIMER_COUNT];
    
    // TODO: add LX and sound cues to GUI
    
    panel.loadFromFile("settings/darkshapes.xml");
    // add to panel
    shapesPanel.setup("Shapes Control");
    shapeSuccess.addListener(this, &DarkShapesScene::onSuccess);
    shapesPanel.add(shapeSuccess.setup("Success"));
}

void DarkShapesScene::drawGui() {
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
void DarkShapesScene::onShapeModeSelect(int & i) {
    //shapeRenderer.showShape((ShapeRenderer::shapeMode)i);
}

void DarkShapesScene::onCountdownComplete(int& i) {
    ofLogVerbose() << "DarkShapesScene::onCountdownComlete for " << name;
    if (subsceneI > 17 && subsceneI < 50) {
        // Shape game mode
        if (currentShapeGame != NULL) {
            ShapeGame::State state = currentShapeGame->getNextState();
            if (state == ShapeGame::INACTIVE) {
                nextSubscene(currentShapeGame->endScene + 1);
            }
            else {
                int id = currentShapeGame->getSceneForState(state);
                nextSubscene(id);
            }
        }
    }
    else {
        nextSubscene();
    }
    
}

void DarkShapesScene::onSuccess() {
    //if (currentShapeGame != NULL) {
    //osc->sendSoundCue(soundCueGood);
    currentShapeGame->success();
    ofLogNotice() << "DarkShapesScene::onSuccess, now play " + ofToString(currentShapeGame->sceneI);
    nextSubscene(currentShapeGame->sceneI);
    //}
}
//////////////////////////////////////////////////////////////////////////////////
// oF event handlers
//////////////////////////////////////////////////////////////////////////////////

