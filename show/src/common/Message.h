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
#include "Displayable.h"

class Message : public Sequencable, public Displayable {
public:
    
    struct Params {
        string message;
        float timeIn;
        float timeHold=-1;
        float timeOut=-1;
        bool loop=false;
    };
    
    Message();
	void setup(string fontPath, int fontSize);
	void update();
    void draw();
    void draw(int x, int y);
    
    void show(string message, float timeIn=1, float timeHold=-1, float timeOut=1, bool loop=false);
    void hide();
    float getWidth();
    float getHeight();
    
    ofxFTGLSimpleLayout layout;
    string messageString;
    ofColor colour;

protected:
private:
    ofxFTGLFont* font;
    
};