//
//  Countdown.cpp
//  show
//
//  Created by Chris Mullany on 21/09/2015.
//
//

#include "Countdown.h"

Countdown::Countdown() {
}

void Countdown::setup() {
}

void Countdown::update() {
    if(enabled) {
        float currentTime = ofGetElapsedTimef();
        float diff = currentTime - startTime;
        progress = countdownDuration - diff;
        if(progress <= 0) {
            stop();
            ofLogVerbose() << "countdown completed";
            int i = 0;
            ofNotifyEvent(countdownCompleteEvent, i, this);
        }
    }
//    if (tween.isRunning() && enabled) {
//        progress = tween.update();
//        if (tween.isCompleted()) {
//            ofLogVerbose() << "countdown tween.isCompleted()";
//            int i = 0;
//            ofNotifyEvent(countdownCompleteEvent, i, this);
//        }
//    }
}

void Countdown::draw() {
}

//////////////////////////////////////////////////////////////////////////////////
// public
//////////////////////////////////////////////////////////////////////////////////
void Countdown::start(float duration) {
    enabled = true;
//    tween.setParameters(easinglinear, ofxTween::easeIn, duration, 0, duration*1000, 0);
//    tween.start();
    startTime = ofGetElapsedTimef();
    countdownDuration = duration;
}
void Countdown::stop() {
    progress = 0;
    enabled = false;
}

bool Countdown::isComplete() {
//    return (tween.isCompleted() && enabled);
    return (progress <= 0 && enabled);
}
//////////////////////////////////////////////////////////////////////////////////
// protected
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
// private
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
// custom event handlers
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
// oF event handlers
//////////////////////////////////////////////////////////////////////////////////
