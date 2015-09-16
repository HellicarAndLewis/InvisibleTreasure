//
//  Message.h
//  show
//
//  Created by Chris Mullany on 16/09/2015.
//
//

#pragma once
#include "ofMain.h"
#include "ofxFTGL.h"
#include "Sequencable.h"

class Message : public Sequencable {
public:
    Message();
	void setup(string fontPath, int fontSize);
	void update();
	void draw();
    
    void show(string message, float timeIn, float timeHold=-1, float timeOut=-1, bool loop=false);
    
    ofxFTGLSimpleLayout layout;
    string messageString;
    bool loop;

protected:
private:
    ofxFTGLFont* font;
    
};