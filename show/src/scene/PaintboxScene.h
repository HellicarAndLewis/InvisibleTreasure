//
//  PaintboxScene.h
//  show
//
//  Created by Chris Mullany on 05/09/2015.
//
//

#pragma once
#include "ofMain.h"
#include "SceneBase.h"

// TODO: blob tracking, draw lines with people
//
class PaintboxScene : public SceneBase {
public:
    
    PaintboxScene();
    
    void setup();
    void update();
    void draw();
    void play();
    void stop();
    void setupGui();
    
protected:
private:
    
};