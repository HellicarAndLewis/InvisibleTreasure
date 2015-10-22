//
//  OscClient.cpp
//  show
//
//  Created by Chris Mullany on 02/09/2015.
//
//

#include "OscClient.h"

#define PLAY_SCENE_ADRESS "/scene/playscene"
#define PLAY_SUBSCENE_ADRESS "/scene/playsubscene"
#define PRESENCE_ADRESS "/tracking/presence"
#define VOLUME_ADRESS "/window/volume"
#define VOLUME_TRIGGER_ADRESS "/window/volumetrigger"

OscClient::OscClient() {
    current_msg_string = 0;
    isConnected = false;
}

void OscClient::setup(int id) {
    this->id = id;
}

void OscClient::update() {
    if (!isConnected) return;
    
    // debug only
    for(int i = 0; i < NUM_MSG_STRINGS; i++){
        if(timers[i] < ofGetElapsedTimef()){
            msg_strings[i] = "";
        }
    }
    
    // check for waiting messages
    while(receiver.hasWaitingMessages()){
        // get the next message
        ofxOscMessage m;
        receiver.getNextMessage(&m);
        string msg_string;
        msg_string = m.getAddress();
        int senderId = m.getArgAsInt32(1);

        if (msg_string == PLAY_SCENE_ADRESS) {
            int sceneId = m.getArgAsInt32(0);
            int senderId = m.getArgAsInt32(1);
            if (senderId != this->id) ofNotifyEvent(playSceneEvent, sceneId, this);
        }
        else if (msg_string == PLAY_SUBSCENE_ADRESS) {
            int sceneId = m.getArgAsInt32(0);
            int senderId = m.getArgAsInt32(1);
            if (senderId != this->id) ofNotifyEvent(playSubSceneEvent, sceneId, this);
        }
        else if (msg_string == VOLUME_ADRESS) {
            float volume = m.getArgAsFloat(0);
            int windowId = m.getArgAsInt32(1);
            int senderId = m.getArgAsInt32(2);
            VolumeEventArgs args = VolumeEventArgs(volume, windowId);
            if (senderId != this->id) ofNotifyEvent(volumeEvent, args, this);
        }
        else if (msg_string == VOLUME_TRIGGER_ADRESS) {
            int windowId = m.getArgAsInt32(0);
            int senderId = m.getArgAsInt32(1);
            VolumeEventArgs args = VolumeEventArgs(1, windowId);
            if (senderId != this->id) ofNotifyEvent(volumeTriggerEvent, args, this);
        }
        else if (msg_string == "/kill") {
            int senderId = m.getArgAsInt32(0);
            if (senderId != this->id) std::exit(1);
        }
        
        // Unknown message
        msg_string += ": ";
        for(int i = 0; i < m.getNumArgs(); i++){
            // get the argument type
            msg_string += m.getArgTypeName(i);
            msg_string += ":";
            // display the argument - make sure we get the right type
            if(m.getArgType(i) == OFXOSC_TYPE_INT32){
                msg_string += ofToString(m.getArgAsInt32(i));
            }
            else if(m.getArgType(i) == OFXOSC_TYPE_FLOAT){
                msg_string += ofToString(m.getArgAsFloat(i));
            }
            else if(m.getArgType(i) == OFXOSC_TYPE_STRING){
                msg_string += m.getArgAsString(i);
            }
            else{
                msg_string += "unknown";
            }
        }
        
        // debug only
        msg_strings[current_msg_string] = msg_string;
        timers[current_msg_string] = ofGetElapsedTimef() + 5.0f;
        current_msg_string = (current_msg_string + 1) % NUM_MSG_STRINGS;
        msg_strings[current_msg_string] = "";
    }
}

void OscClient::draw() {
    // debug only
    for(int i = 0; i < NUM_MSG_STRINGS; i++){
        ofDrawBitmapString(msg_strings[i], 10, 40 + 15 * i);
    }
}

//////////////////////////////////////////////////////////////////////////////////
// public
//////////////////////////////////////////////////////////////////////////////////

void OscClient::setupGui() {
    guiName = "OSC";
    panel.setup(guiName, "settings/osc.xml");
    panel.add(receivePort.set("receive port", "12345"));
    // senders
//    panel.add(senderBroadcast.setup("broadcast", "192.168.255.255", "12345"));
    panel.add(senderMaster.setup("master", "192.168.255.42", "12345"));
    panel.add(senderSlave.setup("slave", "192.168.255.43", "12345"));
    for(int i=0; i<4; i++) {
        panel.add(senderWalls[i].setup("wall" + ofToString(i+1), "192.168.255.5" + ofToString(i+1), "12345"));
    }
    panel.add(senderLights.setup("light", "192.168.255.1", "53002"));
    panel.add(senderSound.setup("sound", "192.168.255.1", "8001"));
    // KILL ALL OTHER APPS
    killButton.addListener(this, &OscClient::sendKill);
    panel.add(killButton.setup("kill"));
    panel.loadFromFile("settings/osc.xml");
    
    // setup OSC clients after loading settings
    try {
//        senderBroadcast.connect();
        senderMaster.connect();
        senderSlave.connect();
        for(int i=0; i < 4; i++) {
            senderWalls[i].connect();
        }
        senderLights.connect();
        senderSound.connect();
        receiver.setup(ofToInt(receivePort));
        ofLogError() << "OscClient listening on " << receivePort;
        isConnected = true;
    } catch (std::exception e) {
        ofLogError() << "OscClient cannot conect! Are you connected to the network?";
    }
}

void OscClient::sendPlayScene(int id) {
    ofLogNotice("OscClient::sendPlayScene: " + (string)PLAY_SCENE_ADRESS + " " + ofToString(id));
    if (!isConnected) return;
    ofxOscMessage m;
    m.setAddress(PLAY_SCENE_ADRESS);
    m.addIntArg(id);
    m.addIntArg(this->id);
    senderMaster.client.sendMessage(m);
    senderSlave.client.sendMessage(m);
    for(int i=0; i<4; i++) {
        senderWalls[i].client.sendMessage(m);
    }
//    senderBroadcast.client.sendMessage(m);
}


void OscClient::sendPlaySubScene(int id) {
    ofLogNotice("OscClient::sendPlaySubScene: " + (string)PLAY_SUBSCENE_ADRESS + " " + ofToString(id));
    if (!isConnected) return;
    ofxOscMessage m;
    m.setAddress(PLAY_SUBSCENE_ADRESS);
    m.addIntArg(id);
    m.addIntArg(this->id);
    senderMaster.client.sendMessage(m);
    senderSlave.client.sendMessage(m);
    for(int i=0; i<4; i++) {
        senderWalls[i].client.sendMessage(m);
    }
//    senderBroadcast.client.sendMessage(m);
}

void OscClient::sendPresence(string areaName, int count){
    if (!isConnected) return;
    ofxOscMessage m;
    m.setAddress(PRESENCE_ADRESS);
    m.addStringArg(areaName);
    m.addIntArg(count);
    m.addIntArg(this->id);
    senderMaster.client.sendMessage(m);
    senderSlave.client.sendMessage(m);
    for(int i=0; i<4; i++) {
        senderWalls[i].client.sendMessage(m);
    }
//    senderBroadcast.client.sendMessage(m);
}

void OscClient::sendVolume(float volume, int windowId){
    ofLogVerbose("OscClient::sendVolume: " + (string)VOLUME_ADRESS + " " + ofToString(volume));
    if (!isConnected) return;
    ofxOscMessage m;
    m.setAddress(VOLUME_ADRESS);
    m.addFloatArg(volume);
    m.addIntArg(windowId);
    m.addIntArg(this->id);
    senderMaster.client.sendMessage(m);
    senderSlave.client.sendMessage(m);
    for(int i=0; i<4; i++) {
        senderWalls[i].client.sendMessage(m);
    }
//    senderBroadcast.client.sendMessage(m);
}

void OscClient::sendVolumeTrigger(int windowId) {
    ofLogNotice("OscClient::sendVolumeTrigger: " + (string)VOLUME_ADRESS + " " + ofToString(windowId));
    if (!isConnected) return;
    ofxOscMessage m;
    m.setAddress(VOLUME_TRIGGER_ADRESS);
    m.addIntArg(windowId);
    m.addIntArg(this->id);
    senderMaster.client.sendMessage(m);
    senderSlave.client.sendMessage(m);
    for(int i=0; i<4; i++) {
        senderWalls[i].client.sendMessage(m);
    }
//    senderBroadcast.client.sendMessage(m);
}

void OscClient::sendLightSoundCue(CueParams cue) {
    if (cue.lightCue > 0) {
        sendLightingCue(cue.lightCue, cue.lightList);
    }
    if (cue.soundCue > 0) {
        sendSoundCue(cue.soundCue);
    }
}

void OscClient::sendLightSoundCue(CueWithListParams cue) {
    if (cue.lightCue > 0) {
        sendLightingCue(cue.lightCue, cue.lightList);
    }
    if (cue.soundCue > 0) {
        sendSoundCue(cue.soundCue);
    }
}

void OscClient::sendLightSoundCue(CueWithFloatParams cue) {
    if (cue.lightCue > 0) {
        sendLightingCue(cue.lightCue, cue.lightList);
    }
    if (cue.soundCue > 0) {
        sendSoundCue(cue.soundCue);
    }
}

//
// Send lighting LX cues to nomad lighting desk
// which requires the format /eos/cue/<list number>/<cue number>/fire
//
void OscClient::sendLightingCue(float cue, float list) {
    string address = "/eos/cue/" + ofToString(list) + "/" + ofToString(cue) + "/fire";
    ofLogNotice("OscClient::sendLightingCue: " + address);
    if (!isConnected) return;
    ofxOscMessage m;
    m.setAddress(address);
    senderLights.client.sendMessage(m);
}

//
// Send audio cue to QLab Mac
// '/cue/{number}/start'
//
void OscClient::sendSoundCue(float cue) {
    string address = "/cue/" + ofToString(cue) + "/start";
    ofLogNotice("OscClient::sendSoundCue: " + address);
    if (!isConnected) return;
    ofxOscMessage m;
    m.setAddress(address);
    senderSound.client.sendMessage(m);
}

//
// Send audio voluem cue to QLab Mac?
// '/volume/{number}/start'??
//
void OscClient::sendSoundVolume(float id, float volume) {
    string address = "/volume/" + ofToString(id) + "/" + ofToString(volume);
    ofLogVerbose("OscClient::sendSoundVolume: " + address);
    if (!isConnected) return;
    ofxOscMessage m;
    m.setAddress(address);
    senderSound.client.sendMessage(m);
}

void OscClient::sendKill() {
    string address = "/kill";
    ofLogVerbose("OscClient::sendKill");
    if (!isConnected) return;
    ofxOscMessage m;
    m.setAddress(address);
    m.addIntArg(this->id);
    //senderBroadcast.client.sendMessage(m);
    //std::exit(1);
}

//////////////////////////////////////////////////////////////////////////////////
// protected
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
// private
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
// custom event handlers
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
// oF event handlers
//////////////////////////////////////////////////////////////////////////////////

void OscClient::keyPressed (int key) {
    if(key == 'p'){
        if (!isConnected) return;
        ofLogNotice() << "/ping";
        ofxOscMessage m;
        m.setAddress("/ping");
        m.addIntArg(1);
        senderMaster.client.sendMessage(m);
        senderSlave.client.sendMessage(m);
        for(int i=0; i<4; i++) {
            senderWalls[i].client.sendMessage(m);
        }
        senderSound.client.sendMessage(m);
        senderLights.client.sendMessage(m);
//        senderBroadcast.client.sendMessage(m);
    }
}
