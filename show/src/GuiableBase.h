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

// Inherit from this to add GUI support
// allows for a nested GUI structure:
//  - add child.paramters to parent's panel or
//  - have a parent toggle a child.panel using child.guiEnabled
//
// override setupGUI and setup paramaters or panel e.g.
//    guiName = "YOUR GUI NAME";
//    panel.setup(guiName, "SETTINGS_NAME.xml");
//    panel.add(thing1);
//    panel.add(thing1);
//    panel.loadFromFile("SETTINGS_NAME.xml.xml");
//
class GuiableBase {
public:
    virtual void setupGui(){};
    virtual void drawGui() {
        if (guiEnabled) panel.draw();
    }
    ofxPanel panel;
    ofParameterGroup parameters;
    string guiName = "Guiable";
    ofParameter<bool> guiEnabled = false;
    vector<GuiableBase*> guiables;
};