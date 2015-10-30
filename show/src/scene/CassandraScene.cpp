//
//  CassandraScene.cpp
//  show
//
//  Created by Chris Mullany on 05/09/2015.
//
//

#include "CassandraScene.h"
using namespace ofxCv;
using namespace cv;

#define DIR_CASSANDRA "recordings/cassandra/"
#define DIR_MAIN "recordings/main/"

#define CASSANDRA_DURATION 120 // Cassandra recording lasts for 2 minutes
#define MAIN_DURATION 300 // main recording lasts for 5 minutes


CassandraScene::CassandraScene() {
    name = "Cassandra";
}

void CassandraScene::setup() {
    // subscenes
    subsceneStart = 77;
    subsceneEnd = 82;
    timer.setup("fonts/Arial Black.ttf", 140);
    timer.colour.set(255);
    SceneBase::setup();
}

void CassandraScene::update() {
    if (isWindow()) {
        if (mode == RECORD_CASSANDRA) {
            auto image = vision->ipcamCassandra.grabber->getFrame();
            if (image->isAllocated() && image->height > 1) {
                stringstream stream;
                stream << DIR_CASSANDRA << setw(10) << setfill('0') << cassandraFileCount << ".jpg";
                string filename = stream.str();
                image->saveImage(filename, OF_IMAGE_QUALITY_HIGH);
                //fileCount++;
                cassandraFileCount++;
            }
        }
        else if (mode == RECORD_MAIN) {
            auto image = vision->ipcam.grabber->getFrame();
            if (image->isAllocated() && image->height > 1) {
                stringstream stream;
                stream << DIR_MAIN << setw(10) << setfill('0') << mainFileCount << ".jpg";
                string filename = stream.str();
                image->saveImage(filename, OF_IMAGE_QUALITY_HIGH);
                //filecount++;
                mainFileCount++;
            }
        }
        timer.update();
    }
    SceneBase::update();
}

void CassandraScene::draw() {
    if (isWindow()) {
        vision->isEnabled = true;
        if (mode == PLAYBACK) {
            playbackTime += ofGetLastFrameTime();
            
            // draw the image sequences next to each other
            // if only main is drawing, set the scale to fill the screen
            ofTexture& main = sequenceMain.getTextureForTime(playbackTime);
            float scale = ofGetWidth() / main.getWidth();
            float x = 0;
            float y = (ofGetHeight()/2) - (main.getHeight()*scale*0.5);
            // draw cassandra if it's still playing
            if (playbackTime < timerCassandra) {
                ofTexture& cassandra = sequenceCassandra.getTextureForTime(playbackTime);
                scale = (ofGetWidth()/2) / cassandra.getWidth();
                x = cassandra.getWidth()*scale;
                y = (ofGetHeight()/2) - (main.getHeight()*scale*0.5);
                cassandra.draw(0, y, cassandra.getWidth()*scale, cassandra.getHeight()*scale);
            }
            main.draw(x, y, main.getWidth()*scale, main.getHeight()*scale);
            
            if (sequenceMain.getCurrentFrame() >= sequenceMain.getTotalFrames() - 1 || (playbackTime > timerMain && sequenceMain.getCurrentFrame() < 20)) {
                // sequence is over
                mode == IDLE;
            }
            
        }
        
        if (subsceneI == 80) {
            windowTimer += ofGetLastFrameTime();
            int secondsLeft = timerMain - windowTimer;
            if (secondsLeft > 0) {
                int mins = floor(secondsLeft / 60);
                string secs = ofToString( (secondsLeft % 60) );
                if (secs.length() == 1) secs = "0" + secs;
                timer.messageString = ofToString(mins) + ":" + secs;
                timer.maxWidth = ofGetWidth() * 0.9;
                int stringH = timer.getHeight();
                timer.draw(ofGetWidth()/2, ofGetHeight()*.45 - stringH/2);
            }
        }
        
        if (isDebugMode) {
            if (mode == RECORD_CASSANDRA) {
                auto image = vision->ipcamCassandra.grabber->getFrame();
                image->draw(0, 0);
                ofSetColor(200,0,0);
                ofCircle(ofGetWidth()/2, ofGetHeight()/2, 20);
                ofSetColor(255);
            }
            else if (mode == RECORD_MAIN) {
                vision->ipcam.grabber->getFrame()->draw(0, 0);
                ofSetColor(200,0,0);
                ofCircle(ofGetWidth()/2, ofGetHeight()/2, 20);
                ofSetColor(255);
            }
            else if (mode == PLAYBACK) {
                ofDrawBitmapStringHighlight("playback time: " + ofToString(playbackTime), 10, ofGetHeight()/2);
                ofDrawBitmapStringHighlight("cassandra: " + ofToString(sequenceCassandra.getCurrentFrame()) + "/" + ofToString(sequenceCassandra.getTotalFrames()), 10, ofGetHeight()/2 + 20);
                ofDrawBitmapStringHighlight("main: " + ofToString(sequenceMain.getCurrentFrame()) + "/" + ofToString(sequenceMain.getTotalFrames()), 10, ofGetHeight()/2 + 40);
            }
        }
    }
    SceneBase::draw();
}

//////////////////////////////////////////////////////////////////////////////////
// public
//////////////////////////////////////////////////////////////////////////////////
void CassandraScene::play(int i){
    switch (i) {
        case 77:
            // title, timed
            if (isSlave()) {
                led->show("5. SOMEONE");
            }
            if (isMaster()) {
                countdown->start(timerIntro.get());
                osc->sendSoundCue(soundCueIntro);
            }
            if (isWindow()) setMode(IDLE);
            break;
            
        case 78:
            // audience name, manual cue
            if (isSlave()) {
                led->show(audience.get());
            }
            if (isMaster()) {
                osc->sendLightingCue(lxCueIntro);
                osc->sendSoundCue(soundCueName);
            }
            if (isWindow()) setMode(IDLE);
            break;
            
        case 79:
            // welcome, all windows record cassandra room, timed
            if (isSlave()) {
                led->hide();
                led->show("5. " + ofToUpper(audience.get()) + " KNOWS");
                //led->playQueue();
            }
            if (isWindow()) {
                timer.hide();
                setMode(RECORD_CASSANDRA);
            }
            if (isMaster()) {
                countdown->start(timerCassandra.get());
                osc->sendSoundCue(75);
            }
            break;
            
        case 80:
            // welcome, all windows stop recording cassandra, start recording main room, timed
            if (isSlave()) {
                led->hide();
                //led->queue(LedDisplay::Params(welcome.get(), 1, 5, 1, true)); old replaced with line below
                //led->playQueue();
                //led->show("5. SOMEONE KNOWS");
            }
            if (isWindow()) {
                windowTimer = 0;
                timer.show("", 1, -1, 1);
                setMode(RECORD_MAIN);
            }
            if (isMaster()) {
                countdown->start(timerMain.get());
                osc->sendSoundCue(soundCueTimePassing);
            }
            break;
            
        case 81:
            // all windows stop recording main room, playback both videos split screen, timed
            if (isWindow()) {
                timer.hide();
                setMode(PLAYBACK);
            }
            if (isSlave()) {
                led->hide();
                led->queue(LedDisplay::Params("5. " + ofToUpper(audience.get()) + " KNOWS NOTHING", 0, 1, 5, false, 0));
                led->playQueue();
                //led->show("5. SOMEONE KNOWS NOTHING");
            }
            if (isMaster()) {
                countdown->start(timerMain.get());
                osc->sendSoundCue(soundCuePlayback);
                osc->sendLightingCue(lxCuePlayback);
            }
            break;
        case 82:
            // rabbit angry, etc, timed
            if (isSlave()) {
                led->hide();
                led->queue(LedDisplay::Params(rabbitDisappointed.get(), 0, 2, 0, false));
                led->queue(LedDisplay::Params(rabbitBoss.get(), 0, 1, 0, false, timerOutro.get()));
                led->queue(LedDisplay::Params(boss.get(), 0, 1, 0));
                led->queue(LedDisplay::Params(boss.get(), 0, 1, 0));
                led->queue(LedDisplay::Params(boss.get(), 0, 1, 0));
                led->queue(LedDisplay::Params(boss.get(), 0, 1, 0));
                led->playQueue();
            }
            if (isMaster()) {
                countdown->start(7);
                osc->sendSoundCue(80);
                osc->sendLightingCue(lxCueOutro);
            }
            if (isWindow()) setMode(IDLE);
            break;
        default:
            break;
    }
    SceneBase::play(i);
}

void CassandraScene::stop(){
    // stop/unload/clear things
    SceneBase::stop();
}

void CassandraScene::setupGui() {
    guiName = "Cassandra";
    panel.setup(guiName, "settings/cassandra.xml");
    
    titleGroup.setName("Titles");
    titleGroup.add(title.set("title1", "5. ALL THINK FIB LENS"));
    titleGroup.add(audience.set("title2", "Clare"));
    titleGroup.add(welcome.set("title3", "Welcome"));
    titleGroup.add(rabbitDisappointed.set("title4", "He is disappointed"));
    titleGroup.add(rabbitBoss.set("title5", "He is pleased"));
    titleGroup.add(boss.set("title6", "He is boss"));
    
    timerGroup.setName("Timers");
    timerGroup.add(timerIntro.set("intro", 5, 1, 10));
    timerGroup.add(timerCassandra.set("cassandra", 2*60, 1, 10*60));
    timerGroup.add(timerMain.set("main", 5*60, 1, 10*60));
    timerGroup.add(timerOutro.set("outro", 5, 1, 10));
    panel.add(titleGroup);
    panel.add(timerGroup);
    
    // LX cues
    lxCueGroup.setName("LX Cues");
    lxCueGroup.add(lxCueIntro.set("intro", 34, 0, 100));
    lxCueGroup.add(lxCuePlayback.set("playback", 35, 0, 100));
    lxCueGroup.add(lxCueOutro.set("outro", 0, 0, 100));
    panel.add(lxCueGroup);
    
    // Sound cues
    soundCueGroup.setName("Sound Cues");
    soundCueGroup.add(soundCueIntro.set("intro", 0, 0, 100));
    soundCueGroup.add(soundCueName.set("name", 0, 0, 100));
    soundCueGroup.add(soundCueCassandra.set("cassandra", 0, 0, 100));
    soundCueGroup.add(soundCueTimePassing.set("time passing", 0, 0, 100));
    soundCueGroup.add(soundCuePlayback.set("playback", 0, 0, 100));
    soundCueGroup.add(soundCueOutro.set("outro", 0, 0, 100));
    panel.add(soundCueGroup);
    
    panel.loadFromFile("settings/cassandra.xml");
}
//////////////////////////////////////////////////////////////////////////////////
// protected
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
// private
//////////////////////////////////////////////////////////////////////////////////

void CassandraScene::setMode(Mode mode) {
    this->mode = mode;
    if (mode == PLAYBACK) {
        indexCassandra = 0;
        indexMain = 0;
        playbackTime = 0;
        sequenceCassandra.setExtension("jpg");
        sequenceCassandra.loadSequence(DIR_CASSANDRA);
        sequenceCassandra.setFrameRate(cassandraFileCount/CASSANDRA_DURATION);
        sequenceMain.setExtension("jpg");
        sequenceMain.loadSequence(DIR_MAIN);
        sequenceMain.setFrameRate(mainFileCount/MAIN_DURATION);
    }
    else {
        sequenceCassandra.unloadSequence();
        sequenceMain.unloadSequence();
    }
    if (mode == RECORD_CASSANDRA) {
        prepareRecordingDir(DIR_CASSANDRA);
        vision->setToIPCamCassandra();
    }
    else if (mode == RECORD_MAIN) {
        prepareRecordingDir(DIR_MAIN);
        vision->setToIPCamMain();
    }
}

void CassandraScene::prepareRecordingDir(string path) {
    // remove existing recordings
    if (ofDirectory::doesDirectoryExist(path)) {
        ofDirectory::removeDirectory(path, true);
    }
    ofDirectory::createDirectory(path);
    if(path == DIR_CASSANDRA) cassandraFileCount = 0;
    else mainFileCount = 0;
}
//////////////////////////////////////////////////////////////////////////////////
// custom event handlers
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
// oF event handlers
//////////////////////////////////////////////////////////////////////////////////

