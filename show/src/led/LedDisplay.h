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

//
// LED style display
// messages can be displayed instantly
// or messages can be queued. To play a queue of messages:
//  - call queue(Params params) several times, specifying the time to display each message for
//  - call playQueue()
//
class LedDisplay : public Displayable {
public:
    
    // Params define the state and behaviour of the LED display
    // this includes text and animation timings
    struct Params {
        Message::Params messageParams;
        float countdownDuration=-1.0f;
        bool justCountdown = false;
        
        Params(Message::Params messageParams, float countdownDuration=-1.0f) :
        messageParams(messageParams),
        countdownDuration(countdownDuration){}
        
        Params(string message, float timeIn=0, float timeHold=-1, float timeOut=0, bool loop=false, float countdownDuration=-1.0f) :
        messageParams(message, timeIn, timeHold, timeOut, loop),
        countdownDuration(countdownDuration) {
        }
        
        Params(float countdownDuration) :
        countdownDuration(countdownDuration),
        justCountdown(true) {
        }
    };
    
    LedDisplay();
	void setup(Countdown * countdown);
	void update();
    void draw();
    // queuing
    void queue(Params params);
    void playQueue();
    // show and hide
    void show(Params params);
    void show(string title, float countdownDuration=-1.0f);
    void hide();
    
    Message title1;
    Message title2;
    string title;
    
    bool showCountdown;
    Countdown * countdown;
    vector<Params> paramsQueue;
    
protected:
private:
    void onTitleStateChange(Sequencable::State & state);
    
};