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

class DisplayManager : public GuiableBase {
public:
    
    struct Display {
        ofFbo in;
        ofFbo out;
        ofParameter<ofVec2f> sizeIn;
        ofParameter<ofVec2f> sizeOut;
        ofParameterGroup params;
        
        void refreshFbos() {
            in.allocate(sizeIn.get().x, sizeIn.get().y);
            in.begin();
            ofClear(0);
            in.end();
            out.allocate(sizeOut.get().x, sizeOut.get().y);
            out.begin();
            ofClear(0);
            out.end();
        }
        
        bool sizeChanged() {
            if (in.getWidth() == sizeIn.get().x && in.getHeight() == sizeIn.get().y)
                return false;
            else return true;
        }
        
        void draw(float x=0, float y=0, float scale = 1.0f) {
            in.draw(x, y, in.getWidth()*scale, in.getHeight()*scale);
        }
        
        void drawOutput(float x=0, float y=0, float scale = 1.0f) {
            out.draw(x, y, out.getWidth()*scale, out.getHeight()*scale);
        }
        
        void begin() {
            in.begin();
        }
        void end() {
            in.end();
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
    
    ofImage testPattern;
    // TODO: add params for drawing options?
    // draw in, draw out, draw blended, etc
    ofParameter<bool> scaleToWindow;
    ofParameter<bool> drawOutput;
    ofParameter<bool> drawTestPattern;
    ofParameter<int> activeDisplay;
    //ofParameter<bool> drawBlended;
    

protected:  
private:
    void onScaleToWindow(bool& scale);
    
};