#include "IPCamInput.h"

IPCamInput::IPCamInput() {
}

void IPCamInput::setup() {
    // setup IP cam
    loadCamera();
    IPCameraDef& cam = getCamera();
    SharedIPVideoGrabber c = IPVideoGrabber::makeShared();
    c->setCameraName(cam.getName());
    c->setURI(cam.getURL());
    grabber = c;
    hasImg = false;
}

void IPCamInput::update() {
    grabber->update();
    if(grabber->isFrameNew() && !hasImg) hasImg = true;
}

void IPCamInput::draw() {
    ofSetColor(255);
    float w = ofGetWidth()/2;
    float h = grabber->getHeight() * (w / grabber->getWidth());
    grabber->draw(0, 0, w, h);
}

void IPCamInput::exit() {
}

//////////////////////////////////////////////////////////////////////////////////
// public
//////////////////////////////////////////////////////////////////////////////////
ofPixelsRef IPCamInput::getPixelsRef() {
    if (!getIsReady()) ofLogWarning() << "IPCamInput::getPixelsRef grabber is not ready";
    return grabber->getFrame()->getPixelsRef();
}

bool IPCamInput::isFrameNew() {
    return grabber->isFrameNew();
}

bool IPCamInput::getIsReady() {
    return (hasImg && grabber->isConnected());
}

void IPCamInput::start(){
    ofLogNotice() << "IPCamInput::start";
    grabber->connect();
}

void IPCamInput::stop(){
    ofLogNotice() << "IPCamInput::stop";
    grabber->disconnect();
    hasImg = false;
}

//////////////////////////////////////////////////////////////////////////////////
// protected
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
// private
//////////////////////////////////////////////////////////////////////////////////

IPCameraDef& IPCamInput::getCamera() {
    return ipcam;
}

void IPCamInput::loadCamera() {
    ipcam = IPCameraDef("Dome", CAM_URL, "", "");
}

//////////////////////////////////////////////////////////////////////////////////
// custom event handlers
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
// oF event handlers
//////////////////////////////////////////////////////////////////////////////////
