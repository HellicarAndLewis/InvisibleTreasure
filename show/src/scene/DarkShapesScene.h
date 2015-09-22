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
        int attemptNum = 1;
        int startScene;
        int endScene;
        int sceneI;
        string label;
        ShapeRenderer::ShapeMode shapeMode;
        enum State {
            INTRO=0, SHAPE=1, PLAY=2, OUTRO=3, INACTIVE=4
        } state;
        
        ShapeGame(ShapeRenderer::ShapeMode shapeMode, int startScene, int endScene) {
            this->shapeMode = shapeMode;
            this->startScene = startScene;
            this->endScene = endScene;
            state = INACTIVE;
            if (shapeMode == ShapeRenderer::CIRCLE) label = "Cirlce";
            else if (shapeMode == ShapeRenderer::RECTANGLE) label = "Rectangle";
            else if (shapeMode == ShapeRenderer::TRIANGLE) label = "Triangle";
            else if (shapeMode == ShapeRenderer::STAR) label = "Star";
        }
        void updateSceneI() {
            if (state == INTRO) sceneI = endScene - 3;
            else if (state == SHAPE) sceneI = endScene - 2;
            else if (state == PLAY) sceneI = endScene - 1;
            else if (state == OUTRO) sceneI = endScene;
        }
        void setScene(int scene) {
            int outroOffset = endScene - scene;
            if (outroOffset == 0) state = OUTRO;
            else if (outroOffset == 1) state = PLAY;
            else if (outroOffset == 2) state = SHAPE;
            else if (outroOffset == 3) state = INTRO;
            updateSceneI();
        }
        State next() {
            if (state == INTRO) state = SHAPE;
            else if (state == SHAPE) state = PLAY;
            else if (state == PLAY) state = OUTRO;
            else if (state == OUTRO) {
                attemptNum++;
                if (attemptNum >= MAX_ATTEMPTS) state = INACTIVE;
                else state = PLAY;
            }
            updateSceneI();
            return state;
        }
        void draw() {
            string s = label + " state: " + ofToString((int)state) + " attempt " + ofToString(attemptNum) + "/3";
            ofDrawBitmapStringHighlight(s, 0, 0, ofColor(0,200,0));
        }
    };
    
    
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
    
    vector<ShapeGame> shapeGames;
    ShapeGame * currentShapeGame;
    
    virtual void onCountdownComplete(int& i);
    
};