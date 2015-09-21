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
#define LIGHTBOX_CUE_COUNT 8

class LightboxScene : public SceneBase {
public:
    
    struct HitArea {
        HitArea(string name, ofVec2f position=ofVec2f(.5, .5), float size=.01) : name(name) {
            this->position.set(name+" pos", position, ofVec2f(0,0), ofVec2f(1,1));
            this->size.set(name+" size", size, 0, 1);
        }
        void update() {
            changed = ( (lastBlobCount==0 && blobCount>0) || (lastBlobCount>0 && blobCount==0));
            lastBlobCount = blobCount;
        }
        bool changed = false;
        bool active = false;
        ofParameter<ofVec2f> position;
        ofParameter<float> size;
        ofRectangle rect;
        string name;
        int id;
        int soundId;
        int blobCount = 0;
        int lastBlobCount = 0;
    };
    
    enum PLAY_MODE {
        CENTRE, WALL_1, WALL_2, WALLS_3_4, ALL_ZONES
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
    // cues: reset, centre, wall1, wall2, walls3&4, all, outro
    OscClient::CueParams cues[LIGHTBOX_CUE_COUNT];
    
    vector<HitArea> hitAreas;
    
};