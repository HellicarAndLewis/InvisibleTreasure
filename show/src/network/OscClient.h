//
//  OscClient.h
//  show
//
//  Created by Chris Mullany on 02/09/2015.
//
//

#pragma once
#include "ofMain.h"
#include "ofxOsc.h"
#include "GuiableBase.h"
#define NUM_MSG_STRINGS 20

class OscClient : public GuiableBase {
public:
    
    struct CueParams {
        ofParameter<float> lightCue;
        ofParameter<int> lightList;
        ofParameter<int> soundCue;
        ofParameterGroup params;
        ofParameterGroup setup(string name) {
            params.setName(name);
            params.add(lightCue.set("light cue", 0, 0, 40));
            params.add(lightList.set("light list", 1, 1, 2));
            params.add(soundCue.set("sound cue", 0, 0, 40));
            return params;
        }
    };
    
    struct VolumeEventArgs {
        float volume;
        int windowId;
        VolumeEventArgs(float volume, int windowId) {
            this->volume = volume;
            this->windowId = windowId;
        }
    };
    
    OscClient();
	void setup(int id);
	void update();
	void draw();
	
	void keyPressed(int key);
    
    void setupGui();
    void sendPlayScene(int id);
    void sendPlaySubScene(int id);
    void sendPresence(string areaName, int count);
    void sendVolume(float volume, int windowId);
    void sendVolumeTrigger(int windowId);
    
    void sendLightSoundCue(CueParams cue);
    void sendLightingCue(float cue, float list = 1);
    void sendSoundCue(float cue);
    void sendSoundVolume(float id, float volume);
    
    ofEvent<int> playSceneEvent;
    ofEvent<int> playSubSceneEvent;
    ofEvent<VolumeEventArgs> volumeEvent;
    ofEvent<VolumeEventArgs> volumeTriggerEvent;
    
protected:  
private:
    int id;
    
    ofxOscSender sender;
    ofxOscReceiver receiver;
    int current_msg_string;
    string msg_strings[NUM_MSG_STRINGS];
    float timers[NUM_MSG_STRINGS];
    
    ofParameter<string> info;
    ofParameter<string> sendAddress;
    ofParameter<string> sendPort;
    ofParameter<string> receivePort;
};