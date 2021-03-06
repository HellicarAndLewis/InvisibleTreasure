#include "ofApp.h"


//------------------------------------------------------------------------------
void ofApp::setup() {
    ofSetLogLevel(OF_LOG_VERBOSE);
    loadCamera();
    
    // initialize connection
    IPCameraDef& cam = getCamera();
    SharedIPVideoGrabber c = IPVideoGrabber::makeShared();
    c->setCameraName(cam.getName());
    c->setURI(cam.getURL());
    c->connect();
    // optional resize listener
    ofAddListener(c->videoResized, this, &ofApp::videoResized);
    grabber = c;
}

//------------------------------------------------------------------------------
IPCameraDef& ofApp::getCamera() {
    return ipcam;
}

//------------------------------------------------------------------------------
void ofApp::loadCamera() {
    ipcam = IPCameraDef("Dome", CAM_URL, "", "");
}

//------------------------------------------------------------------------------
void ofApp::videoResized(const void* sender, ofResizeEventArgs& arg) {
    // find the camera that sent the resize event changed
    std::stringstream ss;
    ss << "videoResized: ";
    ss << "Camera connected to: " << grabber->getURI() + " ";
    ss << "New DIM = " << arg.width << "/" << arg.height;
    //ofLogVerbose("ofApp") << ss.str();
}


//------------------------------------------------------------------------------
void ofApp::update() {
    // update the cameras
    grabber->update();
}

//------------------------------------------------------------------------------
void ofApp::draw() {
    ofBackground(0,0,0);
    
    ofSetHexColor(0xffffff);
    
    int w = ofGetWidth();
    int h = ofGetHeight();
    
    float totalKbps = 0;
    float totalFPS = 0;
    
    ofPushMatrix();
    ofSetColor(255,255,255,255);
    grabber->draw(0,0,w,h); // draw the camera
    
    ofEnableAlphaBlending();
    
    // draw the info box
    ofSetColor(0,80);
    ofRect(5,5,w-10,h-10);
    
    float kbps = grabber->getBitRate() / 1000.0f; // kilobits / second, not kibibits / second
    totalKbps+=kbps;
    
    float fps = grabber->getFrameRate();
    totalFPS+=fps;
    
    std::stringstream ss;
    ss << "          NAME: " << grabber->getCameraName() << endl;
    ss << "          HOST: " << grabber->getHost() << endl;
    ss << "           FPS: " << ofToString(fps,  2/*,13,' '*/) << endl;
    ss << "          Kb/S: " << ofToString(kbps, 2/*,13,' '*/) << endl;
    ss << " #Bytes Recv'd: " << ofToString(grabber->getNumBytesReceived(),  0/*,10,' '*/) << endl;
    ss << "#Frames Recv'd: " << ofToString(grabber->getNumFramesReceived(), 0/*,10,' '*/) << endl;
    ss << "Auto Reconnect: " << (grabber->getAutoReconnect() ? "YES" : "NO") << endl;
    ss << " Needs Connect: " << (grabber->getNeedsReconnect() ? "YES" : "NO") << endl;
    ss << "Time Till Next: " << grabber->getTimeTillNextAutoRetry() << " ms" << endl;
    ss << "Num Reconnects: " << ofToString(grabber->getReconnectCount()) << endl;
    ss << "Max Reconnects: " << ofToString(grabber->getMaxReconnects()) << endl;
    ss << "  Connect Fail: " << (grabber->hasConnectionFailed() ? "YES" : "NO");
    
    ofSetColor(255);
    ofDrawBitmapString(ss.str(), 10, 10+12);
    
    ofDisableAlphaBlending();
    
    ofPopMatrix();
    
    // keep track of some totals
    float avgFPS = totalFPS;
    float avgKbps = totalKbps;
    
    ofEnableAlphaBlending();
    ofSetColor(0,80);
    ofRect(5,5, 150, 40);
    
    ofSetColor(255);
    // ofToString formatting available in 0072+
    //ofDrawBitmapString(" AVG FPS: " + ofToString(avgFPS,2/*,7,' '*/), 10,17);
    //ofDrawBitmapString("AVG Kb/S: " + ofToString(avgKbps,2/*,7,' '*/), 10,29);
    //ofDrawBitmapString("TOT Kb/S: " + ofToString(totalKbps,2/*,7,' '*/), 10,41);
    ofDisableAlphaBlending();
    
}

//------------------------------------------------------------------------------
void ofApp::keyPressed(int key) {
    if(key == ' ') {
        // initialize connection
        ofRemoveListener(grabber->videoResized, this, &ofApp::videoResized);
        SharedIPVideoGrabber c = IPVideoGrabber::makeShared();
        IPCameraDef& cam = getCamera();
        c->setUsername(cam.getUsername());
        c->setPassword(cam.getPassword());
        Poco::URI uri(cam.getURL());
        c->setURI(uri);
        c->connect();
        
        grabber = c;
    }
}
