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
        led->label = "MAKE SOME SHAPES";
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

//////////////////////////////////////////////////////////////////////////////////
// public
//////////////////////////////////////////////////////////////////////////////////
void DarkShapesScene::play(){
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

