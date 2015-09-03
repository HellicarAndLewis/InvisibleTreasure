//
//  SceneManager.cpp
//  show
//
//  Created by Chris Mullany on 02/09/2015.
//
//

#include "SceneManager.h"

SceneManager::SceneManager() {
}

void SceneManager::setup() {
    scenes.push_back(&shadows);
    scenes.push_back(&ignite);
    scenes.push_back(&lightbox);
    sceneIndex = 0;
    sceneIn = scenes[sceneIndex];
    sceneOut = NULL;
    font.loadFont("fonts/verdana.ttf", 30);
    for (auto scene: scenes) {
        scene->font = &font;
        scene->setup();
        ofAddListener(scene->stateChangeEvent, this, &SceneManager::onSceneChange);
    }
}

void SceneManager::update() {
    for (auto scene: scenes)
        scene->update();
}

void SceneManager::draw() {
    for (auto scene: scenes)
        if (scene->state != SceneBase::INACTIVE) scene->draw();
}

void SceneManager::exit() {
}

//////////////////////////////////////////////////////////////////////////////////
// public
//////////////////////////////////////////////////////////////////////////////////
void SceneManager::playScene(int id) {
    // if sceneOut is not NULL we're already animating out a scene
    // TODO: check for sceneOut != NULL and handle?
    if (id >= 0 && id < scenes.size()) {
        sceneIndex = id;
        sceneOut = sceneIn;
        sceneIn = scenes[id];
        sceneOut->stop();
    }
    else ofLogWarning() << "in SceneManager::playScene, " << id << " is out of bounds";
    
}
void SceneManager::nextScene(){
    if (++sceneIndex < scenes.size()-1) {
        playScene(sceneIndex);
    }
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
void SceneManager::onSceneChange(SceneBase::State & state) {
    if (state == SceneBase::INACTIVE) {
        ofLogNotice() << "SceneManager::onSceneChange: INACTIVE, play next scene";
        if (sceneIn != NULL) {
            sceneIn->play();
            sceneOut = NULL;
        }
    }
}

//////////////////////////////////////////////////////////////////////////////////
// oF event handlers
//////////////////////////////////////////////////////////////////////////////////

void SceneManager::keyPressed (int key) {
    switch (key) {
        case '1':
            playScene(0);
            break;
        case '2':
            playScene(1);
            break;
        case '3':
            playScene(2);
            break;
        default:
            break;
    }
}

void SceneManager::keyReleased (int key) {}

void SceneManager::mouseMoved(int x, int y) {}

void SceneManager::mouseDragged(int x, int y, int button) {}

void SceneManager::mousePressed(int x, int y, int button) {}

void SceneManager::mouseReleased(int x, int y, int button) {}

void SceneManager::windowResized(int w, int h) {}

void SceneManager::dragEvent(ofDragInfo dragInfo) {}

void SceneManager::gotMessage(ofMessage msg) {}
