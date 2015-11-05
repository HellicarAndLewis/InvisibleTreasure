//
//  PaintboxScene.h
//  show
//
//  Created by Chris Mullany on 05/09/2015.
//
//

#pragma once
#include "ofMain.h"
#include "SceneBase.h"
#include "ImageElement.h"
//#include "Paintbrush.h"

#define NUM_BRUSHES 56
#define NUM_ERASERS 6

class paintBrush {
public:
    ofColor col;
    ofVec2f loc;
    ofVec2f vel;
    
    ofRectangle bounds;
    
    float inverseDamping;
    
    float scale;
    float noiseCounter;
    float noiseStep;
    
    void setup(int width, int height) {
        loc = ofVec2f(ofRandom(width), ofRandom(height));
        vel = ofVec2f(0, 0);
        bounds = ofRectangle(0, 0, width, height);
        scale = ofRandom(2, 5);
        noiseCounter = ofRandom(1000);
        noiseStep = 0.1;
        int min = 100;
        int max = 255;
        col = ofColor(ofRandom(min, max), ofRandom(min, max), ofRandom(min, max));
        inverseDamping = 1.0;
        
    };
    void update(FlowFinder* flowFinder) {
        float mappedX = ofMap(loc.x, bounds.getX(), bounds.getWidth(), 1, flowFinder->image.width-1, true);
        float mappedY = ofMap(loc.y, bounds.getY(), bounds.getHeight(), 1, flowFinder->image.height-1, true);
        vel = flowFinder->flow.getAverageFlowInRegion(ofRectangle(mappedX, mappedY, 1, 1));
        loc += vel*inverseDamping;
    };
    void draw(ofImage* brushImage) {
        ofSetColor(col.r, col.g, col.b);
        ofPushMatrix();
        ofTranslate(loc.x, loc.y);
        ofRotateZ(ofRandom(360));
        //float scale = ofClamp((rect.width)/brushImage->width, 0.5, 4);
//        scale *= ofNoise(noiseCounter);
//        noiseCounter += noiseStep;
        int w = brushImage->width*scale * ofNoise(noiseCounter);
        int h = brushImage->height*scale * ofNoise(noiseCounter);
        brushImage->draw(-w/2, -h/2, w, h);
        ofPopMatrix();
    };
    void checkEdges() {
        if(loc.x > bounds.getWidth()) {
            loc.x = bounds.getWidth();
            vel.x *= -1;
        }
        else if (loc.x < bounds.getX()) {
            loc.x = bounds.getX();
            vel.x *= -1;
        }
        if(loc.y > bounds.getHeight()) {
            loc.y = bounds.getHeight();
            vel.y *= -1;
        }
        else if (loc.y < bounds.getY()) {
            loc.y = bounds.getY();
            vel.y *= -1;
        }
    }
};

// TODO: blob tracking, draw lines with people
//
class PaintboxScene : public SceneBase {
public:
    
    enum Mode {
        INACTIVE, LINES, ERASER, FADE
    } mode;
    
    PaintboxScene();
    
    void setup();
    void update();
    void draw();
    void drawMasterProjection();
    void play(int i);
    void stop();
    void setupGui();
    
protected:
private:
    void setMode(Mode state);
    void clearCanvas();
    ofColor getColour(int blob);
    bool getIsActive();
    bool paintBrushesSetup;
    
    ofFbo canvas;
    map<int, ofColor> colours;
    ofImage brushImage;
    float totalBlobArea;
    
    paintBrush brushes[NUM_BRUSHES];
    paintBrush erasers[NUM_ERASERS];
    
    ImageElement imageElement;
    ofParameter<float> imageDelay;
    
    ofParameter<float> minAreaEraser;
    ofParameter<float> minAreaBlack;
    
    ofParameter<float> brushSpeed;
    ofParameter<float> eraserSpeed;
    
    // gui
    ofParameterGroup titleGroup;
    ofParameter<string> title;
    ofParameter<string> sleeping;
    ofParameter<string> back;
    ofParameter<string> bonusTime;
    ofParameter<string> bonusGame;
    ofParameter<string> goingDark;
    ofParameter<string> pleaseEnjoy;
    
    // timers
    ofParameterGroup timerGroup;
    ofParameter<int> timerIntro;
    ofParameter<int> timerLines;
    ofParameter<int> timerErase;
    ofParameter<int> timerBack;
    ofParameter<int> timerBonus;
    ofParameter<int> timerOutro;
    
    // LX cues
    ofParameter<int> lxCueIntro;
    ofParameter<int> lxCueOutro;
    ofParameterGroup lxCueGroup;
    
    // Sound cues
    ofParameter<int> soundCueSoundScape;
    ofParameter<int> soundCueSoundScape2;
    ofParameter<int> soundCueOutro;
    ofParameterGroup soundCueGroup;
    
};