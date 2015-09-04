//
//  ShadowsScene.h
//  show
//
//  Created by Chris Mullany on 02/09/2015.
//
//

#pragma once
#include "ofMain.h"
#include "SceneBase.h"

class ShadowsScene : public SceneBase {
public:
    ShadowsScene();
    
	void setup();
	void update();
	void draw();
    void play();
    void stop();
	
    void setupGui();

protected:  
private:
    ofVideoPlayer video;
};