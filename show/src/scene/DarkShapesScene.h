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

// TODO: create a class/struct for each window shape
// inherit from common shapeBase, implement INTRO,OUTRO,ACTIVE,INACTIVE states
//
class DarkShapesScene : public SceneBase {
public:
    
    enum ShapeMode {
        TRIANGLE=0,CIRCLE,SQUARE,SHAPE_MODE_COUNT
    } shapeMode;
    
    DarkShapesScene();
    
	void setup();
	void update();
	void draw();
    
    void play();
    void stop();
    void setupGui();
    void drawGui();
    
    // gui
    ofParameter<int> modeSelector;
    void onShapeModeSelect(int & i);

protected:
private:
    ShapeRenderer shapeRenderer;
    
};