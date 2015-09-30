//
//  LightboxScene.h
//  show
//
//  Created by Chris Mullany on 02/09/2015.
//
//

#pragma once
#include "ofMain.h"
#include "SceneBase.h"
#include "ImageElement.h"
#define LIGHTBOX_CUE_COUNT 8

class LightboxScene : public SceneBase {
public:
    
    // Hit area is a trigger zone in the space
    // It's defined by a rect which can be set via the GUI
    struct HitArea {
        HitArea(string name, ofVec2f position=ofVec2f(.5, .5), float size=.01) : name(name) {
            this->position.set(name+" pos", position, ofVec2f(0,0), ofVec2f(1,1));
            this->size.set(name+" size", size, 0, 1);
        }
        void update() {
            bool wasTriggered = getIsTriggered();
            smoothed = ofLerp(smoothed, blobCount, 0.1f);
            changed = (getIsTriggered() != wasTriggered);
        }
        bool getIsTriggered() {
            return (smoothed > 0.1);
        }
        bool changed = false;
        bool active = false;
        // area pposition, size and resulting rectangle
        ofParameter<ofVec2f> position;
        ofParameter<float> size;
        ofRectangle rect;
        // meaningful name
        string name = "";
        // volume resulting from blobs in this area
        float volume = 0;
        // blob tracking in this area
        int blobCount = 0;
        int lastBlobCount = 0;
        float smoothed = 0;
    };
    
    // Play mode determines which zones are active
    // and how the trigger zones affect sound volume
    enum PLAY_MODE {
        CENTRE, WALL_1, WALL_2, WALLS_3_4, WAITING, ALL_ZONES
    } playMode;
    
    LightboxScene();
    
	void setup();
	void update();
	void draw();
    void drawMasterScreen();
    void play(int i);
    void stop();
    void setupGui();
    void drawGui();

protected:  
private:
    
    void sendActiveCue();
    void refreshHitAreas();
    OscClient::CueParams getCueForArea(string name);
    
    // gui
    ofParameter<string> title;
    ofParameter<string> waiting;
    ofParameter<string> phase2;
    ofParameter<string> bonusGame;
    // cues: reset, centre, wall1, wall2, wall3, wall4, all, outro
    OscClient::CueParams cues[LIGHTBOX_CUE_COUNT];
    
    bool zonesActive = false;
    bool isHeroActive = false;
    vector<HitArea> hitAreas;
    ImageElement imageElement;
    
};