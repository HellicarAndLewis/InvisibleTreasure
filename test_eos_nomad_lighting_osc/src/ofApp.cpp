#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){

	ofBackground(40, 100, 40);

	// open an outgoing connection to HOST:PORT
	sender.setup(HOST, PORT);
    
    imgAsBuffer = ofBufferFromFile("sendImageTest.jpg", true);

}

//--------------------------------------------------------------
void ofApp::update(){

}

//--------------------------------------------------------------
void ofApp::draw(){
    
    if(img.getWidth() > 0){
        ofDrawBitmapString("Image:", 10, 160);
        img.draw(10, 180);
    }
    
	// display instructions
	string buf;
	buf = "sending osc messages to" + string(HOST) + ofToString(PORT);
	ofDrawBitmapString(buf, 10, 20);

	//ofDrawBitmapString("press A to send osc message [/test 1 3.5 hello <time>]", 10, 80);
    ofDrawBitmapString("press A to send osc message [/eos/cue/1/1/fire]", 10, 80);
    ofDrawBitmapString("press S to send osc message [/eos/cue/1/0.5/fire]", 10, 100);
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
	if(key == 'a' || key == 'A'){
		ofxOscMessage m;
		m.setAddress("/eos/cue/fire=1");
		//m.addIntArg(1);
		//m.addFloatArg(3.5f);
		sender.sendMessage(m);
        cout << "Sent /eos/cue/fire=1" << endl;
	}
    if(key == 's' || key == 'S'){
        ofxOscMessage m;
        m.setAddress("/eos/cue/1/0.5/fire");
        //m.addIntArg(1);
        //m.addFloatArg(3.5f);
        sender.sendMessage(m);
        cout << "Sent /eos/cue/1/0.5/fire" << endl;
    }
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y){
}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){
}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){
    
}

