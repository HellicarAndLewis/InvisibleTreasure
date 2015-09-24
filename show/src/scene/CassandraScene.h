//
//  CassandraScene.h
//  show
//
//  Created by Chris Mullany on 05/09/2015.
//
//

#pragma once
#include "ofMain.h"
#include "SceneBase.h"

class CassandraScene : public SceneBase {
public:
    
    CassandraScene();
    
    void setup();
    void update();
    void draw();
    void play(int i);
    void stop();
    void setupGui();
    
protected:
private:
    
    // gui
    ofParameterGroup titleGroup;
    ofParameter<string> title;
    ofParameter<string> audience;
    ofParameter<string> welcome;
    ofParameter<string> rabbitDisappointed;
    ofParameter<string> rabbitBoss;
    ofParameter<string> boss;
    // timers
    ofParameterGroup timerGroup;
    ofParameter<int> timerIntro;
    ofParameter<int> timerCassandra;
    ofParameter<int> timerMain;
    ofParameter<int> timerOutro;
};