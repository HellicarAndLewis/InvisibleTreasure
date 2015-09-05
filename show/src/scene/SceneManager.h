//
//  SceneManager.h
//  show
//
//  Created by Chris Mullany on 02/09/2015.
//
//

#pragma once
#include "ofMain.h"
#include "AllSceneIncludes.h"
#include "LedDisplay.h"
#include "AppModel.h"
#include "OscClient.h"
#include "Mic.h"

// Scene Manager
// - Setup and sequence of each scene
// - Common things such as mic input, vision input and fonts
//   are setup and passed to each scene as pointers
// - creates a GUI panel that allows each scene GUI to be toggled
//
class SceneManager : public GuiableBase {
public:
    SceneManager();
    
    void setup(AppModel* model, OscClient* osc, VisionManager* vision);
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
    
    void setupGui();
    void drawGui();
    

protected:  
private:
    
    // passed in from app
    OscClient* osc;
    AppModel* model;
    
    // common display things
    LedDisplay led;
    Mic mic;
    ofTrueTypeFont font;
    
    // gui
    ofxButton nextSceneButton;
    ofParameter<int> sceneSelctor;
    void onSceneSelect(int & i);
    
    // Scenes
    ShadowsScene shadows;
    IgniteScene ignite;
    LightboxScene lightbox;
    DarkShapesScene darkShapes;
    FlantsScene flants;
    PaintboxScene paintbox;
    DancingDarkScene dancingDark;
    CassandraScene cassandra;
    SitinScene sitin;
    
    vector<SceneBase*> scenes;
    int sceneIndex;
    SceneBase* sceneIn;
    SceneBase* sceneOut;
    
    // event listeners
    void onModeChange(AppModel::Mode& mode);
    void onSceneChange(SceneBase::State & state);
    void onPlayScene(int& id);
};