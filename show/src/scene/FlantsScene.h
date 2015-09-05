//
//  FlantsScene.h
//  show
//
//  Created by Chris Mullany on 05/09/2015.
//
//

#pragma once
#include "ofMain.h"
#include "SceneBase.h"

// TODO: particles, blob tracking, contours, errosion
//
class FlantsScene : public SceneBase {
public:
    
    FlantsScene();
    
    void setup();
    void update();
    void draw();
    void play();
    void stop();
    void setupGui();
    
protected:
private:
    
};