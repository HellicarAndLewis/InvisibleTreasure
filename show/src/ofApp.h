#pragma once

#include "ofMain.h"
#include "ofxArgs.h"
#include "AppModel.h"
#include "ofxRemoteUIServer.h"
#include "ofxRemoteUIofParamaterSync.h"
#include "ofxGui.h"
#include "input/VisionManager.h"
#include "scene/SceneManager.h"
#include "network/OscClient.h"
#include "GuiableBase.h"


class ofApp : public ofBaseApp, GuiableBase {
public:
    ofApp(ofxArgs* args);
	void setup();
	void update();
	void draw();
    
    void keyPressed(int key);
    void keyReleased(int key);
    void mouseMoved(int x, int y );
    void mouseDragged(int x, int y, int button);
    void mousePressed(int x, int y, int button);
    void mouseReleased(int x, int y, int button);
    void windowResized(int w, int h);
    
private:
    
    void setupGui();
    void drawGui();
    
    AppModel appModel;
    VisionManager vision;
    SceneManager sceneManager;
    OscClient osc;
    
    ofParameter<bool> debug;
    ofxRemoteUIofParamaterSync ruiBridge;
};
