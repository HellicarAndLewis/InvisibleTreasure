//
//  SceneBase.h
//  show
//
//  Created by Chris Mullany on 02/09/2015.
//
//

#pragma once
#include "ofMain.h"
#include "AppModel.h"
#include "LedDisplay.h"
#include "Mic.h"
#include "GuiableBase.h"
#include "VisionManager.h"
#include "OscClient.h"
#include "Sequencable.h"

// Each Scene shoud inherit from SceneBase
// provides pointers to common objects
//
class SceneBase : public GuiableBase, public Sequencable {
public:
    
    SceneBase();
    
	virtual void setup();
	virtual void update();
    virtual void draw();
    virtual void play();
    virtual void play(int i);
    virtual void stop();
	
	void keyPressed(int key);
	void keyReleased(int key);
	void mouseMoved(int x, int y );
	void mouseDragged(int x, int y, int button);
	void mousePressed(int x, int y, int button);
	void mouseReleased(int x, int y, int button);
	void windowResized(int w, int h);
	void dragEvent(ofDragInfo dragInfo);
	void gotMessage(ofMessage msg);
    
    
    // common pointers
    OscClient * osc;
    VisionManager* vision;
    LedDisplay* led;
    Mic* mic;
    ofTrueTypeFont* font;
    AppModel* appModel;
    
    // scenes can have child scenes
    map<int, SceneBase> subscenes;
    int subsceneStart;
    int subsceneEnd;
    int subsceneI;
    
    bool isDebugMode;
    string name;
    string modeLabel;
    AppModel::Mode mode;

protected:
private:
    
};