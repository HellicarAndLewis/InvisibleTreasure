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
#include "ProjectionManager.h"

class DisplayManager : public GuiableBase {
public:
    
    struct Display {
        ofFbo in;
        ofParameter<ofVec2f> sizeIn;
        ofParameterGroup params;
        
        void refreshFbos() {
            in.allocate((int)sizeIn.get().x, (int)sizeIn.get().y);
            in.begin();
            ofClear(0);
            in.end();
        }
        
        bool sizeChanged() {
            if (in.getWidth() == sizeIn.get().x && in.getHeight() == sizeIn.get().y)
                return false;
            else return true;
        }
        
        void draw(float x=0, float y=0, float scale = 1.0f) {
            in.draw(x, y, in.getWidth()*scale, in.getHeight()*scale);
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
    void drawGui();
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
    ofParameter<bool> isCalibrating;
    
    ofParameterGroup displaySizes;
    

protected:  
private:
    
    void onScaleToWindow(bool& scale);
    void allocateProjectorsFbo();
    
    ProjectionManager projectionManager;
};