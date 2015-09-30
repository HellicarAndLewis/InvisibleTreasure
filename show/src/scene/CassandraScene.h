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
#include "ofxImageSequence.h"
#include "Message.h"

class CassandraScene : public SceneBase {
public:
    
    enum Mode {
        IDLE, RECORD_CASSANDRA, RECORD_MAIN, PLAYBACK
    } mode;
    
    CassandraScene();
    
    void setup();
    void update();
    void draw();
    void play(int i);
    void stop();
    void setupGui();
    
protected:
private:
    
    void setMode(Mode mode);
    void prepareRecordingDir(string path);
    
    ofxImageSequence sequenceCassandra;
    ofxImageSequence sequenceMain;
    int fileCount;
    int indexCassandra;
    int indexMain;
    float playbackTime;
    float windowTimer;
    Message timer;
    
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
    
    // LX cues
    ofParameter<int> lxCueIntro;
    ofParameter<int> lxCuePlayback;
    ofParameter<int> lxCueOutro;
    ofParameterGroup lxCueGroup;
    
    // Sound cues
    ofParameter<int> soundCueIntro;
    ofParameter<int> soundCueName;
    ofParameter<int> soundCueCassandra;
    ofParameter<int> soundCueTimePassing;
    ofParameter<int> soundCuePlayback;
    ofParameter<int> soundCueOutro;
    ofParameterGroup soundCueGroup;
};