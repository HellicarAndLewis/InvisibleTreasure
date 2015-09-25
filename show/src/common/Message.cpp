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
    layout.loadFont(fontPath, fontSize);
    layout.setAlignment(FTGL_ALIGN_LEFT);
}

void Message::update() {
    Sequencable::update();
}

void Message::draw() {
    draw(0, 0);
}

void Message::draw(int x, int y) {
    ofPushStyle();
    if (state == INTRO) ofSetColor(colour, 255 * progress);
    else if (state == OUTRO) ofSetColor(colour, 255 * (1-progress));
    else if (state == INTERACTIVE) ofSetColor(colour, 255);
    else ofSetColor(colour, 0);
    layout.drawString(messageString, x, y + layout.getAscender());
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
    Sequencable::play();
}

void Message::hide() {
    Sequencable::stop();
}

float Message::getWidth(){
    return layout.getStringBoundingBox(messageString, 0, 0).getWidth();
}

float Message::getHeight(){
    float height = layout.getStringBoundingBox(messageString, 0, 0).getHeight();
    return height;
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
