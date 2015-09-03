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

void SceneManager::setup(AppModel* model, OscClient* osc) {
    
    this->model = model;
    ofAddListener(this->model->modeChangeEvent, this, &SceneManager::onModeChange);
    this->osc = osc;
    ofAddListener(this->osc->playSceneEvent, this, &SceneManager::onPlayScene);
    
    mic.setup();
    led.setup();
    
    scenes.push_back(&shadows);
    scenes.push_back(&ignite);
    scenes.push_back(&lightbox);
    sceneIn = NULL;
    sceneOut = NULL;
    font.loadFont("fonts/verdana.ttf", 30);
    for (auto scene: scenes) {
        scene->mic = &mic;
        scene->led = &led;
        scene->font = &font;
        scene->mode = model->mode;
        scene->modeLabel = model->modeString;
        scene->setup();
        ofAddListener(scene->stateChangeEvent, this, &SceneManager::onSceneChange);
    }
    
    // setup GUI elements
    nextSceneButton.addListener(this, &SceneManager::nextScene);
    sceneIndex.set("scene number", 0, 0, scenes.size()-1);
    
    playScene(0);
}

void SceneManager::update() {
    for (auto scene: scenes)
        if (scene->state != SceneBase::INACTIVE) scene->update();
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
    if (id >= 0 && id < scenes.size()) {
        sceneIndex = id;
        if (sceneIn == NULL) {
            sceneIn = scenes[id];
            sceneIn->play();
        }
        else {
            sceneOut = sceneIn;
            sceneIn = scenes[id];
            sceneOut->stop();
        }
        if (model->mode == AppModel::MASTER) osc->sendPlayScene(id);
    }
    else ofLogWarning() << "in SceneManager::playScene, " << id << " is out of bounds";
    
}

void SceneManager::nextScene(){
    if (++sceneIndex < scenes.size()-1) {
        playScene(sceneIndex);
    }
}

void SceneManager::setupGui() {
    guiName = "Scene Manager";
    panel.setup(guiName, "settings/scenes.xml");
    panel.add(nextSceneButton.setup("next"));
    panel.add(sceneIndex);
    panel.loadFromFile("settings/scenes.xml");
    //guiables.push_back(this);
    //return guiables;
}

void SceneManager::drawGui() {
    GuiableBase::drawGui();
    //for (auto guiable: guiables) guiable->drawGui();
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
void SceneManager::onModeChange(AppModel::Mode& mode){
    for (auto scene: scenes) {
        scene->mode = model->mode;
        scene->modeLabel = model->modeString;
    }
}

void SceneManager::onSceneChange(SceneBase::State & state) {
    if (state == SceneBase::INACTIVE) {
        ofLogNotice() << "SceneManager::onSceneChange: INACTIVE, play next scene";
        if (sceneIn != NULL) {
            sceneIn->play();
            sceneOut = NULL;
        }
    }
}

void SceneManager::onPlayScene(int& id) {
    if (model->mode != AppModel::MASTER) {
        playScene(id);
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
