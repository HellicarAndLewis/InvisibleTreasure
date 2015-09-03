//
//  OscClient.cpp
//  show
//
//  Created by Chris Mullany on 02/09/2015.
//
//

#include "OscClient.h"
#define PLAY_SCENE_ADRESS "/playscene"

OscClient::OscClient() {
    sendAddress = "192.168.0.255";
    sendPort = "12345";
    receivePort = "12345";
}

void OscClient::setup() {
    sender.setup(sendAddress, ofToInt(sendPort));
    receiver.setup(ofToInt(receivePort));
    current_msg_string = 0;
}

void OscClient::update() {
    // hide old messages
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
        
        if (msg_string == PLAY_SCENE_ADRESS) {
            int id = m.getArgAsInt32(0);
            ofNotifyEvent(playSceneEvent, id, this);
        }
        
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
        // add to the list of strings to display
        msg_strings[current_msg_string] = msg_string;
        timers[current_msg_string] = ofGetElapsedTimef() + 5.0f;
        current_msg_string = (current_msg_string + 1) % NUM_MSG_STRINGS;
        // clear the next line
        msg_strings[current_msg_string] = "";
    }
}

void OscClient::draw() {
    for(int i = 0; i < NUM_MSG_STRINGS; i++){
        ofDrawBitmapString(msg_strings[i], 10, 40 + 15 * i);
    }
}

void OscClient::exit() {
}


//////////////////////////////////////////////////////////////////////////////////
// public
//////////////////////////////////////////////////////////////////////////////////

void OscClient::setupGui() {
    guiName = "OSC";
    parameters.setName(guiName);
    parameters.add(sendAddress.set("to IP", "192.168.0.255"));
    parameters.add(sendPort.set("to port", "12345"));
    panel.setup(parameters, "settings/osc.xml");
    panel.loadFromFile("settings/osc.xml");
    //vector<GuiableBase*> guiables;
    //guiables.push_back(this);
    //return guiables;
}

void OscClient::sendPlayScene(int id) {
    ofxOscMessage m;
    m.setAddress(PLAY_SCENE_ADRESS);
    m.addIntArg(id);
    sender.sendMessage(m);
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
        ofxOscMessage m;
        m.setAddress("/ping");
        m.addIntArg(1);
        sender.sendMessage(m);
    }
}

void OscClient::keyReleased (int key) {}

void OscClient::mouseMoved(int x, int y) {}

void OscClient::mouseDragged(int x, int y, int button) {}

void OscClient::mousePressed(int x, int y, int button) {}

void OscClient::mouseReleased(int x, int y, int button) {}

void OscClient::windowResized(int w, int h) {}

void OscClient::dragEvent(ofDragInfo dragInfo) {}

void OscClient::gotMessage(ofMessage msg) {}
