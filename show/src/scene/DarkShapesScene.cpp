//
//  DarkShapesScene.cpp
//  show
//
//  Created by Chris Mullany on 02/09/2015.
//
//

#include "DarkShapesScene.h"

DarkShapesScene::DarkShapesScene() {
    name = "Dark Shapes";
    averageVolume = 0;
}

void DarkShapesScene::setup() {
    audioMirror = true  ;
    int n = 10;
    levels.assign(n, 0.0);
    previousLevels.assign(n, 0.0);
    radiusMin.set("radius min", 10, 0, 500);
    radiusMax.set("radius max", 200, 0, 500);
    noiseScale.set("noise scale", 80, 0, 500);
    threshold.set("threshold", 0.5, 0.0, 1.0);
    debugDraw.set("debug draw", false);
}

void DarkShapesScene::update() {
    SceneBase::update();
}

void DarkShapesScene::draw() {
    if (mode==AppModel::SLAVE) {
        led->label = "MAKE SOME SHAPES";
        led->draw();
    }
    else if (mode==AppModel::WINDOW) {
        // TODO: draw shapes
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
    mic->start();
    setState(INTRO);
}

void DarkShapesScene::stop(){
    mic->stop();
    setState(OUTRO);
}

void DarkShapesScene::setupGui() {
    guiName = "Dark Shapes";
    panel.setup(guiName, "settings/darkshapes.xml");
    panel.add(radiusMin);
    panel.add(radiusMax);
    panel.add(noiseScale);
    panel.add(threshold);
    panel.add(debugDraw);
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

//////////////////////////////////////////////////////////////////////////////////
// oF event handlers
//////////////////////////////////////////////////////////////////////////////////

