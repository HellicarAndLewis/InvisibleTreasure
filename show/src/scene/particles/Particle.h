#pragma once
#include "ofMain.h"

enum particleMode{
	PARTICLE_MODE_ATTRACT = 0,
	PARTICLE_MODE_REPEL
};

class Particle{
    
public:
    Particle();
    
    void setMode(particleMode newMode);	
    void setAttractPoints( vector <ofPoint> * attract );
    void setNeighbours(vector <Particle> * neighbours);
    
    void reset();
    void update();
    void draw();
	
    float InvSqrt(float x){
        float xhalf = 0.5f * x;
        int i = *(int*)&x; // store floating-point bits in integer
        i = 0x5f3759d5 - (i >> 1); // initial guess for Newton's method
        x = *(float*)&i; // convert new bits into float
        x = x*(1.5f - xhalf*x*x); // One round of Newton's method
        return x;
    }
    
    ofPoint pos;
    ofPoint vel;
    ofPoint frc;
    
    float drag; 
    float uniqueVal;
    float scale;
    
    particleMode mode;
    ofColor color;
    
    vector <ofPoint> trail;
    vector <ofPoint> * attractPoints;
    vector <Particle> * neighbours;
    ofPoint closestPt;
    bool flock;
    
    float minUnique, maxUnique;
    float minDrag, maxDrag;
    
    int id;
};