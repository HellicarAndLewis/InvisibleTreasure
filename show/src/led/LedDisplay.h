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
#include "GuiableBase.h"

//
// LED style display
// messages can be displayed instantly
// or messages can be queued. To play a queue of messages:
//  - call queue(Params params) several times, specifying the time to display each message for
//  - call playQueue()
//
class LedDisplay : public Displayable, public GuiableBase {
public:
    
    //////////////////////////////////////////////////////////////////////////////////
    // Params
    //////////////////////////////////////////////////////////////////////////////////
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
    
    //////////////////////////////////////////////////////////////////////////////////
    // LED Display
    //////////////////////////////////////////////////////////////////////////////////
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
    
    void setupGui();
    
    // title1 is the text/title
    Message title1;
    string title;
    // title2 is the countdown
    Message title2;
    
    // pointer to the countdown
    // so we can get the time remaining from it
    bool showCountdown;
    Countdown * countdown;
    
    // params queue for sequencing messages
    vector<Params> paramsQueue;
    
protected:
private:
    
    // listener for title state change
    // so we can show the next message if there is one
    void onTitleStateChange(Sequencable::State & state);
    
    // GUI controls for position
    ofParameter<float> percentYTitle;
    ofParameter<float> percentYCountdown;
    
};