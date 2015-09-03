//
//  SceneManager.h
//  show
//
//  Created by Chris Mullany on 02/09/2015.
//
//

#pragma once
#include "ofMain.h"
#include "ofEvents.h"
#include "SceneBase.h"
#include "ShadowsScene.h"
#include "IgniteScene.h"
#include "LightboxScene.h"
#include "LedDisplay.h"
#include "AppModel.h"
#include "OscClient.h"

class SceneManager {
public:
    SceneManager();
    
    void setup(AppModel* model, OscClient* osc);
	void update();
	void draw();
	void exit();
    
    void playScene(int id);
    void nextScene();
	
	void keyPressed(int key);
	void keyReleased(int key);
	void mouseMoved(int x, int y );
	void mouseDragged(int x, int y, int button);
	void mousePressed(int x, int y, int button);
	void mouseReleased(int x, int y, int button);
	void windowResized(int w, int h);
	void dragEvent(ofDragInfo dragInfo);
	void gotMessage(ofMessage msg);

protected:  
private:
    OscClient* osc;
    AppModel* model;
    
    // common views
    LedDisplay led;
    
    // Scenes
    ShadowsScene shadows;
    IgniteScene ignite;
    LightboxScene lightbox;
    vector<SceneBase*> scenes;
    SceneBase* sceneIn;
    SceneBase* sceneOut;
    int sceneIndex;
    
    ofTrueTypeFont font;
    
    void onSceneChange(SceneBase::State & state);
    void onPlayScene(int& id);
};