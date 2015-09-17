//
//  SceneManager.cpp
//  show
//
//  Created by Chris Mullany on 02/09/2015.
//
//

#include "SceneManager.h"

SceneManager::SceneManager() {
    sceneIndex = -1;
    subSceneIndex = 0;
    subSceneQueued = false;
}

void SceneManager::setup(AppModel* model, OscClient* osc, VisionManager* vision, DisplayManager * displays) {
    
    this->model = model;
    ofAddListener(this->model->modeChangeEvent, this, &SceneManager::onModeChange);
    this->osc = osc;
    ofAddListener(this->osc->playSceneEvent, this, &SceneManager::onPlayScene);
    ofAddListener(this->osc->playSubSceneEvent, this, &SceneManager::onPlaySubScene);
    
    mic.setup();
    led.setup();
    led.setDisplay(&displays->slaveScreen);
    
    // push each scene onto scenes
    // this will determine the scene order
    scenes.push_back(&shadows);
    scenes.push_back(&ignite);
    scenes.push_back(&lightbox);
    scenes.push_back(&darkShapes);
    scenes.push_back(&flants);
    scenes.push_back(&paintbox);
    scenes.push_back(&dancingDark);
    scenes.push_back(&cassandra);
    scenes.push_back(&sitin);
    sceneIn = NULL;
    sceneOut = NULL;
    font.loadFont("fonts/verdana.ttf", 14);
    
    // setup each scene, passing pointers to common/shared things
    // TODO: pass pointer to app model, have scene listen for mode change
    for (auto scene: scenes) {
        scene->displays = displays;
        scene->mode = model->mode;
        scene->appModel = model;
        scene->osc = osc;
        scene->vision = vision;
        scene->mic = &mic;
        scene->led = &led;
        scene->font = &font;
        scene->modeLabel = model->modeString;
        scene->setup();
        ofAddListener(scene->stateChangeEvent, this, &SceneManager::onSceneChange);
    }
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
    if (id >= 0 && id < scenes.size() && id != sceneIndex) {
        sceneIndex = id;
        sceneSelctor = id;
        if (sceneIn == NULL) {
            sceneIn = scenes[id];
            subSceneIndex = sceneIn->subsceneStart;
            sceneIn->play(subSceneIndex);
        }
        // if sceneOut is not NULL we're already animating out a scene
        // animate that scene out first
        // animate the new one in later when onSceneChange is called
        else {
            sceneOut = sceneIn;
            sceneIn = scenes[id];
            sceneOut->stop();
        }
        if (model->mode == AppModel::MASTER) osc->sendPlayScene(id);
    }
}

void SceneManager::nextScene(){
    playScene(sceneIndex+1);
}

void SceneManager::playSubScene(int id) {
    if (id > 0) {
        subSceneIndex = id;

        // get the subscene's parent scene
        int sceneI = getSceneForSubscene(id);
        // if it's the current scene, scene->playSubScene(id);
        // if not, stop current scene, set next scene and next subscene
        if (sceneI == this->sceneIndex) {
            sceneIn->play(id);
        }
        else {
            // this will animate the old scene before animating in the new one
            subSceneQueued = true;
            playScene(sceneI);
        }
        if (model->mode == AppModel::MASTER) osc->sendPlaySubScene(id);
    }
}

void SceneManager::nextSubScene(){
    subSceneIndex++;
    playSubScene(subSceneIndex);
}

void SceneManager::setupGui() {
    guiName = "Scene Manager";
    
    // setup GUI elements
    nextSceneButton.addListener(this, &SceneManager::nextScene);
    sceneSelctor.addListener(this, &SceneManager::onSceneSelect);
    sceneSelctor.set("scene", 0, 0, scenes.size()-1);
    nextSubSceneButton.addListener(this, &SceneManager::nextSubScene);
    subSceneIndex.addListener(this, &SceneManager::onSubSceneSelect);
    int subSceneMax = scenes.back()->subsceneEnd;
    subSceneIndex.set("sub scene", 0, 0, subSceneMax);
    // add to panel
    panel.setup(guiName, "settings/scenes.xml");
    panel.add(nextSubSceneButton.setup("next subscene"));
    panel.add(subSceneIndex);
    panel.add(nextSceneButton.setup("next scene"));
    panel.add(sceneSelctor);
    
    // child panels
    for (auto scene: scenes)
        guiables.push_back((GuiableBase*)scene);
    
    parameters.setName("Scene GUIs");
    for (auto guiable: guiables) {
        guiable->setupGui();
        parameters.add(guiable->guiEnabled.set(guiable->guiName, false));
        guiable->panel.setPosition(270*2, 10);
    }
    panel.add(parameters);
    
    panel.loadFromFile("settings/scenes.xml");
}

void SceneManager::drawGui() {
    if (guiEnabled) {
        GuiableBase::drawGui();
        for (auto guiable: guiables) guiable->drawGui();
    }
}

//////////////////////////////////////////////////////////////////////////////////
// protected
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
// private
//////////////////////////////////////////////////////////////////////////////////
int SceneManager::getSceneForSubscene(int subsceneI) {
    int sceneI = -1;
    int i = 0;
    for (auto scene: scenes) {
        if (subsceneI >= scene->subsceneStart && subsceneI <= scene->subsceneEnd) {
            sceneI = i;
        }
        i++;
    }
    return sceneI;
}

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
    // if scene has changed to INACTIVE, it's just closed
    // if there's a scene waiting to play next, play it
    if (state == SceneBase::INACTIVE) {
        ofLogVerbose() << "SceneManager::onSceneChange: INACTIVE, play next scene";
        if (sceneIn != NULL) {
            if (!subSceneQueued) subSceneIndex = sceneIn->subsceneStart;
            sceneIn->play(subSceneIndex);
            sceneOut = NULL;
            subSceneQueued = false;
        }
    }
}

void SceneManager::onPlayScene(int& id) {
    if (model->mode != AppModel::MASTER) {
        playScene(id);
    }
}

void SceneManager::onPlaySubScene(int& id) {
    if (model->mode != AppModel::MASTER) {
        playSubScene(id);
    }
}

void SceneManager::onSceneSelect(int & i) {
    playScene(i);
}
void SceneManager::onSubSceneSelect(int & i) {
    playSubScene(i);
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
        case OF_KEY_RIGHT:
            nextSubScene();
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
