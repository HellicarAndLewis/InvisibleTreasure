//
//  SceneBase.h
//  show
//
//  Created by Chris Mullany on 02/09/2015.
//
//

#pragma once
#include "ofMain.h"
#include "ofxTween.h"
#include "AppModel.h"
#include "LedDisplay.h"
#include "Mic.h"
#include "GuiableBase.h"
#include "VisionManager.h"
#include "OscClient.h"

struct SceneEventArgs {
    float time;
};

// Each Scene shoud inherit from SceneBase
// provides common pointers, tweening, state management and drawing
//
class SceneBase : public GuiableBase  {
public:
    enum State {
        INACTIVE, INTRO, INTERACTIVE, OUTRO
    } state;
    
    SceneBase();
    
	virtual void setup();
	virtual void update();
	virtual void draw();
    virtual void play();
    virtual void stop();
    void setState(State state);
	
	void keyPressed(int key);
	void keyReleased(int key);
	void mouseMoved(int x, int y );
	void mouseDragged(int x, int y, int button);
	void mousePressed(int x, int y, int button);
	void mouseReleased(int x, int y, int button);
	void windowResized(int w, int h);
	void dragEvent(ofDragInfo dragInfo);
	void gotMessage(ofMessage msg);
    
    ofEvent<State> stateChangeEvent;
    
    // common pointers
    OscClient * osc;
    VisionManager* vision;
    LedDisplay* led;
    Mic* mic;
    ofTrueTypeFont* font;
    
    bool isDebugMode;
    string name;
    string modeLabel;
    AppModel::Mode mode;

protected:
    ofxTween tween;
    ofxEasingLinear easinglinear;
    float progress;
    
private:
    
};