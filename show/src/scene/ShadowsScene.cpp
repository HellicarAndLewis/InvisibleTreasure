//
//  ShadowsScene.cpp
//  show
//
//  Created by Chris Mullany on 02/09/2015.
//
//

#include "ShadowsScene.h"

ShadowsScene::ShadowsScene() {
    name = "Shadows";
}

void ShadowsScene::setup() {
}

void ShadowsScene::update() {
    SceneBase::update();
}

void ShadowsScene::draw() {
    if (mode == AppModel::SLAVE) {
        // Draw video
    }
    SceneBase::draw();
}

//////////////////////////////////////////////////////////////////////////////////
// public
//////////////////////////////////////////////////////////////////////////////////
void ShadowsScene::play(){
    setState(INTRO);
}

void ShadowsScene::stop(){
    setState(OUTRO);
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

void ShadowsScene::keyPressed (int key) {}

void ShadowsScene::keyReleased (int key) {}

void ShadowsScene::mouseMoved(int x, int y) {}

void ShadowsScene::mouseDragged(int x, int y, int button) {}

void ShadowsScene::mousePressed(int x, int y, int button) {}

void ShadowsScene::mouseReleased(int x, int y, int button) {}

void ShadowsScene::windowResized(int w, int h) {}

void ShadowsScene::dragEvent(ofDragInfo dragInfo) {}

void ShadowsScene::gotMessage(ofMessage msg) {}
