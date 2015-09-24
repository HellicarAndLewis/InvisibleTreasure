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
    if (mode==AppModel::SLAVE) {}
    else if (mode==AppModel::WINDOW) {}
    else if (mode==AppModel::MASTER) {}
    SceneBase::update();
}

void CassandraScene::draw() {
    if (mode==AppModel::SLAVE) {}
    else if (mode==AppModel::WINDOW) {}
    else if (mode==AppModel::MASTER) {}
    SceneBase::draw();
}

//////////////////////////////////////////////////////////////////////////////////
// public
//////////////////////////////////////////////////////////////////////////////////
void CassandraScene::play(int i){
    switch (i) {
        case 77:
            // title, timed
            break;
        case 78:
            // audience name, manual cue
            break;
        case 79:
            // welcome, all windows record cassandra room, timed
            break;
        case 80:
            // welcome, all windows stop recording cassandra, start recording main room, timed
            break;
        case 81:
            // all windows stop recording main room, playback both videos split screen, timed
            break;
        case 82:
            // rabbit angry, etc, timed
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
    timerGroup.add(timerOutro.set("outro", 6, 1, 10));
    
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

