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
        void set(string name, ofVec2f position=ofVec2f(.5, .5), float size=.01) {
            this->name = name;
            this->position.set("pos", position, ofVec2f(-1,-1), ofVec2f(2,2));
            this->size.set("size", size, 0, 2);
            group.add(this->position);
            group.add(this->size);
            group.add(this->soundCueIn.set("sound in", 0, -1, 100));
            group.add(this->soundCueOut.set("sound out", 0, -1, 100));
            group.setName(name);
        }
        void update() {
            bool wasTriggered = getIsTriggered();
            smoothed = ofLerp(smoothed, presence, 0.1f);
            changed = (getIsTriggered() != wasTriggered);
        }
        bool getIsTriggered() {
            return (smoothed > 0.1);
        }
        bool changed = false;
        bool active = false;
        ofRectangle rect;
        // meaningful name
        string name = "";
        // volume resulting from blobs in this area
        float volume = 0;
        // blob tracking in this area
        int presence = 0;
        int blobCount = 0;
        int lastBlobCount = 0;
        float smoothed = 0;
        
        // area pposition, size and resulting rectangle
        ofParameter<ofVec2f> position;
        ofParameter<float> size;
        ofParameter<int> soundCueIn;
        ofParameter<int> soundCueOut;
        ofParameterGroup group;
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
    bool hitTest(HitArea& hit, ofRectangle blob);
    
    // gui
    ofParameter<string> title;
    ofParameter<string> waiting;
    ofParameter<string> phase2;
    ofParameter<string> bonusGame;
    // cues: reset, centre, wall1, wall2, wall3, wall4, all, outro
    OscClient::CueParams cues[LIGHTBOX_CUE_COUNT];
    OscClient::CueParams CenterBlinkingAllOff;
    OscClient::CueParams WaitingIntro;
    OscClient::CueParams SystemReadyIntro;
    
    bool zonesActive = false;
    bool isHeroActive = false;
    HitArea hitAreas[5];
    ImageElement imageElement;
    
};