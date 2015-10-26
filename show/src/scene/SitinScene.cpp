//

//  SitinScene.cpp

//  show

//

//  Created by Chris Mullany on 05/09/2015.

//

//



#include "SitinScene.h"

using namespace ofxCv;

using namespace cv;



SitinScene::SitinScene() {
    
    name = "Sitin";
    
}



void SitinScene::setup() {
    
    // subscenes
    subsceneStart = 83;
    subsceneEnd = 86;
    
    hitArea.set("hitArea");
    hitArea.active = true;
    
    SceneBase::setup();
    
}



void SitinScene::update() {
    
    if (isSlave()) {
        
        imageElement.update();
        
    }
    
    SceneBase::update();
    
}



void SitinScene::draw() {
    
    SceneBase::draw();
    
}



void SitinScene::drawSlaveProjection() {
    
    imageElement.draw();
    
}



//////////////////////////////////////////////////////////////////////////////////

// public

//////////////////////////////////////////////////////////////////////////////////

void SitinScene::play(int i){
    
    switch (i) {
            
        case 83:
            
            // title, manual cue
            
            if (isSlave()) {
                led->hide();
                led->queue(LedDisplay::Params(title, 0, 360, 0, false));
                led->queue(LedDisplay::Params("6. SIT IN", 0, 1000, 0, false));
                led->playQueue();
            }
            
            if (isMaster()) {
                countdown->stop();
                
                osc->sendLightingCue(lxCueIntro);
                
                osc->sendSoundCue(soundCueIntro);
                
                hitArea.active = true;                
            }
            
            break;
            
        case 84:
            
            // the end, 10 seconds
            
            if (isSlave()) {
                led->hide();
                led->show(theEnd);
            }
            
            if (isMaster()) {
                
                countdown->start(timerTheEnd);
                
                osc->sendLightingCue(lxCueTheEnd);
                
                osc->sendSoundCue(soundCueTheEnd);
                
                hitArea.active = false;
                
            }
            
            break;
            
        case 85:
            
            // final audio track, 10 seconds
            
            if (isSlave()) led->hide();
            
            if (isMaster()) {
                
                countdown->start(timerFadeUp);
                
                osc->sendLightingCue(lxCueOutro);
                
                osc->sendSoundCue(soundCueOutro);
                
            }
            
            break;
            
        case 86:
            
            // image, manual cue
            
            if (isSlave()) {
                
                imageElement.setup("images/static.jpg");
                
                imageElement.setDisplay(&displays->slaveProjection);
                
                imageElement.show();
                
                led->hide();
                
            }
            
            if (isMaster()) {
                
                countdown->stop();
                
            }
            
            break;
            
        default:
            
            break;
            
    }
    
    SceneBase::play(i);
    
}



void SitinScene::stop(){
    
    SceneBase::stop();
    
}



void SitinScene::setupGui() {
    
    guiName = "Sitin";
    
    panel.setup(guiName, "settings/sitin.xml");
    
    titleGroup.setName("Titles");
    
    titleGroup.add(title.set("title", "SIN IT"));
    
    titleGroup.add(theEnd.set("end", "System power failure"));
    
    timerGroup.setName("Timers");
    
    timerGroup.add(timerTheEnd.set("the end", 10, 1, 60));
    
    timerGroup.add(timerFadeUp.set("fade up", 10, 1, 60));
    
    panel.add(titleGroup);
    
    panel.add(timerGroup);
    
    
    
    // LX cues
    
    lxCueGroup.setName("LX Cues");
    
    lxCueGroup.add(lxCueIntro.set("intro", 36, 0, 100));
    
    lxCueGroup.add(lxCueTheEnd.set("the end", 38, 0, 100));
    
    lxCueGroup.add(lxCueOutro.set("outro", 39, 0, 100));
    
    panel.add(lxCueGroup);
    
    
    
    // Sound cues
    
    soundCueGroup.setName("Sound Cues");
    
    soundCueGroup.add(soundCueIntro.set("intro", 0, 0, 100));
    
    soundCueGroup.add(soundCueTheEnd.set("the end", 0, 0, 100));
    
    soundCueGroup.add(soundCueOutro.set("outro", 0, 0, 100));
    
    panel.add(soundCueGroup);
    
    
    
    //Hit Area
    
    panel.add(hitArea.group);
    
    panel.loadFromFile("settings/sitin.xml");
    
}



void SitinScene::drawMasterScreen() {
    
    // get vision image
    
    // do cv tracking
    
    // fire off OSC when blobs enter certain areas
    
    ContourTracker& tracker = *vision->getTracker();
    if( tracker.bgLearningTime != 2000) tracker.bgLearningTime = 2000;
    ofxCv::ContourFinder& contourFinder = tracker.contourFinder;
    
    float targetWidth = MIN(ofGetWidth(), displays->masterScreen.sizeIn->x) * 0.5;
    float scale = targetWidth / tracker.thresholded.width; //1.98758;
    ofPushStyle();
    ofPushMatrix();
    ofTranslate(10, 10);
    ofSetColor(120);
    //ofPushMatrix();
    ofScale(scale, scale);
    tracker.thresholded.draw(0, 0);
    //ofPopMatrix();
    ofSetColor(255);
    float width = tracker.thresholded.width;
    float height = tracker.thresholded.height;
    int blobCount;
    bool isHit;
    ofNoFill();
    ofSetLineWidth(4);
    
    // iterate over tracked blobs
    hitArea.presence = 0;

    for(int i = 0; i < contourFinder.size(); i++) {
        
        // blob rect and position
        
        auto rect = toOf(contourFinder.getBoundingRect(i));
        
        auto center = toOf(contourFinder.getCenter(i));
        
        isHit = false;
        
        // set the hit area's rect, scaled to the tracked area
        float scaledX = hitArea.position.get().x * width;
        float scaledY = hitArea.position.get().y * height;
        float scaledW = hitArea.size * width;
        float scaledH = hitArea.size * height;
        hitArea.rect.setFromCenter(scaledX, scaledY, scaledW, scaledH);
        // hit test, increase hit area blob count to track the total
        if (rect.intersects(hitArea.rect)) {
            isHit = true;
            hitArea.blobCount++;
            
            hitArea.presence = 1;
        }
        
        
        
        // draw the blob rect
        
        // brighter if it's in an area
        
        if (isHit) {
            
            ofSetHexColor(0x43E06D);
        }
        
        else {
            
            ofSetHexColor(0x385BE0);
        }
        
        ofRect(rect);
        
    }
    
    
    
    ofSetColor(255, 255, 255, 100);
    float y = 10;
    float x = width + 10;
    bool heroChanged = false;
    bool anyChanged = false;
    
    if(hitArea.active) {
        hitArea.update();
        ofSetHexColor(0xF7D63E);
        if (hitArea.changed) {
            cout<<"hit area changed"<<endl;
            if (hitArea.getIsTriggered()) {
            cout<<"hit area triggered: sending Sound Queue"<<endl;
                // send sound on
                ofLogNotice() << "     SOUND ON FOR " << hitArea.name;
                osc->sendSoundCue(hitArea.soundCueIn);
                osc->sendLightingCue(hitArea.lightingCueIn);
                ofSetHexColor(0xF7D63E);
            }
            else {
                cout<<"hit area untriggered: sending Sound Queue"<<endl;
                // send sound off
                ofLogNotice() << "     SOUND OFF FOR " << hitArea.name;
                osc->sendSoundCue(hitArea.soundCueOut);
                osc->sendLightingCue(hitArea.lightingCueOut);
                // ofSetHexColor(0xF7D63E);
            }
        }
        ofRect(hitArea.rect);
        ofDrawBitmapString(hitArea.name, hitArea.rect.getCenter()-ofPoint(30,0));
        //ofTranslate(ofGetWidth()/2, 0);
        ofPushMatrix();
        ofTranslate(tracker.thresholded.width, 0);
        //To-Do replace magic numbers
        ofScale(0.5, 0.5);
        drawVision();
        ofPopMatrix();
    }
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