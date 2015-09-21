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
#include "Displayable.h"
#include "Countdown.h"

class LedDisplay : public Displayable {
public:
    LedDisplay();
	void setup(Countdown * countdown);
	void update();
    void draw();
    void show(string title, float countdownDuration=-1.0f);
    
    Message title1;
    Message title2;
    string title;
    
    bool showCountdown;
    Countdown * countdown;
    
protected:
private:
    
};