//
//  Message.h
//  show
//
//  Created by Chris Mullany on 16/09/2015.
//
//

#pragma once
#include "ofMain.h"
#include "ofxTextSuite.h"
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
        
        Params(string message, float timeIn=0, float timeHold=-1, float timeOut=0, bool loop=false) :
        message(message), timeIn(timeIn), timeHold(timeHold), timeOut(timeOut), loop(loop){}
        
        Params(){}
    };
    
    Message();
	void setup(string fontPath, int fontSize);
	void update();
    void draw();
    void draw(int x, int y);
    
    void show(Params params);
    void show(string message, float timeIn=0, float timeHold=-1, float timeOut=0, bool loop=false);
    void hide();
    float getWidth();
    float getHeight();
    
    ofxTextBlock textBlock;
    TextBlockAlignment alignment;  //constants for controlling state
    string messageString;
    ofColor colour;
    int maxWidth;

protected:
private:
    
};