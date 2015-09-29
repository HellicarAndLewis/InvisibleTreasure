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


CassandraScene::CassandraScene() {
    name = "Cassandra";
}

void CassandraScene::setup() {
    // subscenes
    subsceneStart = 77;
    subsceneEnd = 82;
    SceneBase::setup();
}

void CassandraScene::update() {
    if (isWindow()) {
        if (mode == RECORD_CASSANDRA) {
            auto image = vision->ipcamCassandra.grabber->getFrame();
            if (image->isAllocated() && image->height > 1) {
                stringstream stream;
                stream << DIR_CASSANDRA << setw(10) << setfill('0') << fileCount << ".jpg";
                string filename = stream.str();
                image->saveImage(filename, OF_IMAGE_QUALITY_HIGH);
                fileCount++;
            }
        }
        else if (mode == RECORD_MAIN) {
            auto image = vision->ipcam.grabber->getFrame();
            if (image->isAllocated() && image->height > 1) {
                stringstream stream;
                stream << DIR_MAIN << setw(10) << setfill('0') << fileCount << ".jpg";
                string filename = stream.str();
                image->saveImage(filename, OF_IMAGE_QUALITY_HIGH);
                fileCount++;
            }
        }
    }
    SceneBase::update();
}

void CassandraScene::draw() {
    if (isWindow()) {
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
            
        }
        else if (mode == RECORD_CASSANDRA) {
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
                led->show(title.get());
            }
            if (isMaster()) {
                countdown->start(timerIntro.get());
                // TODO: sound cue?
            }
            if (isWindow()) setMode(IDLE);
            break;
            
        case 78:
            // audience name, manual cue
            if (isSlave()) {
                led->show(audience.get());
            }
            if (isMaster()) {
                // TODO: LX cue
                // TODO: sound cue?
            }
            if (isWindow()) setMode(IDLE);
            break;
            
        case 79:
            // welcome, all windows record cassandra room, timed
            if (isSlave()) {
                led->queue(LedDisplay::Params(welcome.get(), 1, 5, 1, true));
                led->playQueue();
            }
            if (isWindow()) {
                setMode(RECORD_CASSANDRA);
            }
            if (isMaster()) {
                countdown->start(timerCassandra.get());
                // TODO: sund cue
            }
            break;
            
        case 80:
            // welcome, all windows stop recording cassandra, start recording main room, timed
            if (isSlave()) {
                led->queue(LedDisplay::Params(welcome.get(), 1, 5, 1, true));
                led->playQueue();
            }
            if (isWindow()) {
                setMode(RECORD_MAIN);
            }
            if (isMaster()) {
                countdown->start(timerMain.get());
                // TODO: sund cue
            }
            break;
            
        case 81:
            // all windows stop recording main room, playback both videos split screen, timed
            if (isWindow()) {
                setMode(PLAYBACK);
            }
            if (isMaster()) {
                // TODO: LX cue
                // TODO: sund cue
            }
            break;
        case 82:
            // rabbit angry, etc, timed
            if (isSlave()) {
                led->queue(LedDisplay::Params(rabbitDisappointed.get(), 0, 1, 0, false, timerOutro.get()));
                led->queue(LedDisplay::Params(rabbitBoss.get(), 0, 1, 0));
                led->queue(LedDisplay::Params(rabbitDisappointed.get(), 0, 1, 0));
                led->queue(LedDisplay::Params(rabbitBoss.get(), 0, 1, 0));
                led->queue(LedDisplay::Params(boss.get(), 0, 1, 0));
                led->queue(LedDisplay::Params(boss.get(), 0, 1, 0));
                led->playQueue();
            }
            if (isMaster()) {
                countdown->start(timerOutro.get() + 1);
                // TODO: LX cue
                // TODO: sund cue
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
    titleGroup.add(title.set("1", "Cassandra"));
    titleGroup.add(audience.set("2", "Audience"));
    titleGroup.add(welcome.set("3", "Welcome"));
    titleGroup.add(rabbitDisappointed.set("4", "The Rabbit is disappointed"));
    titleGroup.add(rabbitBoss.set("5", "The Rabbit is boss"));
    titleGroup.add(boss.set("6", "is boss"));
    
    timerGroup.setName("Timers");
    timerGroup.add(timerIntro.set("intro", 5, 1, 10));
    timerGroup.add(timerCassandra.set("cassandra", 2*60, 1, 10*60));
    timerGroup.add(timerMain.set("main", 5*60, 1, 10*60));
    timerGroup.add(timerOutro.set("outro", 5, 1, 10));
    
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
        sequenceCassandra.setFrameRate(23);
        sequenceMain.setExtension("jpg");
        sequenceMain.loadSequence(DIR_MAIN);
        sequenceMain.setFrameRate(23);
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
    fileCount = 0;
}
//////////////////////////////////////////////////////////////////////////////////
// custom event handlers
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
// oF event handlers
//////////////////////////////////////////////////////////////////////////////////

