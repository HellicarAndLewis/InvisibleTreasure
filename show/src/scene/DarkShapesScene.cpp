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
    
    float targetWidth = MIN(ofGetWidth(), displays->masterScreen.sizeIn->x) * 0.7;
    float scale = targetWidth / tracker.thresholded.width;
    float col1Width;
    
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
            countdown->start(5);
        }
    }
    else if (i > 17 && i < 50) {
        for (auto & game : shapeGames) {
            if (i >= game.startScene && i <= game.endScene) {
                game.setScene(i);
                currentShapeGame = &game;
                switch (game.state) {
                    case ShapeGame::INTRO:
                        shapeRenderer.showShape(game.shapeMode);
                        if (isMaster()) countdown->start(2);
                        break;
                    case ShapeGame::PLAY:
                        shapeRenderer.showShape(game.shapeMode);
                        if (isSlave()) led->show("", 10);
                        if (isMaster()) countdown->start(10);
                        break;
                    case ShapeGame::FAIL:
                        if (isSlave()) led->show(game.label);
                        shapeRenderer.hide();
                        if (isMaster()) {
                            // TODO: send bad sound
                            countdown->start(4);
                        }
                        break;
                    case ShapeGame::PASS:
                        shapeRenderer.hide(true);
                        if (isMaster()) {
                            // TODO: send bad sound
                            countdown->start(3);
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
        if (isSlave()) led->show(nextLevel.get(), 5);
        if (isMaster()) {
            countdown->start(5);
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
    panel.add(title.set("title1", "Bonus Game"));
    panel.add(playAgain.set("title2", "Play Again"));
    panel.add(goingDark.set("title3", "Going Dark"));
    panel.add(nextLevel.set("title4", "Next Level"));
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
            ShapeGame::State state = currentShapeGame->next();
            if (state == ShapeGame::INACTIVE) play(currentShapeGame->endScene + 1);
            else play(currentShapeGame->sceneI);
        }
    }
    else {
        nextSubscene();
    }
    
}

void DarkShapesScene::onSuccess() {
    ofLogNotice() << "DarkShapesScene::onSuccess";
    if (currentShapeGame != NULL) {
        currentShapeGame->success();
        play(currentShapeGame->sceneI);
    }
}
//////////////////////////////////////////////////////////////////////////////////
// oF event handlers
//////////////////////////////////////////////////////////////////////////////////

