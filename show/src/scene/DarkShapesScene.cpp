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
    
    float targetWidth = MIN(ofGetWidth(), displays->masterScreen.sizeIn->x) * 0.7;
    float scale = targetWidth / tracker.thresholded.width;
    float col1Width = 0;
    
    ofPushStyle();
    ofPushMatrix();
    {
        ofTranslate(10, 10);
        ofPushMatrix();
        {
            ofScale(scale, scale);
            ofSetColor(255);
            tracker.thresholded.draw(0, 0);
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
    if (i == 17) {
        if (isSlave()) led->show(title.get());
        if (isMaster()) {
            countdown->start(timers[0]);
        }
    }
    else if (i > 17 && i < 50) {
        shapeRenderer.invert = (i >= 34);
        for (auto & game : shapeGames) {
            if (i >= game.startScene && i <= game.endScene) {
                game.setScene(i);
                currentShapeGame = &game;
                switch (game.state) {
                    case ShapeGame::INTRO:
                        shapeRenderer.showShape(game.shapeMode);
                        if (isMaster()) countdown->start(timers[1]);
                        if (isSlave()) led->hide();
                        break;
                    case ShapeGame::PLAY:
                        shapeRenderer.showShape(game.shapeMode);
                        if (isSlave()) led->show("", timers[2]);
                        if (isMaster()) countdown->start(timers[2]);
                        break;
                    case ShapeGame::FAIL:
                        if (isSlave()) {
                            led->queue(LedDisplay::Params(game.label, 0, 1, 0, false, 0));
                            led->queue(LedDisplay::Params(playAgain.get(), 0, 1, 0));
                            led->queue(LedDisplay::Params(game.label, 0, 1, 0));
                            led->queue(LedDisplay::Params(playAgain.get(), 0, 1, 0));
                            led->playQueue();
                        }
                        shapeRenderer.hide();
                        if (isMaster()) {
                            // TODO: send bad sound
                            countdown->start(timers[3]);
                        }
                        break;
                    case ShapeGame::PASS:
                        shapeRenderer.hide(true);
                        if (isSlave()) led->hide();
                        if (isMaster()) {
                            // TODO: send bad sound
                            countdown->start(timers[4]);
                        }
                        break;
                        
                    default:
                        break;
                }
            }
        }
    }
    // outro
    else if (i == 50) {
        if (isSlave()) {
            led->queue(LedDisplay::Params(bonusComplete.get(), 0, timers[5], 0, false, 0));
            led->queue(LedDisplay::Params(heIs.get(), 0, timers[6], 0, false, 0));
            led->queue(LedDisplay::Params(nextLevel.get(), 0, timers[7], 0, false, timers[7]));
            led->playQueue();
        }
        if (isMaster()) {
            countdown->start(timers[5] + timers[6] + timers[7]);
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
    panel.add(bonusComplete.set("bonus complete", "Bonus game complete"));
    panel.add(heIs.set("he is", "He is [tbc]"));
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
            if (state == ShapeGame::INACTIVE) nextSubscene(currentShapeGame->endScene + 1);
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
    if (currentShapeGame != NULL) {
        currentShapeGame->success();
        ofLogNotice() << "DarkShapesScene::onSuccess, now play " + ofToString(currentShapeGame->sceneI);
        nextSubscene(currentShapeGame->sceneI);
    }
}
//////////////////////////////////////////////////////////////////////////////////
// oF event handlers
//////////////////////////////////////////////////////////////////////////////////

