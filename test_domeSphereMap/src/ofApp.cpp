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
    
    for (int y=0; y<grabber->getHeight(); y++) {
        for (int x=0; x<grabber->getWidth(); x++) {
            ofVec2f dewarped = fishToSphere(ofVec2f(x, y));
            ofColor c = grabber->getFrame()->getColor(dewarped.x, dewarped.y);
            ofSetColor(c);
            ofRect(x, y, 1, 1);
        }
    }
    
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

ofVec2f ofApp::fishToSphere(ofVec2f destCoord)
{
    ofVec2f pfish;
    float theta,phi,r;
    ofVec3f psph;
    
    float FOV = 3.141592654; // FOV of the fisheye, eg: 180 degrees
    float width = grabber->getWidth();
    float height = grabber->getHeight();
    
    // Polar angles
    theta = 2.0 * 3.14159265 * (destCoord.x / width - 0.5); // -pi to pi
    phi = 3.14159265 * (destCoord.y / height - 0.5);	// -pi/2 to pi/2
    
    // Vector in 3D space
    psph.x = cos(phi) * sin(theta);
    psph.y = cos(phi) * cos(theta);
    psph.z = sin(phi);
    
    // Calculate fisheye angle and radius
    theta = atan2(psph.z,psph.x);
    phi = atan2(sqrt(psph.x*psph.x+psph.z*psph.z),psph.y);
    r = width * phi / FOV;
    
    // Pixel in fisheye space
    pfish.x = 0.5 * width + r * cos(theta);
    pfish.y = 0.5 * width + r * sin(theta);
    
    pfish;
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
