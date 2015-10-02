//
//  ProjectionManager.h
//  show
//
//  Created by Chris Mullany on 29/09/2015.
//
//

#pragma once
#include "ofMain.h"
#include "GuiableBase.h"
#include "ofxProjectorBlend.h"
#define OFFSET_MAX 100

//
// Calibration for the four master projectors
// Uses ofxProjectorBlend for 2x2 edge blending
// Simple quad warping and texture coordinates control
//
class ProjectionManager : public GuiableBase  {
public:
    
    
    //////////////////////////////////////////////////////////////////////////////////
    // Projection
    //////////////////////////////////////////////////////////////////////////////////
    // wrapper for settings for a single projector image
    // provides control over each corner point for the texture input and the quad output
    // rects are calculated by dividing the overall projection FBO into 4 (2x2)
    // offsets are GUI configurable to allow each corner of the rect to be altered
    struct Projection {
        ofRectangle rectIn;
        ofRectangle rectOut;
        ofParameter<ofVec2f> rectInOffsets[4];
        ofParameter<ofVec2f> rectOutOffsets[4];
        ofParameterGroup group;
        
        ofParameterGroup set(string name) {
            group.setName(name);
            // rect in offsets (texture coordinates)
            group.add(rectInOffsets[0].set("IN btm left", ofVec2f(), ofVec2f(-OFFSET_MAX, -OFFSET_MAX), ofVec2f(OFFSET_MAX, OFFSET_MAX)));
            group.add(rectInOffsets[1].set("IN top left", ofVec2f(), ofVec2f(-OFFSET_MAX, -OFFSET_MAX), ofVec2f(OFFSET_MAX, OFFSET_MAX)));
            group.add(rectInOffsets[2].set("IN top right", ofVec2f(), ofVec2f(-OFFSET_MAX, -OFFSET_MAX), ofVec2f(OFFSET_MAX, OFFSET_MAX)));
            group.add(rectInOffsets[3].set("IN btm right", ofVec2f(), ofVec2f(-OFFSET_MAX, -OFFSET_MAX), ofVec2f(OFFSET_MAX, OFFSET_MAX)));
            // rect out offsets (quad vertices)
            group.add(rectOutOffsets[0].set("OUT btm left", ofVec2f(), ofVec2f(-OFFSET_MAX, -OFFSET_MAX), ofVec2f(OFFSET_MAX, OFFSET_MAX)));
            group.add(rectOutOffsets[1].set("OUT top left", ofVec2f(), ofVec2f(-OFFSET_MAX, -OFFSET_MAX), ofVec2f(OFFSET_MAX, OFFSET_MAX)));
            group.add(rectOutOffsets[2].set("OUT top right", ofVec2f(), ofVec2f(-OFFSET_MAX, -OFFSET_MAX), ofVec2f(OFFSET_MAX, OFFSET_MAX)));
            group.add(rectOutOffsets[3].set("OUT btm right", ofVec2f(), ofVec2f(-OFFSET_MAX, -OFFSET_MAX), ofVec2f(OFFSET_MAX, OFFSET_MAX)));
            return group;
        }
        
        void draw() {
            // draw a quad with 4 vertices and 4 texture coordinates
            // each point can be offset using GUI controls
            glBegin(GL_QUADS);
            // bottom left
            drawTexCoord(rectIn.getBottomLeft(), rectInOffsets[0]);
            drawVertex( rectOut.getBottomLeft(), rectOutOffsets[0]);
            // top left
            drawTexCoord(rectIn.getTopLeft(), rectInOffsets[1]);
            drawVertex( rectOut.getTopLeft(), rectOutOffsets[1]);
            // top right
            drawTexCoord(rectIn.getTopRight(), rectInOffsets[2]);
            drawVertex( rectOut.getTopRight(), rectOutOffsets[2]);
            // bottom right
            drawTexCoord(rectIn.getBottomRight(), rectInOffsets[3]);
            drawVertex( rectOut.getBottomRight(), rectOutOffsets[3]);
            glEnd();
        }
        
        void drawTexCoord(ofVec2f basePoint, ofVec2f offsetPoint) {
            float x = basePoint.x + offsetPoint.x;
            float y = basePoint.y + offsetPoint.y;
            glTexCoord2f(x, y);
        }
        
        void drawVertex(ofVec2f basePoint, ofVec2f offsetPoint) {
            float x = ofClamp(basePoint.x + offsetPoint.x, rectOut.getLeft(), rectOut.getRight());
            float y = ofClamp(basePoint.y + offsetPoint.y, rectOut.getTop(), rectOut.getBottom());
            glVertex3f(x, y, 0);
        }
        
        // This just draws a nice block colour so you can identify projectors!
        void drawCalibration(ofColor colour) {
            ofPushStyle();
            ofSetColor(colour, 100);
            draw();
            ofPopStyle();
        }
    };
    
    
    //////////////////////////////////////////////////////////////////////////////////
    // Projection Manager
    //////////////////////////////////////////////////////////////////////////////////
    ProjectionManager();
    
	void setup(int w=1024, int h=768);
	void update();
	void draw(int x, int y, float scale);
    
    void beginTop();
    void endTop();
    void beginBottom();
    void endBottom();
    void setupGui();
    
    int width;
    int height;
    
    // Edge blending
    // We need three:
    // we blend two images horizontally
    // then blend the composite images vertically
    ofxProjectorBlend blender[3];
    
    // GUI controls for edge blending
    ofParameter<float> gamma;
    ofParameter<float> luminance;
    ofParameter<float> power;
    ofParameter<bool> showBlend;
    ofParameterGroup group;
    
    // Four projection structs, one for each projector
    Projection projection[4];

protected:
private:
    
};