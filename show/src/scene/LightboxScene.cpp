//
//  LightboxScene.cpp
//  show
//
//  Created by Chris Mullany on 02/09/2015.
//
//

#include "LightboxScene.h"
using namespace ofxCv;
using namespace cv;

LightboxScene::LightboxScene() {
    name = "Lightbox";
}

void LightboxScene::setup() {
    hitAreas[0].set("centre");
    hitAreas[1].set("window1");
    hitAreas[2].set("window2");
    hitAreas[3].set("window3");
    hitAreas[4].set("window4");
    // subscenes
    subsceneStart = 10;
    subsceneEnd = 16;
    SceneBase::setup();
}

void LightboxScene::update() {
    if (isWindow()) {
        imageElement.update();
    }
    SceneBase::update();
}

void LightboxScene::draw() {
    if (isWindow()) {
        imageElement.draw(ofRectangle(0, 0, ofGetWidth(), ofGetHeight()));
    }
    SceneBase::draw();
}

void LightboxScene::drawMasterScreen() {
    
    // get vision image
    // do cv tracking
    // fire off OSC when blobs enter certain areas
    ContourTracker& tracker = *vision->getTracker();
    ofxCv::ContourFinder& contourFinder = tracker.contourFinder;
    
    
    float targetWidth = MIN(ofGetWidth(), displays->masterScreen.sizeIn->x) * 0.5;
    float scale = targetWidth / tracker.thresholded.width;
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
    bool allBlobsInAreas = true;
    for(int i = 0; i < contourFinder.size(); i++) {
        // blob rect and position
        auto rect = toOf(contourFinder.getBoundingRect(i));
        auto center = toOf(contourFinder.getCenter(i));
        isHit = false;
        
        // iterate over all hit areas
        for (auto & hitArea: hitAreas) {
            if (hitArea.active) {
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
            }
            if (!isHeroActive && hitArea.name != "centre") {
                hitArea.presence = 0;
            }
        }
        
        // draw the blob rect
        // brighter if it's in an area
        if (isHit) {
            ofSetHexColor(0x43E06D);
        }
        else {
            ofSetHexColor(0x385BE0);
            allBlobsInAreas = false;
        }
        ofRect(rect);
    }
    
    
    // Check central / hero area
    // this always controls lighting
    // also determines whether or not other areas are active
    
    // now check each hit area to see how many people are in it
    // hitArea[0] is centre/hero: this controls light state
    // hitArea[0] also determines whether or not other areas are active
    // all areas control sound volume
    ofSetColor(255, 255, 255, 100);
    float y = 10;
    float x = width + 10;
    bool heroChanged = false;
    bool anyChanged = false;
    
    
    
    for (int i=0; i<5; i++) {
        HitArea& hitArea = hitAreas[i];
        
        if (playMode == ALL_ZONES) {
            if (!allBlobsInAreas) hitArea.presence = 0;
        }
        
        if (hitArea.active) {
            hitArea.update();
            if (hitArea.changed) anyChanged = true;
            
            // Blobs in this area
            // trigger zone is ACTIVE
            if (hitArea.getIsTriggered()) {
                // is this the hero/centre? Send lighing cue based on the current mode
                if (hitArea.name == "centre") {
                    isHeroActive = true;
                    heroChanged = (hitArea.changed);
                }
                // this zone and hero are active
                ofSetHexColor(0xF7D63E);
            }
            
            // No blobs in this area
            // Trigger sone is INACTIVE
            else  {
                // if this is centre/hero send lx reset
                if (hitArea.name == "centre") {
                    isHeroActive = false;
                    heroChanged = (hitArea.changed);
                }
                ofSetHexColor(0xA8912A);
            }
            
            if (hitArea.changed) {
                if (hitArea.getIsTriggered()) {
                    // send sound on
                    ofLogNotice() << "     SOUND ON FOR " << hitArea.name;
                    osc->sendSoundCue(hitArea.soundCueIn);
                }
                else {
                    // send sound off
                    ofLogNotice() << "     SOUND OFF FOR " << hitArea.name;
                    osc->sendSoundCue(hitArea.soundCueOut);
                }
            }
            
            ofRect(hitArea.rect);
            ofDrawBitmapString(hitArea.name, hitArea.rect.getCenter()-ofPoint(30,0));
        }
        
        // draw bars for presence
        ofPushStyle();
        ofFill();
        ofSetColor(50);
        ofRect(x, y, ofGetWidth()/2, 40);
        ofSetColor(255);
        ofRect(x, y, hitArea.smoothed*ofGetWidth()/2, 40);
        ofDrawBitmapStringHighlight(hitArea.name + " presence:" + ofToString(hitArea.presence), x, y+20);
        y+=40;
        ofPopStyle();
        
        // reset the count to 0 ready for the next loop
        hitArea.blobCount = 0;
        hitArea.presence = 0;
    }
    
    // Check to see if we need to send a new LX cue
    bool lastZonesActive = zonesActive;
    if ((playMode == ALL_ZONES || playMode == WAITING) && anyChanged) {
        if (allBlobsInAreas && isHeroActive) {
            if (playMode == WAITING) nextSubscene();
            zonesActive = true;
        }
        else zonesActive = false;
    }
    else if (playMode != ALL_ZONES && heroChanged) {
        if (isHeroActive) zonesActive = true;
        else zonesActive = false;
    }
    
    if (zonesActive != lastZonesActive) {
        if (zonesActive) sendActiveCue();
        else if(playMode == ALL_ZONES || playMode == WAITING) osc->sendLightingCue(cues[0].lightCue);
        else osc->sendLightingCue(CenterBlinkingAllOff.lightCue);
    }
    
    ofPopMatrix();
    ofPopStyle();
}

//////////////////////////////////////////////////////////////////////////////////
// public
//////////////////////////////////////////////////////////////////////////////////
void LightboxScene::play(int i){
    
    // interactive / play modes
    // centre, wall 1, wall 2, walls 3 & 4
    if (i >= 10 && i <= 13) {
        if (isSlave()) led->show(title.get());
        if (isMaster()) {
            if (i == 10) {
                playMode = CENTRE;
                countdown->start(30);
            }
            else if (i == 11) {
                playMode = WALL_1;
                countdown->start(30);
            }
            else if (i == 12) {
                playMode = WALL_2;
                countdown->start(30);
            }
            else if (i == 13) {
                playMode = WALLS_3_4;
                countdown->start(60);
            }
        }
    }
    
    // Waiting for all players to be in trigger zones
    else if (i == 14) {
        if (isSlave()) {
            led->show(waiting.get());
        }
        if (isMaster()) {
            countdown->stop();
            osc->sendLightSoundCue(WaitingIntro);
            playMode = WAITING;
        }
    }
    
    // All zones active
    // everyone must be in a zone
    else if (i == 15) {
        if (isSlave()) {
            led->hide();
            led->queue(LedDisplay::Params(phase2.get(), 0, 5, 1, false, 0));
        }
        if (isMaster()) {
            countdown->start(4 * 60);
            osc->sendLightSoundCue(SystemReadyIntro);
            playMode = ALL_ZONES;
        }
        if (isWindow()) imageElement.hide();
    }
    
    // outro
    else if (i == 16) {
        if (isSlave()) {
            led->hide();
            led->queue(LedDisplay::Params("", 0, 2, 0, false));
            led->queue(LedDisplay::Params(bonusGame, 0, 6, 0, false));
            led->queue(LedDisplay::Params(bonusGame, 0, 5, 0, false, 5));
            led->queue(LedDisplay::Params(bonusGame, 0, 8, 0, false));
            led->queue(LedDisplay::Params("", 0, 2, 0, false));
            led->playQueue();
        }//led->show(bonusGame.get(), countdownDuration);
        if (isMaster()) {
            countdown->start(countdownDuration);
            osc->sendLightSoundCue(cues[LIGHTBOX_CUE_COUNT-1]);
        }
        if (isWindow()) {
            // pink/yellow image
            imageElement.setup("images/bonusGame.jpg");
            imageElement.show();
        }
    }
    
    // refresh hit areas for all subscenes
    if (isMaster()) {
        vision->isEnabled = true;
        refreshHitAreas();
    }
    
    SceneBase::play(i);
}

void LightboxScene::stop(){
    if (isWindow()) imageElement.hide();
    SceneBase::stop();
}

void LightboxScene::setupGui() {
    guiName = "Lightbox";
    panel.setup(guiName, "settings/lightbox.xml");
    // titles, times, cues
    panel.add(title.set("title1", "OHMNARY"));
    panel.add(waiting.set("title2", "In it to win it"));
    panel.add(phase2.set("title3", "System ready"));
    panel.add(bonusGame.set("title4", "Bonus Game"));
    panel.add(countdownDuration.set("countdown", 5, 0, 20));
    // cues: 0:reset, 1:centre, 2:wall1, 3:wall2, 4:wall3, 5:wall4, 6:all, 7:outro
    int lxNum = 3;
    for (int i=0; i<LIGHTBOX_CUE_COUNT; i++) {
        panel.add(cues[i].setup("Cue " + ofToString(i)));
        cues[i].lightCue = lxNum;
        cues[i].soundCue = lxNum;
        lxNum++;
    }
    panel.add(CenterBlinkingAllOff.setup("CenterBlinkingAllOff", 1));
    panel.add(SystemReadyIntro.setup("SystemReadyIntro", 1));
    panel.add(WaitingIntro.setup("WaitingIntro", 1));
    
    // add hit areas
    // positions and sizes are normalized
    // so they can later be mapped to the tracking rect
    for (auto hit: hitAreas) {
        panel.add(hit.group);
    }
    panel.loadFromFile("settings/lightbox.xml");
}

void LightboxScene::drawGui() {
    GuiableBase::drawGui();
}


//////////////////////////////////////////////////////////////////////////////////
// protected
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
// private
//////////////////////////////////////////////////////////////////////////////////

void LightboxScene::sendActiveCue() {
    // cues: 0:reset, 1:centre, 2:wall1, 3:wall2, 4:wall3, 5:wall4, 6:all, 7:outro
    switch (playMode) {
        case CENTRE:
            osc->sendLightingCue(cues[1].lightCue);
            break;
        case WALL_1:
            osc->sendLightingCue(cues[2].lightCue);
            break;
        case WALL_2:
            osc->sendLightingCue(cues[3].lightCue);
            break;
        case WALLS_3_4:
        case WAITING:
            osc->sendLightingCue(cues[4].lightCue);
            osc->sendLightingCue(cues[5].lightCue);
            break;
        case ALL_ZONES:
            osc->sendLightingCue(cues[6].lightCue);
            break;
        default:
            break;
    }
}

void LightboxScene::refreshHitAreas() {
    for (auto & hitArea: hitAreas) hitArea.active = false;
    switch (playMode) {
        case CENTRE:
            hitAreas[0].active = true;
            break;
        case WALL_1:
            hitAreas[0].active = true;
            hitAreas[1].active = true;
            break;
        case WALL_2:
            hitAreas[0].active = true;
            hitAreas[1].active = true;
            hitAreas[2].active = true;
            break;
        case WALLS_3_4:
        case WAITING:
            for (auto & hitArea: hitAreas) hitArea.active = true;
            break;
        case ALL_ZONES:
            for (auto & hitArea: hitAreas) hitArea.active = true;
            break;
        default:
            break;
    }
}

OscClient::CueParams LightboxScene::getCueForArea(string name){
    if (name == "centre") return cues[1];
    if (name == "window1") return cues[2];
    if (name == "window2") return cues[3];
    if (name == "window3") return cues[4];
    if (name == "window4") return cues[5];
}
//////////////////////////////////////////////////////////////////////////////////
// custom event handlers
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
// oF event handlers
//////////////////////////////////////////////////////////////////////////////////