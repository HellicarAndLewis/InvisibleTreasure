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
    audioMirror = true;
    int n = 10;
    levels.assign(n, 0.0);
    previousLevels.assign(n, 0.0);
    radiusMin.set("radius min", 10, 0, 500);
    radiusMax.set("radius max", 200, 0, 500);
    noiseScale.set("noise scale", 80, 0, 500);
    threshold.set("threshold", 0.5, 0.0, 1.0);
    debugDraw.set("debug draw", false);
    
    // subscenes
    subsceneStart = 3;
    subsceneEnd = 9;
    SceneBase::setup();
}

void IgniteScene::update() {
    if (isWindow() && getWindowActive()) {
        //setMaxDecay(0.995);
        //setPeakDecay(0.96);
        //mic->fftLive.setPeakDecay(0.5);
        //mic->fftLive.setMaxDecay(0.5);
        mic->fftLive.setThreshold(threshold);
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
    if (isWindow() && getWindowActive()) {
        // draw a blob based on the average volume of the mic input
        float x = ofGetWidth()/2;
        float y = ofGetHeight()/2;
        int pointCount = 30;
        //if (levels.size() > 0) {
        //    pointCount = ofMap(levels[levels.size()-1], 0, 1, 10, 100, true);
        //}
        float outerRadius = ofMap(averageVolume, 0, 0.8, radiusMin, radiusMax);
        ofPath path;
        path.newSubPath();
        auto angleChangePerPt = ((pi) * 2) / pointCount;
        auto angle = pi * 0.5;
        path.moveTo(x + (outerRadius * cos(angle)), y + (outerRadius * sin(angle)));
        float time = ofGetElapsedTimef();
        ofPoint firstPoint;
        for (int i = 0; i < pointCount; i++) {
            auto rad = outerRadius + ofNoise(cos(angle)*time, sin(angle)*time) * noiseScale * averageVolume;
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
    if (debugDraw) {
        mic->draw();
    }
}

//////////////////////////////////////////////////////////////////////////////////
// public
//////////////////////////////////////////////////////////////////////////////////
void IgniteScene::play(int i){
    
    // init the LED display
    if (isSlave()) {
        if (i == 3) {
            led->show("");
        }
        else if (i > 3 && i < 9) {
            led->show(title1.get() + ofToString(i));
            //osc->sendLightSoundCue(cue2);
        }
        else if (i == 9) {
            led->show(title2, (int)countdownDuration);
            //osc->sendLightSoundCue(cue2);
        }
    }
    
    // start audio input if this is the active window
    if (isWindow() && getWindowActive()) mic->start();
    
    SceneBase::play(i);
}

void IgniteScene::stop(){
    mic->stop();
    SceneBase::stop();
}

void IgniteScene::setupGui() {
    guiName = "Ignite";
    panel.setup(guiName, "settings/ignite.xml");
    // titles, times, cues
    panel.add(title1.set("title 1", "Ignite The Space"));
    panel.add(title2.set("title 2", "Next Level"));
    panel.add(countdownDuration.set("countdown", 5, 0, 20));
    for (int i=0; i<CUE_COUNT; i++) {
        cues[i].lightCue = i;
        cues[i].lightList = 2;
        panel.add(cues[i].setup("Cue " + ofToString(i)));
    }
    // audio
    panel.add(radiusMin);
    panel.add(radiusMax);
    panel.add(noiseScale);
    panel.add(threshold);
    panel.add(debugDraw);
    panel.loadFromFile("settings/ignite.xml");
}

void IgniteScene::drawGui() {
    GuiableBase::drawGui();
}

//////////////////////////////////////////////////////////////////////////////////
// protected
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
// private
//////////////////////////////////////////////////////////////////////////////////
bool IgniteScene::getWindowActive() {
    bool active = false;
    if (mode==AppModel::WINDOW) {
        if (appModel->windowId == 1 && subsceneI == 4) active = true;
        if (appModel->windowId == 2 && subsceneI == 5) active = true;
        if (appModel->windowId == 3 && subsceneI == 6) active = true;
        if (appModel->windowId == 4 && subsceneI == 7) active = true;
        if (subsceneI == 8) active = true;
    }
    return active;
}
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
