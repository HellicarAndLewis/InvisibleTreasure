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

void LedDisplay::setup(Countdown * countdown) {
    this->countdown = countdown;
    title1.setup("fonts/led_board-7.ttf", 60);
    title2.setup("fonts/led_board-7.ttf", 60);
}

void LedDisplay::update() {
    title1.messageString = title;
    title2.messageString = "";
    if (showCountdown) {
        int progress = ceil(countdown->progress);
        if (progress > 0) title2.messageString = ofToString(progress);
    }
}

void LedDisplay::draw() {
    title1.layout.setLineLength(getDisplayRect().width);
    title2.layout.setLineLength(getDisplayRect().width);
    display->begin();
    ofClear(39, 36, 37);
    //ofSetColor(39, 36, 37);
    //ofRect(getDisplayRect());
    ofSetColor(226, 148, 57);
    int stringW = title1.getWidth();
    title1.draw(getDisplayRect().getCenter().x-title1.getWidth()/2, getDisplayRect().getHeight()*0.3);
    
    stringW = title2.getWidth();
    title2.draw(getDisplayRect().getCenter().x-title2.getWidth()/2, getDisplayRect().getHeight()*0.6);
    ofSetColor(255);
    display->end();
}

//////////////////////////////////////////////////////////////////////////////////
// public
//////////////////////////////////////////////////////////////////////////////////

void LedDisplay::show(string title, float countdownDuration) {
    title1.show(title, 1);
    this->title = title;
    showCountdown = (countdownDuration > 0);
    countdown->start(countdownDuration);
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
