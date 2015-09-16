//
//  LedDisplay.h
//  show
//
//  Created by Chris Mullany on 02/09/2015.
//
//

#pragma once
#include "ofMain.h"
#include "Message.h"

class LedDisplay {
public:
    LedDisplay();
	void setup();
	void update();
    void draw();
    void show(string title, float countdown=-1.0f);
    
    Message message;
    string title;
    
    bool showCountdown;
    int countdownStart;
    int countdownDuration;
    string countdownString;
    
protected:  
private:
    
};