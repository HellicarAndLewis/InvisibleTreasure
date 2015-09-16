//
//  LedDisplay.cpp
//  show
//
//  Created by Chris Mullany on 02/09/2015.
//
//

#include "LedDisplay.h"

LedDisplay::LedDisplay() {
}

void LedDisplay::setup() {
    message.setup("fonts/led_board-7.ttf", 30);
}

void LedDisplay::update() {
    message.messageString = title;
    if (showCountdown) {
        int elapsed = ofGetElapsedTimeMillis() - countdownStart;
        int timer = ceil((countdownDuration - elapsed)/1000);
        if (timer > 0)
            message.messageString += "\n" + ofToString(timer);
    }
}

void LedDisplay::draw() {
    ofSetColor(250);
    ofRect(0, 0, ofGetWidth()/2, ofGetHeight()/2);
    ofSetColor(20);
    message.draw();
}

//////////////////////////////////////////////////////////////////////////////////
// public
//////////////////////////////////////////////////////////////////////////////////

void LedDisplay::show(string title, float countdown) {
    message.show(title, 1);
    this->title = title;
    showCountdown = (countdown > 0);
    countdownDuration = countdown * 1000;
    countdownStart = ofGetElapsedTimeMillis();
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
