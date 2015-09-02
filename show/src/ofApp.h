#pragma once

#include "ofMain.h"
#include "ofxArgs.h"
#include "led/LedDisplay.h"
#include "input/VisionManager.h"
#include "scene/SceneManager.h"
#include "network/OscClient.h"

class ofApp : public ofBaseApp {
public:
    enum MODE {MASTER, SLAVE, WINDOW} mode;
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
    string modeString;
    VisionManager vision;
    SceneManager sceneManager;
    LedDisplay led;
    OscClient osc;
};
