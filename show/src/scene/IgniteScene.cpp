//
//  IgniteScene.cpp
//  show
//
//  Created by Chris Mullany on 02/09/2015.
//
//

#include "IgniteScene.h"

IgniteScene::IgniteScene() {
    name = "Ignite";
    averageVolume = 0;
}

void IgniteScene::setup() {
    audioMirror = false;
    int n = 10;
    levels.assign(n, 0.0);
    previousLevels.assign(n, 0.0);
}

void IgniteScene::update() {
    if (mode == AppModel::WINDOW) {
        //setMaxDecay(0.995);
        //setPeakDecay(0.96);
        //setThreshold(0.5);
        //mic->fftLive.setPeakDecay(0.5);
        //mic->fftLive.setMaxDecay(0.5);
        mic->update();
        averageVolume = ofLerp(averageVolume, mic->fftLive.getAveragePeak(), 0.3);
        int n  = levels.size();
        if (!audioMirror) n *= 2;
        float * audioData = new float[n];
        mic->fftLive.getFftPeakData(audioData, n);
        // populate levels for mapping to blob?
        for(int i=0; i<levels.size(); i++) {
            previousLevels[i] = levels[i];
            float audioValue = audioData[i];
            levels[i] = audioValue;
        }
        delete[] audioData;
    }
    SceneBase::update();
}

void IgniteScene::draw() {
    if (mode==AppModel::SLAVE) {
        led->label = "MAKE SOME NOISE";
        led->draw();
    }
    if (mode==AppModel::WINDOW) {
        // draw a blob based on the average volume of the mic input
        float x = ofGetWidth()/2;
        float y = ofGetHeight()/2;
        int pointCount = 30;
        //if (levels.size() > 0) {
        //    pointCount = ofMap(levels[levels.size()-1], 0, 1, 10, 100, true);
        //}
        float outerRadius = 10 + (averageVolume * 300);
        ofPath path;
        path.newSubPath();
        auto angleChangePerPt = ((pi) * 2) / pointCount;
        auto angle = pi * 0.5;
        path.moveTo(x + (outerRadius * cos(angle)), y + (outerRadius * sin(angle)));
        float time = ofGetElapsedTimef();
        ofPoint firstPoint;
        for (int i = 0; i < pointCount; i++) {
            auto rad = outerRadius + ofNoise(cos(angle)*time, sin(angle)*time) * 80 * averageVolume;
            auto vertX = x + (rad * cos(angle));
            auto vertY = y + (rad * sin(angle));
            if (i==0) firstPoint.set(vertX, vertY);
            path.curveTo(vertX, vertY);
            //path.circle(vertX, vertY, 10);
            angle += angleChangePerPt;
        };
        path.curveTo(firstPoint);
        path.setColor(ofColor(255));
        path.close();
        path.draw();
    }
    SceneBase::draw();
}

//////////////////////////////////////////////////////////////////////////////////
// public
//////////////////////////////////////////////////////////////////////////////////
void IgniteScene::play(){
    mic->start();
    setState(INTRO);
}

void IgniteScene::stop(){
    mic->stop();
    setState(OUTRO);
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

void IgniteScene::keyPressed (int key) {}

void IgniteScene::keyReleased (int key) {}

void IgniteScene::mouseMoved(int x, int y) {}

void IgniteScene::mouseDragged(int x, int y, int button) {}

void IgniteScene::mousePressed(int x, int y, int button) {}

void IgniteScene::mouseReleased(int x, int y, int button) {}

void IgniteScene::windowResized(int w, int h) {}

void IgniteScene::dragEvent(ofDragInfo dragInfo) {}

void IgniteScene::gotMessage(ofMessage msg) {}
