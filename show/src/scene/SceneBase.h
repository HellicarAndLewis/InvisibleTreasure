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

struct SceneEventArgs {
    float time;
};

class SceneBase {
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
    ofTrueTypeFont* font;
    AppModel::Mode mode;
    
    bool isDebugMode;
    string name;
    string modeLabel;

protected:
    ofxTween tween;
    ofxEasingLinear easinglinear;
    float progress;
    
private:
    
};