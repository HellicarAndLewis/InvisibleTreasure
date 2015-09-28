//
//  Countdown.h
//  show
//
//  Created by Chris Mullany on 21/09/2015.
//
//

#pragma once
#include "ofMain.h"
#include "ofxTween.h"

class Countdown {
public:
    Countdown();
    
	void setup();
	void update();
	void draw();
    void start(float duration);
    void stop();
    
    bool isComplete();
    
    float progress;
    ofEvent<int> countdownCompleteEvent;

protected:
    bool enabled;
    ofxTween tween;
    ofxEasingLinear easinglinear;
    
private:
    
};