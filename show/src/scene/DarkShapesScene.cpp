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
    
    shapeGames.push_back(ShapeGame(ShapeRenderer::CIRCLE, 18, 20));
    shapeGames.push_back(ShapeGame(ShapeRenderer::RECTANGLE, 21, 24));
    shapeGames.push_back(ShapeGame(ShapeRenderer::TRIANGLE, 25, 28));
    shapeGames.push_back(ShapeGame(ShapeRenderer::STAR, 29, 32));
    shapeGames.push_back(ShapeGame(ShapeRenderer::CIRCLE, 33, 36));
    shapeGames.push_back(ShapeGame(ShapeRenderer::RECTANGLE, 37, 40));
    shapeGames.push_back(ShapeGame(ShapeRenderer::TRIANGLE, 41, 44));
    shapeGames.push_back(ShapeGame(ShapeRenderer::STAR, 45, 48));
    currentShapeGame = NULL;
    
    // TODO: listen to OSC events for shape mode changes
    
    // subscenes
    subsceneStart = 17;
    subsceneEnd = 50;
    SceneBase::setup();
}

void DarkShapesScene::update() {
    shapeRenderer.update();
    if (isMaster()) {
        // TODO: draw CV debug in window 1
        // TODO: draw tracked blobs/shapes in window 2
    }
    SceneBase::update();
}

void DarkShapesScene::draw() {
    if (isWindow()) {
        shapeRenderer.draw();
    }
    SceneBase::draw();
}

void DarkShapesScene::drawMasterScreen() {
    ContourTracker& tracker = *vision->getTracker();
    ofxCv::ContourFinder& contourFinder = tracker.contourFinder;
    
    float scale = tracker.thresholded.width / displays->masterScreen.sizeIn->x;
    
    ofPushStyle();
    ofPushMatrix();
    ofTranslate(10, 10);
    ofSetColor(255);
    tracker.thresholded.draw(0, 0);
    
    ofTranslate(tracker.thresholded.width, 0);
    if (currentShapeGame != NULL) currentShapeGame->draw();
    
    ofTranslate(0, 100);
    ofScale(0.3, 0.3);
    shapeRenderer.draw();
    
    ofPopMatrix();
    ofPopStyle();
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
                        if (isMaster()) countdown->start(3);
                        break;
                    case ShapeGame::SHAPE:
                        shapeRenderer.showShape(game.shapeMode);
                        if (isMaster()) countdown->start(2);
                        break;
                    case ShapeGame::PLAY:
                        shapeRenderer.showShape(game.shapeMode);
                        if (isSlave()) led->show("", 10);
                        if (isMaster()) countdown->start(10);
                        break;
                    case ShapeGame::OUTRO:
                        if (isSlave()) led->show(game.label);
                        shapeRenderer.hide();
                        if (isMaster()) countdown->start(4);
                        break;
                        
                    default:
                        break;
                }
            }
        }
    }
    /*
    else if (i <= 20) {
        shapeMode == CIRCLE;
    }
    else if (i <= 24) {
        shapeMode == RECTANGLE;
    }
    else if (i <= 28) {
        shapeMode == TRIANGLE;
    }
    else if (i <= 32) {
        shapeMode == STAR;
    }
    // Dark modes
    else if (i <= 36) {
        shapeMode == CIRCLE;
    }
    else if (i <= 40) {
        shapeMode == RECTANGLE;
    }
    else if (i <= 44) {
        shapeMode == TRIANGLE;
    }
    else if (i <= 48) {
        shapeMode == STAR;
    }
    else if (i == 49) {
        // lx cue 19, good sound
    }
     */
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
    //panel.add(modeSelector);
    panel.loadFromFile("settings/darkshapes.xml");
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
            if (state == ShapeGame::INACTIVE) nextSubscene();
            else play(currentShapeGame->sceneI);
        }
    }
    else {
        nextSubscene();
    }
    
}
//////////////////////////////////////////////////////////////////////////////////
// oF event handlers
//////////////////////////////////////////////////////////////////////////////////

