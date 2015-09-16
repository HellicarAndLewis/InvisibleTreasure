//
//  AppModel.h
//  show
//
//  Created by Chris Mullany on 03/09/2015.
//
//

#pragma once
#include "ofMain.h"

// App level properties and states
//
class AppModel {
public:
    
    AppModel();
	void setup();
	void update();
    
    enum Mode {MASTER, SLAVE, WINDOW} mode;
    string modeString;
    
    int windowId;
    
    ofEvent<Mode> modeChangeEvent;
    
    void setMode(string modeString) {
        this->modeString = modeString;
        if (modeString == "MASTER") mode = MASTER;
        else if (modeString == "SLAVE") mode = SLAVE;
        else mode = WINDOW;
        ofNotifyEvent(modeChangeEvent, mode, this);
    }

protected:  
private:
    
};