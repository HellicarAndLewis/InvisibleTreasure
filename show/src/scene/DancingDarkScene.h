//
//  DancingDarkScene.h
//  show
//
//  Created by Chris Mullany on 05/09/2015.
//
//

#pragma once
#include "ofMain.h"
#include "SceneBase.h"

class DancingDarkScene : public SceneBase {
public:
    
    DancingDarkScene();
    
    void setup();
    void update();
    void draw();
    void play();
    void stop();
    void setupGui();
    
protected:
private:
    
};