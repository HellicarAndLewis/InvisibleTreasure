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
    
    warp.allocate(ofGetWidth(), ofGetHeight(), 5, 4, 80, GL_RGB);
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
    if(bUseWarp){
        warp.begin();
        {
            grabber->draw(0, 0, ofGetWidth(), ofGetHeight());
        }
        warp.end();
        
    }
}

//------------------------------------------------------------------------------
void ofApp::draw() {
    ofBackground(0,0,0);
    
    ofSetHexColor(0xffffff);
    
    if(bUseWarp){
        // draw using the bezier warp
        warp.draw();
    }
    else {
        
        int w = ofGetWidth();
        int h = ofGetHeight();
        
        float totalKbps = 0;
        float totalFPS = 0;
        
        ofPushMatrix();
        ofSetColor(255,255,255,255);
        grabber->draw(0,0,w,h); // draw the camera
        
        ofPopMatrix();
        
        ofSetColor(255);
        
        ofDisableAlphaBlending();
    }
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
    switch (key) {
        case 'w':
            bUseWarp = !bUseWarp;
            break;
        case 'r':
            warp.resetWarpGrid();
            break;
        case 's':
            warp.toggleShowWarpGrid();
            break;
        case 'p':
            warp.setWarpGridPosition(100, 100, (ofGetWidth() - 200), (ofGetHeight() - 200));
            break;
        case OF_KEY_UP:
            warp.setWarpGrid(warp.getNumXPoints(), warp.getNumYPoints() + 1);
            break;
        case OF_KEY_RIGHT:
            warp.setWarpGrid(warp.getNumXPoints() + 1, warp.getNumYPoints());
            break;
        case OF_KEY_DOWN:
            warp.setWarpGrid(warp.getNumXPoints(), warp.getNumYPoints() - 1);
            break;
        case OF_KEY_LEFT:
            warp.setWarpGrid(warp.getNumXPoints() - 1, warp.getNumYPoints());
            break;
    }
}
