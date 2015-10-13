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

//
// Master:
// sends OSC to a broadcast address to make other macs play subscenes
// sends OSC to lighting desk software
// sends OSC to sound desk software
// receives mic volume levels from windows
//
// Windows:
// send mic volume to broadcast address
//
// Windows and Slave:
// receive scene play commands
//
class OscClient : public GuiableBase {
public:
    
    
    //////////////////////////////////////////////////////////////////////////////////
    // Sender
    //////////////////////////////////////////////////////////////////////////////////
    // We need three senders: lights, sounds and broadcast
    // Each sender has a GUI group
    struct Sender {
        ofxOscSender client;
        ofParameter<string> address;
        ofParameter<string> port;
        ofParameterGroup group;
        ofParameterGroup setup(string name, string address, string port) {
            group.setName(name);
            group.add(this->address.set("address", address));
            group.add(this->port.set("port", port));
            return group;
        }
        void connect() {
            client.setup(address, ofToInt(port));
        }
    };
    
    
    //////////////////////////////////////////////////////////////////////////////////
    // Sound/Light cue paramaters
    //////////////////////////////////////////////////////////////////////////////////
    // Structs for wrapping up cues: useful for combining sound and light commands
    // Standard/normal is default light list 1, cues are ints
    struct CueParams {
        int lightList = 1;
        ofParameter<int> lightCue;
        ofParameter<int> soundCue;
        ofParameterGroup params;
        ofParameterGroup setup(string name, int lightCue=0, int soundCue=0) {
            params.setName(name);
            params.add(this->lightCue.set("light cue", lightCue, 0, 100));
            params.add(this->soundCue.set("sound cue", soundCue, 0, 100));
            return params;
        }
    };
    
    // With configurable light list
    struct CueWithListParams {
        ofParameter<int> lightCue;
        ofParameter<int> lightList;
        ofParameter<int> soundCue;
        ofParameterGroup params;
        ofParameterGroup setup(string name, int lightCue = 0, int lightList = 2, int soundCue = 0) {
            params.setName(name);
            params.add(this->lightCue.set("light cue", lightCue, 0, 100));
            params.add(this->lightList.set("light list", lightList, 0, 5));
            params.add(this->soundCue.set("sound cue", soundCue, 0, 100));
            return params;
        }
    };
    
    // with a float light cue e.g. for shadows scene light cue 0.5
    struct CueWithFloatParams {
        int lightList = 1;
        ofParameter<float> lightCue;
        ofParameter<int> soundCue;
        ofParameterGroup params;
        ofParameterGroup setup(string name, float lightCue=0.5, int soundCue=0) {
            params.setName(name);
            params.add(this->lightCue.set("light cue", lightCue, 0, 100));
            params.add(this->soundCue.set("sound cue", soundCue, 0, 100));
            return params;
        }
    };
    
    
    //////////////////////////////////////////////////////////////////////////////////
    // Volume event
    //////////////////////////////////////////////////////////////////////////////////
    // Fired when master gets a new volume message from a window
    struct VolumeEventArgs {
        float volume;
        int windowId;
        VolumeEventArgs(float volume, int windowId) {
            this->volume = volume;
            this->windowId = windowId;
        }
    };
    
    
    //////////////////////////////////////////////////////////////////////////////////
    // OSC Client
    //////////////////////////////////////////////////////////////////////////////////
    
    OscClient();
	void setup(int id);
	void update();
	void draw();
	void keyPressed(int key);
    void setupGui();
    
    // Broadcast messages
    void sendPlayScene(int id);
    void sendPlaySubScene(int id);
    void sendPresence(string areaName, int count);
    void sendVolume(float volume, int windowId);
    void sendVolumeTrigger(int windowId);
    
    // Light and sound cues
    void sendLightSoundCue(CueParams cue);
    void sendLightSoundCue(CueWithListParams cue);
    void sendLightSoundCue(CueWithFloatParams cue);
    void sendLightingCue(float cue, float list = 1);
    void sendSoundCue(float cue);
    void sendSoundVolume(float id, float volume);
    
    // Careful now!
    void sendKill();
    
    // Events for the rest of the app
    // when new messages are received
    ofEvent<int> playSceneEvent;
    ofEvent<int> playSubSceneEvent;
    ofEvent<VolumeEventArgs> volumeEvent;
    ofEvent<VolumeEventArgs> volumeTriggerEvent;
    
protected:  
private:
    int id;
    bool isConnected;
    
    Sender senderBroadcast;
//    Sender senderMaster;
//    Sender senderSlave;
//    Sender senderWalls[4];
    Sender senderLights;
    Sender senderSound;
    
    ofxOscReceiver receiver;
    int current_msg_string;
    string msg_strings[NUM_MSG_STRINGS];
    float timers[NUM_MSG_STRINGS];
    
    ofParameter<string> receivePort;
    ofxButton killButton;
};