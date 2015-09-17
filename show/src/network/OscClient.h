//
//  OscClient.h
//  show
//
//  Created by Chris Mullany on 02/09/2015.
//
//

#pragma once
#include "ofMain.h"
#include "ofxOsc.h"
#include "GuiableBase.h"
#define NUM_MSG_STRINGS 20

class OscClient : public GuiableBase {
public:
    OscClient();
    
	void setup();
	void update();
	void draw();
	void exit();
	
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
    void sendPlayScene(int id);
    void sendPlaySubScene(int id);
    void sendPresence(string areaName, int count);
    
    ofEvent<int> playSceneEvent;
    ofEvent<int> playSubSceneEvent;
    
protected:  
private:
    ofxOscSender sender;
    ofxOscReceiver receiver;
    int current_msg_string;
    string msg_strings[NUM_MSG_STRINGS];
    float timers[NUM_MSG_STRINGS];
    
    ofParameter<string> info;
    ofParameter<string> sendAddress;
    ofParameter<string> sendPort;
    ofParameter<string> receivePort;
};