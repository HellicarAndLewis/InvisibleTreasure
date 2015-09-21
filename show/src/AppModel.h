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

    enum Mode {MASTER, SLAVE, WINDOW} mode;
    string modeString;
    int id;
    int windowId;
    ofEvent<Mode> modeChangeEvent;
    
    void setMode(string modeString);

protected:  
private:
    
};