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

class LightboxScene : public SceneBase {
public:
    
    struct HitArea {
        HitArea(string name, ofVec2f position=ofVec2f(.5, .5), float size=.01) : name(name) {
            this->position.set(name+" pos", position, ofVec2f(0,0), ofVec2f(1,1));
            this->size.set(name+" size", size, 0, 1);
        }
        ofParameter<ofVec2f> position;
        ofParameter<float> size;
        ofRectangle rect;
        string name;
        int id;
        int blobCount;
    };
    
    LightboxScene();
    
	void setup();
	void update();
	void draw();
    void play(int i);
    void stop();
    void setupGui();
    void drawGui();

protected:  
private:
    vector<HitArea> hitAreas;
    
};