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

//
// Display manager
// Slave and Master have multiple displays (screen + projectors)
// this class manages FBOs that relate to a display
// these FBOs can then be scaled into a single screen for testing and dev
// or they can be 100% resulting in output across the external projections
//
class DisplayManager : public GuiableBase {
public:
    
    
    //////////////////////////////////////////////////////////////////////////////////
    // Display
    //////////////////////////////////////////////////////////////////////////////////
    // Wrapper for each display
    // contains an FBO for the display
    // and a target display size (x=width, y=height)
    struct Display {
        ofFbo in;
        ofParameter<ofVec2f> sizeIn;
        
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
    
    
    
    //////////////////////////////////////////////////////////////////////////////////
    // Display Manager
    //////////////////////////////////////////////////////////////////////////////////
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
    
    // Displays for all sreens and projectors
    Display slaveScreen;
    Display slaveProjection;
    Display masterScreen;
    Display masterProjection;
    ofImage testPattern;
    
    // GUI
    ofParameter<bool> scaleToWindow;
    ofParameter<bool> drawOutput;
    ofParameter<bool> drawTestPattern;
    ofParameter<int> activeDisplay;
    ofParameter<bool> isCalibrating;
    ofParameter<bool> identifyProjectors;
    ofParameterGroup displaySizes;
    
protected:  
private:
    
    void onScaleToWindow(bool& scale);
    ofTrueTypeFont font;
    ProjectionManager projectionManager;
};