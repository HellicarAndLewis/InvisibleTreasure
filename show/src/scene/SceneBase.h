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
#include "DisplayManager.h"

// Each Scene shoud inherit from SceneBase
// provides pointers to common objects
//
class SceneBase : public GuiableBase, public Sequencable {
public:
    
    SceneBase();
    
	virtual void setup();
	virtual void update();
    virtual void draw();
    virtual void drawSlaveProjection(){}
    virtual void drawMasterProjection(){}
    virtual void drawMasterScreen(){}
    virtual void play();
    virtual void play(int i);
    virtual void stop();
    
    // common pointers
    // passed into each scene by SceneManager
    DisplayManager * displays;
    OscClient * osc;
    VisionManager* vision;
    LedDisplay* led;
    Countdown* countdown;
    Mic* mic;
    ofTrueTypeFont* font;
    AppModel* appModel;
    
    // scenes can have child scenes (subscenes)
    // a subscene is more like a state, but each has a unique int id
    // ids are counted globally across all scenes
    int subsceneStart;
    int subsceneEnd;
    int subsceneI;
    ofEvent<int> nextSubsceneEvent;
    
    bool isDebugMode;
    string name;
    string modeLabel;
    AppModel::Mode mode;

protected:
    void nextSubscene();
    bool isMaster();
    bool isSlave();
    bool isWindow();
    
    // Master / Slave display drawing methods
    bool beginMasterScreenDraw();
    bool endMasterScreenDraw();
    bool beginMasterProjectionDraw();
    bool endMasterProjectionDraw();
    bool beginSlaveScreenDraw();
    bool endSlaveScreenDraw();
    bool beginSlaveProjectionDraw();
    bool endSlaveProjectionDraw();
    
    // App mode change listner
    // only used for dev/debugging when switching modes at runtime
    void onModeChange(AppModel::Mode& mode);
    
    void onCountdownComplete(int& i);
    
    // common scene params
    ofParameter<int> countdownDuration;
    
private:
    
};