 //
//  Message.cpp
//  show
//
//  Created by Chris Mullany on 16/09/2015.
//
//

#include "Message.h"

Message::Message() {
    timeIn = 1;
    timeHold = 5;
    timeOut = 1;
    colour.set(255);
}

void Message::setup(string fontPath, int fontSize) {
    textBlock.init(fontPath, fontSize);
    textBlock.wrapTextX(ofGetWidth());
}

void Message::update() {
    Sequencable::update();
}

void Message::draw() {
    draw(0, 0);
}

void Message::draw(int x, int y) {
    if (state == INACTIVE) return;
    ofPushStyle();
    
    /*
    ofSetColor(255, 255, 0);
    ofCircle(x, y, 4);
    ofSetColor(200, 0, 0);
    ofRectangle bounds;
    bounds.set(x - (getWidth()/2), y, getWidth(), getHeight());
    ofRect(bounds);
    */
    
    // fade in/out
    float alpha = 0;
    if (state == INTRO) alpha = 255 * progress;
    else if (state == OUTRO) alpha = 255 * (1-progress);
    else if (state == INTERACTIVE) alpha = 255;
    if (getHeight() < 1) alpha = 0;
    
    // HACK: there's an exceptional circumstance in 3 subscenes
    // in which "Welcome" appears 6 times
    // then "Welcome?" once and repeats ad infinitum
    // altough not ideal, this was the lesser of several evils
    if (loopNum % 7 == 0) textBlock.setText(messageString + "?");
    else textBlock.setText(messageString);
    
    textBlock.setColor(colour.r, colour.g, colour.b, alpha);
    textBlock.wrapTextX(maxWidth);
    textBlock.drawCenter(x, y);
    
    ofPopStyle();
}

//////////////////////////////////////////////////////////////////////////////////
// public
//////////////////////////////////////////////////////////////////////////////////
void Message::show(Params params) {
    show(params.message, params.timeIn, params.timeHold, params.timeOut, params.loop);
}

void Message::show(string message, float timeIn, float timeHold, float timeOut, bool loop) {
    this->messageString = message;
    this->timeIn = timeIn;
    this->timeHold = timeHold;
    this->timeOut = timeOut;
    this->loop = loop;
    textBlock.setText(messageString);
    textBlock.setColor(colour.r, colour.g, colour.b, 0);
    textBlock.wrapTextX(maxWidth);
    Sequencable::play();
}

void Message::hide() {
    Sequencable::stop();
}

float Message::getWidth(){
    return textBlock.getWidth();
}

float Message::getHeight(){
    return textBlock.getHeight();
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
