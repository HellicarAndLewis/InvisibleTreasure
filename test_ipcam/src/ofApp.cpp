#include "ofApp.h"


//------------------------------------------------------------------------------
void ofApp::setup() {
    ofSetLogLevel(OF_LOG_VERBOSE);
    loadCameras();
    
    // initialize connection
    for(std::size_t i = 0; i < NUM_CAMERAS; i++) {
        IPCameraDef& cam = getNextCamera();
        SharedIPVideoGrabber c = IPVideoGrabber::makeShared();
        c->setCameraName(cam.getName());
        c->setURI(cam.getURL());
        c->connect();
        // optional resize listener
        ofAddListener(c->videoResized, this, &ofApp::videoResized);
        grabbers.push_back(c);
    }
}

//------------------------------------------------------------------------------
IPCameraDef& ofApp::getNextCamera() {
    nextCamera = (nextCamera + 1) % ipcams.size();
    return ipcams[nextCamera];
}

//------------------------------------------------------------------------------
void ofApp::loadCameras() {
    ipcams.push_back(IPCameraDef("Dome", CAM_URL, "", ""));
    nextCamera = ipcams.size();
}

//------------------------------------------------------------------------------
void ofApp::videoResized(const void* sender, ofResizeEventArgs& arg) {
    // find the camera that sent the resize event changed
    for(std::size_t i = 0; i < NUM_CAMERAS; i++) {
        if(sender == &grabbers[i]) {
            std::stringstream ss;
            ss << "videoResized: ";
            ss << "Camera connected to: " << grabbers[i]->getURI() + " ";
            ss << "New DIM = " << arg.width << "/" << arg.height;
            ofLogVerbose("ofApp") << ss.str();
        }
    }
}


//------------------------------------------------------------------------------
void ofApp::update() {
    // update the cameras
    for(std::size_t i = 0; i < grabbers.size(); i++) {
        grabbers[i]->update();
    }
}

//------------------------------------------------------------------------------
void ofApp::draw() {
    ofBackground(0,0,0);
    
    ofSetHexColor(0xffffff);
    
    int row = 0;
    int col = 0;
    
    int x = 0;
    int y = 0;
    
    int w = ofGetWidth() / NUM_COLS;
    int h = ofGetHeight() / NUM_ROWS;
    
    float totalKbps = 0;
    float totalFPS = 0;
    
    for(std::size_t i = 0; i < grabbers.size(); i++) {
        x = col * w;
        y = row * h;
        
        // draw in a grid
        row = (row + 1) % NUM_ROWS;
        if(row == 0) {
            col = (col + 1) % NUM_COLS;
        }
        
        
        ofPushMatrix();
        ofTranslate(x,y);
        ofSetColor(255,255,255,255);
        grabbers[i]->draw(0,0,w,h); // draw the camera
        
        ofEnableAlphaBlending();
        
        // draw the info box
        ofSetColor(0,80);
        ofRect(5,5,w-10,h-10);
        
        float kbps = grabbers[i]->getBitRate() / 1000.0f; // kilobits / second, not kibibits / second
        totalKbps+=kbps;
        
        float fps = grabbers[i]->getFrameRate();
        totalFPS+=fps;
        
        std::stringstream ss;
        ss << "          NAME: " << grabbers[i]->getCameraName() << endl;
        ss << "          HOST: " << grabbers[i]->getHost() << endl;
        ss << "           FPS: " << ofToString(fps,  2/*,13,' '*/) << endl;
        ss << "          Kb/S: " << ofToString(kbps, 2/*,13,' '*/) << endl;
        ss << " #Bytes Recv'd: " << ofToString(grabbers[i]->getNumBytesReceived(),  0/*,10,' '*/) << endl;
        ss << "#Frames Recv'd: " << ofToString(grabbers[i]->getNumFramesReceived(), 0/*,10,' '*/) << endl;
        ss << "Auto Reconnect: " << (grabbers[i]->getAutoReconnect() ? "YES" : "NO") << endl;
        ss << " Needs Connect: " << (grabbers[i]->getNeedsReconnect() ? "YES" : "NO") << endl;
        ss << "Time Till Next: " << grabbers[i]->getTimeTillNextAutoRetry() << " ms" << endl;
        ss << "Num Reconnects: " << ofToString(grabbers[i]->getReconnectCount()) << endl;
        ss << "Max Reconnects: " << ofToString(grabbers[i]->getMaxReconnects()) << endl;
        ss << "  Connect Fail: " << (grabbers[i]->hasConnectionFailed() ? "YES" : "NO");
        
        ofSetColor(255);
        ofDrawBitmapString(ss.str(), 10, 10+12);
        
        ofDisableAlphaBlending();
        
        ofPopMatrix();
    }
    
    // keep track of some totals
    float avgFPS = totalFPS / NUM_CAMERAS;
    float avgKbps = totalKbps / NUM_CAMERAS;
    
    ofEnableAlphaBlending();
    ofSetColor(0,80);
    ofRect(5,5, 150, 40);
    
    ofSetColor(255);
    // ofToString formatting available in 0072+
    ofDrawBitmapString(" AVG FPS: " + ofToString(avgFPS,2/*,7,' '*/), 10,17);
    ofDrawBitmapString("AVG Kb/S: " + ofToString(avgKbps,2/*,7,' '*/), 10,29);
    ofDrawBitmapString("TOT Kb/S: " + ofToString(totalKbps,2/*,7,' '*/), 10,41);
    ofDisableAlphaBlending();
    
}

//------------------------------------------------------------------------------
void ofApp::keyPressed(int key) {
    if(key == ' ') {
        // initialize connection
        for(std::size_t i = 0; i < NUM_CAMERAS; i++) {
            ofRemoveListener(grabbers[i]->videoResized, this, &ofApp::videoResized);
            SharedIPVideoGrabber c = IPVideoGrabber::makeShared();
            IPCameraDef& cam = getNextCamera();
            c->setUsername(cam.getUsername());
            c->setPassword(cam.getPassword());
            Poco::URI uri(cam.getURL());
            c->setURI(uri);
            c->connect();
            
            grabbers[i] = c;
        }
    }
}
