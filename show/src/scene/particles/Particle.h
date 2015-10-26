#pragma once
#include "ofMain.h"

class Particle{
    
public:
    enum Mode {
        EAT_GREEN, EAT_GROW, EAT_NOTHING, EXPLODE
    };
    
    Particle();
    
    void setMode(Mode newMode);
    void setAttractPoints( vector <ofRectangle> * attract );
    void setNeighbours(vector <Particle> * neighbours);
    
    void reset();
    void reset(ofRectangle bounds);
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
    
    void eat();
    void setScale(float scale);
    
    ofPoint pos;
    ofPoint vel;
    ofPoint frc;
    
    float drag; 
    float uniqueVal;
    float scale;
    
    Mode mode;
    ofColor color;
    float colorInhibitor;
    
    vector <ofPoint> trail;
    vector <ofRectangle> * attractPoints;
    vector <Particle> * neighbours;
    ofPoint closestPt;
    bool flock;
    bool isFull;
    bool isEating;
    bool eatBlobs;
    bool inShape;
    
    float minUnique, maxUnique;
    float minDrag, maxDrag;
    float fullness;
    
    ofRectangle bounds;
    
    int id;
};