//
//  SitinScene.h
//  show
//
//  Created by Chris Mullany on 05/09/2015.
//
//

#pragma once
#include "ofMain.h"
#include "SceneBase.h"

class SitinScene : public SceneBase {
public:
    
    SitinScene();
    
    void setup();
    void update();
    void draw();
    void play(int i);
    void stop();
    void setupGui();
    
protected:
private:
    
};