//
//  DancingDarkScene.h
//  show
//
//  Created by Chris Mullany on 05/09/2015.
//
//

#pragma once
#include "ofMain.h"
#include "SceneBase.h"
#define DANCING_TUNE_COUNT 10

class DancingDarkScene : public SceneBase {
public:
    
    struct Tune {
        ofParameter<string> ledTitle;
        ofParameter<int> soundCueNum;
        ofParameter<float> duration;
        ofParameterGroup group;
        ofParameterGroup set(string title, string ledTitle, int duration, int soundCueNum) {
            group.add(this->ledTitle.set("LED title", ledTitle));
            group.add(this->duration.set("duration", duration, -1, 180));
            group.add(this->soundCueNum.set("sound cue", soundCueNum, -1, 100));
            group.setName(title);
            return  group;
        }
    };
    
    DancingDarkScene();
    
    void setup();
    void update();
    void draw();
    void play(int i);
    void stop();
    void setupGui();
    
protected:
private:
    Tune tunes[DANCING_TUNE_COUNT];
    ofParameterGroup tunesGroup;
    
    // LX cues
    ofParameter<int> lxCueBlackout;
    ofParameter<int> lxCueLightsUp;
    ofParameter<int> lxCueOutro;
    ofParameterGroup lxCueGroup;
    
};