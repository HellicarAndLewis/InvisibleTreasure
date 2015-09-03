//
//  GuiableBase.h
//  show
//
//  Created by Chris Mullany on 03/09/2015.
//
//

#pragma once
#include "ofMain.h"
#include "ofxGui.h"

class GuiableBase {
public:
    virtual void setupGui() = 0;
    virtual void drawGui() {
        if (guiEnabled) panel.draw();
    }
    ofxPanel panel;
    ofParameterGroup parameters;
    string guiName = "Guiable";
    ofParameter<bool> guiEnabled = false;
    vector<GuiableBase*> guiables;
};