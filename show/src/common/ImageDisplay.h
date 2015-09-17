//
//  ImageDisplay.h
//  show
//
//  Created by Chris Mullany on 17/09/2015.
//
//

#pragma once
#include "ofMain.h"
#include "Sequencable.h"

class ImageDisplay : public Sequencable  {
public:
    ImageDisplay();
    
	void setup();
	void update();
	void draw();
    
    void show(float duration = 1.0f);
    void hide(float duration = 1.0f);

protected:  
private:
    
};