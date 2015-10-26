//
//  Sequencable.h
//  show
//
//  Created by Chris Mullany on 04/09/2015.
//
//

#pragma once
#include "ofMain.h"
#include "ofxTween.h"

//
// Inherit from this to add sequencable states to a class!
// each state has a time/duration that can be 0 or more seconds
// Scenes inherit from this to manage scene sequencing
// LED messages inherit to allow message sequencing
//
class Sequencable {
public:
    enum State {
        INACTIVE=0, INTRO, INTERACTIVE, OUTRO
    } state = INACTIVE;
    
    virtual void update();
    virtual void play();
    virtual void stop();
    void setState(State state, float delay = 0.0);
    
    ofEvent<State> stateChangeEvent;
    float progress;
    
protected:
    ofxTween tween;
    ofxEasingLinear easinglinear;
    float timeIn = 0.5f;
    float timeHold = -1.0f;
    float timeOut = 0.5f;
    bool loop = false;
    int loopNum;
    
private:
    
};