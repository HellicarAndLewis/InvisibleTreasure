//
//  AppModel.cpp
//  show
//
//  Created by Chris Mullany on 03/09/2015.
//
//

#include "AppModel.h"

void AppModel::setMode(string modeString) {
    this->modeString = modeString;
    if (modeString == "MASTER") mode = MASTER;
    else if (modeString == "SLAVE") mode = SLAVE;
    else mode = WINDOW;
    ofNotifyEvent(modeChangeEvent, mode, this);
}

//////////////////////////////////////////////////////////////////////////////////
// public
//////////////////////////////////////////////////////////////////////////////////

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
