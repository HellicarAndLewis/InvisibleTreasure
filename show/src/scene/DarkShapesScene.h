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
#define DARK_SHAPES_TIMER_COUNT 6

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
            INTRO=0, PLAY=2, FAIL=3, PASS=4, INACTIVE=5
        } state;
        
        ShapeGame(ShapeRenderer::ShapeMode shapeMode, int startScene, int endScene) {
            this->shapeMode = shapeMode;
            this->startScene = startScene;
            this->endScene = endScene;
            state = INACTIVE;
            if (shapeMode == ShapeRenderer::CIRCLE) label = "Circle";
            else if (shapeMode == ShapeRenderer::RECTANGLE) label = "Rectangle";
            else if (shapeMode == ShapeRenderer::TRIANGLE) label = "Triangle";
            else if (shapeMode == ShapeRenderer::STAR) label = "Star";
        }
        void updateSceneI() {
            sceneI = getSceneForState(state);
        }
        int getSceneForState(State state) {
            if (state == INTRO) return endScene - 3;
            else if (state == PLAY) return endScene - 2;
            else if (state == FAIL) return endScene - 1;
            else if (state == PASS) return endScene;
            else return -1;
        }
        void setScene(int scene) {
            int outroOffset = endScene - scene;
            if (outroOffset == 0) state = PASS;
            else if (outroOffset == 1) state = FAIL;
            else if (outroOffset == 2) state = PLAY;
            else if (outroOffset == 3) state = INTRO;
            updateSceneI();
        }
        State getNextState() {
            State nextState;
            if (state == INTRO) nextState = PLAY;
            else if (state == PLAY) nextState = FAIL;
            else if (state == FAIL) {
                attemptNum++;
                if (attemptNum > MAX_ATTEMPTS) nextState = INACTIVE;
                else nextState = PLAY;
            }
            else if (state == PASS) nextState = INACTIVE;
            return nextState;
        }
        void success() {
            state = PASS;
            updateSceneI();
        }
        void draw() {
            string s = label + " state: " + getStateString() + " attempt " + ofToString(attemptNum) + "/3";
            ofDrawBitmapStringHighlight(s, 5, 5);
        }
        string getStateString() {
            if (state == INTRO) return "intro";
            else if (state == PLAY) return "play";
            else if (state == FAIL) return "fail";
            else if (state == PASS) return "pass";
            else if (state == INACTIVE) return "inactive";
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
    
    ofParameterGroup timerGroup;
    ofParameter<int> timers[DARK_SHAPES_TIMER_COUNT];
    
    ofxPanel shapesPanel;
    ofxButton shapeSuccess;
    void onSuccess();
    
    vector<ShapeGame> shapeGames;
    ShapeGame * currentShapeGame;
    
    virtual void onCountdownComplete(int& i);
    
};