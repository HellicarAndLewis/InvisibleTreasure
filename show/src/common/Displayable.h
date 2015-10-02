//
//  Displayable.h
//  show
//
//  Created by Chris Mullany on 17/09/2015.
//
//

#pragma once
#include "ofMain.h"
#include "DisplayManager.h"

//
// Inherit from this if you'll only ever be drawing to a single display
// only really used by LED Display
//
class Displayable {
public:
    Displayable() {
        display = NULL;
    }
    
    ofRectangle getDisplayRect() {
        ofRectangle rect;
        if (display != NULL) {
            rect.set(0, 0, display->in.getWidth(), display->in.getHeight());
        }
        return rect;
    }
    
    void setDisplay(DisplayManager::Display * display) {
        this->display = display;
    }
    
protected:
    DisplayManager::Display * display;
    
private:
    
};