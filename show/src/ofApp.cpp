#include "ofApp.h"

void ofApp::setup() {
    vision.setup();
}

void ofApp::update() {
    vision.update();
}

void ofApp::draw() {
    vision.draw();
    ofDrawBitmapStringHighlight(ofToString(ofGetFrameRate()), 10, ofGetHeight() - 20);
}

void ofApp::keyPressed (int key) {
    vision.keyPressed(key);
}

void ofApp::keyReleased (int key) {}

void ofApp::mouseMoved(int x, int y) {}

void ofApp::mouseDragged(int x, int y, int button) {}

void ofApp::mousePressed(int x, int y, int button) {}

void ofApp::mouseReleased(int x, int y, int button) {}

void ofApp::windowResized(int w, int h) {}