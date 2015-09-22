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
    
    // TODO: listen to OSC events for shape mode changes
    
    // subscenes
    subsceneStart = 17;
    subsceneEnd = 50;
    SceneBase::setup();
}

void DarkShapesScene::update() {
    if (mode==AppModel::SLAVE) {
    }
    else if (mode==AppModel::WINDOW) {
        shapeRenderer.update();
    }
    else if (mode==AppModel::MASTER) {
        // TODO: draw CV debug in window 1
        // TODO: draw tracked blobs/shapes in window 2
    }
    SceneBase::update();
}

void DarkShapesScene::draw() {
    if (mode==AppModel::SLAVE) {
        led->title = "MAKE SOME SHAPES";
        led->draw();
    }
    else if (mode==AppModel::WINDOW) {
        shapeRenderer.draw();
    }
    else if (mode==AppModel::MASTER) {
        // TODO: draw CV debug in window 1
        // TODO: draw tracked blobs/shapes in window 2 
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
    // outro
    else if (i == 50) {
        if (isSlave()) led->show(nextLevel.get(), 5);
        if (isMaster()) {
            countdown->start(5);
        }
    }
    shapeRenderer.showShape(0);
    SceneBase::play();
}

void DarkShapesScene::stop(){
    SceneBase::stop();
}

void DarkShapesScene::setupGui() {
    guiName = "Dark Shapes";
    panel.setup(guiName, "settings/darkshapes.xml");
    panel.add(modeSelector);
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
    shapeRenderer.showShape(i);
}
//////////////////////////////////////////////////////////////////////////////////
// oF event handlers
//////////////////////////////////////////////////////////////////////////////////

