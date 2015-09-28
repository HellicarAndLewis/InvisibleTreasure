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
        if (mode == PLAYBACK) {
        }
        else if (mode == RECORD_CASSANDRA) {
            string filename = "recordings/cassandra/" + ofToString(fileCount) + ".jpg";
            vision->ipcamCassandra.grabber->getFrame()->saveImage(filename, OF_IMAGE_QUALITY_HIGH);
            fileCount++;
        }
        else if (mode == RECORD_MAIN) {
            string filename = "recordings/main/" + ofToString(fileCount) + ".jpg";
            vision->ipcam.grabber->getFrame()->saveImage(filename, OF_IMAGE_QUALITY_HIGH);
            fileCount++;
        }
    }
    SceneBase::update();
}

void CassandraScene::draw() {
    if (isWindow()) {
        if (mode == PLAYBACK) {
            // TODO: playback image sequence
            if (sequenceCassandra.isLoaded())
                sequenceCassandra.getFrameForTime(ofGetElapsedTimef())->draw(0,0);
        }
        else if (mode == RECORD_CASSANDRA) {
            vision->ipcamCassandra.grabber->getFrame()->draw(0, 0);
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
            break;
        case 79:
            // welcome, all windows record cassandra room, timed
            if (isSlave()) {
                led->queue(LedDisplay::Params(welcome.get(), 1, 5, 1, true));
                led->playQueue();
            }
            if (isWindow()) {
                vision->setToIPCamCassandra();
                mode = RECORD_CASSANDRA;
                fileCount = 0;
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
                vision->setToIPCamMain();
                mode = RECORD_MAIN;
                fileCount = 0;
            }
            if (isMaster()) {
                countdown->start(timerMain.get());
                // TODO: sund cue
            }
            break;
        case 81:
            // all windows stop recording main room, playback both videos split screen, timed
            if (isWindow()) {
                mode = PLAYBACK;
                sequenceCassandra.enableThreadedLoad(true);
                sequenceCassandra.setExtension("jpg");
                sequenceCassandra.loadSequence("recordings/cassandra");
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

//////////////////////////////////////////////////////////////////////////////////
// custom event handlers
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
// oF event handlers
//////////////////////////////////////////////////////////////////////////////////

