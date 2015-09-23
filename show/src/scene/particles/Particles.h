#pragma once

#include "ofMain.h"
#include "Particle.h"

class Particles {

public:
    
    Particles();
    void setup();
    void update();
    void draw(bool drawGrey=false);
    void resetParticles();
    void updateAttractPoints();
    void setParticleNum(int num);
    void applyParticleProps();
    
    void fastNormalize(ofVec3f& vec);
    float* normalizeData;
    void fastNormalize2(ofVec3f &vecA, ofVec3f &vecB);
    
    float InvSqrt(float x){
        float xhalf = 0.5f * x;
        int i = *(int*)&x; // store floating-point bits in integer
        i = 0x5f3759d5 - (i >> 1); // initial guess for Newton's method
        x = *(float*)&i; // convert new bits into float
        x = x*(1.5f - xhalf*x*x); // One round of Newton's method
        return x;
    }
    
    particleMode currentMode;
    string currentModeStr; 

    vector <Particle> p;
    vector <ofPoint> attractPoints;
    vector <ofPoint> attractPointsWithMovement;

    ofImage particleTexture;

    ofVbo vbo;
    ofShader shader;
    ofTexture texture;
    
    float colourLerpAmt;
    float minDrag, maxDrag;
    
		
};
