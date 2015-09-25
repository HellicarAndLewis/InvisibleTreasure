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
    title1.colour.set(226, 148, 57);
    title2.colour.set(226, 148, 57);
}

void LedDisplay::update() {
    title1.update();
    title2.update();
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
    // title 1
    int stringW = title1.getWidth();
    title1.draw(getDisplayRect().getCenter().x-title1.getWidth()/2, getDisplayRect().getHeight()*0.3);
    // title 2  / countdown
    stringW = title2.getWidth();
    title2.draw(getDisplayRect().getCenter().x-title2.getWidth()/2, getDisplayRect().getHeight()*0.6);
    ofSetColor(255);
    display->end();
}

//////////////////////////////////////////////////////////////////////////////////
// public
//////////////////////////////////////////////////////////////////////////////////

// TODO: expand to allow queued messages
// show(params).show(params) etc
// LED params are Message params and float countdown
//
void LedDisplay::show(string title, float countdownDuration) {
    title1.show(title, 1);
    this->title = title;
    showCountdown = (countdownDuration > 0);
    countdown->start(countdownDuration);
}

void LedDisplay::hide() {
    title1.hide();
    title2.hide();
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
