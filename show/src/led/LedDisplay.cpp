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
    title1.setup("fonts/led_board-7.ttf", 80);
    title2.setup("fonts/led_board-7.ttf", 80);
    title1.colour.set(226, 148, 57);
    title2.colour.set(226, 148, 57);
    
    ofAddListener(title1.stateChangeEvent, this, &LedDisplay::onTitleStateChange);
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
    title1.maxWidth = getDisplayRect().width * 0.9;
    title2.maxWidth = getDisplayRect().width * 0.9;
    
    display->begin();
    ofClear(39, 36, 37, 255);
    // title 1
    int stringH = title1.getHeight();
    title1.draw(getDisplayRect().getCenter().x, getDisplayRect().getHeight()*0.3 - stringH/2);
    // title 2  / countdown
    stringH = title2.getHeight();
    title2.draw(getDisplayRect().getCenter().x, getDisplayRect().getHeight()*0.6);
    ofSetColor(255);
    display->end();
}

//////////////////////////////////////////////////////////////////////////////////
// public
//////////////////////////////////////////////////////////////////////////////////

// TODO: expand to allow queued messages
// show(params).show(params) etc
// LED params are Message params and float countdown
// keep vector<LED::Params>, when messages ends, pop the next and show it
//
void LedDisplay::queue(Params params) {
    paramsQueue.push_back(params);
}


void LedDisplay::playQueue() {
    if (paramsQueue.size() > 0) {
        Params params = paramsQueue.front();
        show(params);
        paramsQueue.erase(paramsQueue.begin());
    }
}

void LedDisplay::show(Params params) {
    if (!params.justCountdown) {
        title1.show(params.messageParams);
        this->title = params.messageParams.message;
    }
    if (params.countdownDuration > 0) {
        title2.show("");
        showCountdown = true;
        countdown->start(params.countdownDuration);
    }
}

void LedDisplay::show(string title, float countdownDuration) {
    title1.show(title);
    this->title = title;
    title2.show("");
    showCountdown = (countdownDuration > 0);
    countdown->start(countdownDuration);
}

void LedDisplay::hide() {
    paramsQueue.clear();
    title1.hide();
    title2.hide();
}
//////////////////////////////////////////////////////////////////////////////////
// protected
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
// private
//////////////////////////////////////////////////////////////////////////////////

void LedDisplay::onTitleStateChange(Sequencable::State & state) {
    if (state == Sequencable::INACTIVE) {
        ofLogVerbose() << "LedDisplay::onTitleStateChange: INACTIVE, play next message";
        playQueue();
    }
}

//////////////////////////////////////////////////////////////////////////////////
// custom event handlers
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
// oF event handlers
//////////////////////////////////////////////////////////////////////////////////
