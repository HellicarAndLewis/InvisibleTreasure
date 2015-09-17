//
//  DisplayManager.h
//  show
//
//  Created by Chris Mullany on 17/09/2015.
//
//

#pragma once
#include "ofMain.h"
#include "GuiableBase.h"

class DisplayManager : GuiableBase {
public:
    
    struct Display {
        ofFbo in;
        ofFbo out;
        ofParameter<bool> useWindowSize;
        ofParameter<ofVec2f> pos;
        ofParameter<ofVec2f> sizeIn;
        ofParameter<ofVec2f> sizeOut;
        ofParameterGroup params;
        
        void refreshFbos() {
            in.allocate(sizeIn.get().x, sizeOut.get().y);
            out.allocate(sizeIn.get().x, sizeOut.get().y);
        }
        
        bool getSizesEqual() {
            if (in.getWidth() == out.getWidth() && in.getHeight() == out.getHeight())
                return true;
            else return false;
        }
        
        void draw() {
            in.draw(pos.get());
        }
    };
    
    DisplayManager();
	void setup();
	void update();
	void draw();
    void drawSlave();
    void drawMaster();
    void setupGui();
    void refreshFbos();
    
	void windowResized(int w, int h);
    
    Display slaveScreen;
    Display slaveProjection;
    Display masterScreen;
    Display masterProjection;
    
    // TODO: add params for drawing options?
    // draw in, draw out, draw blended, etc

protected:  
private:
    
};