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
    radiusMin.set("radius min", 10, 0, 500);
    radiusMax.set("radius max", 200, 0, 500);
    noiseScale.set("noise scale", 80, 0, 500);
    threshold.set("threshold", 0.5, 0.0, 1.0);
    debugDraw.set("debug draw", false);
    // subscenes
    subsceneStart = 3;
    subsceneEnd = 9;
    // master needs to listen out for window volumes
    ofAddListener(this->osc->volumeEvent, this, &IgniteScene::onWindowVolume);
    ofAddListener(this->osc->volumeTriggerEvent, this, &IgniteScene::onWindowVolumeTrigger);
    SceneBase::setup();
}

void IgniteScene::update() {
    
    if (isWindow() && getWindowActive()) {
        mic->fftLive.setThreshold(threshold);
        mic->update();
        averageVolume = ofLerp(averageVolume, mic->fftLive.getAveragePeak(), 0.3);
        osc->sendVolume(averageVolume, appModel->windowId);
    }
    
    if (isMaster()) minTime.update();
    // In Subscene 8, each window is active
    // master listens to volumes sent over OSC
    // and triggers next scene when all have reached max
    if (subsceneI == 8 && isMaster()) {
        // Every window is active, need to get overall average/max
        bool allActive = true;
        for (int i = 0; i<WINDOW_COUNT; i++) {
            if (!windowTriggers[i]) allActive = false;
        }
        if (allActive) {
            ofLogNotice() << "IgniteScene::update target volume reached FOR ALL";
            nextSubscene();
        }
    }
    
    SceneBase::update();
}

void IgniteScene::draw() {
    /*
    if (isWindow() && getWindowActive()) {
        // draw a blob based on the average volume of the mic input
        float x = ofGetWidth()/2;
        float y = ofGetHeight()/2;
        int pointCount = 30;
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
     */
    
    if (isMaster()) {
        for (int i = 0; i<WINDOW_COUNT; i++) {
            if (windowTriggers[i]) ofSetColor(255);
            else ofSetColor(150);
            float h = windowVolumes[i] * (ofGetHeight()/2);
            ofRect(i*40, ofGetHeight(), 40, -h);
        }
        //int remaining = targetFrames - targetHitCount;
        ofDrawBitmapStringHighlight("Min time remaining: " + ofToString(minTime.progress), 10, ofGetHeight() - 220, ofColor(200, 0, 0));
        ofDrawBitmapStringHighlight("Audio peak count: " + ofToString(targetHitCount)+"/"+ofToString(targetFrames), 10, ofGetHeight() - 200, ofColor(200, 0, 0));
        ofSetColor(255);
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
    
    // Initial subscene is blank
    if (i == 3) {
        if (isSlave()) led->show(title1.get());
        if (isMaster()) {
            countdown->start(0);//timeIntro.get());
        }
    }
    if(i == 4) {
        //osc->sendSoundCue(3); //Another Horrible Hack I'M SORRY CHRIS!
        if (isMaster()) {
            osc->sendLightSoundCue(intro);
            //osc->sendSoundCue(intro.soundCue);
        }
    }
    if(i == 5) {
        if (isMaster()) {
//            osc->sendLightingCue(FourToFive.lightCue, FourToFive.lightList);
//            osc->sendSoundCue(FourToFive.soundCue);
            osc->sendLightSoundCue(FourToFive);
        }
    }
    if(i == 6) {
        if (isMaster()) {
//            osc->sendLightingCue(FiveToSix.lightCue, FiveToSix.lightList);
//            osc->sendSoundCue(FiveToSix.soundCue);
            osc->sendLightSoundCue(FiveToSix);

        }
    }
    if(i == 7) {
        if (isMaster()) {
//            osc->sendLightingCue(SixToSeven.lightCue, SixToSeven.lightList);
//            osc->sendSoundCue(SixToSeven.soundCue);
            osc->sendLightSoundCue(SixToSeven);

        }
    }
    if(i == 8) {
        if (isMaster()) {
            osc->sendLightSoundCue(SevenToEight);
            osc->sendSoundCue(6.5);
//            osc->sendLightingCue(SevenToEight.lightCue, SevenToEight.lightList);
//            osc->sendSoundCue(SevenToEight.soundCue);
        }
    }
    
    // Active mic scenes, 1 to 4, then all 4
    if (i >= 4 && i <= 8) {
        if (isSlave()) {
            led->show(title1.get());
        }
        if (isMaster()) {
            int cueI = i - 4;
            countdown->stop();
            minTime.start(minMicTime.get());
            // reset values
            for (int i = 0; i<WINDOW_COUNT; i++) {
                windowTriggers[i] = false;
                windowVolumes[i] = 0;
                //osc->sendLightSoundCue(outCues[i]);
            }
        }
        // start audio input if this is the active window
        if (isWindow() && getWindowActive()) {
            targetHitCount = 0;
            mic->start();
        }
    }
    
    // countdown outro
    else if (i == 9) {
        if (isSlave()) {
            led->hide();
            led->queue(LedDisplay::Params(title1.get(), 0, 1, 15, false, 0));
            led->queue(LedDisplay::Params("", 0, 14, 0, false, 0));
            led->queue(LedDisplay::Params("Level Completed", 0, 2, 0, false, 0));
            led->queue(LedDisplay::Params(title2, 0, 5, 0, false, 5));
            led->playQueue();
            //led->show(title2, countdownDuration);
        }
        if (isMaster()) {
            countdown->start(countdownDuration);
            cout<<"sent lighting cue 2 and sound cue 7"<<endl;
            osc->sendLightingCue(2);
            osc->sendSoundCue(7);
        }
    }
    
    SceneBase::play(i);
}

void IgniteScene::stop(){
    mic->stop();
    SceneBase::stop();
}

void IgniteScene::setupGui() {
    guiName = "Ignite";
    panel.setup(guiName, "settings/ignite.xml");
    
    // titles, times
    panel.add(title1.set("title 1", "VERY LOUD I CONE"));
    panel.add(title2.set("title 2", "Next Level"));
    panel.add(minMicTime.set("min mic time", 20, 1, 60));
    panel.add(timeIntro.set("intro time", 30, 1, 60));
    panel.add(countdownDuration.set("countdown", 37, 0, 20));
    
    // audio
    panel.add(targetVolume.set("target volume", 0.5, 0, 1));
    panel.add(targetFrames.set("target frames", 60, 1, 180));
    panel.add(threshold);
    panel.add(debugDraw);
    
    // cues
    for (int i=0; i<CUE_COUNT; i++) {
        panel.add(inCues[i].setup("Cue in" + ofToString(i), (i+1)*2));
        panel.add(outCues[i].setup("Cue out" + ofToString(i), ((i+1)*2)+1));
    }
    panel.add(intro.setup("Wall 1 Open", 1));
    panel.add(FourToFive.setup("Wall 2 Open", 1));
    panel.add(FiveToSix.setup("Wall 3 Open", 1));
    panel.add(SixToSeven.setup("Wall 4 Open", 1));
    panel.add(SevenToEight.setup("All Walls Open", 1));
    panel.add(nextCue.setup("Cue Next", 2));
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
        if (appModel->windowId == 1 && subsceneI >= 3) active = true;
        if (appModel->windowId == 2 && subsceneI >= 5) active = true;
        if (appModel->windowId == 3 && subsceneI >= 6) active = true;
        if (appModel->windowId == 4 && subsceneI >= 7) active = true;
        if (subsceneI > 8) active = false;
    }
    return active;
}

// Master only
// listener for window volume OSC messages
void IgniteScene::onWindowVolume(OscClient::VolumeEventArgs& args) {
    if (isMaster()) {
        int i = args.windowId - 1;
        if (args.windowId == subsceneI - 3 || subsceneI == 8) {
            //ofLogVerbose() << "window : " + ofToString(i) + " vol : " + ofToString(args.volume);
            float vol = args.volume;
            windowVolumes[i] = vol;
            if (vol > targetVolume) {
               if (subsceneI != 8) osc->sendLightSoundCue(inCues[i]);
                //osc->sendLightingCue(inCues[i].lightCue);
                //osc->sendSoundCue(inCues[i].soundCue);
                if (targetHitCount++ > targetFrames && minTime.isComplete()) {
                    if (subsceneI == 8) {
                        // this is the final mode in which all windows are active
                        // mark this window as triggered
                        // when all are triggered, next scene will be called
                        windowTriggers[i] = true;
                    } else {
                        nextSubscene();
                        ofLogNotice() << "IgniteScene::update target volume reached, triggering next scene";
                        targetHitCount = 0;
                    }
                }
            }
            else {
                //osc->sendLightSoundCue(outCues[i]);
            }
        }
    }
}

void IgniteScene::onWindowVolumeTrigger(OscClient::VolumeEventArgs& args) {
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
