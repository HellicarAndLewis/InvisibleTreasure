//
//  DarkShapesScene.h
//  show
//
//  Created by Chris Mullany on 02/09/2015.
//
//

#pragma once
#include "ofMain.h"
#include "SceneBase.h"
#include "ShapeRenderer.h"
#define MAX_ATTEMPTS 3
#define DARK_SHAPES_CUE_COUNT 11

// TODO: cv blob tracking and shape detection
//
class DarkShapesScene : public SceneBase {
public:
    
    struct ShapeGame{
        int attemptNum = 0;
    };
    
    enum ShapeMode {
        TRIANGLE=0,CIRCLE,SQUARE,RECTANGLE,STAR, SHAPE_MODE_COUNT
    } shapeMode;
    
    DarkShapesScene();
    
	void setup();
	void update();
	void draw();
    
    void drawMasterScreen();
    void play(int i);
    void stop();
    void setupGui();
    void drawGui();
    
    // gui
    ofParameter<int> modeSelector;
    void onShapeModeSelect(int & i);

protected:
private:
    ShapeRenderer shapeRenderer;
    // gui
    ofParameter<string> title;
    ofParameter<string> playAgain;
    ofParameter<string> goingDark;
    ofParameter<string> nextLevel;
    // cues: reset, centre, wall1, wall2, wall3, wall4, all, outro
    OscClient::CueParams cues[DARK_SHAPES_CUE_COUNT];
    
};