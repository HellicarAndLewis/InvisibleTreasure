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

// Inherit from this to add sequencable states to a class
// Scenes inherit from this to manage scene sequencing
//
class Sequencable {
public:
    enum State {
        INACTIVE=0, INTRO, INTERACTIVE, OUTRO
    } state = INACTIVE;
    
    virtual void update();
    virtual void play();
    virtual void stop();
    void setState(State state);
    
    ofEvent<State> stateChangeEvent;
    
protected:
    ofxTween tween;
    ofxEasingLinear easinglinear;
    float progress;
    float timeIn = 0.5f;
    float timeHold = -1.0f;
    float timeOut = 0.5f;
    bool loop = false;
    
private:
    
};