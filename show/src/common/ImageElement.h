//
//  ImageElement.h
//  show
//
//  Created by Chris Mullany on 17/09/2015.
//
//

#pragma once
#include "ofMain.h"
#include "Sequencable.h"

class ImageElement : public Sequencable  {
public:
    ImageElement();
	void setup(string path);
    void draw();
    void draw(ofRectangle& rect);
    void show(float duration = 1.0f);
    void hide(float duration = 1.0f);
    
    ofImage image;

protected:  
private:
    
};