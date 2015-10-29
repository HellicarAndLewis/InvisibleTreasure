//
//  Paintbrush.h
//  test_paintboxFlow
//
//  Created by James Bentley on 10/29/15.
//
//

#ifndef __test_paintboxFlow__Paintbrush__
#define __test_paintboxFlow__Paintbrush__

#include "ofMain.h"
#include "ofxIntegrator.h"
#include "ofxCv.h"

class paintBrush {
    ofColor col;
    ofVec2f loc;
    ofVec2f vel;
    ofVec2f acc;
    
    void setup(int width, int height);
    void update(ofxCv::FlowFarneback*);
    void draw();
};

#endif /* defined(__test_paintboxFlow__Paintbrush__) */
