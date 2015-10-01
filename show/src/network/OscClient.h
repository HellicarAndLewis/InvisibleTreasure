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
        int lightList = 1;
        ofParameter<int> lightCue;
        ofParameter<int> soundCue;
        ofParameterGroup params;
        ofParameterGroup setup(string name, int lightCue=0, int soundCue=0) {
            params.setName(name);
            params.add(this->lightCue.set("light cue", lightCue, 0, 40));
            params.add(this->soundCue.set("sound cue", soundCue, 0, 40));
            return params;
        }
    };
    
    struct CueWithListParams {
        ofParameter<int> lightCue;
        ofParameter<int> lightList;
        ofParameter<int> soundCue;
        ofParameterGroup params;
        ofParameterGroup setup(string name, int lightCue = 0, int lightList = 2, int soundCue = 0) {
            params.setName(name);
            params.add(this->lightCue.set("light cue", lightCue, 0, 40));
            params.add(this->lightList.set("light list", lightList, 0, 5));
            params.add(this->soundCue.set("sound cue", soundCue, 0, 40));
            return params;
        }
    };
    
    struct CueWithFloatParams {
        int lightList = 1;
        ofParameter<float> lightCue;
        ofParameter<int> soundCue;
        ofParameterGroup params;
        ofParameterGroup setup(string name, float lightCue=0.5, int soundCue=0) {
            params.setName(name);
            params.add(this->lightCue.set("light cue", lightCue, 0, 40));
            params.add(this->soundCue.set("sound cue", soundCue, 0, 40));
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
    void sendLightSoundCue(CueWithListParams cue);
    void sendLightSoundCue(CueWithFloatParams cue);
    
    void sendLightingCue(float cue, float list = 1);
    void sendSoundCue(float cue);
    void sendSoundVolume(float id, float volume);
    
    void sendKill();
    
    ofxButton killButton;
    ofEvent<int> playSceneEvent;
    ofEvent<int> playSubSceneEvent;
    ofEvent<VolumeEventArgs> volumeEvent;
    ofEvent<VolumeEventArgs> volumeTriggerEvent;
    
protected:  
private:
    int id;
    bool isConnected;
    
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